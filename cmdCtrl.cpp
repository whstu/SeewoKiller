#include "./cmdCtrl.h"

HWND hwnd = GetConsoleWindow();
void SetColorAndBackground(int ForgC, int BackC) {//单个字的颜色
//1深蓝，2深绿，3深青，4深红，5深紫，6深黄，7灰白（默认），8深灰
//9浅蓝，10浅绿，11浅青，12浅红，13浅紫，14浅黄，15白色，0黑色
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}
void gotoxy(long long x, long long y,bool CN) {
	COORD pos;
	if(CN){
		pos.X = 2 * x;
	}else{
		pos.X=x;
	}
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

void prints(const string& taskname, int result, int taskColorForg, int taskColorBack, const string& connectString) {
	//Print Status
	SetColorAndBackground(taskColorForg, taskColorBack);
	cout << taskname;
	SetColorAndBackground(7, 0);
	cout << connectString;
	if (result == 0) {
		SetColorAndBackground(0, 10);
		cout << "Done";
	} else {
		SetColorAndBackground(0, 12);
		cout << "Failed";
	}
	SetColorAndBackground(7, 0);
	return;
}

void ClearLine(bool ClearNextLine) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		// 获取当前光标位置
		COORD cursorPos = csbi.dwCursorPosition;

		// 计算当前行的起始位置
		COORD startPos = {0, cursorPos.Y};
		COORD startPos2 = {0, cursorPos.Y + 1};

		// 计算需要清除的字符数（从行首到行尾）
		DWORD charsToWrite = csbi.dwSize.X - 1;  // 一行长度减1

		// 填充空格字符
		DWORD written;
		FillConsoleOutputCharacter(
		    hConsole,           // 控制台句柄
		    ' ',                // 填充字符
		    charsToWrite,       // 填充数量
		    startPos,           // 起始位置
		    &written            // 实际写入数量
		);
		if (ClearNextLine) {
			DWORD written;
			FillConsoleOutputCharacter(
			    hConsole,           // 控制台句柄
			    ' ',                // 填充字符
			    charsToWrite,       // 填充数量
			    startPos2,           // 起始位置
			    &written            // 实际写入数量
			);
		}
		// 将光标移回行首
		//SetConsoleCursorPosition(hConsole, startPos);
		gotoxy(cursorPos.X,cursorPos.Y,false);
	}
}
