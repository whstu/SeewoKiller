#ifndef EXEWATCHER_H
#define EXEWATCHER_H

#include "./main.h"
#include <atomic>

using namespace std;

namespace ProcessLimiter {
	bool WatchAndLimit(const wstring& exeName,
					   DWORD cpuPercent = 50,
					   SIZE_T memoryMB  = 512,
					   DWORD pollIntervalMs = 500,
					   atomic<bool>* stopFlag = nullptr,
					   bool verbose = true);
	bool WatchAndLimit(const string& exeName,
					   DWORD cpuPercent = 50,
					   SIZE_T memoryMB  = 512,
					   DWORD pollIntervalMs = 500,
					   atomic<bool>* stopFlag = nullptr,
					   bool verbose = true);
}

#endif
