#include "./exeWatcher.h"
#include <tlhelp32.h>
#include <atomic>
#include <set>
#include <sstream>

namespace ProcessLimiter {
	
	// ---------- 编码转换 ----------
	inline wstring ToWide(const string& s) {
		if (s.empty()) return {};
		UINT cp = CP_UTF8;
		int wlen = MultiByteToWideChar(cp, 0, s.c_str(), (int)s.size(), nullptr, 0);
		if (wlen <= 0) {
			cp = CP_ACP;
			wlen = MultiByteToWideChar(cp, 0, s.c_str(), (int)s.size(), nullptr, 0);
			if (wlen <= 0) return {};
		}
		wstring w(wlen, L'\0');
		MultiByteToWideChar(cp, 0, s.c_str(), (int)s.size(), &w[0], wlen);
		return w;
	}
	
	inline string ToGBK(const wstring& ws) {
		if (ws.empty()) return {};
		int len = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), (int)ws.size(),
									  nullptr, 0, nullptr, nullptr);
		if (len <= 0) return {};
		string s(len, '\0');
		WideCharToMultiByte(CP_ACP, 0, ws.c_str(), (int)ws.size(),
							&s[0], len, nullptr, nullptr);
		return s;
	}
	
	inline void LogOut(const wstring& ws, bool toErr = false) {
		string s = ToGBK(ws);
		if (toErr) std::cerr << s;
		else       std::cout << s;
		std::cout.flush();
		std::cerr.flush();
	}
	
	template <typename... Args>
	inline void Log(bool toErr, Args&&... args) {
		std::wostringstream oss;
		(oss << ... << args);
		LogOut(oss.str(), toErr);
	}
	
	// ---------- 进程枚举 ----------
	vector<DWORD> FindProcessesByName(const wstring& exeName) {
		vector<DWORD> result;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE) return result;
		PROCESSENTRY32W pe{};
		pe.dwSize = sizeof(pe);
		if (Process32FirstW(hSnap, &pe)) {
			do {
				if (_wcsicmp(pe.szExeFile, exeName.c_str()) == 0)
					result.push_back(pe.th32ProcessID);
			} while (Process32NextW(hSnap, &pe));
		}
		CloseHandle(hSnap);
		return result;
	}
	
	// ---------- Job 配置 ----------
	bool ConfigureJob(HANDLE hJob, DWORD cpuPercent, SIZE_T memBytes, bool verbose) {
		JOBOBJECT_CPU_RATE_CONTROL_INFORMATION cpuLimit{};
		cpuLimit.ControlFlags = JOB_OBJECT_CPU_RATE_CONTROL_ENABLE |
		JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP;
		cpuLimit.CpuRate = cpuPercent * 100;
		if (!SetInformationJobObject(hJob, JobObjectCpuRateControlInformation,
									 &cpuLimit, sizeof(cpuLimit))) {
			if (verbose)
				Log(true, L"[!] CPU 限制设置失败 (错误 ", GetLastError(),
					L"，需 Win8.1+)\n");
		}
		
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION memLimit{};
		memLimit.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_PROCESS_MEMORY;
		memLimit.ProcessMemoryLimit = memBytes;
		if (!SetInformationJobObject(hJob, JobObjectExtendedLimitInformation,
									 &memLimit, sizeof(memLimit))) {
			if (verbose)
				Log(true, L"[!] 内存限制设置失败 (错误 ", GetLastError(), L")\n");
			return false;
		}
		return true;
	}
	
	// ---------- 分配结果 ----------
	enum class AssignResult { Success, AlreadyInJob, Failed };
	
	AssignResult TryAssign(HANDLE hJob, DWORD pid, bool verbose) {
		HANDLE hProc = OpenProcess(
								   PROCESS_SET_QUOTA | PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION,
								   FALSE, pid);
		if (!hProc) {
			if (verbose)
				Log(true, L"[!] OpenProcess(PID ", pid, L") 失败 (错误 ",
					GetLastError(), L")\n");
			return AssignResult::Failed;
		}
		
		BOOL bInJob = FALSE;
		if (IsProcessInJob(hProc, NULL, &bInJob) && bInJob) {
			CloseHandle(hProc);
			return AssignResult::AlreadyInJob;
		}
		
		BOOL ok = AssignProcessToJobObject(hJob, hProc);
		DWORD err = ok ? 0 : GetLastError();
		CloseHandle(hProc);
		if (ok) {
			if (verbose) Log(false, L"[+] 已限制 PID ", pid, L"（Job 硬限制）\n");
			return AssignResult::Success;
		}
		if (verbose)
			Log(true, L"[!] PID ", pid, L" 加入作业失败 (错误 ", err, L")\n");
		return AssignResult::Failed;
	}
	
	// ---------- 回退软限制 ----------
	// 用于进程已在别人的 Job 中、无法加入我们 Job 的情况：
	//   1) 优先级降到 IDLE_PRIORITY_CLASS（软 CPU 限制）
	//   2) 设置工作集上下限，并周期性修剪（软内存限制）
	bool ApplyFallbackLimits(DWORD pid, SIZE_T memBytes, bool verbose) {
		HANDLE hProc = OpenProcess(
								   PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA,
								   FALSE, pid);
		if (!hProc) {
			if (verbose)
				Log(true, L"[!] 回退限制: 无法打开 PID ", pid, L" (错误 ",
					GetLastError(), L")\n");
			return false;
		}
		
		if (!SetPriorityClass(hProc, IDLE_PRIORITY_CLASS)) {
			if (verbose)
				Log(true, L"[!] 回退限制: PID ", pid, L" SetPriorityClass 失败 (错误 ",
					GetLastError(), L")\n");
		}
		
		// 设置工作集上下限（硬上下限仅影响物理内存工作集，不影响提交大小）
		SIZE_T maxWs = memBytes;
		SIZE_T minWs = 0;
		SetProcessWorkingSetSize(hProc, minWs, maxWs);
		// 立即修剪一次
		SetProcessWorkingSetSize(hProc, (SIZE_T)-1, (SIZE_T)-1);
		
		CloseHandle(hProc);
		return true;
	}
	
	// 每次轮询都调用，把工作集多余页刷到页面文件
	void TrimWorkingSet(DWORD pid) {
		HANDLE hProc = OpenProcess(PROCESS_SET_QUOTA, FALSE, pid);
		if (hProc) {
			SetProcessWorkingSetSize(hProc, (SIZE_T)-1, (SIZE_T)-1);
			CloseHandle(hProc);
		}
	}
	
	// ---------- 主循环 ----------
	struct Config {
		wstring exeName;
		DWORD  cpuPercent   = 50;
		SIZE_T memoryMB     = 512;
		DWORD  pollIntervalMs = 500;
		bool   verbose      = true;
	};
	
	bool WatchAndLimit(const Config& cfg, atomic<bool>* stopFlag) {
		if (cfg.exeName.empty()) return false;
		
		wstring target = cfg.exeName;
		size_t slash = target.find_last_of(L"\\/");
		if (slash != wstring::npos) target = target.substr(slash + 1);
		
		DWORD cpu = (cfg.cpuPercent >= 1 && cfg.cpuPercent <= 100) ? cfg.cpuPercent : 50;
		SIZE_T memMB = cfg.memoryMB ? cfg.memoryMB : 512;
		SIZE_T memBytes = memMB * 1024ULL * 1024ULL;
		
		HANDLE hJob = CreateJobObjectW(nullptr, nullptr);
		if (!hJob) {
			if (cfg.verbose)
				Log(true, L"CreateJobObject 失败: ", GetLastError(), L"\n");
			return false;
		}
		if (!ConfigureJob(hJob, cpu, memBytes, cfg.verbose)) {
			CloseHandle(hJob);
			return false;
		}
		
		if (cfg.verbose) {
			Log(false, L"正在监控 \"", target, L"\"  [CPU ", cpu,
				L"%, 内存 ", memMB, L"MB]\n");
		}
		
		set<DWORD> limited;    // 已加入我们 Job 的 PID
		set<DWORD> throttled;  // 已在别的 Job 中、使用回退软限制的 PID
		set<DWORD> failed;     // 打不开 / 分配彻底失败的 PID
		
		while (!stopFlag || !stopFlag->load()) {
			auto pids = FindProcessesByName(target);
			set<DWORD> current(pids.begin(), pids.end());
			
			// 清理已退出的 PID
			for (auto it = limited.begin();   it != limited.end();   ) current.count(*it) ? ++it : (it = limited.erase(it));
			for (auto it = throttled.begin(); it != throttled.end(); ) current.count(*it) ? ++it : (it = throttled.erase(it));
			for (auto it = failed.begin();    it != failed.end();    ) current.count(*it) ? ++it : (it = failed.erase(it));
			
			for (DWORD pid : current) {
				// 已在我们 Job 中，无需处理
				if (limited.count(pid)) continue;
				
				// 已在回退模式：每次轮询修剪工作集
				if (throttled.count(pid)) {
					TrimWorkingSet(pid);
					continue;
				}
				
				// 彻底失败的进程不再重试
				if (failed.count(pid)) continue;
				
				// 首次遇到
				AssignResult r = TryAssign(hJob, pid, cfg.verbose);
				if (r == AssignResult::Success) {
					limited.insert(pid);
				} else if (r == AssignResult::AlreadyInJob) {
					if (ApplyFallbackLimits(pid, memBytes, cfg.verbose)) {
						throttled.insert(pid);
						if (cfg.verbose)
							Log(false, L"[*] PID ", pid,
								L" 已在别的 Job 中，已应用回退软限制 (IDLE 优先级 + 工作集修剪)\n");
					} else {
						failed.insert(pid);
					}
				} else {
					failed.insert(pid);
				}
			}
			Sleep(cfg.pollIntervalMs);
		}
		
		CloseHandle(hJob);
		if (cfg.verbose) Log(false, L"[*] 监控已停止，限制解除。\n");
		return true;
	}
	
	bool WatchAndLimit(const wstring& exeName,
					   DWORD cpuPercent,
					   SIZE_T memoryMB,
					   DWORD pollIntervalMs,
					   atomic<bool>* stopFlag,
					   bool verbose) {
		Config c;
		c.exeName        = exeName;
		c.cpuPercent     = cpuPercent;
		c.memoryMB       = memoryMB;
		c.pollIntervalMs = pollIntervalMs;
		c.verbose        = verbose;
		return WatchAndLimit(c, stopFlag);
	}
	
	bool WatchAndLimit(const string& exeName,
					   DWORD cpuPercent,
					   SIZE_T memoryMB,
					   DWORD pollIntervalMs,
					   atomic<bool>* stopFlag,
					   bool verbose) {
		return WatchAndLimit(ToWide(exeName), cpuPercent, memoryMB,
							 pollIntervalMs, stopFlag, verbose);
	}
	
} // namespace ProcessLimiter
