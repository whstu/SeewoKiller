#include "./main.h"
#include "./cmdCtrl.h"
#include "./files.h"

#include "./game.h"
using namespace GAME;

using namespace std;

char path[MAX_PATH];
string executable_path;
size_t position;
string xwbbpath;
int dwMajorInt;
int dwMinorInt;
bool fastboot = false;

/*HWND hwnd = GetConsoleWindow();
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
}*/
/*屏蔽关闭按钮*/
void connot_close_button() {
	HMENU hmenu = GetSystemMenu(hwnd, false);
	ifstream file(".\\settings\\enable-close-window-button.seewokiller");
	string value;
	getline(file, value);
	if (value != "true") {
		RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		SetWindowLong(hwnd, GWL_STYLE,
		              GetWindowLong(hwnd, GWL_STYLE) | WS_MINIMIZEBOX | WS_THICKFRAME);
	} else {
		SetWindowLong(hwnd, GWL_STYLE,
		              GetWindowLong(hwnd, GWL_STYLE) & ~(WS_MINIMIZEBOX | WS_THICKFRAME));
	}
	ShowWindow(hwnd, SW_MAXIMIZE);//最大化
	SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
	DrawMenuBar(hwnd);
	ReleaseDC(hwnd, NULL);
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
	string tf[5]={"true","false"};
	string dir=".\\settings\\write-log-when-killapp.seewokiller";
	check_config_avaliable(dir,tf,2,"false");
	change_word("设置",SearchForAddress(word.setting,"在晚自习制裁/循环清任务时启用日志"),true,dir);

	dir=".\\settings\\enable-close-window-button.seewokiller";
	check_config_avaliable(dir,tf,2,"false");
	change_word("设置",SearchForAddress(word.setting,"允许使用“关闭”按钮"),true,dir);

	if (fileExist(".\\settings\\start.seewokiller") == false) {
		ofstream file(".\\settings\\start.seewokiller");
		file << "0";
		file.close();
		word.setting[5] = "启动设置-当前:总是询问";
	} else {
		ifstream file(".\\settings\\start.seewokiller");
		string value;
		getline(file, value);
		file.close();
		if (value == "n") {
			word.setting[5] = "启动设置-当前:总是使用新UI";
		} else if (value == "o") {
			word.setting[5] = "启动设置-当前:总是使用旧UI";
		} else if (value == "0") {
			word.setting[5] = "启动设置-当前:总是询问";
		} else {
			ofstream file(".\\settings\\start.seewokiller");
			file << "0";
			file.close();
			word.setting[5] = "启动设置-当前:总是询问";
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
	//总是新UI
	ifstream start(".\\settings\\start.seewokiller");
	string startv;
	getline(start, startv);
	start.close();
	if (startv == "n") {
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
			ReleaseTaskbarInterface();
			exit(0);
		}
	}
	//-------
	dwMajorInt = static_cast<int>(dwMajor);
	dwMinorInt = static_cast<int>(dwMinor);
	float version = dwMajorInt + dwMinorInt * 0.1;
	if (SkipCheckWinVer == false and startv == "0") {
		if (version >= 6.1 and fileExist(".\\gui.exe") == true) {
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
					ReleaseTaskbarInterface();
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
	cout << "\nSeewo Killer 2.1 (Expelliarmus)\n";
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
	cout << "\n新版界面基于PyQt5和qfluentwidgets，网址https://qfluentwidgets.com/\n";
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
			word.settingn = 12;
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

void help(string name = "seewofreeze") {
	if (name == "seewofreeze") {
		cls
		cout << "欢迎使用冰点还原帮助程序。\n";
		cout << "正在检测...\n";
		S(500);
		if (fileExist(".\\SeewoFreeze\\SeewoFreezeUI.exe") == false) {
			cls
			cout << "检测到冰点还原软件不存在。\n";
			cout << "请检查是否安装了冰点还原模块。\n";
			cout << "\n如果您确认已经安装过冰点还原模块但仍无法使用，那么可能是杀毒软件问题。\n";
			cout << "请关闭杀毒软件后重试。\n";
			cout << "\n对于Windows安全中心，请进行以下操作：";
			cout << "打开Windows安全中心\n";
			cout << "选择“病毒和威胁防护”\n";
			cout << "找到所有“篡改”“扫描”选项，全部关闭\n";
			cout << "重新安装希沃克星。";
			system("pause");
			cls
			return;
		}
		cls
		cout << "请尝试执行以下操作。\n";
		cout << "选择希沃克星安装目录下的SeewoFreeze文件夹，打开属性\n";
		cout << "选择“安全”选项卡\n";
		cout << "选择“高级”\n";
		cout << "点击“禁用继承”\n";
		cout << "选择“从此对象中删除所有已继承的权限”\n";
		cout << "\n回到“高级”界面，点击“添加”\n";
		cout << "点击“选择主体”，在“对象名称”文本框中输入以下内容：\n";
		system("whoami");
		cout << "勾选“完全控制”\n";
		cout << "确定（如果窗口太大，可以用键盘回车）\n";
		cout << "确定，关闭属性\n应用时一定要应用于子文件和子文件夹！！！！\n";
		cout << "再次尝试冰点还原。\n";
		system("pause");
		return;
	}
	if (name == "command") {
		cls
		cout << "希沃克星允许使用命令行直接启动指定任务。\n";
		cout << "晚自习制裁：";
		SetColorAndBackground(0, 7);
		cout << executable_path << "\\SeewoKiller.exe wanzixi";
		SetColorAndBackground(7, 0);
		cout << "\n\n循环清任务：";
		SetColorAndBackground(0, 7);
		cout << executable_path << "\\SeewoKiller.exe taskkill";
		SetColorAndBackground(7, 0);
		cout << "\n\n连点器：";
		SetColorAndBackground(0, 7);
		cout << executable_path << "\\SeewoKiller.exe liandianqi";
		SetColorAndBackground(7, 0);
		cout << "\n\n冰点破解：";
		SetColorAndBackground(0, 7);
		cout << executable_path << "\\SeewoKiller.exe seewofreeze";
		SetColorAndBackground(7, 0);
		cout << "\n";
		while (true) {
			cout << "按b并回车以返回";
			string b;
			cin >> b;
			if (b == "b") {
				return;
			}
		}
	}
}

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
				cout << "界面没有显示？试试设置中的冰点还原疑难解答！\n";
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
			} else if (s == "冰点还原疑难解答") {
				help();
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
			} else if (s.find("在晚自习制裁/循环清任务时启用日志")!=string::npos) {
				string value=read_config(".\\settings\\write-log-when-killapp.seewokiller");
				value = "当前:" + value + "\n你要将此设置更改为什么？\n点击\"是\"设置为true，点击\"否\"设置为false，点击\"取消\"忽略修改";
				int ans = MessageBox(hwnd, value.c_str(), _T("修改变量"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						write_config(".\\settings\\write-log-when-killapp.seewokiller","true");
						if (MessageBox(hwnd, _T("修改完成，是否立即重新加载配置文件？\n日志文件保存在log文件夹下"), _T("提示"), MB_YESNO) == IDYES) {
							cls
							poweron(true);
						}
						break;
					}
					case IDNO: {
						write_config(".\\settings\\write-log-when-killapp.seewokiller","false");
						if (MessageBox(hwnd, _T("修改完成，是否立即重新加载配置文件？"), _T("提示"), MB_YESNO) == IDYES) {
							cls
							poweron(true);
						}
						break;
					}
				}
				s = "-1";
				continue;
			} else if (s == "命令行帮助") {
				help("command");
				s = "-1";
				continue;
			} else if (s == "打开日志文件夹") {
				string command = "explorer.exe \"" + executable_path + "\\log\\\"";
				system(command.c_str());
				s = "-1";
				continue;
			} else if (s.find("允许使用“关闭”按钮")!=string::npos) {
				string value=read_config(".\\settings\\enable-close-window-button.seewokiller");
				value = "当前:" + value + "\n你要将此设置更改为什么？\n点击\"是\"设置为true，点击\"否\"设置为false，点击\"取消\"忽略修改";
				int ans = MessageBox(hwnd, value.c_str(), _T("修改变量"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						write_config(".\\settings\\enable-close-window-button.seewokiller","true");
						MessageBox(hwnd, _T("修改完成，重启软件生效。"), _T("提示"), MB_OK);
						cls
						poweron(true);
						break;
					}
					case IDNO: {
						write_config(".\\settings\\enable-close-window-button.seewokiller","false");
						MessageBox(hwnd, _T("修改完成，重载配置文件后生效。"), _T("提示"), MB_OK);
						cls
						poweron(true);
						break;
					}
				}
				s = "-1";
				continue;
			} else if (s == "启动设置-当前:总是询问" or s == "启动设置-当前:总是使用旧UI" or s == "启动设置-当前:总是使用新UI") {
				ifstream file(".\\settings\\start.seewokiller");
				string value;
				getline(file, value);
				file.close();
				cout << "当前:" << s ;
				cout << "\n你要将此设置更改为什么？\n";
				cout << "输入\"0\"设置为“总是询问”，\n";
				cout << "输入\"1\"设置为“总是新UI”，\n";
				cout << "输入\"2\"设置为“总是旧UI”，\n";
				cout << "输入\"-1\"忽略修改\n";
				cout << "请输入：";
				int ans;
				cin >> ans;
				while (ans > 2 or ans < -1 or cin.fail()) {
					cout << "输入错误，请重试：";
					cin.clear();//清除错误数据
					cin.ignore(numeric_limits<streamsize>::max(), '\n');//丢弃错误输入
					cin >> ans;
				}
				switch (ans) {
					case 0: {
						ofstream file(".\\settings\\start.seewokiller");
						file << "0";
						file.close();
						MessageBox(hwnd, _T("修改完成，重启软件生效。"), _T("提示"), MB_OK);
						cls
						poweron(true);
						break;
					}
					case 1: {
						ofstream file(".\\settings\\start.seewokiller");
						file << "n";
						file.close();
						MessageBox(hwnd, _T("修改完成，即刻生效。"), _T("提示"), MB_OK);
						cls
						poweron(true);
						break;
					}
					case 2: {
						ofstream file(".\\settings\\start.seewokiller");
						file << "o";
						file.close();
						MessageBox(hwnd, _T("修改完成，即刻生效。"), _T("提示"), MB_OK);
						cls
						poweron(true);
						break;
					}
					default:
						break;
				}
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
					game("numberdamn");
				}
				if (d == "五子棋") {
					setfont(20);
					game("wzq");
					setfont(30);
				}
				if (d == "飞机大战") {
					cls
					setfont(20);
					game("fjdz");
					setfont(30);
				}
				if (d == "恶魔轮盘赌") {
					cls
					setfont(20);
					game("emlpd");
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
					word.settingn = 11;
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
				game("wzq");
			}
			if (cmd[2] == "-numberdamn") {
				game("numberdamn");
			}
			if (cmd[2] == "-fjdz") {
				ShowWindow(hwnd, SW_MAXIMIZE);
				game("fjdz");
			}
			if (cmd[2] == "-emlpd") {
				game("emlpd");
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
			if (cmd[2] == "-button") {
				ifstream file(".\\settings\\enable-close-window-button.seewokiller");
				string value;
				getline(file, value);
				file.close();
				value = "此设置仅用于旧UI\n当前:" + value + "\n你要将此设置更改为什么？\n点击\"是\"设置为true，点击\"否\"设置为false，点击\"取消\"忽略修改";
				int ans = MessageBox(hwnd, value.c_str(), _T("修改变量"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						ofstream file(".\\settings\\enable-close-window-button.seewokiller");
						file << "true";
						file.close();
						MessageBox(hwnd, _T("修改完成，重启软件生效。"), _T("提示"), MB_OK);
						break;
					}
					case IDNO: {
						ofstream file(".\\settings\\enable-close-window-button.seewokiller");
						file << "false";
						file.close();
						MessageBox(hwnd, _T("修改完成，重载配置文件后生效。"), _T("提示"), MB_OK);
						break;
					}
				}
			}
			if (cmd[2] == "-start") {
				ifstream file(".\\settings\\start.seewokiller");
				string value;
				getline(file, value);
				file.close();
				string s;
				if (value == "0") {
					s = "总是询问";
				} else if (value == "n") {
					s = "总是新UI";
				} else if (value == "o") {
					s = "总是旧UI";
				}
				cout << "当前:" << s ;
				cout << "\n你要将此设置更改为什么？\n";
				cout << "输入\"0\"设置为“总是询问”，\n";
				cout << "输入\"1\"设置为“总是新UI”，\n";
				cout << "输入\"2\"设置为“总是旧UI”，\n";
				cout << "输入\"-1\"忽略修改\n";
				cout << "请输入：";
				int ans;
				cin >> ans;
				while (ans > 2 or ans < -1 or cin.fail()) {
					cout << "输入错误，请重试：";
					cin.clear();//清除错误数据
					cin.ignore(numeric_limits<streamsize>::max(), '\n');//丢弃错误输入
					cin >> ans;
				}
				switch (ans) {
					case 0: {
						ofstream file(".\\settings\\start.seewokiller");
						file << "0";
						file.close();
						MessageBox(hwnd, _T("修改完成，重启软件生效。"), _T("提示"), MB_OK);
						break;
					}
					case 1: {
						ofstream file(".\\settings\\start.seewokiller");
						file << "n";
						file.close();
						MessageBox(hwnd, _T("修改完成，即刻生效。"), _T("提示"), MB_OK);
						break;
					}
					case 2: {
						ofstream file(".\\settings\\start.seewokiller");
						file << "o";
						file.close();
						MessageBox(hwnd, _T("修改完成，即刻生效。"), _T("提示"), MB_OK);
						break;
					}
					default:
						break;
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
