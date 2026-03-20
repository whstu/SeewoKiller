/*
* CameraRec.cpp —— 纯控制台摄像头录制程序
*
* 编译方法（Dev-C++ / MinGW）：
*   1. 项目 → 项目属性 → 参数 → 链接器 中添加：
*          -lvfw32 -lwinmm -lole32
*   2. 确保链接器参数里【没有】 -mwindows，保留控制台窗口
*   3. 入口函数为 CamRecMain()，由外部 main.cpp 调用
*/

#include <windows.h>
#include <vfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <string>
#include "./CameraRec.h"
#include "./cmdCtrl.h"
#include "./main.h"

// -------------------------------------------------------
// 全局变量
// -------------------------------------------------------
static HWND   g_hWndCap     = NULL;
static HWND   g_hWndHost    = NULL;
static bool   g_bRecording  = false;
static char   g_szFileName[MAX_PATH] = {0};
static DWORD  g_dwStartTime = 0;
static HANDLE g_hPumpThread = NULL;

// -------------------------------------------------------
// 视频格式结构
// -------------------------------------------------------
struct VideoFormat {
	int   width;
	int   height;
	int   fps;
	int   bitDepth;
	DWORD biCompression; // BI_RGB=0 表示未压缩 RGB
};

// -------------------------------------------------------
// 隐藏宿主窗口过程
// -------------------------------------------------------
static LRESULT CALLBACK HostWndProc(HWND hWnd, UINT msg,
									WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY) { PostQuitMessage(0); return 0; }
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// -------------------------------------------------------
// 消息泵线程
// -------------------------------------------------------
static DWORD WINAPI MessagePumpThread(LPVOID /*param*/)
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	
	WNDCLASS wc      = {0};
	wc.lpfnWndProc   = HostWndProc;
	wc.hInstance     = hInst;
	wc.lpszClassName = "CamRecHostWnd";
	RegisterClass(&wc);
	
	g_hWndHost = CreateWindow("CamRecHostWnd", "",
							  WS_OVERLAPPED, 0, 0, 1, 1,
							  NULL, NULL, hInst, NULL);
	if (!g_hWndHost) return 1;
	
	g_hWndCap = capCreateCaptureWindow("CamCapture",
									   WS_CHILD,
									   0, 0, 320, 240,
									   g_hWndHost, 1);
	if (!g_hWndCap) { DestroyWindow(g_hWndHost); return 1; }
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

// -------------------------------------------------------
// 等待捕获窗口就绪
// -------------------------------------------------------
static bool WaitForCaptureWindow(int timeoutMs = 3000)
{
	int elapsed = 0;
	while (!g_hWndCap && elapsed < timeoutMs)
	{ Sleep(50); elapsed += 50; }
	return g_hWndCap != NULL;
}

// -------------------------------------------------------
// 停止录制（带守卫）
// -------------------------------------------------------
static void StopRecording()
{
	if (!g_hWndCap || !g_bRecording) return;
	capCaptureStop(g_hWndCap);
	g_bRecording = false;
}

// -------------------------------------------------------
// 刷新控制台状态行
// -------------------------------------------------------
static void PrintStatus()
{
	DWORD elapsed = (GetTickCount() - g_dwStartTime) / 1000;
	int m = (int)(elapsed / 60);
	int s = (int)(elapsed % 60);
	printf("\r  [ 录制中 ]  时长: %02d:%02d   按 S 停止...", m, s);
	fflush(stdout);
}

// -------------------------------------------------------
// 辅助：确保目录存在
// -------------------------------------------------------
static void EnsureDirectory(const char* path)
{
	if (!CreateDirectory(path, NULL))
	{
		// ERROR_ALREADY_EXISTS 是正常情况，不报错
		if (GetLastError() != ERROR_ALREADY_EXISTS)
			printf("  [!] 无法创建目录: %s\n", path);
	}
}

// -------------------------------------------------------
// 步骤①：设置保存路径
//   默认：<程序所在目录>\rec\<YYYYMMDD_HHMMSS>.avi
// -------------------------------------------------------
static void SetupSavePath()
{
	// 获取程序所在目录
	char szExePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szExePath, MAX_PATH);
	char* pSlash = strrchr(szExePath, '\\');
	if (pSlash) *(pSlash + 1) = '\0'; // 保留末尾反斜杠
	
	// 当前时间戳
	SYSTEMTIME st;
	GetLocalTime(&st);
	char szTimestamp[32];
	sprintf(szTimestamp, "%04d%02d%02d_%02d%02d%02d",
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond);
	
	char szDefault[MAX_PATH];
	sprintf(szDefault, "%srec\\%s.avi", szExePath, szTimestamp);
	
	printf("\n------------------------------------------------\n");
	printf("  [设置保存路径]\n");
	printf("  默认: %s\n", szDefault);
	printf("  直接回车使用默认 (.\\rec\\YYMMDD_hhmmss.avi)，或输入自定义完整路径 (.avi):\n");
	printf("  > ");
	fflush(stdout);
	
	char szInput[MAX_PATH] = {0};
	if (!fgets(szInput, MAX_PATH, stdin)) szInput[0] = '\0';
	
	// 去掉末尾换行/回车
	int len = (int)strlen(szInput);
	while (len > 0 && (szInput[len-1] == '\n' || szInput[len-1] == '\r'))
		szInput[--len] = '\0';
	
	if (len == 0)
	{
		// 使用默认，先确保 rec 目录存在
		char szRecDir[MAX_PATH];
		sprintf(szRecDir, "%srec", szExePath);
		EnsureDirectory(szRecDir);
		strcpy(g_szFileName, szDefault);
		printf("  → 使用默认路径。\n");
	}
	else
	{
		// 确保自定义路径的父目录存在
		char szParent[MAX_PATH];
		strcpy(szParent, szInput);
		char* p = strrchr(szParent, '\\');
		if (p) { *p = '\0'; EnsureDirectory(szParent); }
		strcpy(g_szFileName, szInput);
		printf("  → 保存路径: %s\n", g_szFileName);
	}
}

// -------------------------------------------------------
// 读取摄像头当前（默认）格式
// -------------------------------------------------------
static VideoFormat GetDefaultFormat()
{
	VideoFormat fmt = {640, 480, 30, 24, BI_RGB}; // 兜底值
	
	DWORD cbSize = capGetVideoFormatSize(g_hWndCap);
	if (cbSize > 0)
	{
		BITMAPINFO* pBmi = (BITMAPINFO*)malloc(cbSize);
		if (pBmi)
		{
			if (capGetVideoFormat(g_hWndCap, pBmi, cbSize))
			{
				fmt.width         = pBmi->bmiHeader.biWidth;
				fmt.height        = abs(pBmi->bmiHeader.biHeight);
				fmt.bitDepth      = pBmi->bmiHeader.biBitCount;
				fmt.biCompression = pBmi->bmiHeader.biCompression;
			}
			free(pBmi);
		}
	}
	
	CAPTUREPARMS cp = {0};
	capCaptureGetSetup(g_hWndCap, &cp, sizeof(cp));
	if (cp.dwRequestMicroSecPerFrame > 0)
		fmt.fps = (int)(1000000.0 / cp.dwRequestMicroSecPerFrame + 0.5);
	
	return fmt;
}

// -------------------------------------------------------
// 辅助：读取一个正整数，失败返回 fallback
// -------------------------------------------------------
static int ReadPositiveInt(const char* prompt, int fallback)
{
	printf("%s", prompt);
	int val = 0;
	if (scanf("%d", &val) != 1 || val <= 0) val = fallback;
	int c; while ((c = getchar()) != '\n' && c != EOF);
	return val;
}

// -------------------------------------------------------
// 步骤②：交互式设置视频格式
// -------------------------------------------------------
static VideoFormat SetupVideoFormat()
{
	VideoFormat def = GetDefaultFormat();
	
	printf("\n------------------------------------------------\n");
	printf("  [设置录制参数]\n");
	printf("  摄像头默认: %dx%d  %d fps  %d bit\n",
		   def.width, def.height, def.fps, def.bitDepth);
	SetColorAndBackground(0,7);
	printf("  [!]您可以修改录制参数，但是摄像头大概率会不支持。\n     如果您不知道您在做什么，请不要修改此选项。    \n");
	SetColorAndBackground(7,0);
	printf("  是否修改？(y/n): ");
	fflush(stdout);
	
	char ans = (char)_getch();
	printf("%c\n", ans);
	if (ans != 'y' && ans != 'Y')
	{
		printf("  → 使用默认参数。\n");
		return def;
	}
	
	VideoFormat fmt = def;
	
	// ---- 分辨率 ----
	printf("\n  [ 分辨率 ]\n");
	printf("  1. 1920 x 1080  (Full HD)\n");
	printf("  2. 1280 x 720   (HD 720p)\n");
	printf("  3.  854 x 480   (480p)\n");
	printf("  4.  640 x 480   (VGA)\n");
	printf("  5.  320 x 240   (QVGA)\n");
	printf("  6. 自定义\n");
	printf("  7. 保持默认 (%dx%d)\n", def.width, def.height);
	
	int pick = 0;
	do {
		printf("  选择 (1~7): ");
		if (scanf("%d", &pick) != 1) pick = 0;
		int c; while ((c = getchar()) != '\n' && c != EOF);
		if (pick < 1 || pick > 7) { printf("  无效，请重试。\n"); pick = 0; }
	} while (!pick);
	
	switch (pick) {
		case 1: fmt.width = 1920; fmt.height = 1080; break;
		case 2: fmt.width = 1280; fmt.height = 720;  break;
		case 3: fmt.width =  854; fmt.height = 480;  break;
		case 4: fmt.width =  640; fmt.height = 480;  break;
		case 5: fmt.width =  320; fmt.height = 240;  break;
	case 6:
		fmt.width  = ReadPositiveInt("  宽度 (像素): ", def.width);
		fmt.height = ReadPositiveInt("  高度 (像素): ", def.height);
		break;
		default: break; // 7: 保持 def
	}
	
	// ---- 帧率 ----
	printf("\n  [ 帧率 ]\n");
	printf("  1. 60 fps\n");
	printf("  2. 30 fps\n");
	printf("  3. 25 fps\n");
	printf("  4. 15 fps\n");
	printf("  5.  5 fps\n");
	printf("  6. 自定义\n");
	printf("  7. 保持默认 (%d fps)\n", def.fps);
	
	pick = 0;
	do {
		printf("  选择 (1~7): ");
		if (scanf("%d", &pick) != 1) pick = 0;
		int c; while ((c = getchar()) != '\n' && c != EOF);
		if (pick < 1 || pick > 7) { printf("  无效，请重试。\n"); pick = 0; }
	} while (!pick);
	
	switch (pick) {
		case 1: fmt.fps = 60; break;
		case 2: fmt.fps = 30; break;
		case 3: fmt.fps = 25; break;
		case 4: fmt.fps = 15; break;
		case 5: fmt.fps =  5; break;
		case 6: fmt.fps = ReadPositiveInt("  帧率 (fps): ", def.fps); break;
		default: break;
	}
	
	// ---- 色深 ----
	printf("\n  [ 色深 ]\n");
	printf("  1. 32 bit (RGBA)\n");
	printf("  2. 24 bit (RGB，推荐)\n");
	printf("  3. 16 bit\n");
	printf("  4. 保持默认 (%d bit)\n", def.bitDepth);
	
	pick = 0;
	do {
		printf("  选择 (1~4): ");
		if (scanf("%d", &pick) != 1) pick = 0;
		int c; while ((c = getchar()) != '\n' && c != EOF);
		if (pick < 1 || pick > 4) { printf("  无效，请重试。\n"); pick = 0; }
	} while (!pick);
	
	switch (pick) {
		case 1: fmt.bitDepth = 32; break;
		case 2: fmt.bitDepth = 24; break;
		case 3: fmt.bitDepth = 16; break;
		default: break;
	}
	
	printf("\n  → 最终参数: %dx%d  %d fps  %d bit\n",
		   fmt.width, fmt.height, fmt.fps, fmt.bitDepth);
	return fmt;
}

// -------------------------------------------------------
// 将 VideoFormat 写入捕获窗口
// -------------------------------------------------------
static void ApplyVideoFormat(const VideoFormat& fmt)
{
	// --- 视频格式 ---
	DWORD cbSize = capGetVideoFormatSize(g_hWndCap);
	if (cbSize == 0) cbSize = sizeof(BITMAPINFOHEADER);
	
	BITMAPINFO* pBmi = (BITMAPINFO*)calloc(1, cbSize);
	if (pBmi)
	{
		capGetVideoFormat(g_hWndCap, pBmi, cbSize); // 先读取现有值
		pBmi->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		pBmi->bmiHeader.biWidth       = fmt.width;
		pBmi->bmiHeader.biHeight      = fmt.height;
		pBmi->bmiHeader.biBitCount    = (WORD)fmt.bitDepth;
		pBmi->bmiHeader.biCompression = fmt.biCompression;
		pBmi->bmiHeader.biSizeImage   = 0; // 由驱动计算
		
		if (!capSetVideoFormat(g_hWndCap, pBmi, cbSize))
			printf("  [!] 视频格式设置失败，驱动可能不支持该参数组合，将使用驱动默认值。\n");
		
		free(pBmi);
	}
	
	// --- 帧率及捕获参数 ---
	CAPTUREPARMS cp = {0};
	capCaptureGetSetup(g_hWndCap, &cp, sizeof(cp));
	cp.dwRequestMicroSecPerFrame = (DWORD)(1000000.0 / fmt.fps + 0.5);
	cp.fAbortLeftMouse           = FALSE;
	cp.fAbortRightMouse          = FALSE;
	cp.fMakeUserHitOKToCapture   = FALSE;
	cp.fYield                    = TRUE;
	cp.wNumVideoRequested        = 60;
	capCaptureSetSetup(g_hWndCap, &cp, sizeof(cp));
}

// -------------------------------------------------------
// CamRecMain —— 对外入口
// -------------------------------------------------------
int CamRecMain(void)
{
	SetConsoleTitle("摄像头录制程序");
	
	printf("================================================\n");
	printf("        摄像头录制程序 (VFW / AVI)\n");
	printf("================================================\n\n");
	
	// --------------------------------------------------
	// 1. 枚举摄像头
	// --------------------------------------------------
	char szDeviceName[128], szDeviceVersion[128];
	std::vector<int> validDrivers;
	
	printf("[*] 正在枚举可用摄像头...\n");
	for (int i = 0; i < 10; i++)
	{
		if (capGetDriverDescription(i,
									szDeviceName,    sizeof(szDeviceName),
									szDeviceVersion, sizeof(szDeviceVersion)))
		{
			printf("    %d. %-30s  %s\n",
				   (int)validDrivers.size() + 1,
				   szDeviceName, szDeviceVersion);
			validDrivers.push_back(i);
		}
	}
	printf("\n");
	
	if (validDrivers.empty())
	{
		printf("[!] 未找到可用的摄像头设备。\n");
		printf("按任意键退出...\n");
		_getch(); return 1;
	}
	
	// --------------------------------------------------
	// 2. 选择摄像头
	// --------------------------------------------------
	int choice = 0;
	do {
		printf("请选择摄像头编号 (1~%d): ", (int)validDrivers.size());
		if (scanf("%d", &choice) != 1) choice = 0;
		int ch; while ((ch = getchar()) != '\n' && ch != EOF);
		if (choice < 1 || choice > (int)validDrivers.size())
		{ printf("    无效输入，请重试。\n"); choice = 0; }
	} while (!choice);
	
	int driverIndex = validDrivers[choice - 1];
	
	// --------------------------------------------------
	// 3. 启动消息泵线程
	// --------------------------------------------------
	printf("\n[*] 正在初始化捕获窗口...\n");
	g_hPumpThread = CreateThread(NULL, 0, MessagePumpThread, NULL, 0, NULL);
	if (!g_hPumpThread)
	{ printf("[!] 无法创建消息泵线程。\n"); _getch(); return 1; }
	
	if (!WaitForCaptureWindow())
	{ printf("[!] 捕获窗口初始化超时。\n"); _getch(); return 1; }
	
	// --------------------------------------------------
	// 4. 连接摄像头驱动
	// --------------------------------------------------
	if (!capDriverConnect(g_hWndCap, driverIndex))
	{
		printf("[!] 无法连接到摄像头驱动（索引 %d）。\n", driverIndex);
		PostMessage(g_hWndHost, WM_CLOSE, 0, 0);
		_getch(); return 1;
	}
	printf("[+] 摄像头已连接。\n");
	S(1000);
	// --------------------------------------------------
	// 5. 可选：选择视频压缩器
	// --------------------------------------------------
	cls
	printf("[+] 摄像头已连接。\n");
	SetColorAndBackground(0,7);
	printf(" 设置视频压缩器 ");
	SetColorAndBackground(7,0);
	printf("- 设置录制参数 - 设置保存路径 - 开始录制\n\n");

	SetColorAndBackground(0,7);
	printf("[!]在大多数希沃电脑上，压缩器似乎没什么用处。");
	SetColorAndBackground(7,0);
	printf("\n是否要设置视频压缩器？(y/n): ");
	char ans = (char)_getch();
	printf("%c\n", ans);
	if (ans == 'y' || ans == 'Y')
	{
		if (capDlgVideoCompression(g_hWndCap))
			printf("[+] 已选择视频压缩器。\n");
		else
			printf("[-] 未选择，将使用未压缩格式（文件可能较大）。\n");
	}

	system("pause");
	
	// --------------------------------------------------
	// 6. 设置录制参数（分辨率 / 帧率 / 色深）
	// --------------------------------------------------
	cls
	printf("[+] 摄像头已连接。\n");
	SetColorAndBackground(7,0);
	printf(" 设置视频压缩器 -");
	SetColorAndBackground(0,7);
	printf(" 设置录制参数 ");
	SetColorAndBackground(7,0);
	printf("- 设置保存路径 - 开始录制\n\n");

	VideoFormat fmt = SetupVideoFormat();
	ApplyVideoFormat(fmt);

	system("pause");
	// --------------------------------------------------
	// 7. 设置保存路径
	// --------------------------------------------------
	cls
	printf("[+] 摄像头已连接。\n");
	SetColorAndBackground(7,0);
	printf(" 设置视频压缩器 - 设置录制参数 -");
	SetColorAndBackground(0,7);
	printf(" 设置保存路径 ");
	SetColorAndBackground(7,0);
	printf("- 开始录制\n\n");

	SetupSavePath();  // 结果写入 g_szFileName
	
	if (!capFileSetCaptureFile(g_hWndCap, g_szFileName))
	{
		printf("[!] 设置录像文件失败: %s\n", g_szFileName);
		PostMessage(g_hWndHost, WM_CLOSE, 0, 0);
		_getch(); return 1;
	}
	printf("[+] 视频将保存到: %s\n", g_szFileName);

	system("pause");

	// --------------------------------------------------
	// 8. 开始录制
	// --------------------------------------------------
	cls
	printf("[+] 摄像头已连接。\n");
	SetColorAndBackground(7,0);
	printf(" 设置视频压缩器 - 设置录制参数 - 设置保存路径 -");
	SetColorAndBackground(0,10);
	printf(" 开始录制 \n\n");
	SetColorAndBackground(7,0);
	gotoxy(0,5);
	printf("3");
	S(1000);
	printf("\b2");
	S(1000);
	printf("\b1");
	S(1000);
	printf("\b开始！！！");
	S(1000);
	cls

	if (!capCaptureSequence(g_hWndCap))
	{
		printf("[!] 启动录制失败。\n");
		capDriverDisconnect(g_hWndCap);
		PostMessage(g_hWndHost, WM_CLOSE, 0, 0);
		_getch(); return 1;
	}
	
	g_bRecording  = true;
	g_dwStartTime = GetTickCount();
	
	printf("\n================================================\n");
	SetColorAndBackground(10,0);
	printf("  录制已开始！\n");
	SetColorAndBackground(7,0);
	printf("  在此控制台窗口按 [S] 键停止录制。\n");
	printf("================================================\n");
	
	// --------------------------------------------------
	// 9. 主循环：刷新时长 + 监听 'S' 键
	// --------------------------------------------------
	while (g_bRecording)
	{
		PrintStatus();
		if (_kbhit())
		{
			int key = _getch();
			if (key == 's' || key == 'S') StopRecording();
		}
		Sleep(500);
	}
	
	// --------------------------------------------------
	// 10. 收尾
	// --------------------------------------------------
	printf("\n\n[+] 录制结束。\n");
	printf("[+] 文件已保存: %s\n\n", g_szFileName);
	
	if (g_hWndCap)
	{
		capSetCallbackOnFrame(g_hWndCap,       NULL);
		capSetCallbackOnVideoStream(g_hWndCap, NULL);
		capSetCallbackOnError(g_hWndCap,       NULL);
		capSetCallbackOnStatus(g_hWndCap,      NULL);
		capDriverDisconnect(g_hWndCap);
	}
	if (g_hWndHost)
		PostMessage(g_hWndHost, WM_CLOSE, 0, 0);
	
	if (g_hPumpThread)
	{
		WaitForSingleObject(g_hPumpThread, 2000);
		CloseHandle(g_hPumpThread);
		g_hPumpThread = NULL;
	}
	
	printf("按任意键退出...\n");
	_getch();
	return 0;
}
