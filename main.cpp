#include <bits/stdc++.h>
#include <cstdio>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <iostream>
#include <cstring>
#include <ctime>
#include <io.h>
#include <fstream>
#include <sys/stat.h>
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
		cfi.FontWeight = FW_NORMAL;//字体粗细 FW_BOLD,原始为FW_NORMAL
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
	S(1000);
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
	S(3000);
	cout << "\n";
	//写入版本号
	CreateDirectory("./info", NULL );
	ofstream info("./info/info.txt");
	info << "Seewo Killer 0.1_Build20240818125505_DeveloperBeta1" << endl;
	info << "版本代号000101" << endl;
	info << "注意：请不要在此处留下重要信息，因为此文件会被SlytherinOS覆盖！";
	info.close();
	//----------
	cout << "\nSeewo Killer 0.1_Build20240818125505_DeveloperBeta1\n";
	cout << "\n版本代号000101\n";
	cout << "\nSeewo Killer\n";
	cout << "\n卓然第三帝国联合赞助\n";
}

int main() {
	//start.powerOn();
	//cls
	//S(10);
	//cls
	cout << "正在结束进程：轻录播\n";
	cout << "TASKKILL /F /IM EasiRecorder.exe\n";
	system("TASKKILL /F /IM EasiRecorder.exe");
	system("\"C:\\Program Files (x86)\\Seewo\\EasiRecorder\\Uninstall.exe\"");
	cout << "完成\n";

	cout << "正在结束进程：班级优化大师\n";
	cout << "TASKKILL /F /IM EasiCare.exe\n";
	system("TASKKILL /F /IM EasiCare.exe");
	cout << "TASKKILL /F /IM EasiCareLauncher.exe\n";
	system("TASKKILL /F /IM EasiCareLauncher.exe");
	system("\"C:\\Program Files (x86)\\Seewo\\Easicare\\Uninstall.exe\"");

	cout<<"卸载EasiAgent\n";
	system("\"C:\\Program Files (x86)\\Seewo\\EasiAgent\\Uninstall.exe\"");
	system("pause");
	return 0;
}
/*
Seewo Killer 版本0.1
最后更新:20240818

更新日志
20240818
*/
