#include <bits/stdc++.h>
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <string>
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
bool closeapp = false;

int box = 1/*板块*/, boxn = 4/*板块总数*/;
struct Word {
	string box[5] {"NULL", "常用", "核心功能", "附加功能", "设置"};
	int recentn = 3;
	string recent[4] = {"NULL", "晚自习制裁模式", "一键防屏保", "小游戏>>>"};
	int alln = 7;
	string all[8] = {"NULL", "循环清任务", "一键卸载", "晚自习制裁模式", "一键防屏保", "小游戏>>>", "恶搞>>>", "注册表>>>"};
	int moren = 3;
	string more[4] = {"NULL", "冰点还原破解", "AI", "计算π"};
	int settingn = 5;
	string setting[6] = {"NULL", "退出", "在晚自习制裁/循环清任务时启用日志", "使用新版界面", "启动初学者引导", "关于"};
	int gamen = 3;
	string game[4] = {"NULL", "返回", "数字炸弹", "五子棋"};
	int joken = 3;
	string joke[4] = {"NULL", "返回", "杀WPS+希沃白板+希沃视频展台", "提取U盘文件"};
	int regn = 11;
	string reg[12] = {"NULL", "返回", "一键禁用(暂不可用)", "一键启用(暂不可用)", "禁用任务栏菜单", "启用任务栏菜单", "禁用快捷键", "启用快捷键", "启用显示登录详细信息", "禁用显示登录详细信息", "登录时显示提示", "取消登录时显示提示"};
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
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~(WS_MINIMIZEBOX);
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	ShowWindow(hwnd, SW_MAXIMIZE);//最大化
	DestroyMenu(hmenu);
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
				if (MessageBox(NULL, _T("你确实要跳过吗？\n本引导程序将会保留在设置板块中"), _T("鸡叫"), MB_YESNO) == IDYES) {
					cls
					return;
				}
			}
		}
	}
	cls
}
void poweron(bool SkipCheckWinVer) {
	//1深蓝，2深绿，3深青，4深红，5深紫，6深黄，7灰白（默认），8深灰
	//9浅蓝，10浅绿，11浅青，12浅红，13浅紫，14浅黄，15白色，0黑色
	connot_close_button();
	setfont(30);
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
	S(200);
	//检测Windows版本
	typedef void(__stdcall * NTPROC)(DWORD*, DWORD*, DWORD*);
	HINSTANCE hinst = LoadLibrary(TEXT("ntdll.dll"));//加载DLL
	NTPROC GetNtVersionNumbers = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers");//获取函数地址
	DWORD dwMajor, dwMinor, dwBuildNumber;
	GetNtVersionNumbers(&dwMajor, &dwMinor, &dwBuildNumber);
	gotoxy(15, 14);
	printf("Windows版本: %d.%d", dwMajor, dwMinor);
	cout << "(3/4)      ";
	gotoxy(15, 16);
	cout << "[=======             ]";
	S(400);
	dwMajorInt = static_cast<int>(dwMajor);
	dwMinorInt = static_cast<int>(dwMinor);
	float version = dwMajorInt + dwMinorInt * 0.1;
	if (SkipCheckWinVer == false) {
		if (version >= 6.1) {
			if (MessageBox(NULL, _T("检测到你的系统为Windows 7+，\n是否使用全新UI？"), _T("提示"), MB_OKCANCEL) == 1) {
				string guipath = executable_path + "\\gui.exe";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(guipath.c_str()));//有权限的都可以打开
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
				DWORD dwExitCode;
				if (fSuccess) {
					gotoxy(15, 16);
					cout << "[=============       ]";
					closeapp = true;
					S(10);
					gotoxy(15, 16);
					cout << "[====================]";
					S(200);
					return;
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
	S(10);
	gotoxy(15, 16);
	cout << "[====================]";
	S(100);
	setfont(30);
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
	cout << "\n希沃克星\n";
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
	cout << "按b+回车返回\n";
	string ans;
	while (true) {
		cin >> ans;
		if (ans == "b") {
			return;
		}
		if (ans == "zhuoran") {
			cout << "   =====       =====       =====\n";
			S(10);
			cout << " //     \\\\    ||    \\\\   //     \\\\\n";
			S(10);
			cout << " ||     ||    ||    ||   ||     ||\n";
			S(10);
			cout << " ||     ||    ||    //   ||     ||\n";
			S(10);
			cout << " ||     ||    ||====     ||     ||\n";
			S(10);
			cout << " ||     ||    ||         ||     ||\n";
			S(10);
			cout << " ||     ||    ||         ||     ||\n";
			S(10);
			cout << " \\\\     //    ||         \\\\     //\n";
			S(10);
			cout << "   =====      ||           =====\n";
			S(10);
			system("pause");
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
		cout << "正在结束进程：轻录播\n";
		cout << "TASKKILL /F /IM EasiRecorder.exe\n";
		system("TASKKILL /F /IM EasiRecorder.exe");
		if (KillSeewoService == true) {
			cout << "正在结束进程：希沃管家\n";
			cout << "TASKKILL /F /IM SeewoServiceAssistant.exe\n";
			system("TASKKILL /F /IM SeewoServiceAssistant.exe");
			cout << "TASKKILL /F /IM SeewoAbility.exe\n";
			system("TASKKILL /F /IM SeewoAbility.exe");
			cout << "TASKKILL /F /IM SeewoCore.exe\n";
			system("TASKKILL /F /IM SeewoCore.exe");
		}
		if (Wanzixi == true) {
			system("taskkill /f /t /im taskmgr.exe");
			cout << "正在结束进程：设置\n";
			cout << "TASKKILL /F /IM SystemSettings.exe\n";
			system("TASKKILL /F /IM SystemSettings.exe");
			cout << "正在结束进程：控制面板\n";
			cout << "TASKKILL /F /FI \"WINDOWTITLE eq 网络连接\"\n";
			system("taskkill /f /fi \"WINDOWTITLE eq 网络连接\"");
			system("taskkill /f /fi \"WINDOWTITLE eq 控制面板\\网络和 Internet\\网络连接\"");
			cout << "正在结束进程：Edge\n";
			cout << "TASKKILL /F /IM msedge.exe\n";
			system("TASKKILL /F /IM msedge.exe");
			cout << "正在结束进程：IE\n";
			cout << "TASKKILL /F /IM iexplore.exe\n";
			system("TASKKILL /F /IM iexplore.exe");
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

void pingbaoservice() {
	SetColorAndBackground(4, 6);
	cout << "警告：请勿用于正常上课！\n";
	SetColorAndBackground(0, 7);
	cout << "每100秒点击屏幕一次，请将鼠标移动至合适位置\n";
	system("pause");
	long long i = 1;
	while (true) {
		if ( _kbhit() ) {
			char x = _getch();
			switch (x) {
				case ' ': {
					return;
				}
			}
		}
		S(100000);
		cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << i;
		POINT cur_pos;
		GetCursorPos(&cur_pos);
		mouse_event(MOUSEEVENTF_LEFTDOWN, cur_pos.x, cur_pos.y, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, cur_pos.x, cur_pos.y, 0, 0);
		i++;
	}
}
typedef long long LL;
struct GAME {
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
} game;

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
		MessageBox(NULL, _T("此功能可以提取任意文件夹的所有内容，且支持U盘。\n你现在需要设置这些文件的位置和拷贝后存储的位置。"), _T("提示"), MB_OK);
		if (MessageBox(NULL, _T("本软件不对你使用此功能造成的任何损失(包括但不限于驱逐电教、被叫去和老师喝茶等)负责，请慎重考虑！"), _T("警告"), MB_YESNO | MB_ICONWARNING) == IDNO) {
			return;
		}
		MessageBox(NULL, _T("你需要在接下来的控制台中输入文件来源和拷贝后的文件去向。"), _T("提示"), MB_OK);
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
	void head() {
		system("title 希沃克星");
		cls
		gotoxy(0, 0);
		cout << "Seewo Killer";
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
				head();
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
			} else if (s == "循环清任务") {
				taskkill(true, false);
			} else if (s == "一键卸载") {
				uninstall();
				s = "-1";
				continue;
			} else if (s == "AI") {
				setfont(20);
				string aipath = executable_path + "\\ai.exe";
				system(aipath.c_str());
				system("pause");
				setfont(30);
				s = "-1";
				continue;
			} else if (s == "计算π") {
				setfont(20);
				string aipath = executable_path + "\\pai.exe";
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
				DWORD dwExitCode;
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
			} else if (s == "一键防屏保") {
				pingbaoservice();
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
			} else if (s == "在晚自习制裁/循环清任务时启用日志-当前:false" or s == "在晚自习制裁/循环清任务时启用日志-当前:true") {
				ifstream file(".\\settings\\write-log-when-killapp.seewokiller");
				string value;
				getline(file, value);
				value = "当前:" + value + "\n你要将此设置更改为什么？\n点击\"是\"设置为true，点击\"否\"设置为false，点击\"取消\"忽略修改";
				int ans = MessageBox(NULL, value.c_str(), _T("修改变量"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "true";
						file.close();
						if (MessageBox(NULL, _T("修改完成，是否立即重新加载配置文件？\n日志文件保存在log文件夹下"), _T("提示"), MB_YESNO) == IDYES) {
							cls
							poweron(true);
						}
						break;
					}
					case IDNO: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "false";
						file.close();
						if (MessageBox(NULL, _T("修改完成，是否立即重新加载配置文件？"), _T("提示"), MB_YESNO) == IDYES) {
							cls
							poweron(true);
						}
						break;
					}
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
					game.numberdamn();
					d = "返回";
				}
				if (d == "五子棋") {
					setfont(20);
					game.wzq.wzqmain();
					setfont(30);
					d = "返回";
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
					d = "返回";
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
					d = "返回";
				}
				if (d == "启用任务栏菜单") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
					regedit("HKEY_CURRENT_USER", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
					restartexp();
					d = "返回";
				}
				if (d == "禁用快捷键") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "1");
					restartexp();
					d = "返回";
				}
				if (d == "启用快捷键") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "0");
					restartexp();
					d = "返回";
				}
				if (d == "启用显示登录详细信息") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "1");
					cout << "修改完成，请注销以检查是否修改成功。\n";
					if (MessageBox(NULL, _T("你是否要现在注销？"), _T("注销确认"), MB_OKCANCEL) == 1) {//1确定，2取消
						system("shutdown /l");
					}
					system("pause");
					d = "返回";
				}
				if (d == "禁用显示登录详细信息") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "0");
					cout << "修改完成，请注销以检查是否修改成功。\n";
					system("pause");
					d = "返回";
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
					if (MessageBox(NULL, _T("你是否要现在注销？"), _T("注销确认"), MB_OKCANCEL) == 1) {//1确定，2取消
						system("shutdown /l");
					}
					system("pause");
					d = "返回";
				}
				if (d == "取消登录时显示提示") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", "");
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", "");
					cout << "修改完成，请注销以检查是否修改成功。\n";
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
		string cmd[100];
		for (int i = 0; i < argc; i++) {
			cmd[i] = argv[i];
		}
		//不需要admin
		if (cmd[1] == "about") {
			about();
			return 0;
		}
		if (cmd[1] == "game") { //游戏
			if (argc <= 2) {
				cout << "参数缺失，程序自动退出\n关于game的使用方法\n";
				cout << "-wzq五子棋\n-numberdamn数字炸弹\n";
			}
			if (cmd[2] == "-wzq") {
				ShowWindow(hwnd, SW_MAXIMIZE);
				game.wzq.wzqmain();
			}
			if (cmd[2] == "-numberdamn") {
				ShowWindow(hwnd, SW_MAXIMIZE);
				game.numberdamn();
			}
			return 0;
		}
		//需要admin
		if (IsUserAnAdmin() == false) {
			cout << "命令行未取得管理员权限，程序无法运行。\n请使用管理员权限启动终端。";
			return 0;
		}
		if (cmd[1] == "run") { //运行
			if (argc <= 2) {
				cout << "参数缺失，程序自动退出\n关于run的使用方法\n";
				cout << "-newui强制新UI\n-oldui强制旧UI\n";
				return 0;
			}
			if (cmd[2] == "-skipcheck") {
				skipcheck = true;
			}
			if (cmd[2] == "-oldui") {
				string unfreezepath = executable_path + "\\SeewoKiller.exe run -skipcheck";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//有权限的都可以打开
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
				DWORD dwExitCode;
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
				DWORD dwExitCode;
				if (fSuccess) {
					return 0;
				}
			}
		}
		if (cmd[1] == "taskkill") {
			while (true) {
				taskkill(true, false);
			}
		}
		if (cmd[1] == "wanzixi") {
			system("title 制裁晚自习");
			while (true) {
				taskkill(true, true);
			}
			return 0;
		}
		if (cmd[1] == "uninstall") {
			uninstall();
			return 0;
		}
		if (cmd[1] == "pingbao") {
			system("title 一键防屏保");
			pingbaoservice();
			return 0;
		}
		if (cmd[1] == "seewofreeze") {
			cout << "\n请先关闭冰点窗口后再继续操作。\n";
			string unfreezepath = executable_path + "\\SeewoFreeze\\SeewoFreezeUI.exe --startup-with-main-window";
			STARTUPINFO si = { sizeof(si) };//0
			PROCESS_INFORMATION pi;
			LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//有权限的都可以打开
			BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
			DWORD dwExitCode;
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
				int ans = MessageBox(NULL, value.c_str(), _T("修改变量"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "true";
						file.close();
						MessageBox(NULL, _T("设置完成\n日志文件保存在log文件夹下"), _T("修改变量"), MB_OK);
						break;
					}
					case IDNO: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "false";
						file.close();
						MessageBox(NULL, _T("设置完成"), _T("修改变量"), MB_OK);
						break;
					}
				}
			}
			return 0;
		}
		if (cmd[1] == "joke") { //恶搞
			if (argc <= 2) {
				cout << "参数缺失，程序自动退出\n关于joke的使用方法\n";
				cout << "-killapp杀上课用软件\n";
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
					if (MessageBox(NULL, _T("你是否要现在注销？"), _T("注销确认"), MB_OKCANCEL) == 1) {//1确定，2取消
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
					if (MessageBox(NULL, _T("你是否要现在注销？"), _T("注销确认"), MB_OKCANCEL) == 1) {//1确定，2取消
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
	poweron(skipcheck);
	if (closeapp == true) {
		return 0;
	}
	cls
	//S(10);
	//if (MessageBox(NULL, _T("你干嘛哎呦"), _T("鸡叫"), MB_OKCANCEL) == 2) {
	//	return 0;
	//}//返回1确定，2取消
	//获取程序路径
	system("title 希沃克星");
	lc.lcmain();
	return 0;
}
