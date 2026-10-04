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
	
	// 宽字符串 -> GBK
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
	
	// 统一输出：转成 GBK 再走 std::cerr / std::cout
	inline void LogOut(const wstring& ws, bool toErr = false) {
		string s = ToGBK(ws);
		if (toErr) std::cerr << s;
		else       std::cout << s;
		std::cout.flush();
		std::cerr.flush();
	}
	
	// 便捷拼接
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
		// CPU 速率硬上限（Win8.1+）
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
		
		// 内存上限
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
	
	bool TryAssign(HANDLE hJob, DWORD pid, bool verbose) {
		HANDLE hProc = OpenProcess(
								   PROCESS_SET_QUOTA | PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION,
								   FALSE, pid);
		if (!hProc) {
			if (verbose)
				Log(true, L"[!] OpenProcess(PID ", pid, L") 失败 (错误 ",
					GetLastError(), L")\n");
			return false;
		}
		
		// 检查目标进程是否已在某个 Job 中
		BOOL bInJob = FALSE;
		if (IsProcessInJob(hProc, NULL, &bInJob) && bInJob) {
			if (verbose)
				Log(true, L"[!] PID ", pid,
					L" 已在某个 Job 中，无法加入新 Job（Windows 不允许嵌套 Job）\n");
			CloseHandle(hProc);
			return false;
		}
		
		BOOL ok = AssignProcessToJobObject(hJob, hProc);
		DWORD err = ok ? 0 : GetLastError();
		CloseHandle(hProc);
		if (ok) {
			if (verbose) Log(false, L"[+] 已限制 PID ", pid, L"\n");
			return true;
		}
		if (verbose)
			Log(true, L"[!] PID ", pid, L" 加入作业失败 (错误 ", err, L")\n");
		return false;
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
		
		set<DWORD> limited;
		set<DWORD> failed;
		while (!stopFlag || !stopFlag->load()) {
			auto pids = FindProcessesByName(target);
			set<DWORD> current(pids.begin(), pids.end());
			
			for (auto it = limited.begin(); it != limited.end(); ) {
				if (current.count(*it) == 0) it = limited.erase(it);
				else ++it;
			}
			for (auto it = failed.begin(); it != failed.end(); ) {
				if (current.count(*it) == 0) it = failed.erase(it);
				else ++it;
			}
			
			for (DWORD pid : current) {
				if (limited.count(pid) || failed.count(pid)) continue;
				if (TryAssign(hJob, pid, cfg.verbose)) {
					limited.insert(pid);
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
