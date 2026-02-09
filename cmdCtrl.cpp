#include <windows.h>


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
