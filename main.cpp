#include <bits/stdc++.h>
#include <windows.h>
#include <iostream>
#include <ctime>

//获取管理员权限所需
#include <tchar.h>
#include <shellapi.h>

//猜数字：cin错误去除
#include <limits>

#define S(i) Sleep(i)
#define cls system("cls");
#define ei else if
using namespace std;
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
struct CMDset {
	void setfont(int size) {//字体、大小、粗细
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof cfi;
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = size;//设置字体大小
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_BOLD;//字体粗细 FW_BOLD,原始为FW_NORMAL
		wcscpy_s(cfi.FaceName, L"Consolas");//设置字体，必须是控制台已有的
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_FONT_INFO consoleCurrentFont;
		GetCurrentConsoleFont(handle, FALSE, &consoleCurrentFont);
	}
} cmdset;

struct Start {
	void powerOn() {
		ShowWindow(hwnd, SW_MAXIMIZE);
		cmdset.setfont(30);
		cout << "\n  ";
		SetColorAndBackground(4, 12);
		cout << "W";
		SetColorAndBackground(6, 14);
		cout << "H";
		SetColorAndBackground(2, 10);
		cout << "S";
		SetColorAndBackground(1, 9);
		cout << "T";
		SetColorAndBackground(13, 5);
		cout << "U";
		SetColorAndBackground(0, 0);
		cout << " ";
		SetColorAndBackground(15, 8);
		cout << "Studio";
		SetColorAndBackground(7, 0);
		S(5000);
		cls
		gotoxy(13, 10);
		cout << "S";
		Sleep(1000);
		cout << "\b\bS  ";
		S(100);
		cout << "\b\b\b\bS  ";
		S(100);
		cout << "\b\b\b\bS  ";
		S(100);
		cout << "\b\b\b\bS  ";
		S(100);
		cout << "\b\b\b\bS  ";
		S(100);
		cout << "\b\b\b\bS \b";
		S(100);
		cout << "e";
		S(100);
		cout << "e";
		S(100);
		cout << "w";
		S(100);
		cout << "o";
		S(100);
		cout << " ";
		S(100);
		cout << " ";
		S(100);
		cout << "K";
		S(100);
		cout << "i";
		S(100);
		cout << "l";
		S(100);
		cout << "l";
		S(100);
		cout << "e";
		S(100);
		cout << "r";
		S(4000);
		system("cls");
		cmdset.setfont(16);
		return;
	}
} start;

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
	cout << "\nSeewo Killer 1.1\n";
	cout << "\n版本代号OPO 1\n";
	cout << "\n希沃克星\n";
	cout << "\n卓然第三帝国联合赞助\n";
	cout << "按b+回车返回\n";
	string ans;
	while (true) {
		cin >> ans;
		if (ans == "b") {
			return;
		}
		if (ans == "zhuoran") {
			cout << "   =====       =====       =====        //\n";
			S(10);
			cout << " //     \\\\    ||    \\\\   //     \\\\    / ||\n";
			S(10);
			cout << " ||     ||    ||    ||   ||     ||      ||\n";
			S(10);
			cout << " ||     ||    ||    //   ||     ||      ||\n";
			S(10);
			cout << " ||     ||    ||====     ||     ||      ||\n";
			S(10);
			cout << " ||     ||    ||         ||     ||      ||\n";
			S(10);
			cout << " ||     ||    ||         ||     ||      ||\n";
			S(10);
			cout << " \\\\     //    ||         \\\\     //      ||\n";
			S(10);
			cout << "   =====      ||           =====     ========\n";
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

void taskkill(bool KillSeewoService) {
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
	return;
}

void uninstall() {
	cls
	cout << "正在卸载EasiRecorder\n";
	system("\"C:\\Program Files (x86)\\Seewo\\EasiRecorder\\Uninstall.exe\"");
	cout << "正在卸载Easicare\n";
	system("\"C:\\Program Files (x86)\\Seewo\\Easicare\\Uninstall.exe\"");
	cout << "正在卸载EasiAgent\n";
	system("\"C:\\Program Files (x86)\\Seewo\\EasiAgent\\Uninstall.exe\"");
	return;
}

struct GAME {
	void numberdamn() {
		cls
		cmdset.setfont(300);
		cout << "数";
		S(100);
		cout << "字";
		S(100);
		cout << "炸";
		S(150);
		gotoxy(0, 0);
		cls
		S(100);
		cmdset.setfont(500);
		SetColorAndBackground(6, 4);
		cout << "DAMN";
		SetColorAndBackground(7, 0);
		S(700);
		cls
		cmdset.setfont(30);
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
		cmdset.setfont(16);
	}
} game;

int main() {
	system("title 正在初始化");
	srand((unsigned)time(NULL));
	system("title 正在检测管理员");
	if (IsUserAnAdmin() == false) {
		if (getadmin() == false) {
			return 0;
		}
	}
	system("title Seewo Killer Starting");
	start.powerOn();
	cls
	//S(10);
	//if (MessageBox(NULL, _T("你干嘛哎呦"), _T("鸡叫"), MB_OKCANCEL) == 2) {
	//	return 0;
	//}//返回1确定，2取消
	//获取程序路径
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	string executable_path = path;
	size_t position = executable_path.find_last_of('\\');
	executable_path = executable_path.substr(0, position);
	string xwbbpath = executable_path;
	//选择界面
	while (true) {
		cls
		system("title 希沃克星");
		int choose;
		SetColorAndBackground(7, 0);
		cout << "1:退出  2:循环清任务  3:一键卸载不需要的软件  4:冰点解冻";
		SetColorAndBackground(6, 5);
		cout << "Beta";
		SetColorAndBackground(7, 0);
		cout << "  5:晚自习制裁模式  6:获取管理员权限  7:小游戏  8:关于";
		cout << "\n请选择：";
		cin >> choose;
		switch (choose) {
			case 1:
				return 0;
			case 2: {
				system("title 循环清任务");
				while (true) {
					taskkill(true);
					cls
				}
			}
			case 3: {
				system("title 卸载");
				uninstall();
				break;
			}
			case 4: {
				system("title 冰点还原");
				string password = "seewofreeze";
				string input;
				for (;;) {
					cout << "\n请输入密码：";
					cin >> input;
					if (input == password) {
						break;
					} else {
						cout << "密码错误";
						continue;
					}
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
				break;
			}
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
			case 5: {
				system("title 制裁晚自习");
				while (true) {
					taskkill(true);
					cout << "正在结束进程：设置\n";
					cout << "TASKKILL /F /IM SystemSettings.exe\n";
					system("TASKKILL /F /IM SystemSettings.exe");
					cout << "正在结束进程：控制面板\n";
					cout << "TASKKILL /F /FI \"WINDOWTITLE eq 网络连接\"\n";
					system("taskkill /f /fi \"WINDOWTITLE eq 网络连接\"");
					cout<<"正在结束进程：Edge\n";
					cout << "TASKKILL /F /IM msedge.exe\n";
					system("TASKKILL /F /IM msedge.exe");
					cls
				}
			}
			case 6: {
				if (IsUserAnAdmin() == false) {
					if (getadmin() == false) {
						return 0;
					}
				} else {
					cout << "已经获得管理员权限！\n";
				}
				system("pause");
				break;
			}
			case 7: {
				system("title 玩游戏");
				int ans;
				cout << "1:数字炸弹  2:返回\n";
				cout << "请选择游戏:";
				cin >> ans;
				switch (ans) {
					case 1: {
						game.numberdamn();
						break;
					}
					case 2:
						break;
					default: {
						if (cin.fail()) {
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							cout << "\n输入错误，请检查是否输入了字母";
						}
					}
				}
				break;
			}
			case 8: {
				system("title 关于");
				about();
				break;
			}
			default: {
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "\n输入错误，请检查是否输入了字母";
				}
				break;
			}
		}
	}
	return 0;
}
