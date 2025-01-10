#include <bits/stdc++.h>
#include <windows.h>
#include <iostream>
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

#define S(i) Sleep(i)
#define cls system("cls");
#define ei else if
using namespace std;

char path[MAX_PATH];
string executable_path;
size_t position;
string xwbbpath;

int box = 1/*板块*/, boxn = 3/*板块总数*/;
struct Word {
	string box[4] {"NULL", "常用", "所有", "设置"};
	int recentn = 3;
	string recent[4] = {"NULL", "晚自习制裁模式", "一键防屏保", "小游戏"};
	int alln = 8;
	string all[9] = {"NULL", "循环清任务", "一键卸载", "冰点解冻", "晚自习制裁模式", "一键防屏保", "小游戏", "恶搞", "注册表"};
	int settingn = 2;
	string setting[5] = {"NULL", "退出", "关于"};
	int gamen = 3;
	string game[4] = {"NULL", "返回", "数字炸弹", "五子棋"};
	int joken = 2;
	string joke[3] = {"NULL", "返回", "杀WPS+希沃白板+希沃视频展台"};
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

void poweron() {
	connot_close_button();
	setfont(30);
	S(500);
	SetColorAndBackground(0, 4);
	for (long long i = 0; i < 4; i++) {
		for (long long j = 0; j < 150; j++) {
			cout << " ";
		}
		S(10);
		cout << "\n";
	}
	SetColorAndBackground(0, 6);
	for (long long i = 0; i < 150; i++) {
		cout << " ";
	}
	S(10);
	cout << "\n";
//第2行
	for (int i = 0; i < 3; i++) {//W-1
		SetColorAndBackground(0, 6);
		cout << "   ";
		SetColorAndBackground(0, 0);
		cout << "  ";
	}
	SetColorAndBackground(0, 6);
	cout << "   ";
	SetColorAndBackground(0, 0);//H-1a
	cout << "  ";
	SetColorAndBackground(0, 6);//H-1mid
	cout << "       ";
	SetColorAndBackground(0, 0);//H-1b
	cout << "  ";
	SetColorAndBackground(0, 6);
	cout << "      ";
	SetColorAndBackground(0, 0);//S-1
	cout << "         ";
	SetColorAndBackground(0, 6);
	cout << "    ";
	SetColorAndBackground(0, 0);//T-1
	cout << "            ";
	SetColorAndBackground(0, 6);
	cout << "    ";
	SetColorAndBackground(0, 0);//U-1a
	cout << "  ";
	SetColorAndBackground(0, 6);
	cout << "       ";
	SetColorAndBackground(0, 0);//U-1b
	cout << "  ";
	SetColorAndBackground(0, 6);
	cout << "                    ";
	S(10);
	cout << "\n";
//第3,4行
	for (int a = 0; a < 2; a++) {
		for (int i = 0; i < 3; i++) {//W-2
			SetColorAndBackground(0, 6);
			cout << "   ";
			SetColorAndBackground(0, 0);
			cout << "  ";
		}
		SetColorAndBackground(6, 6);
		cout << "   ";
		SetColorAndBackground(0, 0);//H-2a
		cout << "  ";
		SetColorAndBackground(6, 6);//H-2mid
		cout << "       ";
		SetColorAndBackground(0, 0);//H-2b
		cout << "  ";
		SetColorAndBackground(6, 6);
		cout << "    ";
		SetColorAndBackground(0, 0); //S-2
		cout << "  ";
		SetColorAndBackground(6, 6);
		cout << "                  ";
		SetColorAndBackground(0, 0); //T-2
		cout << "  ";
		SetColorAndBackground(6, 6);
		cout << "         ";
		SetColorAndBackground(0, 0); //U-2a
		cout << "  ";
		SetColorAndBackground(6, 6);
		cout << "       ";
		SetColorAndBackground(0, 0); //U-2b
		cout << "  ";
		SetColorAndBackground(6, 6);
		cout << "                    ";
		S(10);
		cout << "\n";
	}
//第5行
	for (int i = 0; i < 3; i++) {//W-4
		SetColorAndBackground(0, 6);
		cout << "   ";
		SetColorAndBackground(0, 0);
		cout << "  ";
	}
	SetColorAndBackground(0, 6);
	cout << "   ";
	SetColorAndBackground(0, 0); //H-4
	cout << "           ";
	SetColorAndBackground(0, 6);
	cout << "      ";
	SetColorAndBackground(0, 0); //S-4
	cout << "       ";
	SetColorAndBackground(0, 6);
	cout << "           ";
	SetColorAndBackground(0, 0); //T-4
	cout << "  ";
	SetColorAndBackground(0, 6);
	cout << "         ";
	SetColorAndBackground(0, 0); //U-4a
	cout << "  ";
	SetColorAndBackground(0, 6);
	cout << "       ";
	SetColorAndBackground(0, 0); //U-4b
	cout << "  ";
	SetColorAndBackground(0, 6);
	cout << "                    ";
	S(10);
	cout << "\n";
//第1行
	for (int i = 0; i < 3; i++) {//W-5
		SetColorAndBackground(0, 2);
		cout << "   ";
		SetColorAndBackground(0, 0);
		cout << "  ";
	}
	SetColorAndBackground(0, 2);
	cout << "   ";
	SetColorAndBackground(0, 0);//H-5a
	cout << "  ";
	SetColorAndBackground(0, 2);//H-5mid
	cout << "       ";
	SetColorAndBackground(0, 0);//H-5b
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "             ";
	SetColorAndBackground(0, 0); //S-5
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "         ";
	SetColorAndBackground(0, 0); //T-5
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "         ";
	SetColorAndBackground(0, 0); //U-5a
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "       ";
	SetColorAndBackground(0, 0); //U-5b
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "                    ";
	S(10);
	cout << "\n";
//第2行
	for (int i = 0; i < 3; i++) {//W-6
		SetColorAndBackground(0, 2);
		cout << "   ";
		SetColorAndBackground(0, 0);
		cout << "  ";
	}
	SetColorAndBackground(0, 2);
	cout << "   ";
	SetColorAndBackground(0, 0);//H-6a
	cout << "  ";
	SetColorAndBackground(0, 2);//H-6mid
	cout << "       ";
	SetColorAndBackground(0, 0);//H-6b
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "             ";
	SetColorAndBackground(0, 0); //S-6
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "         ";
	SetColorAndBackground(0, 0); //T-6
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "         ";
	SetColorAndBackground(0, 0); //U-6a
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "       ";
	SetColorAndBackground(0, 0); //U-6b
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "                    ";
	S(10);
	cout << "\n";
//第3行
	SetColorAndBackground(0, 2);
	cout << "     ";
	SetColorAndBackground(0, 0);
	cout << "   ";
	SetColorAndBackground(0, 2);
	cout << "  ";
	SetColorAndBackground(0, 0);
	cout << "   ";
	SetColorAndBackground(0, 2);
	cout << "     ";
	SetColorAndBackground(0, 0);//H-7a
	cout << "  ";
	SetColorAndBackground(0, 2);//H-7mid
	cout << "       ";
	SetColorAndBackground(0, 0);//H-7b
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "    ";
	SetColorAndBackground(0, 0); //S-7
	cout << "         ";
	SetColorAndBackground(0, 2);
	cout << "           ";
	SetColorAndBackground(0, 0); //T-7
	cout << "  ";
	SetColorAndBackground(0, 2);
	cout << "           ";
	SetColorAndBackground(0, 0); //U-7a
	cout << "       ";
	SetColorAndBackground(0, 2);
	cout << "                      ";
	S(10);
	cout << "\n";

	SetColorAndBackground(0, 2);
	for (long long i = 0; i < 2; i++) {
		for (long long j = 0; j < 150; j++) {
			cout << " ";
		}
		S(10);
		cout << "\n";
	}
	SetColorAndBackground(0, 1);
	for (long long i = 0; i < 31; i++) {
		cout << " ";
	}
	SetColorAndBackground(0, 7);
	cout << " SEEWO  KILLER ";
	SetColorAndBackground(0, 1);
	for (long long i = 0; i < 100; i++) {
		cout << " ";
	}
	SetColorAndBackground(0, 1);
	for (long long i = 0; i < 4; i++) {
		for (long long j = 0; j < 150; j++) {
			cout << " ";
		}
		S(10);
		cout << "\n";
	}
	SetColorAndBackground(0, 5);
	for (long long i = 0; i < 5; i++) {
		for (long long j = 0; j < 150; j++) {
			cout << " ";
		}
		S(10);
		cout << "\n";
	}
	SetColorAndBackground(7, 0);
	gotoxy(0, 0);
	S(1000);
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
	cout << "\nSeewo Killer 2.0 Beta\n";
	cout << "\n版本代号  郑子谦\n";
	cout << "\n希沃克星\n";
	cout << "\n卓然第三帝国https://whstu.us.kg/提供技术支持\n";
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
	while (true) {
		if ( _kbhit() ) {
			return;
		}
		system("taskkill /f /t /im taskmgr.exe");
		system("taskkill /f /t /im cmd.exe");
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
			cout << "正在结束进程：设置\n";
			cout << "TASKKILL /F /IM SystemSettings.exe\n";
			system("TASKKILL /F /IM SystemSettings.exe");
			cout << "正在结束进程：控制面板\n";
			cout << "TASKKILL /F /FI \"WINDOWTITLE eq 网络连接\"\n";
			system("taskkill /f /fi \"WINDOWTITLE eq 网络连接\"");
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
		for (ans = rand() % (max - min); ans == 0; ans = rand() % (max - min)) {
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
} joke;

struct Launcher {
	int listname(bool allowA, bool allowD, string liststring[], int n) {
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
						return -1;
					}
					case 'd': {
						if (allowD == true) {
							if (box < boxn) {
								box++;
							}
						}
						return -1;
					}
					case ' ': {
						return channel;
					}
				}
			}
		}
		return -2;
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
		head();
		int s = listname(true, true, word.recent, word.recentn);
		while (1) {
			//主页面
			switch (s) {
				case 1: {
					switch (box) {
						case 1: {
							box = 2;
							s = 4;
							break;
						}
						case 2: {
							taskkill(true, false);
							break;
						}
						case 3: {
							//退出
							return;
						}
					}
					break;
				}
				case 2: {
					switch (box) {
						case 1: {
							s = 5;
							box = 2;
							break;
						}
						case 2: {
							uninstall();
							s = -1;
							break;
						}
						case 3: {
							about();
							s = -1;
							break;
						}
					}
					break;
				}
				case 3: {
					switch (box) {
						case 1: {
							s = 6;
							box = 2;
							break;
						}
						case 2: {
							system("title 冰点还原");
							bool back = false;
							string password = "seewofreeze";
							string input;
							for (;;) {
								cout << "\n请输入密码(输入0返回)：";
								cin >> input;
								if (input == password) {
									break;
								} else {
									if (input == "0") {
										back = true;
										break;
									} else {
										cout << "密码错误";
									}
								}
							}
							if (back == true) {
								break;
							}
							string unfreezepath = executable_path + "\\!SeewoFreezeUI.bat";
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
							system("pause");
							s = -1;
							break;
						}
					}
					break;
				}
				case 4: {
					switch (box) {
						case 2: {
							system("title 制裁晚自习");
							taskkill(true, true);
							s=-1;
							break;
						}
					}
					break;
				}
				case 5: {
					switch (box) {
						case 2: {
							pingbaoservice();
							s=-1;
							break;
						}
					}
					break;
				}
				case 6: {
					switch (box) {
						case 2: {
							head();
							int d = listname(true, true, word.game, word.gamen);
							switch (d) {
								case 1: {
									d = 0;
									s = -1;
									break;
								}
								case 2: {
									game.numberdamn();
									break;
								}
								case 3: {
									setfont(20);
									game.wzq.wzqmain();
									setfont(30);
									break;
								}
							}
							break;
						}
					}
					break;
				}
				case 7: {
					switch (box) {
						case 2: {
							head();
							int d = listname(true, true, word.joke, word.joken);
							switch (d) {
								case 1: {
									d = 0;
									s = -1;
									break;
								}
								case 2: {
									joke.kill();
									break;
								}
							}
							break;
						}
					}
					break;
				}
				case 8: {
					switch (box) {
						case 2: {
							head();
							int d = listname(true, true, word.reg, word.regn);
							switch (d) {
								case 1: {
									d = 0;
									s = -1;
									break;
								}
								case 2: {
									//全部禁用
									break;
								}
								case 3: {
									//全部恢复
									break;
								}
								case 4: {
									regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "1");
									regedit("HKEY_CURRENT_USER", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "1");
									system("TASKKILL /F /IM explorer.exe");
									cout << "杀进程成功，5秒后尝试重启\n";
									Sleep(5000);
									system("start C:\\Windows\\explorer.exe");
									cout << "恢复中\n";
									Sleep(2000);
									system("start C:\\Windows\\explorer.exe");
									break;
								}
								case 5: {
									regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
									regedit("HKEY_CURRENT_USER", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
									system("TASKKILL /F /IM explorer.exe");
									cout << "杀进程成功，5秒后尝试重启\n";
									Sleep(5000);
									system("start C:\\Windows\\explorer.exe");
									cout << "恢复中\n";
									Sleep(2000);
									system("start C:\\Windows\\explorer.exe");
									break;
								}
								case 6: {
									regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "1");
									system("TASKKILL /F /IM explorer.exe");
									cout << "杀进程成功，5秒后尝试重启\n";
									Sleep(5000);
									system("start C:\\Windows\\explorer.exe");
									cout << "恢复中\n";
									Sleep(2000);
									system("start C:\\Windows\\explorer.exe");
									break;
								}
								case 7: {
									regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "0");
									system("TASKKILL /F /IM explorer.exe");
									cout << "杀进程成功，5秒后尝试重启\n";
									Sleep(5000);
									system("start C:\\Windows\\explorer.exe");
									cout << "恢复中\n";
									Sleep(2000);
									system("start C:\\Windows\\explorer.exe");
									break;
								}
								case 8: {
									regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "1");
									cout << "修改完成，请注销以检查是否修改成功。\n";
									if (MessageBox(NULL, _T("注销确认(Beta)"), _T("你是否要现在注销？"), MB_OKCANCEL) == 1) {//1确定，2取消
										system("shutdown /l");
									}
									system("pause");
									break;
								}
								case 9: {
									regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "0");
									cout << "修改完成，请注销以检查是否修改成功。\n";
									system("pause");
									break;
								}
								case 10: {
									char title1[1010100];
									char title2[1010100];
									cout << "请输入主标题(505050字以内)：";
									scanf_s("%s", title1, (unsigned)_countof(title1));
									cout << "请输入副标题(505050字以内)：";
									scanf_s("%s", title2, (unsigned)_countof(title2));
									regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", title1);
									regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", title2);
									cout << "修改完成，请注销以检查是否修改成功。\n";
									if (MessageBox(NULL, _T("注销确认(Beta)"), _T("你是否要现在注销？"), MB_OKCANCEL) == 1) {//1确定，2取消
										system("shutdown /l");
									}
									system("pause");
									break;
								}
								case 11: {
									regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", "");
									regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", "");
									cout << "修改完成，请注销以检查是否修改成功。\n";
									system("pause");
									break;
								}
							}
							break;
						}
					}
					break;
				}
				case -1: {
					head();
					switch (box) {
						case 1: {
							s = listname(true, true, word.recent, word.recentn);
							break;
						}
						case 2: {
							s = listname(true, true, word.all, word.alln);
							break;
						}
						case 3: {
							s = listname(true, true, word.setting, word.settingn);
							break;
						}
					}
					break;
				}
			}
		}
	}
} lc;

int main(int argc, char *argv[]) {
	system("title 正在初始化");
	srand((unsigned)time(NULL));
	system("title 正在检测管理员");
	//启动参数
	if (argc > 1) {
		if (IsUserAnAdmin() == false) {
			cout << "命令行未取得管理员权限，程序无法运行。\n请使用管理员权限启动终端。";
			return 0;
		}
		string cmdinput=argv[1];
		if (cmdinput == "wanzixi") {
			system("title 制裁晚自习");
			while (true) {
				taskkill(true, true);
			}
			return 0;
		}
		if (cmdinput == "uninstall") {
			uninstall();
			return 0;
		}
		if (cmdinput == "pingbao") {
			system("title 一键防屏保");
			pingbaoservice();
			return 0;
		}
		if(cmdinput=="joke"){//恶搞 
			if(argc<=2){
				cout<<"参数缺失，程序自动退出\n";
				return 0;
			}
			string cmdinput2=argv[2];
			if(cmdinput2=="-killapp"){
				joke.kill();
			}
		}
		if(cmdinput=="game"){//游戏 
			if(argc<=2){
				cout<<"参数缺失，程序自动退出\n";
				return 0; 
			}
			string cmdinput2=argv[2];
			if(cmdinput2=="-wzq"){
				game.wzq.wzqmain();
			}
			if(cmdinput2=="-numberdamn"){
				game.numberdamn();
			}
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
	poweron();
	cls
	//S(10);
	//if (MessageBox(NULL, _T("你干嘛哎呦"), _T("鸡叫"), MB_OKCANCEL) == 2) {
	//	return 0;
	//}//返回1确定，2取消
	//获取程序路径
	system("title 希沃克星");
	GetModuleFileNameA(NULL, path, MAX_PATH);
	executable_path = path;
	position = executable_path.find_last_of('\\');
	executable_path = executable_path.substr(0, position);
	xwbbpath = executable_path;
	lc.lcmain();
	/*case 5: {
					string xwbbsetpath = xwbbpath + "\\set.bat";
					STARTUPINFO si = { sizeof(si) };//0
					PROCESS_INFORMATION pi;
					LPTSTR szCommandLine = _tcsdup(TEXT(xwbbsetpath.c_str()));//有权限的都可以打开
					BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
					DWORD dwExitCode;
					if (fSuccess) { //把主进程暂停，等待子进程终止
						CloseHandle(pi.hThread);
						//暂停主进程的执行，直到child终止，该代码才可以继续运行
						WaitForSingleObject(pi.hProcess, INFINITE);
						CloseHandle(pi.hProcess);
					}
					break;
				}
				case 6: {
					string xwbbsetpath = xwbbpath + "\\restore.bat";
					STARTUPINFO si = { sizeof(si) };//0
					PROCESS_INFORMATION pi;
					LPTSTR szCommandLine = _tcsdup(TEXT(xwbbsetpath.c_str()));//有权限的都可以打开
					BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//参数意义
					DWORD dwExitCode;
					if (fSuccess) { //把主进程暂停，等待子进程终止
						CloseHandle(pi.hThread);
						//暂停主进程的执行，直到child终止，该代码才可以继续运行
						WaitForSingleObject(pi.hProcess, INFINITE);
						CloseHandle(pi.hProcess);
					}
					break;
				}*/
	return 0;
}
