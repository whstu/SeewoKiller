// process_limiter.h  —— 可直接 include 进大程序
// 依赖: -lkernel32  (MinGW-w64 自带)
// #pragma once
#include "./exeWatcher.h"
#include <tlhelp32.h>
#include <atomic>
#include <set>

namespace ProcessLimiter {

// ---------------- 内部工具 ----------------

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

	inline vector<DWORD> FindProcessesByName(const wstring& exeName) {
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

	inline bool ConfigureJob(HANDLE hJob, DWORD cpuPercent, SIZE_T memBytes, bool verbose) {
		// CPU 速率硬上限（Win8.1+）
		JOBOBJECT_CPU_RATE_CONTROL_INFORMATION cpuLimit{};
		cpuLimit.ControlFlags = JOB_OBJECT_CPU_RATE_CONTROL_ENABLE |
		                        JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP;
		cpuLimit.CpuRate = cpuPercent * 100;
		if (!SetInformationJobObject(hJob, JobObjectCpuRateControlInformation,
		                             &cpuLimit, sizeof(cpuLimit))) {
			if (verbose)
				wcerr << L"[!] CPU 限制设置失败 (错误 " << GetLastError()
				      << L"，需 Win8.1+)\n";
		}

		// 内存上限
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION memLimit{};
		memLimit.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_PROCESS_MEMORY;
		memLimit.ProcessMemoryLimit = memBytes;
		if (!SetInformationJobObject(hJob, JobObjectExtendedLimitInformation,
		                             &memLimit, sizeof(memLimit))) {
			if (verbose)
				wcerr << L"[!] 内存限制设置失败 (错误 " << GetLastError() << L")\n";
			return false;
		}
		return true;
	}

	inline bool TryAssign(HANDLE hJob, DWORD pid, bool verbose) {
		HANDLE hProc = OpenProcess(
		                   PROCESS_SET_QUOTA | PROCESS_TERMINATE | PROCESS_QUERY_LIMITED_INFORMATION,
		                   FALSE, pid);
		if (!hProc) {
			if (verbose)
				wcerr << L"[!] OpenProcess(PID " << pid << L") 失败 (错误 "
				      << GetLastError() << L")\n";
			return false;
		}
		BOOL ok = AssignProcessToJobObject(hJob, hProc);
		DWORD err = ok ? 0 : GetLastError();
		CloseHandle(hProc);
		if (ok) {
			if (verbose) wcout << L"[+] 已限制 PID " << pid << L"\n";
			return true;
		}
		if (verbose)
			wcerr << L"[!] PID " << pid << L" 加入作业失败 (错误 " << err << L")\n";
		return false;
	}

// ---------------- 公开接口 ----------------

	struct Config {
		wstring exeName;        // 目标 EXE 文件名（不含路径即可；含路径会自动取文件名）
		DWORD  cpuPercent   = 50;    // CPU 上限百分比 (1~100)
		SIZE_T memoryMB     = 512;   // 内存上限 (MB)
		DWORD  pollIntervalMs = 500; // 轮询间隔
		bool   verbose      = true;  // 是否输出日志
	};

// 阻塞式监控循环。
// stopFlag == nullptr  ->  无限运行（进程退出前不返回）
// stopFlag != nullptr  ->  当 *stopFlag == true 时返回
// 返回值: true 表示正常启动并退出；false 表示初始化失败
	inline bool WatchAndLimit(const Config& cfg,
	                          atomic<bool>* stopFlag = nullptr) {
		if (cfg.exeName.empty()) return false;

		// 若传的是完整路径，只取文件名
		wstring target = cfg.exeName;
		size_t slash = target.find_last_of(L"\\/");
		if (slash != wstring::npos) target = target.substr(slash + 1);

		DWORD cpu = (cfg.cpuPercent >= 1 && cfg.cpuPercent <= 100) ? cfg.cpuPercent : 50;
		SIZE_T memMB = cfg.memoryMB ? cfg.memoryMB : 512;
		SIZE_T memBytes = memMB * 1024ULL * 1024ULL;

		HANDLE hJob = CreateJobObjectW(nullptr, nullptr);
		if (!hJob) {
			if (cfg.verbose)
				wcerr << L"CreateJobObject 失败: " << GetLastError() << L"\n";
			return false;
		}
		if (!ConfigureJob(hJob, cpu, memBytes, cfg.verbose)) {
			CloseHandle(hJob);
			return false;
		}

		if (cfg.verbose) {
			wcout << L"正在监控 \"" << target << L"\"  [CPU " << cpu
			      << L"%, 内存 " << memMB << L"MB]\n";
		}

		set<DWORD> limited;
		while (!stopFlag || !stopFlag->load()) {
			auto pids = FindProcessesByName(target);
			set<DWORD> current(pids.begin(), pids.end());

			// 清理已退出的 PID，防止 PID 复用导致漏判
			for (auto it = limited.begin(); it != limited.end(); ) {
				if (current.count(*it) == 0) it = limited.erase(it);
				else ++it;
			}
			// 对新出现的进程施加限制
			for (DWORD pid : current) {
				if (limited.count(pid)) continue;
				TryAssign(hJob, pid, cfg.verbose);
				limited.insert(pid);  // 成功与否都记下，避免刷屏
			}
			Sleep(cfg.pollIntervalMs);
		}

		CloseHandle(hJob);
		if (cfg.verbose) wcout << L"[*] 监控已停止，限制解除。\n";
		return true;
	}

// --- 便捷重载：直接传参数 ---

	inline bool WatchAndLimit(const wstring& exeName,
	                          DWORD cpuPercent,
	                          SIZE_T memoryMB,
	                          DWORD pollIntervalMs,
	                          atomic<bool>* stopFlag,
	                          bool verbose ) {
		Config c;
		c.exeName        = exeName;
		c.cpuPercent     = cpuPercent;
		c.memoryMB       = memoryMB;
		c.pollIntervalMs = pollIntervalMs;
		c.verbose        = verbose;
		return WatchAndLimit(c, stopFlag);
	}

// 窄字符版本：用 string 时自动转宽字符
	inline bool WatchAndLimit(const string& exeName,
	                          DWORD cpuPercent,
	                          SIZE_T memoryMB,
	                          DWORD pollIntervalMs,
	                          atomic<bool>* stopFlag,
	                          bool verbose ) {
		return WatchAndLimit(ToWide(exeName), cpuPercent, memoryMB,
		                     pollIntervalMs, stopFlag, verbose);
	}

} // namespace ProcessLimiter
