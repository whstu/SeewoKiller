#include <bits/stdc++.h>
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <string>
//任务栏进度条
#define INITGUID
#include <shobjidl.h>
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Ole32.lib")
const GUID IID_ITaskbarList3 = {// 手动定义IID_ITaskbarList3
	0xea1afb91, 0x9e28, 0x4b86,
	{0x90, 0xe9, 0x9e, 0x9f, 0x8a, 0x5e, 0xef, 0xaf}
};
//重启explorer.exe
#include <tlhelp32.h>
//注册表修改
#include <tchar.h>
//游戏
#define N 25
#define Forij(x) for(int i=1;i<=x;i++)for(int j=1;j<=x;j++)
//获取管理员权限所需
#include <tchar.h>
#include <shellapi.h>
//cin错误去除
#include <limits>
//文件控制
#include <fstream>

#define S(i) Sleep(i)
#define cls system("cls");
#define ei else if
using namespace std;

char path[MAX_PATH];
string executable_path;
size_t position;
string xwbbpath;
int dwMajorInt;
int dwMinorInt;
bool fastboot = false;

int box = 1/*板块*/, boxn = 4/*板块总数*/;
struct Word {
	string box[5] {"NULL", "常用", "核心功能", "附加功能", "设置"};
	int recentn = 4;
	string recent[5] = {"NULL", "一键解希沃锁屏", "晚自习制裁模式", "连点器(可防屏保)", "小游戏>>>"};
	int alln = 8;
	string all[9] = {"NULL", "循环清任务(上课防屏保)", "一键卸载", "晚自习制裁模式", "连点器(可防屏保)", "一键解希沃锁屏", "小游戏>>>", "恶搞>>>", "注册表>>>"};
	int moren = 3;
	string more[4] = {"NULL", "冰点还原破解", "AI", "计算π"};
	int settingn = 7;
	string setting[9] = {"NULL", "退出", "在晚自习制裁/循环清任务时启用日志", "打开日志文件夹", "启动初学者引导", "使用新版界面", "重启到fastboot(真的fast!)", "关于", "开发者选项>>>"};
	int gamen = 5;
	string game[6] = {"NULL", "返回", "数字炸弹", "五子棋", "飞机大战", "恶魔轮盘赌"};
	int joken = 3;
	string joke[4] = {"NULL", "返回", "杀WPS+希沃白板+希沃视频展台", "提取U盘文件"};
	int regn = 9;
	string reg[10] = {"NULL", "返回", "禁用任务栏菜单", "启用任务栏菜单", "禁用快捷键", "启用快捷键", "启用显示登录详细信息", "禁用显示登录详细信息", "登录时显示提示", "取消登录时显示提示"};
	int devn = 3;
	string dev[4] = {"NULL", "返回", "释放进度条COM接口", "关闭开发者模式"};
} word;

HWND hwnd = GetConsoleWindow();
void SetColorAndBackground(int ForgC, int BackC) {//单个字的颜色
//1深蓝，2深绿，3深青，4深红，5深紫，6深黄，7灰白（默认），8深灰
//9浅蓝，10浅绿，11浅青，12浅红，13浅紫，14浅黄，15白色，0黑色
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}
void gotoxy(long long x, long long y) {
	COORD pos;
	pos.X = 2 * x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setfont(int size) {//字体、大小、粗细
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = size;//设置字体大小
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_BOLD;//字体粗细 FW_BOLD,原始为FW_NORMAL
	wcscpy_s(cfi.FaceName, L"System");//设置字体，必须是控制台已有的
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFO consoleCurrentFont;
	GetCurrentConsoleFont(handle, FALSE, &consoleCurrentFont);
}

//任务栏进度条
//全局或类成员变量
ITaskbarList3* g_pTaskbar = nullptr;
void InitTaskbarInterface() {
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	CoCreateInstance(
	    CLSID_TaskbarList, nullptr, CLSCTX_ALL,
	    IID_ITaskbarList3, reinterpret_cast<void**>(&g_pTaskbar)
	);
	if (g_pTaskbar) g_pTaskbar->HrInit();
	return;
}
void ReleaseTaskbarInterface() {
	if (g_pTaskbar) g_pTaskbar->Release();
	CoUninitialize();
	return;
}
// 线程函数
void taskbarprocess(TBPFLAG state, int percent = -1) {
	/*
	TBPF_NOPROGRESS无进度条（默认状态）
	TBPF_INDETERMINATE不确定进度（持续动画）
	TBPF_NORMAL正常进度（绿色）
	TBPF_ERROR错误（红色）
	TBPF_PAUSED暂停（黄色）
	*/
	g_pTaskbar->SetProgressState(hwnd, state);
	if (state != TBPF_NOPROGRESS && state != TBPF_INDETERMINATE && (percent >= 0 || percent <= 100)) {
		g_pTaskbar->SetProgressValue(hwnd, percent, 100);
	}
	return;
}

//检查文件是否存在
bool fileExist(const string& filename) {
	ifstream file(filename);
	return file.good();
}

/*注册表*/
#define CHECK_ERROR(func) \
if (ERROR_SUCCESS != (func)) { \
std::cerr << "Error in " << __FUNCTION__ << " at line " << __LINE__ << " with error code " << GetLastError() << std::endl; \
return false; \
}
bool regedit(string root, string regpath, const char* valueName, string form, const char* Value) {
	HKEY hKey = NULL;
	LONG result;
	HKEY hRootKey;
	if (root == "HKEY_CLASSES_ROOT") {
		hRootKey = HKEY_CLASSES_ROOT;
	} else if (root == "HKEY_CURRENT_USER") {
		hRootKey = HKEY_CURRENT_USER;
	} else if (root == "HKEY_LOCAL_MACHINE") {
		hRootKey = HKEY_LOCAL_MACHINE;
	} else if (root == "HKEY_USERS") {
		hRootKey = HKEY_USERS;
	} else if (root == "HKEY_CURRENT_CONFIG") {
		hRootKey = HKEY_CURRENT_CONFIG;
	} else {
		cerr << "Invalid root key." << endl;
		return false;
	}
	result = RegOpenKeyEx(hRootKey, regpath.c_str(), 0, KEY_SET_VALUE, &hKey);
	CHECK_ERROR(result);
	if (form == "REG_SZ" || form == "REG_EXPAND_SZ") {
		result = RegSetValueEx(hKey, valueName, 0, (form == "REG_SZ") ? REG_SZ : REG_EXPAND_SZ, (const BYTE*)Value, strlen(Value) + 1);
	} else if (form == "REG_DWORD") {
		DWORD newValue = static_cast<DWORD>(strtol(Value, nullptr, 10));
		result = RegSetValueEx(hKey, valueName, 0, REG_DWORD, (const BYTE*)&newValue, sizeof(DWORD));
	} else if (form == "REG_BINARY") {
		cerr << "REG_BINARY not implemented correctly." << endl;
		RegCloseKey(hKey);
		return false;
	} else if (form == "REG_QWORD") {
		ULONGLONG newValue = _strtoui64(Value, nullptr, 10);
		result = RegSetValueEx(hKey, valueName, 0, REG_QWORD, (const BYTE*)&newValue, sizeof(ULONGLONG));
	} else if (form == "REG_MULTI_SZ") {
		cerr << "REG_MULTI_SZ not implemented correctly." << endl;
		RegCloseKey(hKey);
		return false;
	} else {
		cerr << "Invalid data type." << endl;
		RegCloseKey(hKey);
		return false;
	}
	CHECK_ERROR(result);
	RegCloseKey(hKey);
	return true;
}
/*重启explorer.exe*/
void restartexp() {
	system("TASKKILL /F /IM explorer.exe");
	cout << "杀进程成功，5秒后尝试重启\n";
	Sleep(5000);
	system("start C:\\Windows\\explorer.exe");
	cout << "恢复中\n";
	Sleep(2000);
	system("start C:\\Windows\\explorer.exe");
}
/*屏蔽关闭按钮*/
void connot_close_button() {
	HMENU hmenu = GetSystemMenu(hwnd, false);
	RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
	SetWindowLong(hwnd, GWL_STYLE,
	              GetWindowLong(hwnd, GWL_STYLE) & ~(WS_MINIMIZEBOX | WS_THICKFRAME));
	ShowWindow(hwnd, SW_MAXIMIZE);//最大化
	SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
	DrawMenuBar(hwnd);
	ReleaseDC(hwnd, NULL);
}

void quickstart() {
	int step = 1;
	cls
	while (step <= 6) {
		cls
		gotoxy(0, 6);
		cout << "\n\n\n这是初学者引导程序。按a返回，按d继续，按s跳过所有\n\n";
		cout << "第" << step << "步，共6步\n\n";
		switch (step) {
			case 1: {
				cout << "首先，请将你的实体/软键盘切换为英文输入法，并关闭大写锁定。\n";
				cout << "希沃克星（经典界面）运行时，请不要点击界面，否则界面将会被选中，希沃克星的进程将会停止。\n";
				break;
			}
			case 2: {
				cout << "按wasd控制上下左右";
				break;
			}
			case 3: {
				cout << "按空格键确定";
				break;
			}
			case 4: {
				cout << "带有\">>>\"的选项包含子项目，可以按空格键打开";
				break;
			}
			case 5: {
				cout << "技巧：通过Windows“任务视图”将希沃克星转移至另一个桌面以躲避大部分老师的检查\n";
				cout << "瓦特工具箱Watt Toolkit可以加速对Steam、Github的访问，网址https://steampp.net/";
				break;
			}
			case 6: {
				cout << "你已经完成了初学者引导程序。欢迎使用希沃克星！\n本引导程序将会保留在“设置”板块中";
				break;
			}
			default:
				return;
		}
		char ch = getch();
		for (;; ch = getch()) {
			if (ch == 'a') {
				if (step > 1) {
					step--;
					break;
				}
			} else if (ch == 'd') {
				step++;
				break;
			} else if (ch == 's') {
				if (MessageBox(hwnd, _T("你确实要跳过吗？\n本引导程序将会保留在设置板块中"), _T("鸡叫"), MB_YESNO) == IDYES) {
					cls
					return;
				}
			}
		}
	}
	cls
}
void poweron(bool SkipCheckWinVer, bool fb = false) {
	//1深蓝，2深绿，3深青，4深红，5深紫，6深黄，7灰白（默认），8深灰
	//9浅蓝，10浅绿，11浅青，12浅红，13浅紫，14浅黄，15白色，0黑色
	if (fb == true) {
		word.moren = 0;
		word.settingn = 1;
		word.gamen = 1;
		word.joken = 1;
		return;
	}
	setfont(30);
	connot_close_button();
	S(500);
	cout << "\n\n\n\n";
	S(10);
	cout << "\n";
//第2行
	for (int i = 0; i < 3; i++) {//W-1
		SetColorAndBackground(0, 0);
		cout << "   ";
		SetColorAndBackground(0, 4);
		cout << "  ";
	}
	SetColorAndBackground(0, 0);
	cout << "   ";
	SetColorAndBackground(0, 4);//H-1a
	cout << "  ";
	SetColorAndBackground(0, 0);//H-1mid
	cout << "       ";
	SetColorAndBackground(0, 4);//H-1b
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "      ";
	SetColorAndBackground(0, 4);//S-1
	cout << "         ";
	SetColorAndBackground(0, 0);
	cout << "    ";
	SetColorAndBackground(0, 4);//T-1
	cout << "            ";
	SetColorAndBackground(0, 0);
	cout << "    ";
	SetColorAndBackground(0, 4);//U-1a
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "       ";
	SetColorAndBackground(0, 4);//U-1b
	cout << "  ";
	SetColorAndBackground(0, 0);
	S(10);
	cout << "\n";
//第3,4行
	for (int a = 0; a < 2; a++) {
		for (int i = 0; i < 3; i++) {//W-2
			SetColorAndBackground(0, 0);
			cout << "   ";
			SetColorAndBackground(0, 6);
			cout << "  ";
		}
		SetColorAndBackground(0, 0);
		cout << "   ";
		SetColorAndBackground(0, 6);//H-2a
		cout << "  ";
		SetColorAndBackground(0, 0);//H-2mid
		cout << "       ";
		SetColorAndBackground(0, 6);//H-2b
		cout << "  ";
		SetColorAndBackground(0, 0);
		cout << "    ";
		SetColorAndBackground(0, 6); //S-2
		cout << "  ";
		SetColorAndBackground(0, 0);
		cout << "                  ";
		SetColorAndBackground(0, 6); //T-2
		cout << "  ";
		SetColorAndBackground(0, 0);
		cout << "         ";
		SetColorAndBackground(0, 6); //U-2a
		cout << "  ";
		SetColorAndBackground(0, 0);
		cout << "       ";
		SetColorAndBackground(0, 6); //U-2b
		cout << "  ";
		SetColorAndBackground(0, 0);
		S(10);
		cout << "\n";
	}
//第5行
	for (int i = 0; i < 3; i++) {//W-4
		SetColorAndBackground(0, 0);
		cout << "   ";
		SetColorAndBackground(0, 2);
		cout << "  ";
	}
	SetColorAndBackground(0, 0);
	cout << "   ";
	SetColorAndBackground(0, 2); //H-4
	cout << "           ";
	SetColorAndBackground(0, 0);
	cout << "      ";
	SetColorAndBackground(0, 2); //S-4
	cout << "       ";
	SetColorAndBackground(0, 0);
	cout << "           ";
	SetColorAndBackground(0, 2); //T-4
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "         ";
	SetColorAndBackground(0, 2); //U-4a
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "       ";
	SetColorAndBackground(0, 2); //U-4b
	cout << "  ";
	SetColorAndBackground(0, 0);
	S(10);
	cout << "\n";
//第1行
	for (int i = 0; i < 3; i++) {//W-5
		SetColorAndBackground(0, 0);
		cout << "   ";
		SetColorAndBackground(0, 1);
		cout << "  ";
	}
	SetColorAndBackground(0, 0);
	cout << "   ";
	SetColorAndBackground(0, 1);//H-5a
	cout << "  ";
	SetColorAndBackground(0, 0);//H-5mid
	cout << "       ";
	SetColorAndBackground(0, 1);//H-5b
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "             ";
	SetColorAndBackground(0, 1); //S-5
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "         ";
	SetColorAndBackground(0, 1); //T-5
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "         ";
	SetColorAndBackground(0, 1); //U-5a
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "       ";
	SetColorAndBackground(0, 1); //U-5b
	cout << "  ";
	SetColorAndBackground(0, 0);
	S(10);
	cout << "\n";
//第2行
	for (int i = 0; i < 3; i++) {//W-6
		SetColorAndBackground(0, 0);
		cout << "   ";
		SetColorAndBackground(0, 1);
		cout << "  ";
	}
	SetColorAndBackground(0, 0);
	cout << "   ";
	SetColorAndBackground(0, 1);//H-6a
	cout << "  ";
	SetColorAndBackground(0, 0);//H-6mid
	cout << "       ";
	SetColorAndBackground(0, 1);//H-6b
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "             ";
	SetColorAndBackground(0, 1); //S-6
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "         ";
	SetColorAndBackground(0, 1); //T-6
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "         ";
	SetColorAndBackground(0, 1); //U-6a
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "       ";
	SetColorAndBackground(0, 1); //U-6b
	cout << "  ";
	SetColorAndBackground(0, 0);
	S(10);
	cout << "\n";
//第3行
	SetColorAndBackground(0, 0);
	cout << "     ";
	SetColorAndBackground(0, 13);
	cout << "   ";
	SetColorAndBackground(0, 0);
	cout << "  ";
	SetColorAndBackground(0, 13);
	cout << "   ";
	SetColorAndBackground(0, 0);
	cout << "     ";
	SetColorAndBackground(0, 13);//H-7a
	cout << "  ";
	SetColorAndBackground(0, 0);//H-7mid
	cout << "       ";
	SetColorAndBackground(0, 13);//H-7b
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "    ";
	SetColorAndBackground(0, 13); //S-7
	cout << "         ";
	SetColorAndBackground(0, 0);
	cout << "           ";
	SetColorAndBackground(0, 13); //T-7
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "           ";
	SetColorAndBackground(0, 13); //U-7a
	cout << "       ";
	SetColorAndBackground(0, 0);
	S(10);
	cout << "\n\n";

	for (int i = 0; i < 36; i++) {
		cout << " ";
	}
	SetColorAndBackground(0, 7);
	cout << " Studio ";
	SetColorAndBackground(7, 0);
	gotoxy(0, 0);
	S(1000);
	//校验文件
	gotoxy(16, 14);
	cout << "正在校验配置文件(1/4)";
	gotoxy(15, 16);
	cout << "[=                   ]";
	taskbarprocess(TBPF_NORMAL, 5);
	S(500);
	//---
	CreateDirectory("./settings", NULL);
	if (fileExist(".\\settings\\write-log-when-killapp.seewokiller") == false) {
		ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
		file << "false";
		file.close();
		word.setting[2] = "在晚自习制裁/循环清任务时启用日志-当前:false";
	} else {
		ifstream file(".\\settings\\write-log-when-killapp.seewokiller");
		string value;
		getline(file, value);
		file.close();
		if (value == "true") {
			word.setting[2] = "在晚自习制裁/循环清任务时启用日志-当前:true";
		} else if (value == "false") {
			word.setting[2] = "在晚自习制裁/循环清任务时启用日志-当前:false";
		} else {
			ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
			file << "false";
			file.close();
			word.setting[2] = "在晚自习制裁/循环清任务时启用日志-当前:false";
		}
	}
	//-----
	gotoxy(16, 14);
	cout << "正在验证系统版本(2/4) ";
	gotoxy(15, 16);
	cout << "[=====               ]";
	taskbarprocess(TBPF_NORMAL, 25);
	S(200);
	//检测Windows版本
	typedef void(__stdcall * NTPROC)(DWORD*, DWORD*, DWORD*);
	HINSTANCE hinst = LoadLibrary(TEXT("ntdll.dll"));//加载DLL
	NTPROC GetNtVersionNumbers = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers");//获取函数地址
	DWORD dwMajor, dwMinor, dwBuildNumber;
	GetNtVersionNumbers(&dwMajor, &dwMinor, &dwBuildNumber);
	gotoxy(15, 14);
	printf("Windows版本: %d.%d", (int)dwMajor, (int)dwMinor);
	cout << "(3/4)      ";
	gotoxy(15, 16);
	cout << "[=======             ]";
	taskbarprocess(TBPF_NORMAL, 35);
	S(400);
	dwMajorInt = static_cast<int>(dwMajor);
	dwMinorInt = static_cast<int>(dwMinor);
	float version = dwMajorInt + dwMinorInt * 0.1;
	if (SkipCheckWinVer == false) {
		if (version >= 6.1) {
			taskbarprocess(TBPF_PAUSED, 35);
			if (MessageBox(hwnd, _T("检测到你的系统为Windows 7+，\n是否使用全新UI？"), _T("提示"), MB_OKCANCEL) == 1) {
				string guipath = executable_path + "\\gui.exe";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(guipath.c_str()));//有权限的都可以打开
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
				if (fSuccess) {
					gotoxy(15, 16);
					cout << "[=============       ]";
					taskbarprocess(TBPF_NORMAL, 65);
					S(10);
					gotoxy(15, 16);
					cout << "[====================]";
					taskbarprocess(TBPF_NORMAL, 100);
					S(200);
					exit(0);
				}
			}//返回1确定，2取消
		}
	}
	/*Windows 10-10.0
	Windows 8.1-6.3
	Windows 8-6.2
	Windows 7-6.1
	Windows Vista-6.0
	Windows XP 64位-5.2
	Windows XP-5.1
	Windows 2000-5.0
	https://learn.microsoft.com/zh-cn/windows/win32/sysinfo/operating-system-version
	*/
	gotoxy(15, 14);
	cout << "正在进行最后的准备(4/4)  ";
	gotoxy(15, 16);
	cout << "[===========         ]";
	taskbarprocess(TBPF_NORMAL, 55);
	S(400);
	if (fileExist(".\\settings\\already-quick-started.seewokiller") == false) {
		cls
		gotoxy(15, 14);
		cout << "你好";
		S(2000);
		gotoxy(15, 14);
		cout << "欢迎使用希沃克星";
		S(3000);
		quickstart();
		ofstream file(".\\settings\\already-quick-started.seewokiller");
		file.close();
	}
	gotoxy(15, 16);
	cout << "[===============     ]";
	taskbarprocess(TBPF_NORMAL, 75);
	S(10);
	gotoxy(15, 16);
	cout << "[====================]";
	taskbarprocess(TBPF_NORMAL, 100);
	S(100);
	setfont(30);
	taskbarprocess(TBPF_NOPROGRESS);
	return;
}

void about() {
	//初始化
	gotoxy(0, 3);
	SetColorAndBackground(7, 0);
	for (int i = 0; i < 15; i++) {
		cout << "                              \n";
	}
	gotoxy(0, 3);
	//--------
	cout << "\nPowered by\n\n ";
	S(500);
	SetColorAndBackground(4, 12);
	cout << " W ";
	SetColorAndBackground(6, 14);
	cout << " H ";
	SetColorAndBackground(2, 10);
	cout << " S ";
	SetColorAndBackground(1, 9);
	cout << " T ";
	SetColorAndBackground(13, 5);
	cout << " U ";
	SetColorAndBackground(0, 0);
	cout << "   ";
	SetColorAndBackground(15, 8);
	cout << " S t u d i o ";
	SetColorAndBackground(7, 0);
	S(500);
	cout << "\n";
	//写入版本号
	/*CreateDirectory("./info", NULL );
	ofstream info("./info/info.txt");
	info << "Seewo Killer 0.3.0.2" << endl;
	info << "版本代号000300002" << endl;
	info << "注意：请不要在此处留下重要信息，因为此文件会被SlytherinOS覆盖！";
	info.close();*/
	cout << "\nSeewo Killer 2.0 (Engorgio)\n";
	cout << "\n卓然第三帝国https://whstu.dpdns.org/提供技术支持";
	cout << "\n代码仓库：https://github.com/whstu/SeewoKiller/";
	cout << "\nSeewoKiller QQ 群：664929698";
	cout << "\n经典界面UI基于SlytherinOS框架\n";
	SetColorAndBackground(10, 0);
	cout << "      Slytherin ";
	SetColorAndBackground(0, 2);
	cout << "O";
	SetColorAndBackground(7, 0);
	cout << "S";
	cout << "\n新版界面基于PyQt5\n";
	cout << "\n技巧：通过Windows“任务视图”将希沃克星转移至另一个桌面以躲避大部分老师的检查\n";
	cout << "瓦特工具箱Watt Toolkit可以加速对Steam、Github的访问，网址https://steampp.net/\n";
	cout << "\n";
	cout << "“Slytherin(TM)”是J.K.Rowling的注册商标，版权归WizardingWorld(R)所有\n";
	cout << "\n按b回车即可返回\n";
	string ans;
	while (true) {
		cin >> ans;
		if (ans == "b") {
			return;
		}
		if (ans == "dev") {
			word.settingn = 8;
			cout << "开发者模式 已开启。\n按b后回车即可返回。\n";
		}
	}
}

BOOL IsUserAnAdmin() {
	BOOL bResult = FALSE;
	SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;
	PSID pSid = NULL;
	if (AllocateAndInitializeSid(&sia, 2,
	                             SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
	                             0, 0, 0, 0, 0, 0, &pSid)) {
		// 检查当前线程或进程的访问令牌是否包含该SID
		if (!CheckTokenMembership(NULL, pSid, &bResult)) {
			// 如果CheckTokenMembership失败，则可能不是管理员，但也可能因为其他原因
			bResult = FALSE;
		}
		// 释放SID
		FreeSid(pSid);
	} else {
		// 如果SID分配失败，则默认不是管理员
		bResult = FALSE;
	}
	return bResult;
}
bool getadmin() {
	// 获取当前程序的完整路径
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);

	// 检查当前用户是否是管理员
	if (!IsUserAnAdmin()) {
		// 如果不是管理员，则以管理员权限运行当前程序
		SHELLEXECUTEINFO sei = {0};
		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		sei.lpFile = szPath;
		sei.nShow = SW_SHOWNORMAL;
		sei.lpVerb = _T("runas"); // 指定操作为以管理员身份运行

		ShellExecuteEx(&sei);
		return false;
	} else {
		// 如果已经是管理员，则正常继续
		cout << "已获得管理员权限\n";
		return true;
	}
}

void taskkill(bool KillSeewoService, bool Wanzixi) {
	ifstream file(".\\settings\\write-log-when-killapp.seewokiller");
	string value;
	getline(file, value);
	bool log = false;
	if (value == "true") {
		CreateDirectory("./log", NULL);
		log = true;
	}
	long long n = 1;
	while (true) {
		if (log == true) {
			time_t now = time(nullptr);
			tm* localTime = localtime(&now);
			string filename = ".\\log\\log-" + to_string(localTime->tm_year + 1900) + "-" + to_string(localTime->tm_mon + 1) + "-" + to_string(localTime->tm_mday) + ".log";
			ofstream file(filename.c_str(), ios::app);
			now = time(nullptr);
			localTime = localtime(&now);
			file << localTime->tm_hour << ":"
			     << (localTime->tm_sec < 10 ? "0" : "") << localTime->tm_min << ":"
			     << (localTime->tm_sec < 10 ? "0" : "") << localTime->tm_sec
			     << "第" << n << "次杀进程" << endl;
			file.close();
			n++;
		}
		thread a1(system, "TASKKILL /F /IM EasiRecorder.exe");
		a1.detach();
		//cout << "正在结束进程：轻录播\n";
		//cout << "TASKKILL /F /IM EasiRecorder.exe\n";
		system("TASKKILL /F /IM EasiRecorder.exe");
		if (KillSeewoService == true) {
			cout << "正在结束进程：希沃管家\n";
			thread b1(system, "TASKKILL /F /IM SeewoServiceAssistant.exe");
			thread b2(system, "TASKKILL /F /IM SeewoAbility.exe");
			thread b3(system, "TASKKILL /F /IM SeewoCore.exe");
			b1.detach();
			b2.detach();
			b3.join();
			//cout << "TASKKILL /F /IM SeewoServiceAssistant.exe\n";
			//system("TASKKILL /F /IM SeewoServiceAssistant.exe");
			//cout << "TASKKILL /F /IM SeewoAbility.exe\n";
			//system("TASKKILL /F /IM SeewoAbility.exe");
			//cout << "TASKKILL /F /IM SeewoCore.exe\n";
			//system("TASKKILL /F /IM SeewoCore.exe");
		}
		if (Wanzixi == true) {
			thread c1(system, "taskkill /f /t /im taskmgr.exe");
			thread c2(system, "TASKKILL /F /IM SystemSettings.exe");
			thread c3(system, "taskkill /f /fi \"WINDOWTITLE eq 网络连接\"");
			thread c4(system, "taskkill /f /fi \"WINDOWTITLE eq 控制面板\\网络和 Internet\\网络连接\"");
			thread c5(system, "TASKKILL /F /IM msedge.exe");
			thread c6(system, "TASKKILL /F /IM iexplore.exe");
			c1.detach();
			c2.detach();
			c3.join();
			c4.detach();
			c5.detach();
			c6.join();
			//system("taskkill /f /t /im taskmgr.exe");
			//cout << "正在结束进程：设置\n";
			//cout << "TASKKILL /F /IM SystemSettings.exe\n";
			//system("TASKKILL /F /IM SystemSettings.exe");
			//cout << "正在结束进程：控制面板\n";
			//cout << "TASKKILL /F /FI \"WINDOWTITLE eq 网络连接\"\n";
			//system("taskkill /f /fi \"WINDOWTITLE eq 网络连接\"");
			//system("taskkill /f /fi \"WINDOWTITLE eq 控制面板\\网络和 Internet\\网络连接\"");
			//cout << "正在结束进程：Edge\n";
			//cout << "TASKKILL /F /IM msedge.exe\n";
			//system("TASKKILL /F /IM msedge.exe");
			//cout << "正在结束进程：IE\n";
			//cout << "TASKKILL /F /IM iexplore.exe\n";
			//system("TASKKILL /F /IM iexplore.exe");
		}
	}
}

void uninstall() {
	cls
	cout << "正在卸载轻录播\n";
	system("\"C:\\Program Files (x86)\\Seewo\\EasiRecorder\\Uninstall.exe\"");
	cout << "正在卸载Easicare\n";
	system("\"C:\\Program Files (x86)\\Seewo\\Easicare\\Uninstall.exe\"");
	cout << "正在卸载EasiAgent\n";
	system("\"C:\\Program Files (x86)\\Seewo\\EasiAgent\\Uninstall.exe\"");
	cout << "正在卸载希沃智能笔助手\n";
	system("\"C:\\Program Files (x86)\\Seewo\\SmartpenService\\Uninstall.exe\"");
	return;
}

void liandianqi() {
	cls
	int gap;
	int x, y;
	SetColorAndBackground(4, 6);
	cout << "警告：请勿用于正常上课！";
	SetColorAndBackground(0, 7);
	cout << "\n\n点击间隔：单位为毫秒，不支持小数，可输入0";
	cout << "\n请输入点击间隔:";
	cin >> gap;
	while (cin.fail() or gap < 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "输入错误，请重试:";
		cin >> gap;
	}
	cout << "\n\n点击坐标：点击屏幕的坐标，输入114514为默认（跟随鼠标位置移动）\n";
	cout << "请输入坐标x(横):";
	cin >> x;
	while (cin.fail() or x < 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "输入错误，请重试:";
		cin >> x;
	}
	cout << "\n请输入坐标y(纵):";
	cin >> y;
	while (cin.fail() or y < 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "输入错误，请重试：";
		cin >> y;
	}
	cout << "每" << gap << "秒点击屏幕一次，请将鼠标移动至合适位置\n";
	system("pause");
	long long i = 1;
	while (true) {
		cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << i;
		POINT cur_pos;
		GetCursorPos(&cur_pos);
		if (x != 114514) {
			cur_pos.x = x;
		}
		if (y != 114514) {
			cur_pos.y = y;
		}
		mouse_event(MOUSEEVENTF_LEFTDOWN, cur_pos.x, cur_pos.y, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, cur_pos.x, cur_pos.y, 0, 0);
		i++;
		S(gap);
	}
}

typedef long long LL;
namespace GAME {
	void numberdamn() {
		cls
		setfont(300);
		cout << "数";
		S(100);
		cout << "字";
		S(100);
		cout << "炸";
		S(150);
		gotoxy(0, 0);
		cls
		S(100);
		setfont(500);
		SetColorAndBackground(6, 4);
		cout << "DAMN";
		SetColorAndBackground(7, 0);
		S(700);
		cls
		setfont(30);
		system("title 数字炸damn");
		long long min = 0, max = 100, ans = 0, input = 0;
		cout << "请输入范围\n";
		while (true) {
			cout << "最大:";
			cin >> max;
			if (max < 2 or cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "最大值必须大于或等于2,或输入错误\n";
			} else {
				break;
			}
		}
		while (true) {
			cout << "最小:";
			cin >> min;
			if (min < 0 or min >= max - 1 or cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "最小值必须大于或等于0,且最小值必须小于最大值-1,或输入错误\n";
			} else {
				break;
			}
		}
		cout << "输入完成，正在取数...\n";
		while (ans == 0) {
			ans = rand() % (max - min);
		}
		ans = ans + min;
		S(500);
		cout << "取数完成，开始游戏！\n";
		S(500);
		cls
		while (true) {
			if (max - min == 1) {
				cout << "程序错误，请重试\n";
				cout << min << "~" << max << "\n";
				break;
			}
			if (max - min == 2) {
				SetColorAndBackground(6, 4);
				cout << "DAMN!!!!!";
				SetColorAndBackground(0, 7);
				cout << "\n下一个人输了！\n炸弹是:" << ans << "\n";
				break;
			}
			cout << min << "~" << max << "\n";
			if (max - min == 3) {
				SetColorAndBackground(6, 4);
				cout << "二选一!!!!!";
				SetColorAndBackground(7, 0);
			}
			cin >> input;
			while (input <= min or input >= max or cin.fail()/*cin数字错误*/) {
				cout << "输入错误\n";
				cin.clear();//清除错误数据
				cin.ignore(numeric_limits<streamsize>::max(), '\n');//丢弃错误输入
				cout << min << "~" << max << "\n";
				cin >> input;
			}
			if (input == ans) {
				SetColorAndBackground(6, 4);
				cout << "DAMN!!!!!";
				SetColorAndBackground(0, 7);
				cout << "\n你输了！\n炸弹是:" << ans << "\n";
				if (max - ans == ans - min) {
					cout << "中位数小丑！";
				}
				break;
			} else {
				if (input < ans) {
					min = input;
				} else {
					max = input;
				}
			}
		}
		system("pause");
		SetColorAndBackground(7, 0);
		setfont(30);
	}
	struct WZQ {
		LL Q, GG;
		void color(LL a) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
		}
		struct Gomoku {
			void color(LL a) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
			}
			LL fx[4][2] = {{1, 1}, {1, 0}, {0, 1}, {1, -1}};
			string C[20] = {"●", "○", "+", "=", "|", "|", "|", "|", "|", "?"};
			LL m[50][50], nx, ny;
			void reset() {
				system("cls");
				memset(m, -1, sizeof(m));
				color(7);
				for (LL i = 1; i <= N; i++) {
					gotoxy(0, i);
					cout << C[4];
					gotoxy(N + 1, i);
					cout << C[4];
					gotoxy(i, 0);
					cout << C[3];
					gotoxy(i, N + 1);
					cout << C[3];
				}
				gotoxy(0, 0);
				cout << C[5];
				gotoxy(0, N + 1);
				cout << C[6];
				gotoxy(N + 1, 0);
				cout << C[7];
				gotoxy(N + 1, N + 1);
				cout << C[8];
				color(3);
				Forij(N) {
					gotoxy(i, j);
					cout << C[2];
				}
				nx = ny = N / 2 + 1;
				gotoxy(nx, ny);
			}
			void _drop(LL x, LL i, LL j) {
				m[i][j] = x;
				gotoxy(i, j);
				color(15);
				cout << C[x];
			}
			LL check() {
				Forij(N) {
					for (LL Fx = 0, tmp, lst, xx, yy; Fx < 4; Fx++) if (m[i][j] != -1) {
							xx = i, yy = j, tmp = 0, lst = m[i][j];
							for (LL k = 1; k <= 5; k++) {
								if (xx > N || yy > N) break;
								if (m[xx][yy] == (lst ^ 1)) {
									break;
								}
								if (m[xx][yy] == lst) tmp++;
								xx += fx[Fx][0], yy += fx[Fx][1];
							}
							if (tmp == 5) {
								return lst;
							}
						}
				}
				return -1;
			}
			LL arnd(LL x, LL y) {
				LL cnt = 0;
				for (LL i = x - 1; i <= x + 1; i++) if (i > 0 && i <= N)
						for (LL j = y - 1; j <= y + 1; j++) if (j > 0 && j <= N)
								if (m[i][j] > -1) cnt++;
				return cnt;
			}
			void get_val(LL x, LL y, LL &val) {
				val = 0;
				Forij(N) {
					for (LL Fx = 0, tmp, tk, xx, yy; Fx < 4; Fx++) {
						xx = i, yy = j, tmp = tk = 0;
						for (LL k = 1; k <= 5; k++) {
							if (xx > N || yy > N) {
								tmp = 0;
								break;
							}
							if (m[xx][yy] == (x ^ 1)) {
								tmp = 0;
								break;
							}
							if (m[xx][yy] == x) tmp++, tk += (1 << (k - 1));
							xx += fx[Fx][0], yy += fx[Fx][1];
						}
						switch (tmp) {
							case 5:
								val += 8000000000;
								break;
							case 4:
								val += 1000 + 350 * y;
								break;
							case 3:
								val += (tk == 14) ? (300 + 600 * y) : (300 + 200 * y);
								break;
							case 2:
								val += 3 + 2 * y;
								break;
							case 1:
								val += 1 + y;
								break;
						}
					}
				}
			}
			void AI(LL x) {
				LL best, brnd, bi, bj, v1, v2, kkk;
				best = -2147483647;
				brnd = -2147483647;
				Forij(N) if (m[i][j] == -1) {
					m[i][j] = x;
					get_val(x, 10, v1); //gotoxy(N+5,N/2);printf("%d                ",v1);
					get_val(x ^ 1, 80, v2); //gotoxy(N+5,N/2+1);printf("%d             ",v2);
					if (v1 - v2 > best) bi = i, bj = j, best = v1 - v2;
					if (v1 - v2 == best)
						if ((kkk = arnd(i, j)) > brnd)
							brnd = kkk, bi = i, bj = j;
					m[i][j] = -1;
				}
				_drop(x, bi, bj);
			}
			void HM(LL x) {
				char ch = getch();
				for (;; ch = getch()) {
					if (ch == 'w') {
						if (ny > 1) ny--;
					} else if (ch == 's') {
						if (ny < N) ny++;
					} else if (ch == 'a') {
						if (nx > 1) nx--;
					} else if (ch == 'd') {
						if (nx < N)nx++;
					} else if (m[nx][ny] == -1) {
						_drop(x, nx, ny);
						return;
					}
					gotoxy(nx, ny);
				}
			}
		} A;
		void HMAI() {
			A.reset();
			for (GG = -1;;) {
				gotoxy(A.nx, A.ny);
				A.HM(1);
				GG = A.check();
				if (GG > -1) break;
				A.AI(0);
				GG = A.check();
				if (GG > -1) break;
			}
			gotoxy(5, N + 3);
			if (GG == 0) printf("White wins!");
			if (GG == 1) printf("Black wins!");
			gotoxy(5, N + 4);
			printf("按任意键继续");
			getch();
			return;
		}
		void HMHM() {
			A.reset();
			for (GG = -1;;) {
				gotoxy(A.nx, A.ny);
				A.HM(1);
				GG = A.check();
				if (GG > -1) break;
				A.HM(0);
				GG = A.check();
				if (GG > -1) break;
			}
			gotoxy(5, N + 3);
			if (GG == 0) printf("White wins!");
			if (GG == 1) printf("Black wins!");
			gotoxy(5, N + 4);
			printf("按任意键继续");
			getch();
			return;
		}
		void AIAI() {
			A.reset();
			gotoxy(A.nx, A.ny);
			A.HM(1);
			A.HM(0);
			for (GG = -1;;) {
				gotoxy(A.nx, A.ny);
				A.AI(1);
				GG = A.check();
				if (GG > -1) break;
				A.AI(0);
				GG = A.check();
				if (GG > -1) break;
				Sleep(100);
			}
			gotoxy(5, N + 3);
			if (GG == 0) printf("White wins!");
			if (GG == 1) printf("Black wins!");
			gotoxy(5, N + 4);
			printf("按任意键继续");
			getch();
			return;
		}
		void wzqmain() {
			cout << "按1与AI对战  按2双人对战  按3观看AI对战\n";
			char ch = getch();
			for (;; ch = getch()) {
				if (ch == '1') {
					cout << "你选择了与AI决斗！！！\n";
					Sleep(1000);
					system("title 棋盘:人机对战");
					HMAI();
					cls
					return;
				} else if (ch == '2') {
					cout << "你选择了双人对战。\n";
					Sleep(1000);
					system("title 棋盘:人vs人模式");
					HMHM();
					cls
					return;
				} else if (ch == '3') {
					cout << "你选择了观看两个AI对战！！！\n";
					system("title 棋盘:AIvsAI");
					cout << "你需要先在棋盘上随意下2个棋子\n确定？";
					system("pause");
					AIAI();
					cls
					return;
				}
			}
			return;
		}
	} wzq;
	namespace feijidazhan {
		bool end = false;
		typedef struct Frame {
			COORD position[2];
			int flag;
		} Frame;
		void SetPos(COORD a) {
			HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(out, a);
		}
		void SetPos(int i, int j) {
			COORD pos = {static_cast<SHORT>(i), static_cast<SHORT>(j)};
			SetPos(pos);
		}
		void HideCursor() {
			CONSOLE_CURSOR_INFO cursor_info = {1, 0};
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		}

		void drawRow(int y, int x1, int x2, char ch) {
			SetPos(x1, y);
			for (int i = 0; i <= (x2 - x1); i++)
				cout << ch;
		}

		void drawRow(COORD a, COORD b, char ch) {
			if (a.Y == b.Y)
				drawRow(a.Y, a.X, b.X, ch);
			else {
				SetPos(0, 25);
				cout << "error code 01：无法填充行，因为两个坐标的纵坐标(x)不相等";
				system("pause");
			}
		}

		void drawCol(int x, int y1, int y2, char ch) {
			int y = y1;
			while (y != y2 + 1) {
				SetPos(x, y);
				cout << ch;
				y++;
			}
		}

		void drawCol(COORD a, COORD b, char ch) {
			if (a.X == b.X)
				drawCol(a.X, a.Y, b.Y, ch);
			else {
				SetPos(0, 25);
				cout << "error code 02：无法填充列，因为两个坐标的横坐标(y)不相等";
				system("pause");
			}
		}

		void drawFrame(COORD a, COORD b, char row, char col) {
			drawRow(a.Y, a.X + 1, b.X - 1, row);
			drawRow(b.Y, a.X + 1, b.X - 1, row);
			drawCol(a.X, a.Y + 1, b.Y - 1, col);
			drawCol(b.X, a.Y + 1, b.Y - 1, col);
		}
		void drawFrame(int x1, int y1, int x2, int y2, char row, char col) {
			COORD a = {static_cast<SHORT>(x1), static_cast<SHORT>(y1)};
			COORD b = {static_cast<SHORT>(x2), static_cast<SHORT>(y2)};
			drawFrame(a, b, row, col);
		}
		void drawFrame(Frame frame, char row, char col) {
			COORD a = frame.position[0];
			COORD b = frame.position[1];
			drawFrame(a, b, row, col);
		}
		void drawPlaying() {
			drawFrame(0, 0, 48, 24, '=', '|');
			drawFrame(49, 0, 79, 4, '-', '|');
			drawFrame(49, 4, 79, 9, '-', '|');
			drawFrame(49, 9, 79, 20, '-', '|');
			drawFrame(49, 20, 79, 24, '-', '|');
			SetPos(52, 6);
			cout << "得分：";
			SetPos(52, 7);
			cout << "称号：";
			SetPos(52, 10);
			cout << "操作方式：";
			SetPos(52, 12);
			cout << " a,s,d,w 控制战机移动。";
			SetPos(52, 14);
			cout << " p 暂停游戏。";
			SetPos(52, 16);
			cout << " e 退出游戏。";
			SetPos(52, 18);
			cout << " 空格 发射子弹。";
		}

		int random(int a, int b) {
			int c = (rand() % (a - b)) + a;
			return c;
		}

		COORD random(COORD a, COORD b) {
			int x = random(a.X, b.X);
			int y = random(a.Y, b.Y);
			COORD c = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
			return c;
		}
		bool judgeCoordInFrame(Frame frame, COORD spot) {
			if (spot.X >= frame.position[0].X)
				if (spot.X <= frame.position[1].X)
					if (spot.Y >= frame.position[0].Y)
						if (spot.Y <= frame.position[0].Y)
							return true;
			return false;
		}
		void printCoord(COORD a) {
			cout    << "( " << a.X << " , " << a.Y << " )";
		}
		void printFrameCoord(Frame a) {
			printCoord(a.position[0]);
			cout    << " - ";
			printCoord(a.position[1]);
		}
		int drawMenu() {
			SetPos(30, 1);
			cout << "飞 机 大 战";
			drawRow(3, 0, 79, '-');
			drawRow(5, 0, 79, '-');
			SetPos(28, 4);
			cout << "w 和 s 选择， 空格 确定";
			SetPos(15, 11);
			cout << "1. 简单的敌人";
			SetPos(15, 13);
			cout << "2. 冷酷的敌人";
			drawRow(20, 0, 79, '-');
			drawRow(22, 0, 79, '-');
			SetPos(47, 11);
			cout << "简单的敌人：";
			SetPos(51, 13);
			cout << "简单敌人有着较慢的移动速度。";
			SetPos(24, 21);
			cout << " ☆○☆○☆○☆○☆○☆ ";
			int j = 11;
			SetPos(12, j);
			cout << ">>";
			while (1) {
				if ( _kbhit() ) {
					char x = _getch();
					switch (x) {
						case 'w' : {
							if ( j == 13) {
								SetPos(12, j);
								cout << "　";
								j = 11;
								SetPos(12, j);
								cout << ">>";
								SetPos(51, 13);
								cout << "　　　　　　　　　　　　";
								SetPos(47, 11);
								cout << "简单的敌人：";
								SetPos(51, 13);
								cout << "简单敌人有着较慢的移动速度。";
							}
							break;
						}
						case 's' : {
							if ( j == 11 ) {
								SetPos(12, j);
								cout << "　";
								j = 13;
								SetPos(12, j);
								cout << ">>";
								SetPos(51, 13);
								cout << "　　　　　　　　　　　　　　";
								SetPos(47, 11);
								cout << "冷酷的敌人：";
								SetPos(51, 13);
								cout << "冷酷的敌人移动速度较快。";
							}
							break;
						}
						case ' ' : {
							if (j == 8)    return 1;
							else return 2;
						}
					}
				}
			}
		}
		class Game {
		public:
			COORD position[10] = {};
			COORD bullet[10] = {};
			Frame enemy[8] = {};
			int score = 0;
			int rank = 0;
			int rankf = 0;
			string title = "";
			int flag_rank = 0;
			Game ();

			void initPlane();
			void initBullet();
			void initEnemy();

			void planeMove(char);
			void bulletMove();
			void enemyMove();

			void drawPlane();
			void drawPlaneToNull();
			void drawBullet();
			void drawBulletToNull();
			void drawEnemy();
			void drawEnemyToNull();

			void drawThisBulletToNull( COORD );
			void drawThisEnemyToNull( Frame );
			void Pause();
			void Playing();
			void judgePlane();
			void judgeEnemy();
			void Shoot();
			void GameOver();
			void printScore();
		};
		Game::Game() {
			initPlane();
			initBullet();
			initEnemy();
			score = 0;
			rank = 25;
			rankf = 0;
			flag_rank = 0;
		}
		void Game::initPlane() {
			COORD centren = {39, 22};
			position[0].X = position[5].X = position[7].X = position[9].X = centren.X;
			position[1].X = centren.X - 2;
			position[2].X = position[6].X = centren.X - 1;
			position[3].X = position[8].X = centren.X + 1;
			position[4].X = centren.X + 2;
			for (int i = 0; i <= 4; i++)
				position[i].Y = centren.Y;
			for (int i = 6; i <= 8; i++)
				position[i].Y = centren.Y + 1;
			position[5].Y = centren.Y - 1;
			position[9].Y = centren.Y - 2;
		}
		void Game::drawPlane() {
			for (int i = 0; i < 9; i++) {
				SetPos(position[i]);
				if (i != 5)
					cout << "o";
				else if (i == 5)
					cout << "|";
			}
		}
		void Game::drawPlaneToNull() {
			for (int i = 0; i < 9; i++) {
				SetPos(position[i]);
				cout << " ";
			}
		}
		void Game::initBullet() {
			for (int i = 0; i < 10; i++)
				bullet[i].Y = 30;
		}
		void Game::drawBullet() {
			for (int i = 0; i < 10; i++) {
				if ( bullet[i].Y != 30) {
					SetPos(bullet[i]);
					cout << "^";
				}
			}
		}
		void Game::drawBulletToNull() {
			for (int i = 0; i < 10; i++)
				if ( bullet[i].Y != 30 ) {
					COORD pos = {static_cast<SHORT>(bullet[i].X), static_cast<SHORT>(bullet[i].Y + 1)};
					SetPos(pos);
					cout << " ";
				}
		}
		void Game::initEnemy() {
			COORD a = {1, 1};
			COORD b = {45, 15};
			for (int i = 0; i < 8; i++) {
				enemy[i].position[0] = random(a, b);
				enemy[i].position[1].X = enemy[i].position[0].X + 3;
				enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
			}
		}
		void Game::drawEnemy() {
			for (int i = 0; i < 8; i++)
				drawFrame(enemy[i].position[0], enemy[i].position[1], '-', '|');
		}
		void Game::drawEnemyToNull() {
			for (int i = 0; i < 8; i++) {
				drawFrame(enemy[i].position[0], enemy[i].position[1], ' ', ' ');
			}
		}
		void Game::Pause() {
			SetPos(61, 2);
			cout << " ";
			SetPos(61, 2);
			cout << "暂停中...";
			char c = _getch();
			while (c != 'p')
				c = _getch();
			SetPos(61, 2);
			cout << "         ";
		}
		void Game::planeMove(char x) {
			if (x == 'a')
				if (position[1].X != 1)
					for (int i = 0; i <= 9; i++)
						position[i].X -= 2;
			if (x == 's')
				if (position[7].Y != 23)
					for (int i = 0; i <= 9; i++)
						position[i].Y += 1;
			if (x == 'd')
				if (position[4].X != 47)
					for (int i = 0; i <= 9; i++)
						position[i].X += 2;
			if (x == 'w')
				if (position[5].Y != 3)
					for (int i = 0; i <= 9; i++)
						position[i].Y -= 1;
		}
		void Game::bulletMove() {
			for (int i = 0; i < 10; i++) {
				if ( bullet[i].Y != 30) {
					bullet[i].Y -= 1;
					if ( bullet[i].Y == 1 ) {
						COORD pos = {static_cast<SHORT>(bullet[i].X), static_cast<SHORT>(bullet[i].Y + 1)};
						drawThisBulletToNull( pos );
						bullet[i].Y = 30;
					}
				}
			}
		}
		void Game::enemyMove() {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 2; j++)
					enemy[i].position[j].Y++;
				if (24 == enemy[i].position[1].Y) {
					COORD a = {1, 1};
					COORD b = {45, 3};
					enemy[i].position[0] = random(a, b);
					enemy[i].position[1].X = enemy[i].position[0].X + 3;
					enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
				}
			}
		}
		void Game::judgePlane() {
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 9; j++)
					if (judgeCoordInFrame(enemy[i], position[j])) {
						SetPos(62, 1);
						cout << "坠毁";
						drawFrame(enemy[i], '+', '+');
						Sleep(1000);
						GameOver();
						return;
					}
		}
		void Game::drawThisBulletToNull( COORD c) {
			SetPos(c);
			cout << " ";
		}
		void Game::drawThisEnemyToNull( Frame f ) {
			drawFrame(f, ' ', ' ');
		}
		void Game::judgeEnemy() {
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 10; j++)
					if ( judgeCoordInFrame(enemy[i], bullet[j]) ) {
						score += 5;
						drawThisEnemyToNull( enemy[i] );
						COORD a = {1, 1};
						COORD b = {45, 3};
						enemy[i].position[0] = random(a, b);
						enemy[i].position[1].X = enemy[i].position[0].X + 3;
						enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
						drawThisBulletToNull( bullet[j] );
						bullet[j].Y = 30;
					}
		}
		void Game::Shoot() {
			for (int i = 0; i < 10; i++)
				if (bullet[i].Y == 30) {
					bullet[i].X = position[5].X;
					bullet[i].Y = position[5].Y - 1;
					break;
				}
		}
		void Game::printScore() {
			if (score == 120 && flag_rank == 0) {
				rank -= 3;
				flag_rank = 1;
			} else if ( score == 360 && flag_rank == 1) {
				rank -= 5;
				flag_rank = 2;
			} else if ( score == 480 && flag_rank == 2) {
				rank -= 5;
				flag_rank = 3;
			}
			int x = rank / 5;
			SetPos(60, 6);
			cout << score;
			if ( rank != rankf ) {
				SetPos(60, 7);
				if ( x == 5)
					title = "高级飞行员";
				else if ( x == 4)
					title = "神级飞行员";
				else if ( x == 3)
					title = "死神级飞行员";
				else if ( x == 2 )
					title = "海神级飞行员";
				else if ( x == 1 )
					title = "修罗神飞行员";
				cout << title;
			}
			rankf = rank;
		}
		void Game::Playing() {
			drawEnemy();
			drawPlane();
			int flag_bullet = 0;
			int flag_enemy = 0;
			while (1) {
				Sleep(8);
				if (_kbhit()) {
					char x = _getch();
					if ('a' == x || 's' == x || 'd' == x || 'w' == x) {
						drawPlaneToNull();
						planeMove(x);
						drawPlane();
						judgePlane();
						if (end == true) {
							return;
						}
					}
					if ('p' == x)
						Pause();
					if ( ' ' == x)
						Shoot();
					if ( 'e' == x) {
						GameOver();
						return;
					}
				}

				if ( 0 == flag_bullet ) {
					bulletMove();
					drawBulletToNull();
					drawBullet();
					judgeEnemy();
				}
				flag_bullet++;
				if ( 5 == flag_bullet )
					flag_bullet = 0;

				if ( 0 == flag_enemy ) {
					drawEnemyToNull();
					enemyMove();
					drawEnemy();
					judgePlane();
					if (end == true) {
						return;
					}
				}
				flag_enemy++;
				if ( flag_enemy >= rank )
					flag_enemy = 0;
				printScore();
			}
		}
		void Game::GameOver() {
			system("cls");
			COORD p1 = {28, 9};
			COORD p2 = {53, 15};
			drawFrame(p1, p2, '=', '|');
			SetPos(36, 12);
			string str = "Game Over!";
			for (int i = 0; i < str.size(); i++) {
				Sleep(80);
				cout << str[i];
			}
			Sleep(1000);
			system("cls");
			drawFrame(p1, p2, '=', '|');
			SetPos(31, 11);
			cout << "击落敌机：" << score / 5 << " 架";
			SetPos(31, 12);
			cout << "得　　分：" << score;
			SetPos(31, 13);
			cout << "获得称号：" << title;
			SetPos(30, 16);
			Sleep(1000);
			cout << "重来？ 是（y）| 否（n）";
as:
			char x = _getch();
			if (x == 'n') {
				end = true;
				return;
			} else if (x == 'y') {
				end = false;
				system("cls");
				Game game;
				int a = drawMenu();
				if (a == 2)
					game.rank = 20;
				system("cls");
				drawPlaying();
				game.Playing();
			} else goto as;
		}
		void main() {
			end = false;
			srand((int)time(0));
			Game game;
			int a = drawMenu();
			if (a == 2)
				game.rank = 20;
			system("cls");
			drawPlaying();
			game.Playing();
			return;
		}
	}
	namespace emlpd {
		bool end = false;
		int Your = 0, Other = 0;
		string daojuname[] = {"放大镜", "手铐", "小刀", "烟", "饮料"};
		double Yourmoney = 0;
		int huihe = 0;
		bool emoqbq = 1;
		bool ndqbq = 1;

		int shi = 0, kong = 0;
		int q[10] = {}, qlen = 0;
		int Rand(int x, int y) {
			int A = rand(), B = rand();
			return A * 1ll * B % (y - x + 1) + x;
		}
		int T = 0;
		int daojulen = 0;
		int daoju[10] = {};
		int daojulen1 = 0;
		int daoju1[10] = {};
		void build_gun() {
			kong = Rand(1, 4);
			shi = Rand(1, 4);
			qlen = 0;
			printf("%d发实弹,%d发空弹\n", shi, kong);
			int a1 = kong, a2 = shi;
			for (int i = 1; i <= kong + shi; i++) {
				int sum = Rand(1, a1 + a2);
				if (sum <= a1) {
					a1--;
					q[++qlen] = 2;
				} else {
					a2--;
					q[++qlen] = 1;
				}
			}
			if (huihe != 1) {
				int maxn = min(4, 8 - daojulen);
				printf("你获得了%d个道具:\n", maxn);
				daojulen += maxn;
				for (int i = 1; i <= maxn; i++) {
					int kkk = Rand(0, 4);
					daoju[kkk]++;
					cout << daojuname[kkk];
					if (i != maxn) {
						printf(",");
					}
				}
				printf("\n");
				maxn = min(4, 8 - daojulen1);
				printf("恶魔获得了%d个道具:\n", maxn);
				daojulen1 += maxn;
				for (int i = 1; i <= maxn; i++) {
					int kkk = Rand(0, 4);
					daoju1[kkk]++;
					cout << daojuname[kkk];
					if (i != maxn) {
						printf(",");
					}
				}
			}
			printf("\n");
			system("pause");
			system("cls");
		}
		void IsOver() {
			if (Your <= 0) {
				printf("你输了\n");
				system("pause");
				end = true;
				return;
			}
			if (Other <= 0) {
				if (huihe == 3) {
					Yourmoney /= 3;
					printf("你赢了\n你获得了奖金$%.2lf\n", Yourmoney);
					system("pause");
					end = true;
					return;
				} else if (huihe == 2) {
					printf("进入第三回合\n");
					huihe = 3;
					Your = 6;
					Other = 6;
					system("pause");
					system("cls");
					build_gun();
				} else if (huihe == 1) {
					printf("进入第二回合\n");
					huihe = 2;
					Your = 4;
					Other = 4;
					system("pause");
					system("cls");
					build_gun();
				}
			}
		}
		void wait() {
			for (int i = 1; i <= 3; i++) {
				Sleep(500);
				printf(".");
			}
			Sleep(500);
		}
		int Hurt = 1;
		int shoukao_you = 0;
		void Timeyou() {
			int x;
			while (1) {
				if (!ndqbq) {
					printf("你的生命:@#%&%%&@#%@%*&*\n");
				} else {
					printf("你的生命:%d/%d\n", Your, huihe * 2);
				}
				if (!emoqbq) {
					printf("恶魔生命:@$%#^%%^%&*&%%$\n");
				} else {
					printf("恶魔生命:%d/%d\n", Other, huihe * 2);
				}
				printf("剩余实弹数:%d    剩余空弹数:%d\n", shi, kong);
				if (huihe != 1) {
					printf("你现在拥有的道具:\n");
					for (int i = 0; i <= 4; i++) {
						cout << daojuname[i];
						printf("%d", daoju[i]);
						printf("个");
						if (i != 4) {
							printf(",");
						}
					}
					printf("\n");
					printf("恶魔现在拥有的道具:\n");
					for (int i = 0; i <= 4; i++) {
						cout << daojuname[i];
						printf("%d", daoju1[i]);
						printf("个");
						if (i != 4) {
							printf(",");
						}
					}
				}
				printf("\n");
				printf("现在是你的回合\n");
				printf("你要\n1.向恶魔开枪\n2.向自己开枪\n");
				if (huihe != 1) {
					for (int i = 0; i <= 4; i++) {
						printf("%d.使用", i + 3);
						cout << daojuname[i] << '\n';
					}
				}
				scanf("%d", &x);
				if ((huihe == 1 && 1 <= x && x <= 2) || (huihe != 1 && 1 <= x && x <= 7)) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 1) {
				printf("你决定向恶魔开枪");
				T++;
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(shi) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("因为你使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				} else {
					Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					shi--;
					qlen--;
					Other -= Hurt;

					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver();
					if (end == true) {
						return;
					}
					if (huihe == 3 && Other <= 2) {
						printf("恶魔的起搏器断开!\n");
						emoqbq = 1;
						Yourmoney += 5000.0;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						Yourmoney += 1000.0;
						printf("因为你使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 2) {
				printf("你决定向自己开枪");
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
				} else {
					Yourmoney += 5000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
					T++;
					shi--;
					qlen--;
					Your -= Hurt;
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver();
					if (end == true) {
						return;
					}
					if (huihe == 3 && Your <= 2) {
						printf("你的起搏器断开!\n");
						ndqbq = 1;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("因为你使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 3) {
				if (daoju[0]) {
					daoju[0]--;
					daojulen--;
					printf("你使用了放大镜\n");
					wait();
					printf("\n你看到了");
					if (q[qlen] == 1) {
						printf("实弹\n");
						Yourmoney += 2500.0;
					} else {
						printf("空弹\n");
					}
					Sleep(500);
					printf("\n");
				} else {
					printf("你现在没有放大镜\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 4) {
				if (daoju[1]) {
					if (!shoukao_you) {
						daoju[1]--;
						daojulen--;
						printf("你使用了手铐\n");
						printf("你获得了连开两枪的机会\n");
						shoukao_you = 1;
					} else {
						printf("你已经用过手铐了\n");
					}
					Sleep(1145);
					system("cls");
				} else {
					printf("你现在没有手铐\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 5) {
				if (daoju[2]) {
					daoju[2]--;
					daojulen--;
					printf("你使用了小刀\n");
					printf("若下一发为实弹则伤害翻倍\n");
					Hurt *= 2;
					Yourmoney += 500.0;
					Sleep(1145);
					system("cls");
				} else {
					printf("你现在没有小刀\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 6) {
				if (daoju[3]) {
					if (Your ^ 6) {
						daoju[3]--;
						daojulen--;
						printf("你使用了烟\n");
						printf("你回复了一点生命\n");
						Yourmoney += 500.0;
						Your++;
					} else {
						printf("你现在不需要烟\n");
					}
					Sleep(1145);
				} else {
					printf("你现在没有烟\n");
					Sleep(1145);
					system("cls");
				}
			} else {
				if (daoju[4]) {
					daoju[4]--;
					daojulen--;
					printf("你使用了饮料\n");
					wait();
					printf("\n");
					printf("你退了一发");
					if (q[qlen] == 2) {
						printf("空弹");
						kong--;
					} else {
						printf("实弹");
						Yourmoney += 500.0;
						shi--;
					}
					qlen--;
					Sleep(500);
				} else {
					printf("你现在没有饮料\n");
					Sleep(1145);
					system("cls");
				}
			}
			Sleep(1000);
			system("cls");
		}
		int Know = 0;
		int shoukaoemo = 0;
		void fightyou() {
			printf("恶魔决定向你开枪");
			T++;
			wait();
			if (q[qlen] == 2) {
				Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
				kong--;
				qlen--;
				Hurt = 1;
				Know = 0;
				printf("是空弹\n");
				if (shoukaoemo) {
					printf("因为恶魔使用了手铐，所以可以再来一次\n");
					T--;
					Sleep(500);
					shoukaoemo = 0;
				}
			} else {
				Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
				shi--;
				qlen--;
				Your -= Hurt;
				printf("是实弹\n");
				printf("造成了%d点伤害\n", Hurt);
				Hurt = 1;
				Know = 0;
				Sleep(500);
				IsOver();
				if (end == true) {
					return;
				}
				if (huihe == 3 && Your <= 2) {
					printf("你的起搏器断开!\n");
					ndqbq = 1;
				}
				if (shoukaoemo) {
					printf("因为恶魔使用了手铐，所以可以再来一次\n");
					Yourmoney += 1000.0;
					T--;
					Sleep(500);
					shoukaoemo = 0;
				}
			}
		}
		void fightemo() {
			printf("恶魔决定向自己开枪");
			wait();
			if (q[qlen] == 2) {
				Yourmoney += 2000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
				kong--;
				qlen--;
				printf("是空弹\n");
				Know = 0;
			} else {
				Yourmoney += 5000.0 * (1 + (double)(kong) * 1.0 / (double)(shi + kong));
				shi--;
				T++;
				qlen--;
				Other -= Hurt;
				printf("是实弹\n");
				printf("造成了%d点伤害\n", Hurt);
				Hurt = 1;
				Know = 0;
				Sleep(500);
				IsOver();
				if (end == true) {
					return;
				}
				if (huihe == 3 && Other <= 2) {
					printf("恶魔的起搏器断开!\n");
					emoqbq = 1;
					Yourmoney += 5000.0;
				}
				if (shoukaoemo) {
					printf("因为恶魔使用了手铐，所以可以再来一次\n");
					T--;
					Sleep(500);
					shoukaoemo = 0;
				}
			}
		}
		void Timeother() {
			if (!ndqbq) {
				printf("你的生命:@#%&%%&@#%@%*&*\n");
			} else {
				printf("你的生命:%d/%d\n", Your, huihe * 2);
			}
			if (!emoqbq) {
				printf("恶魔生命:@$%#^%%^%&*&%%$\n");
			} else {
				printf("恶魔生命:%d/%d\n", Other, huihe * 2);
			}
			printf("剩余实弹数:%d    剩余空弹数:%d\n", shi, kong);
			if (huihe != 1) {
				printf("恶魔现在拥有的道具:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju1[i]);
					printf("个");
					if (i != 4) {
						printf(",");
					}
				}
			}
			printf("现在是恶魔的回合\n");
			Sleep(1500);
			if (Other != 6) {
				if (daoju1[3]) {
					daoju1[3]--;
					daojulen1--;
					printf("恶魔使用了烟\n");
					printf("恶魔回复了一点生命\n");
					Other++;
					Yourmoney += 500.0;
					Sleep(1145);
					system("cls");
					return ;
				}
			}
			if (Know == 0 && kong == 0) {
				Know = 1;
			}
			if (Know == 0) {
				if (abs(shi - kong) < 2 && kong != 0) {
					if (daoju1[0]) {
						daoju1[0]--;
						daojulen1--;
						printf("恶魔使用了放大镜\n");
						wait();
						printf("\nVery Interesting");
						if (q[qlen] == 1) {
							Yourmoney += 2500.0;
							Know = 1;
						} else {
							Know = 2;
						}
						Sleep(1145);
						system("cls");
						return ;
					}
				}
			} else if (Know == 1) {
				if (daoju1[2]) {
					daoju1[2]--;
					daojulen1--;
					Hurt *= 2;
					printf("恶魔使用了小刀\n");
					printf("若下一发为实弹则伤害翻倍");
					Yourmoney += 500.0;
					Sleep(1145);
					system("cls");
					return;
				} else {
					if (shi >= kong + 1 && daoju1[1] && shoukaoemo != 1) {
						daoju1[1]--;
						daojulen1--;
						shoukaoemo = 1;
						printf("恶魔使用了手铐\n");
						printf("恶魔获得了连开两枪的机会\n");
						Sleep(1145);
						system("cls");
						return ;
					}
					fightyou();
					system("cls");
					return ;
				}
			} else {
				if (daoju1[4]) {
					daoju1[4]--;
					daojulen1--;
					printf("恶魔使用了饮料\n");
					wait();
					printf("\n");
					printf("恶魔退了一发");
					if (q[qlen] == 2) {
						printf("空弹");
						kong--;
					} else {
						printf("实弹");
						shi--;
					}
					Know = 0;
					qlen--;
					Sleep(500);
					Sleep(1145);
					system("cls");
					return ;
				} else {
					fightemo();
					Sleep(1145);
					system("cls");
					return ;
				}
			}
			if (shi >= kong) {
				fightyou();
			} else {
				fightemo();
			}
			Sleep(1145);
			system("cls");
		}
		void Play() {
			while (1) {
				if (end == true) {
					return;
				}
				if (shi == 0) {
					build_gun();
					T = 0;
					continue;
				}
				if (T % 2 == 0) {
					Timeyou();
				} else {
					Timeother();
				}
			}
		}
		void danrenplay() {
			for (int i = 1; i <= 3; i++) {
				printf(".");
//      Sleep(1000);
			}
			printf("\n");
			printf("又来了一位挑战者...\n");
			Sleep(1000);
			int x;
			while (1) {
				printf("准备好参与恶魔的游戏吗？胜者带走奖金，败者将会在此长眠\n1.好的\n2.没问题\n");
				scanf("%d", &x);
				if (x == 1 || x == 2) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			while (1) {
				printf("你清楚我们的规则吗？\n1.清楚\n2.不清楚\n");
				scanf("%d", &x);
				if (x == 1 || x == 2) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 2) {
				for (int i = 1; i <= 3; i++) {
					printf(".");
					Sleep(1000);
				}
				printf("\n");
				printf("规则:\n");
				printf("你和恶魔都各有6点生命\n") ;
				printf("每轮开始前，你将知道一共有几发实弹，几发空弹\n");
				printf("每轮你可以选择对自己开枪，对恶魔开枪或者使用道具\n");
				printf("如果你对自己开枪，若为空弹，则可以继续行动，否则，停止行动\n");
				printf("如果你对恶魔开枪，无论如何，都将停止行动\n");
				printf("第二、三回合每轮开始前双方都将获得4个道具作为补给（上限为8个）\n");
				printf("到了第三回合，若有一方只剩两点生命，则该方的起搏器将被销毁，无法看见自己血量！\n");
				printf("道具一览：\n");
				printf("放大镜：可以知道下一发子弹是空弹还是实弹\n");
				printf("手铐：增加一次本回合的行动次数\n");
				printf("小刀：若下一发子弹为实弹，则伤害翻倍\n");
				printf("烟：可以回复1点体力\n");
				printf("饮料：可以退一发子弹\n");
				system("pause");
				system("cls");
			}
			printf("好吧\n");
			Sleep(1145);
			printf("游戏将要开始了哦\n");
			Sleep(1145);
			huihe = 1;
			Your = 2;
			Other = 2;
			system("cls");
			Play();
			return;
		}
		void IsOver_duo() {
			if (Your <= 0) {
				printf("玩家B赢了\n玩家B获得了奖金$%.2lf\n", Yourmoney);
				system("pause");
				end = true;
				return;
			} else if (Other <= 0) {
				printf("玩家A赢了\n玩家A获得了奖金$%.2lf\n", Yourmoney);
				system("pause");
				end = true;
				return;
			}

		}
		void build_gun_duo() {
			kong = Rand(1, 4);
			shi = Rand(1, 4);
			qlen = 0;
			printf("%d发实弹,%d发空弹\n", shi, kong);
			int a1 = kong, a2 = shi;
			for (int i = 1; i <= kong + shi; i++) {
//      Sleep(50);
				int sum = Rand(1, a1 + a2);
				if (sum <= a1) {
					a1--;
					q[++qlen] = 2;
				} else {
					a2--;
					q[++qlen] = 1;
				}
			}
			int maxn = min(2, 8 - daojulen);
			printf("玩家A获得了%d个道具:\n", maxn);
			daojulen += maxn;
			for (int i = 1; i <= maxn; i++) {
//      Sleep(50);
				int kkk = Rand(0, 4);
				daoju[kkk]++;
				cout << daojuname[kkk];
				if (i != maxn) {
					printf(",");
				}
			}
			printf("\n");
			maxn = min(2, 8 - daojulen1);
			printf("玩家B获得了%d个道具:\n", maxn);
			daojulen1 += maxn;
			for (int i = 1; i <= maxn; i++) {
				int kkk = Rand(0, 4);
				daoju1[kkk]++;
				cout << daojuname[kkk];
				if (i != maxn) {
					printf(",");
				}
			}
			printf("\n");
			system("pause");
			system("cls");
		}
		void Timeyou_duo() {
			int x;
			while (1) {
				printf("玩家A的生命:%d/4\n玩家B的生命:%d/4\n", Your, Other);
				printf("剩余实弹数:%d    剩余空弹数:%d\n", shi, kong);
				printf("玩家A现在拥有的道具:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju[i]);
					printf("个");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("玩家B现在拥有的道具:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju1[i]);
					printf("个");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("现在是玩家A的回合\n");
				printf("玩家A要\n1.向玩家B开枪\n2.向自己开枪\n");
				for (int i = 0; i <= 4; i++) {
					printf("%d.使用", i + 3);
					cout << daojuname[i] << '\n';
				}
				scanf("%d", &x);
				if (1 <= x && x <= 7) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 1) {
				printf("玩家A决定向玩家B开枪");
				T++;
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(shi) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("因为玩家A使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				} else {
					Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					shi--;
					qlen--;
					Other -= Hurt;
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						Yourmoney += 1000.0;
						printf("因为玩家A使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 2) {
				printf("玩家A决定向自己开枪");
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
				} else {
					Yourmoney += 5000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
					T++;
					shi--;
					qlen--;
					Your -= Hurt;
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("因为玩家A使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 3) { //{"放大镜","手铐","小刀","烟","饮料"};
				if (daoju[0]) {
					daoju[0]--;
					daojulen--;
					printf("玩家A使用了放大镜\n");
					wait();
					printf("\n玩家A看到了");
					if (q[qlen] == 1) {
						printf("实弹\n");
						Yourmoney += 2500.0;
					} else {
						printf("空弹\n");
					}
					Sleep(500);
					printf("\n");
				} else {
					printf("玩家A现在没有放大镜\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 4) {
				if (daoju[1]) {
					if (!shoukao_you) {
						daoju[1]--;
						daojulen--;
						printf("玩家A使用了手铐\n");
						printf("玩家A获得了连开两枪的机会\n");
						shoukao_you = 1;
					} else {
						printf("玩家A已经用过手铐了\n");
					}
					Sleep(1145);
					system("cls");
				} else {
					printf("玩家A现在没有手铐\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 5) {
				if (daoju[2]) {
					daoju[2]--;
					daojulen--;
					printf("玩家A使用了小刀\n");
					printf("若下一发为实弹则伤害翻倍\n");
					Yourmoney += 500.0;
					Hurt *= 2;
					Sleep(1145);
					system("cls");
				} else {
					printf("玩家A现在没有小刀\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 6) {
				if (daoju[3]) {
					if (Your ^ 4) {
						daoju[3]--;
						daojulen--;
						printf("玩家A使用了烟\n");
						printf("玩家A回复了一点生命\n");
						Yourmoney += 500.0;
						Your++;
					} else {
						printf("玩家A现在不需要烟\n");
					}
					Sleep(1145);
				} else {
					printf("玩家A现在没有烟\n");
					Sleep(1145);
					system("cls");
				}
			} else {
				if (daoju[4]) {
					daoju[4]--;
					daojulen--;
					printf("玩家A使用了饮料\n");
					wait();
					printf("\n");
					printf("玩家A退了一发");
					if (q[qlen] == 2) {
						printf("空弹");
						kong--;
					} else {
						printf("实弹");
						Yourmoney += 500.0;
						shi--;
					}
					qlen--;
					Sleep(500);
				} else {
					printf("玩家A现在没有饮料\n");
					Sleep(1145);
					system("cls");
				}
			}
			Sleep(1000);
			system("cls");
		}
		void Timeother_duo() {
			int x;
			while (1) {
				printf("玩家A的生命:%d/4\n玩家B的生命:%d/4\n", Your, Other);
				printf("剩余实弹数:%d    剩余空弹数:%d\n", shi, kong);
				printf("玩家A现在拥有的道具:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju[i]);
					printf("个");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("玩家B现在拥有的道具:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju1[i]);
					printf("个");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("现在是玩家B的回合\n");
				printf("玩家B要\n1.向玩家A开枪\n2.向自己开枪\n");
				for (int i = 0; i <= 4; i++) {
					printf("%d.使用", i + 3);
					cout << daojuname[i] << '\n';
				}
				scanf("%d", &x);
				if (1 <= x && x <= 7) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 1) {
				printf("玩家B决定向玩家A开枪");
				T++;
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(shi) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
					if (shoukaoemo == 1) {
						shoukaoemo = 0;
						printf("因为玩家B使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				} else {
					Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					shi--;
					qlen--;
					Your -= Hurt;
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukaoemo == 1) {
						shoukaoemo = 0;
						Yourmoney += 1000.0;
						printf("因为玩家B使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 2) {
				printf("玩家B决定向自己开枪");
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("是空弹\n");
				} else {
					Yourmoney += 5000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
					T++;
					shi--;
					qlen--;
					Other -= Hurt;
					printf("是实弹\n");
					printf("造成了%d点伤害\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("因为玩家B使用了手铐，所以可以再来一次\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 3) {
				if (daoju1[0]) {
					daoju1[0]--;
					daojulen1--;
					printf("玩家B使用了放大镜\n");
					wait();
					printf("\n玩家B看到了");
					if (q[qlen] == 1) {
						printf("实弹\n");
						Yourmoney += 2500.0;
					} else {
						printf("空弹\n");
					}
					Sleep(500);
					printf("\n");
				} else {
					printf("玩家B现在没有放大镜\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 4) {
				if (daoju1[1]) {
					if (!shoukaoemo) {
						daoju1[1]--;
						daojulen1--;
						printf("玩家B使用了手铐\n");
						printf("玩家B获得了连开两枪的机会\n");
						shoukaoemo = 1;
					} else {
						printf("玩家B已经用过手铐了\n");
					}
					Sleep(1145);
					system("cls");
				} else {
					printf("玩家B现在没有手铐\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 5) {
				if (daoju1[2]) {
					daoju1[2]--;
					daojulen1--;
					printf("玩家B使用了小刀\n");
					printf("若下一发为实弹则伤害翻倍\n");
					Yourmoney += 500.0;
					Hurt *= 2;
					Sleep(1145);
					system("cls");
				} else {
					printf("玩家B现在没有小刀\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 6) {
				if (daoju1[3]) {
					if (Other ^ 4) {
						daoju1[3]--;
						daojulen1--;
						printf("玩家B使用了烟\n");
						printf("玩家B回复了一点生命\n");
						Yourmoney += 500.0;
						Other++;
					} else {
						printf("玩家B现在不需要烟\n");
					}
					Sleep(1145);
				} else {
					printf("玩家B现在没有烟\n");
					Sleep(1145);
					system("cls");
				}
			} else {
				if (daoju1[4]) {
					daoju1[4]--;
					daojulen1--;
					printf("玩家B使用了饮料\n");
					wait();
					printf("\n");
					printf("玩家B退了一发");
					if (q[qlen] == 2) {
						printf("空弹");
						kong--;
					} else {
						printf("实弹");
						Yourmoney += 500.0;
						shi--;
					}
					qlen--;
					Sleep(500);
				} else {
					printf("玩家B现在没有饮料\n");
					Sleep(1145);
					system("cls");
				}
			}
			Sleep(1000);
			system("cls");
		}
		int asdasd = 0;
		void duorenplay() {
			while (1) {
				if (end == true) {
					return;
				}
				if (shi == 0) {
					build_gun_duo();
					T = asdasd;
					asdasd++;
					continue;
				}
				if (T % 2 == 0) {
					Timeyou_duo();
				} else {
					Timeother_duo();
				}
			}
		}
		int main() {
			end = false;
			srand(time(0));
			int x;
			while (1) {
				printf("请选择你想要的模式：\n1.单人\n2.双人（此模式中，生命值为4，道具补给为2）\n");
				scanf("%d", &x);
				if (x == 1 || x == 2) {
					break;
				}
				printf("输入不合法\n");
				Sleep(1145);
				system("cls");
			}
			system("cls");
			if (x == 1) {
				danrenplay();
			} else {
				Your = Other = 4;
				duorenplay();
			}
			return 0;
		}
	}
}
using namespace GAME;

struct JOKE { /*恶搞*/
	void kill() {
		while (true) {
			system("TASKKILL /F /IM wps.exe");
			system("TASKKILL /F /IM EasiNote.exe");
			system("TASKKILL /F /IM EasiNote.BrowserSubprocess.exe");
			system("TASKKILL /F /IM swenserver.exe");
			system("TASKKILL /F /IM Cvte.RemoteProcess.exe");
			system("TASKKILL /F /IM EasiCameraGuardian.exe");
			system("TASKKILL /F /IM EasiCamera.exe");
			system("TASKKILL /F /IM DingTalk.exe");
			system("TASKKILL /F /IM WeChat.exe");
		}
	}
	void copy_file() {
		MessageBox(hwnd, _T("此功能可以提取任意文件夹的所有内容，且支持U盘。\n你现在需要设置这些文件的位置和拷贝后存储的位置。"), _T("提示"), MB_OK);
		if (MessageBox(hwnd, _T("本软件不对你使用此功能造成的任何损失(包括但不限于驱逐电教、被叫去和老师喝茶等)负责，请慎重考虑！"), _T("警告"), MB_YESNO | MB_ICONWARNING) == IDNO) {
			return;
		}
		MessageBox(hwnd, _T("你需要在接下来的控制台中输入文件来源和拷贝后的文件去向。"), _T("提示"), MB_OK);
		SetColorAndBackground(7, 0);
		cout << "\n请输入文件来源。\n";
		SetColorAndBackground(4, 7);
		cout << "注意：文件夹名称最后要输入\\。本程序不支持复制单个文件。\n示例：E:\\，F:\\马说课件\\\n";
		SetColorAndBackground(7, 0);
		cout << "请输入(输入1退出程序):";
		string infile;
		cin >> infile;
		if (infile == "1") {
			return;
		}
		SetColorAndBackground(7, 0);
		cout << "\n请输入文件去向。\n";
		SetColorAndBackground(4, 7);
		cout << "注意：文件夹名称最后也要输入\\。\n";
		SetColorAndBackground(7, 0);
		cout << "请输入(输入0使用默认路径D:\\file\\，输入1退出程序):";
		string outfile;
		cin >> outfile;
		if (outfile == "0") {
			outfile = "D:\\file\\";
		}
		if (outfile == "1") {
			return;
		}
		cout << "设置完成。当U盘插入后（目录存在），希沃克星会自动复制其中的文件。\n按任意键后开始搜索。";
		getch();
		cls
		cout << "开始！！\n------------------\n正在等待目录出现";
		string existpath = infile + "temp.dat";
		while (fileExist(existpath.c_str()) == false) {
			ofstream file(existpath.c_str());
			file << "";
			file.close();
		}
		string command = "del \"" + existpath + "\"";
		system(command.c_str());
		cout << "\n已找到文件夹，开始复制\n";
		command = "xcopy \"" + infile + "\" \"" + outfile + "\" /E /I /H /C /Y";
		system(command.c_str());
		cout << "\n";
		system("pause");
		return;
	}
} joke;

struct Launcher {
	string listname(bool allowA, bool allowD, string liststring[], int n) {
		gotoxy(0, 3);
		int channel = 1;
		SetColorAndBackground(7, 0);
		for (int i = 1; i < channel; i++) {
			cout << liststring[i] << "\n";
		}
		SetColorAndBackground(0, 7);
		cout << liststring[channel] << "\n";
		SetColorAndBackground(7, 0);
		for (int i = channel + 1; i <= n; i++) {
			cout << liststring[i] << "\n";
		}
		while (1) {
			if ( _kbhit() ) {
				char x = _getch();
				switch (x) {
					case 's': {
						if (channel < n) {
							channel++;
						}
						gotoxy(0, 3);
						SetColorAndBackground(7, 0);
						for (int i = 1; i < channel; i++) {
							cout << liststring[i] << "\n";
						}
						SetColorAndBackground(0, 7);
						cout << liststring[channel] << "\n";
						SetColorAndBackground(7, 0);
						for (int i = channel + 1; i <= n; i++) {
							cout << liststring[i] << "\n";
						}
						break;
					}
					case 'w': {
						if (channel > 1) {
							channel--;
						}
						gotoxy(0, 3);
						SetColorAndBackground(7, 0);
						for (int i = 1; i < channel; i++) {
							cout << liststring[i] << "\n";
						}
						SetColorAndBackground(0, 7);
						cout << liststring[channel] << "\n";
						SetColorAndBackground(7, 0);
						for (int i = channel + 1; i <= n; i++) {
							cout << liststring[i] << "\n";
						}
						break;
					}
					case 'a': {
						if (allowA == true) {
							if (box > 1) {
								box--;
							}
						}
						return "-1";
					}
					case 'd': {
						if (allowD == true) {
							if (box < boxn) {
								box++;
							}
						}
						return "-1";
					}
					case ' ': {
						return liststring[channel];
					}
				}
			}
		}
		return "-2";
	}
	void head(string WindowTitle = "希沃克星", string cmdTitle = "SeewoKiller") {
		string cmd = "title " + WindowTitle;
		system(cmd.c_str());
		cls
		gotoxy(0, 0);
		if (fastboot == true) {
			cout << cmdTitle;
		} else {
			cout << cmdTitle << "       Windows Innerversion " << dwMajorInt << "." << dwMinorInt;
		}
		gotoxy(0, 1);
		SetColorAndBackground(7, 0);
		for (int i = 1; i < box; i++) {
			cout << "|  " << word.box[i] << "  ";
		}
		SetColorAndBackground(0, 7);
		cout << "|  " << word.box[box] << "  |";
		SetColorAndBackground(7, 0);
		for (int i = box + 1; i <= boxn; i++) {
			cout << "  " << word.box[i] << "  |";
		}
		gotoxy(0, 2);
		cout << "----------------------";
	}
	void lcmain() {
		string s = "-1";
		while (1) {
			//主页面
			if (s == "-1") {
				cls
				if (fastboot == true) {
					head("希沃克星fastboot模式", "SeewoKiller Fastboot");
				} else {
					head();
				}
				switch (box) {
					case 1: {
						s = listname(false, true, word.recent, word.recentn);
						break;
					}
					case 2: {
						s = listname(true, true, word.all, word.alln);
						break;
					}
					case 3: {
						s = listname(true, true, word.more, word.moren);
						break;
					}
					case 4: {
						s = listname(true, false, word.setting, word.settingn);
						break;
					}
				}
			} else if (s == "循环清任务(上课防屏保)") {
				taskkill(true, false);
			} else if (s == "一键卸载") {
				uninstall();
				s = "-1";
				continue;
			} else if (s == "AI") {
				setfont(20);
				string aipath = ".\\ai.exe";
				system(aipath.c_str());
				system("pause");
				setfont(30);
				s = "-1";
				continue;
			} else if (s == "计算π") {
				setfont(20);
				string aipath = ".\\pai.exe";
				system(aipath.c_str());
				setfont(30);
				s = "-1";
				continue;
			} else if (s == "冰点还原破解") {
				system("title 冰点还原");
				cout << "\n请先关闭冰点窗口后再继续操作希沃克星。\n";
				string unfreezepath = executable_path + "\\SeewoFreeze\\SeewoFreezeUI.exe --startup-with-main-window";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//有权限的都可以打开
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
				if (fSuccess) { //把主进程暂停，等待子进程终止
					CloseHandle(pi.hThread);
					//暂停主进程的执行，直到child终止，该代码才可以继续运行
					WaitForSingleObject(pi.hProcess, INFINITE);
					CloseHandle(pi.hProcess);
				}
				//system(".\\SeewoFreeze\\SeewoFreezeUI.exe --startup-with-main-window");
				system("pause");
				s = "-1";
				continue;
			} else if (s == "晚自习制裁模式") {
				system("title 制裁晚自习");
				taskkill(true, true);
				s = "-1";
				continue;
			} else if (s == "连点器(可防屏保)") {
				liandianqi();
				s = "-1";
				continue;
			} else if (s == "一键解希沃锁屏") {
				taskkill(true, true);
				s = "-1";
				continue;
			} else if (s == "退出") {
				return;
			} else if (s == "关于") {
				about();
				s = "-1";
				continue;
			} else if (s == "启动初学者引导") {
				quickstart();
				s = "-1";
				continue;
			} else if (s == "使用新版界面") {
				string guipath = executable_path + "\\gui.exe";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(guipath.c_str()));//有权限的都可以打开
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
				if (fSuccess) {
					return;
				}
			} else if (s == "重启到fastboot(真的fast!)") {
				string fbpath = "./SeewoKiller.exe run fastboot";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(fbpath.c_str()));//有权限的都可以打开
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
				if (fSuccess) {
					return;
				}
			} else if (s == "在晚自习制裁/循环清任务时启用日志-当前:false" or s == "在晚自习制裁/循环清任务时启用日志-当前:true") {
				ifstream file(".\\settings\\write-log-when-killapp.seewokiller");
				string value;
				getline(file, value);
				value = "当前:" + value + "\n你要将此设置更改为什么？\n点击\"是\"设置为true，点击\"否\"设置为false，点击\"取消\"忽略修改";
				int ans = MessageBox(hwnd, value.c_str(), _T("修改变量"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "true";
						file.close();
						if (MessageBox(hwnd, _T("修改完成，是否立即重新加载配置文件？\n日志文件保存在log文件夹下"), _T("提示"), MB_YESNO) == IDYES) {
							cls
							poweron(true);
						}
						break;
					}
					case IDNO: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "false";
						file.close();
						if (MessageBox(hwnd, _T("修改完成，是否立即重新加载配置文件？"), _T("提示"), MB_YESNO) == IDYES) {
							cls
							poweron(true);
						}
						break;
					}
				}
				s = "-1";
				continue;
			} else if (s == "打开日志文件夹") {
				string command = "explorer.exe \"" + executable_path + "\\log\\\"";
				system(command.c_str());
				s = "-1";
				continue;
			} else if (s == "小游戏>>>") {
				head();
				string d = listname(false, false, word.game, word.gamen);
				if (d == "返回") {
					s = "-1";
					continue;
				}
				if (d == "数字炸弹") {
					numberdamn();
				}
				if (d == "五子棋") {
					setfont(20);
					wzq.wzqmain();
					setfont(30);
				}
				if (d == "飞机大战") {
					cls
					setfont(20);
					feijidazhan::main();
					setfont(30);
				}
				if (d == "恶魔轮盘赌") {
					cls
					setfont(20);
					emlpd::main();
					setfont(30);
				}
			} else if (s == "恶搞>>>") {
				head();
				string d = listname(false, false, word.joke, word.joken);
				if (d == "返回") {
					s = "-1";
					continue;
				}
				if (d == "杀WPS+希沃白板+希沃视频展台") {
					joke.kill();
				}
				if (d == "提取U盘文件") {
					joke.copy_file();
				}
			} else if (s == "注册表>>>") {
				head();
				string d = listname(false, false, word.reg, word.regn);
				if (d == "返回") {
					s = "-1";
					continue;
				}
				if (d == "禁用任务栏菜单") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "1");
					regedit("HKEY_CURRENT_USER", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "1");
					restartexp();
				}
				if (d == "启用任务栏菜单") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
					regedit("HKEY_CURRENT_USER", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
					restartexp();
				}
				if (d == "禁用快捷键") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "1");
					restartexp();
				}
				if (d == "启用快捷键") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "0");
					restartexp();
				}
				if (d == "启用显示登录详细信息") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "1");
					cout << "修改完成，请注销以检查是否修改成功。\n";
					if (MessageBox(hwnd, _T("你是否要现在注销？"), _T("注销确认"), MB_OKCANCEL) == 1) {//1确定，2取消
						system("shutdown /l");
					}
					system("pause");
				}
				if (d == "禁用显示登录详细信息") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "0");
					cout << "修改完成，请注销以检查是否修改成功。\n";
					system("pause");
				}
				if (d == "登录时显示提示") {
					char title1[100];
					char title2[100];
					cout << "请输入主标题(50字以内)：";
					scanf_s("%s", title1, (unsigned)_countof(title1));
					cout << "请输入副标题(50字以内)：";
					scanf_s("%s", title2, (unsigned)_countof(title2));
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", title1);
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", title2);
					cout << "修改完成，请注销以检查是否修改成功。\n";
					if (MessageBox(hwnd, _T("你是否要现在注销？"), _T("注销确认"), MB_OKCANCEL) == 1) {//1确定，2取消
						system("shutdown /l");
					}
					system("pause");
				}
				if (d == "取消登录时显示提示") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", "");
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", "");
					cout << "修改完成，请注销以检查是否修改成功。\n";
					system("pause");
				}
			} else if (s == "开发者选项>>>") {
				head();
				string d = listname(false, false, word.dev, word.devn);
				if (d == "返回") {
					s = "-1";
					continue;
				}
				if (d == "释放进度条COM接口") {
					ReleaseTaskbarInterface();
					system("pause");
				}
				if (d == "关闭开发者模式") {
					word.settingn = 7;
					cout << "操作已完成。\n";
					system("pause");
					d = "返回";
				}
			} else {
				cout << "\nError\n";
				system("pause");
				s = "-1";
				continue;
			}
			//end
		}
	}
} lc;

int main(int argc, char *argv[]) {
	system("title 正在初始化");
	InitTaskbarInterface();
	taskbarprocess(TBPF_INDETERMINATE);
	srand((unsigned)time(NULL));
	system("title 正在检测管理员");
	//获取程序路径
	GetModuleFileNameA(NULL, path, MAX_PATH);
	executable_path = path;
	position = executable_path.find_last_of('\\');
	executable_path = executable_path.substr(0, position);
	xwbbpath = executable_path;
	//启动参数
	bool skipcheck = false;
	if (argc > 1) {
		taskbarprocess(TBPF_NOPROGRESS);
		system("title 希沃克星");
		string cmd[100];
		for (int i = 0; i < argc; i++) {
			cmd[i] = argv[i];
		}
		//不需要admin
		if (cmd[1] == "run") { //启动
			if (cmd[2] == "fastboot") {
				fastboot = true;
			}
		}
		if (cmd[1] == "about") {
			about();
			return 0;
		}
		if (cmd[1] == "game") { //游戏
			if (argc <= 2) {
				cout << "参数缺失，程序自动退出\n关于game的使用方法\n";
				cout << "-wzq五子棋\n-numberdamn数字炸弹\n-fjdz飞机大战\n-emlpd恶魔轮盘赌";
			}
			setfont(20);
			cls
			if (cmd[2] == "-wzq") {
				ShowWindow(hwnd, SW_MAXIMIZE);
				wzq.wzqmain();
			}
			if (cmd[2] == "-numberdamn") {
				numberdamn();
			}
			if (cmd[2] == "-fjdz") {
				ShowWindow(hwnd, SW_MAXIMIZE);
				feijidazhan::main();
			}
			if (cmd[2] == "-emlpd") {
				emlpd::main();
			}
			return 0;
		}
		//需要admin
		if (IsUserAnAdmin() == false) {
			cout << "命令行未取得管理员权限，程序无法运行。\n请使用管理员权限启动终端。";
			return 0;
		}
		if (cmd[1] == "ui") { //运行ui
			if (argc <= 2) {
				cout << "参数缺失，程序自动退出\n关于run的使用方法\n";
				cout << "-newui强制新UI\n-oldui强制旧UI\n";
				return 0;
			}
			if (cmd[2] == "-skipcheck") {
				skipcheck = true;
			}
			if (cmd[2] == "-oldui") {
				string unfreezepath = executable_path + "\\SeewoKiller.exe ui -skipcheck";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//有权限的都可以打开
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
				if (fSuccess) {
					return 0;
				}
			}
			if (cmd[2] == "-newui") {
				string unfreezepath = executable_path + "\\gui.exe";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//有权限的都可以打开
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
				if (fSuccess) {
					return 0;
				}
			}
		}
		if (cmd[1] == "taskkill") {
			taskkill(true, false);
			return 0;
		}
		if (cmd[1] == "wanzixi") {
			system("title 制裁晚自习");
			taskkill(true, true);
			return 0;
		}
		if (cmd[1] == "uninstall") {
			uninstall();
			return 0;
		}
		if (cmd[1] == "liandianqi") {
			system("title 连点器");
			liandianqi();
			return 0;
		}
		if (cmd[1] == "seewofreeze") {
			cout << "\n请先关闭冰点窗口后再继续操作。\n";
			string unfreezepath = executable_path + "\\SeewoFreeze\\SeewoFreezeUI.exe --startup-with-main-window";
			STARTUPINFO si = { sizeof(si) };//0
			PROCESS_INFORMATION pi;
			LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//有权限的都可以打开
			BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
			if (fSuccess) { //把主进程暂停，等待子进程终止
				CloseHandle(pi.hThread);
				//暂停主进程的执行，直到child终止，该代码才可以继续运行
				WaitForSingleObject(pi.hProcess, INFINITE);
				CloseHandle(pi.hProcess);
			}
			return 0;
		}
		if (cmd[1] == "setvalue") { //修改配置
			if (argc <= 2) {
				cout << "参数缺失，程序自动退出\n关于setvalue的使用方法\n";
				cout << "-log更改日志设置\n";
			}
			if (cmd[2] == "-log") {
				ifstream file(".\\settings\\write-log-when-killapp.seewokiller");
				string value;
				getline(file, value);
				value = "当前:" + value + "\n你要将此设置更改为什么？\n点击\"是\"设置为true，点击\"否\"设置为false，点击\"取消\"忽略修改";
				int ans = MessageBox(hwnd, value.c_str(), _T("修改变量"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "true";
						file.close();
						MessageBox(hwnd, _T("设置完成\n日志文件保存在log文件夹下"), _T("修改变量"), MB_OK);
						break;
					}
					case IDNO: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "false";
						file.close();
						MessageBox(hwnd, _T("设置完成"), _T("修改变量"), MB_OK);
						break;
					}
				}
			}
			return 0;
		}
		if (cmd[1] == "joke") { //恶搞
			if (argc <= 2) {
				cout << "参数缺失，程序自动退出\n关于joke的使用方法\n";
				cout << "-killapp杀上课用软件\n-copyfile提取U盘文件";
			}
			if (cmd[2] == "-killapp") {
				joke.kill();
			}
			if (cmd[2] == "-copyfile") {
				joke.copy_file();
			}
			return 0;
		}
		if (cmd[1] == "regedit") {
			if (argc <= 2) {
				cout << "参数缺失，程序自动退出\n";
			}
			if (cmd[2] == "-NoTrayContextMenu") {
				if (cmd[3] == "true") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "1");
					regedit("HKEY_CURRENT_USER", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "1");
					restartexp();
				} else if (cmd[3] == "false") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
					regedit("HKEY_CURRENT_USER", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
					restartexp();
				} else {
					cout << "参数错误\n";
				}
			}
			if (cmd[2] == "-NoWinKeys") {
				if (cmd[3] == "true") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "1");
					restartexp();
				} else if (cmd[3] == "false") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "0");
					restartexp();
				} else {
					cout << "参数错误\n";
				}
			}
			if (cmd[2] == "-VerboseStatus") {
				if (cmd[3] == "true") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "1");
					cout << "修改完成，请注销以检查是否修改成功。\n";
					if (MessageBox(hwnd, _T("你是否要现在注销？"), _T("注销确认"), MB_OKCANCEL) == 1) {//1确定，2取消
						system("shutdown /l");
					}
				} else if (cmd[3] == "false") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "0");
					cout << "修改完成，请注销以检查是否修改成功。\n";
				} else {
					cout << "参数错误\n";
				}
			}
			if (cmd[2] == "-legalnotice") {
				if (cmd[3] == "true") {
					char title1[1010100];
					char title2[1010100];
					cout << "请输入主标题(505050字以内)：";
					scanf_s("%s", title1, (unsigned)_countof(title1));
					cout << "请输入副标题(505050字以内)：";
					scanf_s("%s", title2, (unsigned)_countof(title2));
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", title1);
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", title2);
					cout << "修改完成，请注销以检查是否修改成功。\n";
					if (MessageBox(hwnd, _T("你是否要现在注销？"), _T("注销确认"), MB_OKCANCEL) == 1) {//1确定，2取消
						system("shutdown /l");
					}
				} else if (cmd[3] == "false") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", "");
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", "");
					cout << "修改完成，请注销以检查是否修改成功。\n";
				} else {
					cout << "参数错误\n";
				}
			}
			return 0;
		}
	}
	if (IsUserAnAdmin() == false) {
		if (getadmin() == false) {
			return 0;
		}
	}
	//参数的数量
	//cout << argc << " arguments:" << endl;
	//循环打印所有参数
	/*for (int i = 0; i < argc; i++) {
	cout << argv[i] << endl;
	}*/
	//system(".\\SeewoKiller.exe qwert asdfg");
	system("title Seewo Killer Starting");
	poweron(skipcheck, fastboot);
	cls
	//S(10);
	//if (MessageBox(NULL, _T("你干嘛哎呦"), _T("鸡叫"), MB_OKCANCEL) == 2) {
	//	return 0;
	//}//返回IDOK,IDCANCEL
	//thread用法
	//thread t(taskbarprocess,TBPF_INDETERMINATE,0);
	//t.detach();不阻塞主进程
	//t.join();阻塞主进程
	system("title 希沃克星");
	lc.lcmain();
	ReleaseTaskbarInterface();
	return 0;
}
