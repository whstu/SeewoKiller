#include <bits/stdc++.h>
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <string>
//������������
#define INITGUID
#include <shobjidl.h>
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Ole32.lib")
const GUID IID_ITaskbarList3 = {// �ֶ�����IID_ITaskbarList3
	0xea1afb91, 0x9e28, 0x4b86,
	{0x90, 0xe9, 0x9e, 0x9f, 0x8a, 0x5e, 0xef, 0xaf}
};
//����explorer.exe
#include <tlhelp32.h>
//ע����޸�
#include <tchar.h>
//��Ϸ
#define N 25
#define Forij(x) for(int i=1;i<=x;i++)for(int j=1;j<=x;j++)
//��ȡ����ԱȨ������
#include <tchar.h>
#include <shellapi.h>
//cin����ȥ��
#include <limits>
//�ļ�����
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

int box = 1/*���*/, boxn = 4/*�������*/;
struct Word {
	string box[5] {"NULL", "����", "���Ĺ���", "���ӹ���", "����"};
	int recentn = 4;
	string recent[5] = {"NULL", "һ����ϣ������", "����ϰ�Ʋ�ģʽ", "������(�ɷ�����)", "С��Ϸ>>>"};
	int alln = 8;
	string all[9] = {"NULL", "ѭ��������(�Ͽη�����)", "һ��ж��", "����ϰ�Ʋ�ģʽ", "������(�ɷ�����)", "һ����ϣ������", "С��Ϸ>>>", "���>>>", "ע���>>>"};
	int moren = 3;
	string more[4] = {"NULL", "���㻹ԭ�ƽ�", "AI", "�����"};
	int settingn = 7;
	string setting[9] = {"NULL", "�˳�", "������ϰ�Ʋ�/ѭ��������ʱ������־", "����־�ļ���", "������ѧ������", "ʹ���°����", "������fastboot(���fast!)", "����", "������ѡ��>>>"};
	int gamen = 5;
	string game[6] = {"NULL", "����", "����ը��", "������", "�ɻ���ս", "��ħ���̶�"};
	int joken = 3;
	string joke[4] = {"NULL", "����", "ɱWPS+ϣ�ְװ�+ϣ����Ƶչ̨", "��ȡU���ļ�"};
	int regn = 9;
	string reg[10] = {"NULL", "����", "�����������˵�", "�����������˵�", "���ÿ�ݼ�", "���ÿ�ݼ�", "������ʾ��¼��ϸ��Ϣ", "������ʾ��¼��ϸ��Ϣ", "��¼ʱ��ʾ��ʾ", "ȡ����¼ʱ��ʾ��ʾ"};
	int devn = 3;
	string dev[4] = {"NULL", "����", "�ͷŽ�����COM�ӿ�", "�رտ�����ģʽ"};
} word;

HWND hwnd = GetConsoleWindow();
void SetColorAndBackground(int ForgC, int BackC) {//�����ֵ���ɫ
//1������2���̣�3���࣬4��죬5���ϣ�6��ƣ�7�Ұף�Ĭ�ϣ���8���
//9ǳ����10ǳ�̣�11ǳ�࣬12ǳ�죬13ǳ�ϣ�14ǳ�ƣ�15��ɫ��0��ɫ
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}
void gotoxy(long long x, long long y) {
	COORD pos;
	pos.X = 2 * x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setfont(int size) {//���塢��С����ϸ
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = size;//���������С
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_BOLD;//�����ϸ FW_BOLD,ԭʼΪFW_NORMAL
	wcscpy_s(cfi.FaceName, L"System");//�������壬�����ǿ���̨���е�
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFO consoleCurrentFont;
	GetCurrentConsoleFont(handle, FALSE, &consoleCurrentFont);
}

//������������
//ȫ�ֻ����Ա����
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
// �̺߳���
void taskbarprocess(TBPFLAG state, int percent = -1) {
	/*
	TBPF_NOPROGRESS�޽�������Ĭ��״̬��
	TBPF_INDETERMINATE��ȷ�����ȣ�����������
	TBPF_NORMAL�������ȣ���ɫ��
	TBPF_ERROR���󣨺�ɫ��
	TBPF_PAUSED��ͣ����ɫ��
	*/
	g_pTaskbar->SetProgressState(hwnd, state);
	if (state != TBPF_NOPROGRESS && state != TBPF_INDETERMINATE && (percent >= 0 || percent <= 100)) {
		g_pTaskbar->SetProgressValue(hwnd, percent, 100);
	}
	return;
}

//����ļ��Ƿ����
bool fileExist(const string& filename) {
	ifstream file(filename);
	return file.good();
}

/*ע���*/
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
/*����explorer.exe*/
void restartexp() {
	system("TASKKILL /F /IM explorer.exe");
	cout << "ɱ���̳ɹ���5���������\n";
	Sleep(5000);
	system("start C:\\Windows\\explorer.exe");
	cout << "�ָ���\n";
	Sleep(2000);
	system("start C:\\Windows\\explorer.exe");
}
/*���ιرհ�ť*/
void connot_close_button() {
	HMENU hmenu = GetSystemMenu(hwnd, false);
	RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
	SetWindowLong(hwnd, GWL_STYLE,
	              GetWindowLong(hwnd, GWL_STYLE) & ~(WS_MINIMIZEBOX | WS_THICKFRAME));
	ShowWindow(hwnd, SW_MAXIMIZE);//���
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
		cout << "\n\n\n���ǳ�ѧ���������򡣰�a���أ���d��������s��������\n\n";
		cout << "��" << step << "������6��\n\n";
		switch (step) {
			case 1: {
				cout << "���ȣ��뽫���ʵ��/������л�ΪӢ�����뷨�����رմ�д������\n";
				cout << "ϣ�ֿ��ǣ�������棩����ʱ���벻Ҫ������棬������潫�ᱻѡ�У�ϣ�ֿ��ǵĽ��̽���ֹͣ��\n";
				break;
			}
			case 2: {
				cout << "��wasd������������";
				break;
			}
			case 3: {
				cout << "���ո��ȷ��";
				break;
			}
			case 4: {
				cout << "����\">>>\"��ѡ���������Ŀ�����԰��ո����";
				break;
			}
			case 5: {
				cout << "���ɣ�ͨ��Windows��������ͼ����ϣ�ֿ���ת������һ�������Զ�ܴ󲿷���ʦ�ļ��\n";
				cout << "���ع�����Watt Toolkit���Լ��ٶ�Steam��Github�ķ��ʣ���ַhttps://steampp.net/";
				break;
			}
			case 6: {
				cout << "���Ѿ�����˳�ѧ���������򡣻�ӭʹ��ϣ�ֿ��ǣ�\n���������򽫻ᱣ���ڡ����á������";
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
				if (MessageBox(hwnd, _T("��ȷʵҪ������\n���������򽫻ᱣ�������ð����"), _T("����"), MB_YESNO) == IDYES) {
					cls
					return;
				}
			}
		}
	}
	cls
}
void poweron(bool SkipCheckWinVer, bool fb = false) {
	//1������2���̣�3���࣬4��죬5���ϣ�6��ƣ�7�Ұף�Ĭ�ϣ���8���
	//9ǳ����10ǳ�̣�11ǳ�࣬12ǳ�죬13ǳ�ϣ�14ǳ�ƣ�15��ɫ��0��ɫ
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
//��2��
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
//��3,4��
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
//��5��
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
//��1��
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
//��2��
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
//��3��
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
	//У���ļ�
	gotoxy(16, 14);
	cout << "����У�������ļ�(1/4)";
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
		word.setting[2] = "������ϰ�Ʋ�/ѭ��������ʱ������־-��ǰ:false";
	} else {
		ifstream file(".\\settings\\write-log-when-killapp.seewokiller");
		string value;
		getline(file, value);
		file.close();
		if (value == "true") {
			word.setting[2] = "������ϰ�Ʋ�/ѭ��������ʱ������־-��ǰ:true";
		} else if (value == "false") {
			word.setting[2] = "������ϰ�Ʋ�/ѭ��������ʱ������־-��ǰ:false";
		} else {
			ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
			file << "false";
			file.close();
			word.setting[2] = "������ϰ�Ʋ�/ѭ��������ʱ������־-��ǰ:false";
		}
	}
	//-----
	gotoxy(16, 14);
	cout << "������֤ϵͳ�汾(2/4) ";
	gotoxy(15, 16);
	cout << "[=====               ]";
	taskbarprocess(TBPF_NORMAL, 25);
	S(200);
	//���Windows�汾
	typedef void(__stdcall * NTPROC)(DWORD*, DWORD*, DWORD*);
	HINSTANCE hinst = LoadLibrary(TEXT("ntdll.dll"));//����DLL
	NTPROC GetNtVersionNumbers = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers");//��ȡ������ַ
	DWORD dwMajor, dwMinor, dwBuildNumber;
	GetNtVersionNumbers(&dwMajor, &dwMinor, &dwBuildNumber);
	gotoxy(15, 14);
	printf("Windows�汾: %d.%d", (int)dwMajor, (int)dwMinor);
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
			if (MessageBox(hwnd, _T("��⵽���ϵͳΪWindows 7+��\n�Ƿ�ʹ��ȫ��UI��"), _T("��ʾ"), MB_OKCANCEL) == 1) {
				string guipath = executable_path + "\\gui.exe";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(guipath.c_str()));//��Ȩ�޵Ķ����Դ�
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
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
			}//����1ȷ����2ȡ��
		}
	}
	/*Windows 10-10.0
	Windows 8.1-6.3
	Windows 8-6.2
	Windows 7-6.1
	Windows Vista-6.0
	Windows XP 64λ-5.2
	Windows XP-5.1
	Windows 2000-5.0
	https://learn.microsoft.com/zh-cn/windows/win32/sysinfo/operating-system-version
	*/
	gotoxy(15, 14);
	cout << "���ڽ�������׼��(4/4)  ";
	gotoxy(15, 16);
	cout << "[===========         ]";
	taskbarprocess(TBPF_NORMAL, 55);
	S(400);
	if (fileExist(".\\settings\\already-quick-started.seewokiller") == false) {
		cls
		gotoxy(15, 14);
		cout << "���";
		S(2000);
		gotoxy(15, 14);
		cout << "��ӭʹ��ϣ�ֿ���";
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
	//��ʼ��
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
	//д��汾��
	/*CreateDirectory("./info", NULL );
	ofstream info("./info/info.txt");
	info << "Seewo Killer 0.3.0.2" << endl;
	info << "�汾����000300002" << endl;
	info << "ע�⣺�벻Ҫ�ڴ˴�������Ҫ��Ϣ����Ϊ���ļ��ᱻSlytherinOS���ǣ�";
	info.close();*/
	cout << "\nSeewo Killer 2.0 (Engorgio)\n";
	cout << "\n׿Ȼ�����۹�https://whstu.dpdns.org/�ṩ����֧��";
	cout << "\n����ֿ⣺https://github.com/whstu/SeewoKiller/";
	cout << "\nSeewoKiller QQ Ⱥ��664929698";
	cout << "\n�������UI����SlytherinOS���\n";
	SetColorAndBackground(10, 0);
	cout << "      Slytherin ";
	SetColorAndBackground(0, 2);
	cout << "O";
	SetColorAndBackground(7, 0);
	cout << "S";
	cout << "\n�°�������PyQt5\n";
	cout << "\n���ɣ�ͨ��Windows��������ͼ����ϣ�ֿ���ת������һ�������Զ�ܴ󲿷���ʦ�ļ��\n";
	cout << "���ع�����Watt Toolkit���Լ��ٶ�Steam��Github�ķ��ʣ���ַhttps://steampp.net/\n";
	cout << "\n";
	cout << "��Slytherin(TM)����J.K.Rowling��ע���̱꣬��Ȩ��WizardingWorld(R)����\n";
	cout << "\n��b�س����ɷ���\n";
	string ans;
	while (true) {
		cin >> ans;
		if (ans == "b") {
			return;
		}
		if (ans == "dev") {
			word.settingn = 8;
			cout << "������ģʽ �ѿ�����\n��b��س����ɷ��ء�\n";
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
		// ��鵱ǰ�̻߳���̵ķ��������Ƿ������SID
		if (!CheckTokenMembership(NULL, pSid, &bResult)) {
			// ���CheckTokenMembershipʧ�ܣ�����ܲ��ǹ���Ա����Ҳ������Ϊ����ԭ��
			bResult = FALSE;
		}
		// �ͷ�SID
		FreeSid(pSid);
	} else {
		// ���SID����ʧ�ܣ���Ĭ�ϲ��ǹ���Ա
		bResult = FALSE;
	}
	return bResult;
}
bool getadmin() {
	// ��ȡ��ǰ���������·��
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);

	// ��鵱ǰ�û��Ƿ��ǹ���Ա
	if (!IsUserAnAdmin()) {
		// ������ǹ���Ա�����Թ���ԱȨ�����е�ǰ����
		SHELLEXECUTEINFO sei = {0};
		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		sei.lpFile = szPath;
		sei.nShow = SW_SHOWNORMAL;
		sei.lpVerb = _T("runas"); // ָ������Ϊ�Թ���Ա�������

		ShellExecuteEx(&sei);
		return false;
	} else {
		// ����Ѿ��ǹ���Ա������������
		cout << "�ѻ�ù���ԱȨ��\n";
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
			     << "��" << n << "��ɱ����" << endl;
			file.close();
			n++;
		}
		thread a1(system, "TASKKILL /F /IM EasiRecorder.exe");
		a1.detach();
		//cout << "���ڽ������̣���¼��\n";
		//cout << "TASKKILL /F /IM EasiRecorder.exe\n";
		system("TASKKILL /F /IM EasiRecorder.exe");
		if (KillSeewoService == true) {
			cout << "���ڽ������̣�ϣ�ֹܼ�\n";
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
			thread c3(system, "taskkill /f /fi \"WINDOWTITLE eq ��������\"");
			thread c4(system, "taskkill /f /fi \"WINDOWTITLE eq �������\\����� Internet\\��������\"");
			thread c5(system, "TASKKILL /F /IM msedge.exe");
			thread c6(system, "TASKKILL /F /IM iexplore.exe");
			c1.detach();
			c2.detach();
			c3.join();
			c4.detach();
			c5.detach();
			c6.join();
			//system("taskkill /f /t /im taskmgr.exe");
			//cout << "���ڽ������̣�����\n";
			//cout << "TASKKILL /F /IM SystemSettings.exe\n";
			//system("TASKKILL /F /IM SystemSettings.exe");
			//cout << "���ڽ������̣��������\n";
			//cout << "TASKKILL /F /FI \"WINDOWTITLE eq ��������\"\n";
			//system("taskkill /f /fi \"WINDOWTITLE eq ��������\"");
			//system("taskkill /f /fi \"WINDOWTITLE eq �������\\����� Internet\\��������\"");
			//cout << "���ڽ������̣�Edge\n";
			//cout << "TASKKILL /F /IM msedge.exe\n";
			//system("TASKKILL /F /IM msedge.exe");
			//cout << "���ڽ������̣�IE\n";
			//cout << "TASKKILL /F /IM iexplore.exe\n";
			//system("TASKKILL /F /IM iexplore.exe");
		}
	}
}

void uninstall() {
	cls
	cout << "����ж����¼��\n";
	system("\"C:\\Program Files (x86)\\Seewo\\EasiRecorder\\Uninstall.exe\"");
	cout << "����ж��Easicare\n";
	system("\"C:\\Program Files (x86)\\Seewo\\Easicare\\Uninstall.exe\"");
	cout << "����ж��EasiAgent\n";
	system("\"C:\\Program Files (x86)\\Seewo\\EasiAgent\\Uninstall.exe\"");
	cout << "����ж��ϣ�����ܱ�����\n";
	system("\"C:\\Program Files (x86)\\Seewo\\SmartpenService\\Uninstall.exe\"");
	return;
}

void liandianqi() {
	cls
	int gap;
	int x, y;
	SetColorAndBackground(4, 6);
	cout << "���棺�������������ϿΣ�";
	SetColorAndBackground(0, 7);
	cout << "\n\n����������λΪ���룬��֧��С����������0";
	cout << "\n�����������:";
	cin >> gap;
	while (cin.fail() or gap < 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "�������������:";
		cin >> gap;
	}
	cout << "\n\n������꣺�����Ļ�����꣬����114514ΪĬ�ϣ��������λ���ƶ���\n";
	cout << "����������x(��):";
	cin >> x;
	while (cin.fail() or x < 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "�������������:";
		cin >> x;
	}
	cout << "\n����������y(��):";
	cin >> y;
	while (cin.fail() or y < 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "������������ԣ�";
		cin >> y;
	}
	cout << "ÿ" << gap << "������Ļһ�Σ��뽫����ƶ�������λ��\n";
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
		cout << "��";
		S(100);
		cout << "��";
		S(100);
		cout << "ը";
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
		system("title ����ըdamn");
		long long min = 0, max = 100, ans = 0, input = 0;
		cout << "�����뷶Χ\n";
		while (true) {
			cout << "���:";
			cin >> max;
			if (max < 2 or cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "���ֵ������ڻ����2,���������\n";
			} else {
				break;
			}
		}
		while (true) {
			cout << "��С:";
			cin >> min;
			if (min < 0 or min >= max - 1 or cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "��Сֵ������ڻ����0,����Сֵ����С�����ֵ-1,���������\n";
			} else {
				break;
			}
		}
		cout << "������ɣ�����ȡ��...\n";
		while (ans == 0) {
			ans = rand() % (max - min);
		}
		ans = ans + min;
		S(500);
		cout << "ȡ����ɣ���ʼ��Ϸ��\n";
		S(500);
		cls
		while (true) {
			if (max - min == 1) {
				cout << "�������������\n";
				cout << min << "~" << max << "\n";
				break;
			}
			if (max - min == 2) {
				SetColorAndBackground(6, 4);
				cout << "DAMN!!!!!";
				SetColorAndBackground(0, 7);
				cout << "\n��һ�������ˣ�\nը����:" << ans << "\n";
				break;
			}
			cout << min << "~" << max << "\n";
			if (max - min == 3) {
				SetColorAndBackground(6, 4);
				cout << "��ѡһ!!!!!";
				SetColorAndBackground(7, 0);
			}
			cin >> input;
			while (input <= min or input >= max or cin.fail()/*cin���ִ���*/) {
				cout << "�������\n";
				cin.clear();//�����������
				cin.ignore(numeric_limits<streamsize>::max(), '\n');//������������
				cout << min << "~" << max << "\n";
				cin >> input;
			}
			if (input == ans) {
				SetColorAndBackground(6, 4);
				cout << "DAMN!!!!!";
				SetColorAndBackground(0, 7);
				cout << "\n�����ˣ�\nը����:" << ans << "\n";
				if (max - ans == ans - min) {
					cout << "��λ��С��";
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
			string C[20] = {"��", "��", "+", "=", "|", "|", "|", "|", "|", "?"};
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
			printf("�����������");
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
			printf("�����������");
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
			printf("�����������");
			getch();
			return;
		}
		void wzqmain() {
			cout << "��1��AI��ս  ��2˫�˶�ս  ��3�ۿ�AI��ս\n";
			char ch = getch();
			for (;; ch = getch()) {
				if (ch == '1') {
					cout << "��ѡ������AI����������\n";
					Sleep(1000);
					system("title ����:�˻���ս");
					HMAI();
					cls
					return;
				} else if (ch == '2') {
					cout << "��ѡ����˫�˶�ս��\n";
					Sleep(1000);
					system("title ����:��vs��ģʽ");
					HMHM();
					cls
					return;
				} else if (ch == '3') {
					cout << "��ѡ���˹ۿ�����AI��ս������\n";
					system("title ����:AIvsAI");
					cout << "����Ҫ����������������2������\nȷ����";
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
				cout << "error code 01���޷�����У���Ϊ���������������(x)�����";
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
				cout << "error code 02���޷�����У���Ϊ��������ĺ�����(y)�����";
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
			cout << "�÷֣�";
			SetPos(52, 7);
			cout << "�ƺţ�";
			SetPos(52, 10);
			cout << "������ʽ��";
			SetPos(52, 12);
			cout << " a,s,d,w ����ս���ƶ���";
			SetPos(52, 14);
			cout << " p ��ͣ��Ϸ��";
			SetPos(52, 16);
			cout << " e �˳���Ϸ��";
			SetPos(52, 18);
			cout << " �ո� �����ӵ���";
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
			cout << "�� �� �� ս";
			drawRow(3, 0, 79, '-');
			drawRow(5, 0, 79, '-');
			SetPos(28, 4);
			cout << "w �� s ѡ�� �ո� ȷ��";
			SetPos(15, 11);
			cout << "1. �򵥵ĵ���";
			SetPos(15, 13);
			cout << "2. ���ĵ���";
			drawRow(20, 0, 79, '-');
			drawRow(22, 0, 79, '-');
			SetPos(47, 11);
			cout << "�򵥵ĵ��ˣ�";
			SetPos(51, 13);
			cout << "�򵥵������Ž������ƶ��ٶȡ�";
			SetPos(24, 21);
			cout << " ������������ ";
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
								cout << "��";
								j = 11;
								SetPos(12, j);
								cout << ">>";
								SetPos(51, 13);
								cout << "������������������������";
								SetPos(47, 11);
								cout << "�򵥵ĵ��ˣ�";
								SetPos(51, 13);
								cout << "�򵥵������Ž������ƶ��ٶȡ�";
							}
							break;
						}
						case 's' : {
							if ( j == 11 ) {
								SetPos(12, j);
								cout << "��";
								j = 13;
								SetPos(12, j);
								cout << ">>";
								SetPos(51, 13);
								cout << "����������������������������";
								SetPos(47, 11);
								cout << "���ĵ��ˣ�";
								SetPos(51, 13);
								cout << "���ĵ����ƶ��ٶȽϿ졣";
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
			cout << "��ͣ��...";
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
						cout << "׹��";
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
					title = "�߼�����Ա";
				else if ( x == 4)
					title = "�񼶷���Ա";
				else if ( x == 3)
					title = "���񼶷���Ա";
				else if ( x == 2 )
					title = "���񼶷���Ա";
				else if ( x == 1 )
					title = "���������Ա";
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
			cout << "����л���" << score / 5 << " ��";
			SetPos(31, 12);
			cout << "�á����֣�" << score;
			SetPos(31, 13);
			cout << "��óƺţ�" << title;
			SetPos(30, 16);
			Sleep(1000);
			cout << "������ �ǣ�y��| ��n��";
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
		string daojuname[] = {"�Ŵ�", "����", "С��", "��", "����"};
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
			printf("%d��ʵ��,%d���յ�\n", shi, kong);
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
				printf("������%d������:\n", maxn);
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
				printf("��ħ�����%d������:\n", maxn);
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
				printf("������\n");
				system("pause");
				end = true;
				return;
			}
			if (Other <= 0) {
				if (huihe == 3) {
					Yourmoney /= 3;
					printf("��Ӯ��\n�����˽���$%.2lf\n", Yourmoney);
					system("pause");
					end = true;
					return;
				} else if (huihe == 2) {
					printf("��������غ�\n");
					huihe = 3;
					Your = 6;
					Other = 6;
					system("pause");
					system("cls");
					build_gun();
				} else if (huihe == 1) {
					printf("����ڶ��غ�\n");
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
					printf("�������:@#%&%%&@#%@%*&*\n");
				} else {
					printf("�������:%d/%d\n", Your, huihe * 2);
				}
				if (!emoqbq) {
					printf("��ħ����:@$%#^%%^%&*&%%$\n");
				} else {
					printf("��ħ����:%d/%d\n", Other, huihe * 2);
				}
				printf("ʣ��ʵ����:%d    ʣ��յ���:%d\n", shi, kong);
				if (huihe != 1) {
					printf("������ӵ�еĵ���:\n");
					for (int i = 0; i <= 4; i++) {
						cout << daojuname[i];
						printf("%d", daoju[i]);
						printf("��");
						if (i != 4) {
							printf(",");
						}
					}
					printf("\n");
					printf("��ħ����ӵ�еĵ���:\n");
					for (int i = 0; i <= 4; i++) {
						cout << daojuname[i];
						printf("%d", daoju1[i]);
						printf("��");
						if (i != 4) {
							printf(",");
						}
					}
				}
				printf("\n");
				printf("��������Ļغ�\n");
				printf("��Ҫ\n1.���ħ��ǹ\n2.���Լ���ǹ\n");
				if (huihe != 1) {
					for (int i = 0; i <= 4; i++) {
						printf("%d.ʹ��", i + 3);
						cout << daojuname[i] << '\n';
					}
				}
				scanf("%d", &x);
				if ((huihe == 1 && 1 <= x && x <= 2) || (huihe != 1 && 1 <= x && x <= 7)) {
					break;
				}
				printf("���벻�Ϸ�\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 1) {
				printf("��������ħ��ǹ");
				T++;
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(shi) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("�ǿյ�\n");
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("��Ϊ��ʹ�����������Կ�������һ��\n");
						Sleep(500);
						T--;
					}
				} else {
					Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					shi--;
					qlen--;
					Other -= Hurt;

					printf("��ʵ��\n");
					printf("�����%d���˺�\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver();
					if (end == true) {
						return;
					}
					if (huihe == 3 && Other <= 2) {
						printf("��ħ�������Ͽ�!\n");
						emoqbq = 1;
						Yourmoney += 5000.0;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						Yourmoney += 1000.0;
						printf("��Ϊ��ʹ�����������Կ�������һ��\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 2) {
				printf("��������Լ���ǹ");
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("�ǿյ�\n");
				} else {
					Yourmoney += 5000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
					T++;
					shi--;
					qlen--;
					Your -= Hurt;
					printf("��ʵ��\n");
					printf("�����%d���˺�\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver();
					if (end == true) {
						return;
					}
					if (huihe == 3 && Your <= 2) {
						printf("��������Ͽ�!\n");
						ndqbq = 1;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("��Ϊ��ʹ�����������Կ�������һ��\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 3) {
				if (daoju[0]) {
					daoju[0]--;
					daojulen--;
					printf("��ʹ���˷Ŵ�\n");
					wait();
					printf("\n�㿴����");
					if (q[qlen] == 1) {
						printf("ʵ��\n");
						Yourmoney += 2500.0;
					} else {
						printf("�յ�\n");
					}
					Sleep(500);
					printf("\n");
				} else {
					printf("������û�зŴ�\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 4) {
				if (daoju[1]) {
					if (!shoukao_you) {
						daoju[1]--;
						daojulen--;
						printf("��ʹ��������\n");
						printf("������������ǹ�Ļ���\n");
						shoukao_you = 1;
					} else {
						printf("���Ѿ��ù�������\n");
					}
					Sleep(1145);
					system("cls");
				} else {
					printf("������û������\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 5) {
				if (daoju[2]) {
					daoju[2]--;
					daojulen--;
					printf("��ʹ����С��\n");
					printf("����һ��Ϊʵ�����˺�����\n");
					Hurt *= 2;
					Yourmoney += 500.0;
					Sleep(1145);
					system("cls");
				} else {
					printf("������û��С��\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 6) {
				if (daoju[3]) {
					if (Your ^ 6) {
						daoju[3]--;
						daojulen--;
						printf("��ʹ������\n");
						printf("��ظ���һ������\n");
						Yourmoney += 500.0;
						Your++;
					} else {
						printf("�����ڲ���Ҫ��\n");
					}
					Sleep(1145);
				} else {
					printf("������û����\n");
					Sleep(1145);
					system("cls");
				}
			} else {
				if (daoju[4]) {
					daoju[4]--;
					daojulen--;
					printf("��ʹ��������\n");
					wait();
					printf("\n");
					printf("������һ��");
					if (q[qlen] == 2) {
						printf("�յ�");
						kong--;
					} else {
						printf("ʵ��");
						Yourmoney += 500.0;
						shi--;
					}
					qlen--;
					Sleep(500);
				} else {
					printf("������û������\n");
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
			printf("��ħ�������㿪ǹ");
			T++;
			wait();
			if (q[qlen] == 2) {
				Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
				kong--;
				qlen--;
				Hurt = 1;
				Know = 0;
				printf("�ǿյ�\n");
				if (shoukaoemo) {
					printf("��Ϊ��ħʹ�����������Կ�������һ��\n");
					T--;
					Sleep(500);
					shoukaoemo = 0;
				}
			} else {
				Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
				shi--;
				qlen--;
				Your -= Hurt;
				printf("��ʵ��\n");
				printf("�����%d���˺�\n", Hurt);
				Hurt = 1;
				Know = 0;
				Sleep(500);
				IsOver();
				if (end == true) {
					return;
				}
				if (huihe == 3 && Your <= 2) {
					printf("��������Ͽ�!\n");
					ndqbq = 1;
				}
				if (shoukaoemo) {
					printf("��Ϊ��ħʹ�����������Կ�������һ��\n");
					Yourmoney += 1000.0;
					T--;
					Sleep(500);
					shoukaoemo = 0;
				}
			}
		}
		void fightemo() {
			printf("��ħ�������Լ���ǹ");
			wait();
			if (q[qlen] == 2) {
				Yourmoney += 2000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
				kong--;
				qlen--;
				printf("�ǿյ�\n");
				Know = 0;
			} else {
				Yourmoney += 5000.0 * (1 + (double)(kong) * 1.0 / (double)(shi + kong));
				shi--;
				T++;
				qlen--;
				Other -= Hurt;
				printf("��ʵ��\n");
				printf("�����%d���˺�\n", Hurt);
				Hurt = 1;
				Know = 0;
				Sleep(500);
				IsOver();
				if (end == true) {
					return;
				}
				if (huihe == 3 && Other <= 2) {
					printf("��ħ�������Ͽ�!\n");
					emoqbq = 1;
					Yourmoney += 5000.0;
				}
				if (shoukaoemo) {
					printf("��Ϊ��ħʹ�����������Կ�������һ��\n");
					T--;
					Sleep(500);
					shoukaoemo = 0;
				}
			}
		}
		void Timeother() {
			if (!ndqbq) {
				printf("�������:@#%&%%&@#%@%*&*\n");
			} else {
				printf("�������:%d/%d\n", Your, huihe * 2);
			}
			if (!emoqbq) {
				printf("��ħ����:@$%#^%%^%&*&%%$\n");
			} else {
				printf("��ħ����:%d/%d\n", Other, huihe * 2);
			}
			printf("ʣ��ʵ����:%d    ʣ��յ���:%d\n", shi, kong);
			if (huihe != 1) {
				printf("��ħ����ӵ�еĵ���:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju1[i]);
					printf("��");
					if (i != 4) {
						printf(",");
					}
				}
			}
			printf("�����Ƕ�ħ�Ļغ�\n");
			Sleep(1500);
			if (Other != 6) {
				if (daoju1[3]) {
					daoju1[3]--;
					daojulen1--;
					printf("��ħʹ������\n");
					printf("��ħ�ظ���һ������\n");
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
						printf("��ħʹ���˷Ŵ�\n");
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
					printf("��ħʹ����С��\n");
					printf("����һ��Ϊʵ�����˺�����");
					Yourmoney += 500.0;
					Sleep(1145);
					system("cls");
					return;
				} else {
					if (shi >= kong + 1 && daoju1[1] && shoukaoemo != 1) {
						daoju1[1]--;
						daojulen1--;
						shoukaoemo = 1;
						printf("��ħʹ��������\n");
						printf("��ħ�����������ǹ�Ļ���\n");
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
					printf("��ħʹ��������\n");
					wait();
					printf("\n");
					printf("��ħ����һ��");
					if (q[qlen] == 2) {
						printf("�յ�");
						kong--;
					} else {
						printf("ʵ��");
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
			printf("������һλ��ս��...\n");
			Sleep(1000);
			int x;
			while (1) {
				printf("׼���ò����ħ����Ϸ��ʤ�ߴ��߽��𣬰��߽����ڴ˳���\n1.�õ�\n2.û����\n");
				scanf("%d", &x);
				if (x == 1 || x == 2) {
					break;
				}
				printf("���벻�Ϸ�\n");
				Sleep(1145);
				system("cls");
			}
			while (1) {
				printf("��������ǵĹ�����\n1.���\n2.�����\n");
				scanf("%d", &x);
				if (x == 1 || x == 2) {
					break;
				}
				printf("���벻�Ϸ�\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 2) {
				for (int i = 1; i <= 3; i++) {
					printf(".");
					Sleep(1000);
				}
				printf("\n");
				printf("����:\n");
				printf("��Ͷ�ħ������6������\n") ;
				printf("ÿ�ֿ�ʼǰ���㽫֪��һ���м���ʵ���������յ�\n");
				printf("ÿ�������ѡ����Լ���ǹ���Զ�ħ��ǹ����ʹ�õ���\n");
				printf("�������Լ���ǹ����Ϊ�յ�������Լ����ж�������ֹͣ�ж�\n");
				printf("�����Զ�ħ��ǹ��������Σ�����ֹͣ�ж�\n");
				printf("�ڶ������غ�ÿ�ֿ�ʼǰ˫���������4��������Ϊ����������Ϊ8����\n");
				printf("���˵����غϣ�����һ��ֻʣ������������÷��������������٣��޷������Լ�Ѫ����\n");
				printf("����һ����\n");
				printf("�Ŵ󾵣�����֪����һ���ӵ��ǿյ�����ʵ��\n");
				printf("��������һ�α��غϵ��ж�����\n");
				printf("С��������һ���ӵ�Ϊʵ�������˺�����\n");
				printf("�̣����Իظ�1������\n");
				printf("���ϣ�������һ���ӵ�\n");
				system("pause");
				system("cls");
			}
			printf("�ð�\n");
			Sleep(1145);
			printf("��Ϸ��Ҫ��ʼ��Ŷ\n");
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
				printf("���BӮ��\n���B����˽���$%.2lf\n", Yourmoney);
				system("pause");
				end = true;
				return;
			} else if (Other <= 0) {
				printf("���AӮ��\n���A����˽���$%.2lf\n", Yourmoney);
				system("pause");
				end = true;
				return;
			}

		}
		void build_gun_duo() {
			kong = Rand(1, 4);
			shi = Rand(1, 4);
			qlen = 0;
			printf("%d��ʵ��,%d���յ�\n", shi, kong);
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
			printf("���A�����%d������:\n", maxn);
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
			printf("���B�����%d������:\n", maxn);
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
				printf("���A������:%d/4\n���B������:%d/4\n", Your, Other);
				printf("ʣ��ʵ����:%d    ʣ��յ���:%d\n", shi, kong);
				printf("���A����ӵ�еĵ���:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju[i]);
					printf("��");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("���B����ӵ�еĵ���:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju1[i]);
					printf("��");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("���������A�Ļغ�\n");
				printf("���AҪ\n1.�����B��ǹ\n2.���Լ���ǹ\n");
				for (int i = 0; i <= 4; i++) {
					printf("%d.ʹ��", i + 3);
					cout << daojuname[i] << '\n';
				}
				scanf("%d", &x);
				if (1 <= x && x <= 7) {
					break;
				}
				printf("���벻�Ϸ�\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 1) {
				printf("���A���������B��ǹ");
				T++;
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(shi) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("�ǿյ�\n");
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("��Ϊ���Aʹ�����������Կ�������һ��\n");
						Sleep(500);
						T--;
					}
				} else {
					Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					shi--;
					qlen--;
					Other -= Hurt;
					printf("��ʵ��\n");
					printf("�����%d���˺�\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						Yourmoney += 1000.0;
						printf("��Ϊ���Aʹ�����������Կ�������һ��\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 2) {
				printf("���A�������Լ���ǹ");
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("�ǿյ�\n");
				} else {
					Yourmoney += 5000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
					T++;
					shi--;
					qlen--;
					Your -= Hurt;
					printf("��ʵ��\n");
					printf("�����%d���˺�\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("��Ϊ���Aʹ�����������Կ�������һ��\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 3) { //{"�Ŵ�","����","С��","��","����"};
				if (daoju[0]) {
					daoju[0]--;
					daojulen--;
					printf("���Aʹ���˷Ŵ�\n");
					wait();
					printf("\n���A������");
					if (q[qlen] == 1) {
						printf("ʵ��\n");
						Yourmoney += 2500.0;
					} else {
						printf("�յ�\n");
					}
					Sleep(500);
					printf("\n");
				} else {
					printf("���A����û�зŴ�\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 4) {
				if (daoju[1]) {
					if (!shoukao_you) {
						daoju[1]--;
						daojulen--;
						printf("���Aʹ��������\n");
						printf("���A�����������ǹ�Ļ���\n");
						shoukao_you = 1;
					} else {
						printf("���A�Ѿ��ù�������\n");
					}
					Sleep(1145);
					system("cls");
				} else {
					printf("���A����û������\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 5) {
				if (daoju[2]) {
					daoju[2]--;
					daojulen--;
					printf("���Aʹ����С��\n");
					printf("����һ��Ϊʵ�����˺�����\n");
					Yourmoney += 500.0;
					Hurt *= 2;
					Sleep(1145);
					system("cls");
				} else {
					printf("���A����û��С��\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 6) {
				if (daoju[3]) {
					if (Your ^ 4) {
						daoju[3]--;
						daojulen--;
						printf("���Aʹ������\n");
						printf("���A�ظ���һ������\n");
						Yourmoney += 500.0;
						Your++;
					} else {
						printf("���A���ڲ���Ҫ��\n");
					}
					Sleep(1145);
				} else {
					printf("���A����û����\n");
					Sleep(1145);
					system("cls");
				}
			} else {
				if (daoju[4]) {
					daoju[4]--;
					daojulen--;
					printf("���Aʹ��������\n");
					wait();
					printf("\n");
					printf("���A����һ��");
					if (q[qlen] == 2) {
						printf("�յ�");
						kong--;
					} else {
						printf("ʵ��");
						Yourmoney += 500.0;
						shi--;
					}
					qlen--;
					Sleep(500);
				} else {
					printf("���A����û������\n");
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
				printf("���A������:%d/4\n���B������:%d/4\n", Your, Other);
				printf("ʣ��ʵ����:%d    ʣ��յ���:%d\n", shi, kong);
				printf("���A����ӵ�еĵ���:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju[i]);
					printf("��");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("���B����ӵ�еĵ���:\n");
				for (int i = 0; i <= 4; i++) {
					cout << daojuname[i];
					printf("%d", daoju1[i]);
					printf("��");
					if (i != 4) {
						printf(",");
					}
				}
				printf("\n");
				printf("���������B�Ļغ�\n");
				printf("���BҪ\n1.�����A��ǹ\n2.���Լ���ǹ\n");
				for (int i = 0; i <= 4; i++) {
					printf("%d.ʹ��", i + 3);
					cout << daojuname[i] << '\n';
				}
				scanf("%d", &x);
				if (1 <= x && x <= 7) {
					break;
				}
				printf("���벻�Ϸ�\n");
				Sleep(1145);
				system("cls");
			}
			if (x == 1) {
				printf("���B���������A��ǹ");
				T++;
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(shi) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("�ǿյ�\n");
					if (shoukaoemo == 1) {
						shoukaoemo = 0;
						printf("��Ϊ���Bʹ�����������Կ�������һ��\n");
						Sleep(500);
						T--;
					}
				} else {
					Yourmoney += (double)(5000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					shi--;
					qlen--;
					Your -= Hurt;
					printf("��ʵ��\n");
					printf("�����%d���˺�\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukaoemo == 1) {
						shoukaoemo = 0;
						Yourmoney += 1000.0;
						printf("��Ϊ���Bʹ�����������Կ�������һ��\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 2) {
				printf("���B�������Լ���ǹ");
				wait();
				if (q[qlen] == 2) {
					Yourmoney += (double)(2000.0 * (Hurt * 1.0) * (1 + (double)(kong) * 1.0 / (double)(shi + kong)));
					kong--;
					qlen--;
					Hurt = 1;
					printf("�ǿյ�\n");
				} else {
					Yourmoney += 5000.0 * (1 + (double)(shi) * 1.0 / (double)(shi + kong));
					T++;
					shi--;
					qlen--;
					Other -= Hurt;
					printf("��ʵ��\n");
					printf("�����%d���˺�\n", Hurt);
					Hurt = 1;
					Sleep(500);
					IsOver_duo();
					if (end == true) {
						return;
					}
					if (shoukao_you == 1) {
						shoukao_you = 0;
						printf("��Ϊ���Bʹ�����������Կ�������һ��\n");
						Sleep(500);
						T--;
					}
				}
			} else if (x == 3) {
				if (daoju1[0]) {
					daoju1[0]--;
					daojulen1--;
					printf("���Bʹ���˷Ŵ�\n");
					wait();
					printf("\n���B������");
					if (q[qlen] == 1) {
						printf("ʵ��\n");
						Yourmoney += 2500.0;
					} else {
						printf("�յ�\n");
					}
					Sleep(500);
					printf("\n");
				} else {
					printf("���B����û�зŴ�\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 4) {
				if (daoju1[1]) {
					if (!shoukaoemo) {
						daoju1[1]--;
						daojulen1--;
						printf("���Bʹ��������\n");
						printf("���B�����������ǹ�Ļ���\n");
						shoukaoemo = 1;
					} else {
						printf("���B�Ѿ��ù�������\n");
					}
					Sleep(1145);
					system("cls");
				} else {
					printf("���B����û������\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 5) {
				if (daoju1[2]) {
					daoju1[2]--;
					daojulen1--;
					printf("���Bʹ����С��\n");
					printf("����һ��Ϊʵ�����˺�����\n");
					Yourmoney += 500.0;
					Hurt *= 2;
					Sleep(1145);
					system("cls");
				} else {
					printf("���B����û��С��\n");
					Sleep(1145);
					system("cls");
				}
			} else if (x == 6) {
				if (daoju1[3]) {
					if (Other ^ 4) {
						daoju1[3]--;
						daojulen1--;
						printf("���Bʹ������\n");
						printf("���B�ظ���һ������\n");
						Yourmoney += 500.0;
						Other++;
					} else {
						printf("���B���ڲ���Ҫ��\n");
					}
					Sleep(1145);
				} else {
					printf("���B����û����\n");
					Sleep(1145);
					system("cls");
				}
			} else {
				if (daoju1[4]) {
					daoju1[4]--;
					daojulen1--;
					printf("���Bʹ��������\n");
					wait();
					printf("\n");
					printf("���B����һ��");
					if (q[qlen] == 2) {
						printf("�յ�");
						kong--;
					} else {
						printf("ʵ��");
						Yourmoney += 500.0;
						shi--;
					}
					qlen--;
					Sleep(500);
				} else {
					printf("���B����û������\n");
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
				printf("��ѡ������Ҫ��ģʽ��\n1.����\n2.˫�ˣ���ģʽ�У�����ֵΪ4�����߲���Ϊ2��\n");
				scanf("%d", &x);
				if (x == 1 || x == 2) {
					break;
				}
				printf("���벻�Ϸ�\n");
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

struct JOKE { /*���*/
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
		MessageBox(hwnd, _T("�˹��ܿ�����ȡ�����ļ��е��������ݣ���֧��U�̡�\n��������Ҫ������Щ�ļ���λ�úͿ�����洢��λ�á�"), _T("��ʾ"), MB_OK);
		if (MessageBox(hwnd, _T("�����������ʹ�ô˹�����ɵ��κ���ʧ(�����������������̡�����ȥ����ʦ�Ȳ��)���������ؿ��ǣ�"), _T("����"), MB_YESNO | MB_ICONWARNING) == IDNO) {
			return;
		}
		MessageBox(hwnd, _T("����Ҫ�ڽ������Ŀ���̨�������ļ���Դ�Ϳ�������ļ�ȥ��"), _T("��ʾ"), MB_OK);
		SetColorAndBackground(7, 0);
		cout << "\n�������ļ���Դ��\n";
		SetColorAndBackground(4, 7);
		cout << "ע�⣺�ļ����������Ҫ����\\��������֧�ָ��Ƶ����ļ���\nʾ����E:\\��F:\\��˵�μ�\\\n";
		SetColorAndBackground(7, 0);
		cout << "������(����1�˳�����):";
		string infile;
		cin >> infile;
		if (infile == "1") {
			return;
		}
		SetColorAndBackground(7, 0);
		cout << "\n�������ļ�ȥ��\n";
		SetColorAndBackground(4, 7);
		cout << "ע�⣺�ļ����������ҲҪ����\\��\n";
		SetColorAndBackground(7, 0);
		cout << "������(����0ʹ��Ĭ��·��D:\\file\\������1�˳�����):";
		string outfile;
		cin >> outfile;
		if (outfile == "0") {
			outfile = "D:\\file\\";
		}
		if (outfile == "1") {
			return;
		}
		cout << "������ɡ���U�̲����Ŀ¼���ڣ���ϣ�ֿ��ǻ��Զ��������е��ļ���\n���������ʼ������";
		getch();
		cls
		cout << "��ʼ����\n------------------\n���ڵȴ�Ŀ¼����";
		string existpath = infile + "temp.dat";
		while (fileExist(existpath.c_str()) == false) {
			ofstream file(existpath.c_str());
			file << "";
			file.close();
		}
		string command = "del \"" + existpath + "\"";
		system(command.c_str());
		cout << "\n���ҵ��ļ��У���ʼ����\n";
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
	void head(string WindowTitle = "ϣ�ֿ���", string cmdTitle = "SeewoKiller") {
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
			//��ҳ��
			if (s == "-1") {
				cls
				if (fastboot == true) {
					head("ϣ�ֿ���fastbootģʽ", "SeewoKiller Fastboot");
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
			} else if (s == "ѭ��������(�Ͽη�����)") {
				taskkill(true, false);
			} else if (s == "һ��ж��") {
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
			} else if (s == "�����") {
				setfont(20);
				string aipath = ".\\pai.exe";
				system(aipath.c_str());
				setfont(30);
				s = "-1";
				continue;
			} else if (s == "���㻹ԭ�ƽ�") {
				system("title ���㻹ԭ");
				cout << "\n���ȹرձ��㴰�ں��ټ�������ϣ�ֿ��ǡ�\n";
				string unfreezepath = executable_path + "\\SeewoFreeze\\SeewoFreezeUI.exe --startup-with-main-window";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//��Ȩ�޵Ķ����Դ�
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
				if (fSuccess) { //����������ͣ���ȴ��ӽ�����ֹ
					CloseHandle(pi.hThread);
					//��ͣ�����̵�ִ�У�ֱ��child��ֹ���ô���ſ��Լ�������
					WaitForSingleObject(pi.hProcess, INFINITE);
					CloseHandle(pi.hProcess);
				}
				//system(".\\SeewoFreeze\\SeewoFreezeUI.exe --startup-with-main-window");
				system("pause");
				s = "-1";
				continue;
			} else if (s == "����ϰ�Ʋ�ģʽ") {
				system("title �Ʋ�����ϰ");
				taskkill(true, true);
				s = "-1";
				continue;
			} else if (s == "������(�ɷ�����)") {
				liandianqi();
				s = "-1";
				continue;
			} else if (s == "һ����ϣ������") {
				taskkill(true, true);
				s = "-1";
				continue;
			} else if (s == "�˳�") {
				return;
			} else if (s == "����") {
				about();
				s = "-1";
				continue;
			} else if (s == "������ѧ������") {
				quickstart();
				s = "-1";
				continue;
			} else if (s == "ʹ���°����") {
				string guipath = executable_path + "\\gui.exe";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(guipath.c_str()));//��Ȩ�޵Ķ����Դ�
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
				if (fSuccess) {
					return;
				}
			} else if (s == "������fastboot(���fast!)") {
				string fbpath = "./SeewoKiller.exe run fastboot";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(fbpath.c_str()));//��Ȩ�޵Ķ����Դ�
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
				if (fSuccess) {
					return;
				}
			} else if (s == "������ϰ�Ʋ�/ѭ��������ʱ������־-��ǰ:false" or s == "������ϰ�Ʋ�/ѭ��������ʱ������־-��ǰ:true") {
				ifstream file(".\\settings\\write-log-when-killapp.seewokiller");
				string value;
				getline(file, value);
				value = "��ǰ:" + value + "\n��Ҫ�������ø���Ϊʲô��\n���\"��\"����Ϊtrue�����\"��\"����Ϊfalse�����\"ȡ��\"�����޸�";
				int ans = MessageBox(hwnd, value.c_str(), _T("�޸ı���"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "true";
						file.close();
						if (MessageBox(hwnd, _T("�޸���ɣ��Ƿ��������¼��������ļ���\n��־�ļ�������log�ļ�����"), _T("��ʾ"), MB_YESNO) == IDYES) {
							cls
							poweron(true);
						}
						break;
					}
					case IDNO: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "false";
						file.close();
						if (MessageBox(hwnd, _T("�޸���ɣ��Ƿ��������¼��������ļ���"), _T("��ʾ"), MB_YESNO) == IDYES) {
							cls
							poweron(true);
						}
						break;
					}
				}
				s = "-1";
				continue;
			} else if (s == "����־�ļ���") {
				string command = "explorer.exe \"" + executable_path + "\\log\\\"";
				system(command.c_str());
				s = "-1";
				continue;
			} else if (s == "С��Ϸ>>>") {
				head();
				string d = listname(false, false, word.game, word.gamen);
				if (d == "����") {
					s = "-1";
					continue;
				}
				if (d == "����ը��") {
					numberdamn();
				}
				if (d == "������") {
					setfont(20);
					wzq.wzqmain();
					setfont(30);
				}
				if (d == "�ɻ���ս") {
					cls
					setfont(20);
					feijidazhan::main();
					setfont(30);
				}
				if (d == "��ħ���̶�") {
					cls
					setfont(20);
					emlpd::main();
					setfont(30);
				}
			} else if (s == "���>>>") {
				head();
				string d = listname(false, false, word.joke, word.joken);
				if (d == "����") {
					s = "-1";
					continue;
				}
				if (d == "ɱWPS+ϣ�ְװ�+ϣ����Ƶչ̨") {
					joke.kill();
				}
				if (d == "��ȡU���ļ�") {
					joke.copy_file();
				}
			} else if (s == "ע���>>>") {
				head();
				string d = listname(false, false, word.reg, word.regn);
				if (d == "����") {
					s = "-1";
					continue;
				}
				if (d == "�����������˵�") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "1");
					regedit("HKEY_CURRENT_USER", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "1");
					restartexp();
				}
				if (d == "�����������˵�") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
					regedit("HKEY_CURRENT_USER", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
					restartexp();
				}
				if (d == "���ÿ�ݼ�") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "1");
					restartexp();
				}
				if (d == "���ÿ�ݼ�") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "0");
					restartexp();
				}
				if (d == "������ʾ��¼��ϸ��Ϣ") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "1");
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
					if (MessageBox(hwnd, _T("���Ƿ�Ҫ����ע����"), _T("ע��ȷ��"), MB_OKCANCEL) == 1) {//1ȷ����2ȡ��
						system("shutdown /l");
					}
					system("pause");
				}
				if (d == "������ʾ��¼��ϸ��Ϣ") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "0");
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
					system("pause");
				}
				if (d == "��¼ʱ��ʾ��ʾ") {
					char title1[100];
					char title2[100];
					cout << "������������(50������)��";
					scanf_s("%s", title1, (unsigned)_countof(title1));
					cout << "�����븱����(50������)��";
					scanf_s("%s", title2, (unsigned)_countof(title2));
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", title1);
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", title2);
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
					if (MessageBox(hwnd, _T("���Ƿ�Ҫ����ע����"), _T("ע��ȷ��"), MB_OKCANCEL) == 1) {//1ȷ����2ȡ��
						system("shutdown /l");
					}
					system("pause");
				}
				if (d == "ȡ����¼ʱ��ʾ��ʾ") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", "");
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", "");
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
					system("pause");
				}
			} else if (s == "������ѡ��>>>") {
				head();
				string d = listname(false, false, word.dev, word.devn);
				if (d == "����") {
					s = "-1";
					continue;
				}
				if (d == "�ͷŽ�����COM�ӿ�") {
					ReleaseTaskbarInterface();
					system("pause");
				}
				if (d == "�رտ�����ģʽ") {
					word.settingn = 7;
					cout << "��������ɡ�\n";
					system("pause");
					d = "����";
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
	system("title ���ڳ�ʼ��");
	InitTaskbarInterface();
	taskbarprocess(TBPF_INDETERMINATE);
	srand((unsigned)time(NULL));
	system("title ���ڼ�����Ա");
	//��ȡ����·��
	GetModuleFileNameA(NULL, path, MAX_PATH);
	executable_path = path;
	position = executable_path.find_last_of('\\');
	executable_path = executable_path.substr(0, position);
	xwbbpath = executable_path;
	//��������
	bool skipcheck = false;
	if (argc > 1) {
		taskbarprocess(TBPF_NOPROGRESS);
		system("title ϣ�ֿ���");
		string cmd[100];
		for (int i = 0; i < argc; i++) {
			cmd[i] = argv[i];
		}
		//����Ҫadmin
		if (cmd[1] == "run") { //����
			if (cmd[2] == "fastboot") {
				fastboot = true;
			}
		}
		if (cmd[1] == "about") {
			about();
			return 0;
		}
		if (cmd[1] == "game") { //��Ϸ
			if (argc <= 2) {
				cout << "����ȱʧ�������Զ��˳�\n����game��ʹ�÷���\n";
				cout << "-wzq������\n-numberdamn����ը��\n-fjdz�ɻ���ս\n-emlpd��ħ���̶�";
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
		//��Ҫadmin
		if (IsUserAnAdmin() == false) {
			cout << "������δȡ�ù���ԱȨ�ޣ������޷����С�\n��ʹ�ù���ԱȨ�������նˡ�";
			return 0;
		}
		if (cmd[1] == "ui") { //����ui
			if (argc <= 2) {
				cout << "����ȱʧ�������Զ��˳�\n����run��ʹ�÷���\n";
				cout << "-newuiǿ����UI\n-olduiǿ�ƾ�UI\n";
				return 0;
			}
			if (cmd[2] == "-skipcheck") {
				skipcheck = true;
			}
			if (cmd[2] == "-oldui") {
				string unfreezepath = executable_path + "\\SeewoKiller.exe ui -skipcheck";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//��Ȩ�޵Ķ����Դ�
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
				if (fSuccess) {
					return 0;
				}
			}
			if (cmd[2] == "-newui") {
				string unfreezepath = executable_path + "\\gui.exe";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//��Ȩ�޵Ķ����Դ�
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
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
			system("title �Ʋ�����ϰ");
			taskkill(true, true);
			return 0;
		}
		if (cmd[1] == "uninstall") {
			uninstall();
			return 0;
		}
		if (cmd[1] == "liandianqi") {
			system("title ������");
			liandianqi();
			return 0;
		}
		if (cmd[1] == "seewofreeze") {
			cout << "\n���ȹرձ��㴰�ں��ټ���������\n";
			string unfreezepath = executable_path + "\\SeewoFreeze\\SeewoFreezeUI.exe --startup-with-main-window";
			STARTUPINFO si = { sizeof(si) };//0
			PROCESS_INFORMATION pi;
			LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//��Ȩ�޵Ķ����Դ�
			BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
			if (fSuccess) { //����������ͣ���ȴ��ӽ�����ֹ
				CloseHandle(pi.hThread);
				//��ͣ�����̵�ִ�У�ֱ��child��ֹ���ô���ſ��Լ�������
				WaitForSingleObject(pi.hProcess, INFINITE);
				CloseHandle(pi.hProcess);
			}
			return 0;
		}
		if (cmd[1] == "setvalue") { //�޸�����
			if (argc <= 2) {
				cout << "����ȱʧ�������Զ��˳�\n����setvalue��ʹ�÷���\n";
				cout << "-log������־����\n";
			}
			if (cmd[2] == "-log") {
				ifstream file(".\\settings\\write-log-when-killapp.seewokiller");
				string value;
				getline(file, value);
				value = "��ǰ:" + value + "\n��Ҫ�������ø���Ϊʲô��\n���\"��\"����Ϊtrue�����\"��\"����Ϊfalse�����\"ȡ��\"�����޸�";
				int ans = MessageBox(hwnd, value.c_str(), _T("�޸ı���"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "true";
						file.close();
						MessageBox(hwnd, _T("�������\n��־�ļ�������log�ļ�����"), _T("�޸ı���"), MB_OK);
						break;
					}
					case IDNO: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "false";
						file.close();
						MessageBox(hwnd, _T("�������"), _T("�޸ı���"), MB_OK);
						break;
					}
				}
			}
			return 0;
		}
		if (cmd[1] == "joke") { //���
			if (argc <= 2) {
				cout << "����ȱʧ�������Զ��˳�\n����joke��ʹ�÷���\n";
				cout << "-killappɱ�Ͽ������\n-copyfile��ȡU���ļ�";
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
				cout << "����ȱʧ�������Զ��˳�\n";
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
					cout << "��������\n";
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
					cout << "��������\n";
				}
			}
			if (cmd[2] == "-VerboseStatus") {
				if (cmd[3] == "true") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "1");
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
					if (MessageBox(hwnd, _T("���Ƿ�Ҫ����ע����"), _T("ע��ȷ��"), MB_OKCANCEL) == 1) {//1ȷ����2ȡ��
						system("shutdown /l");
					}
				} else if (cmd[3] == "false") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "0");
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
				} else {
					cout << "��������\n";
				}
			}
			if (cmd[2] == "-legalnotice") {
				if (cmd[3] == "true") {
					char title1[1010100];
					char title2[1010100];
					cout << "������������(505050������)��";
					scanf_s("%s", title1, (unsigned)_countof(title1));
					cout << "�����븱����(505050������)��";
					scanf_s("%s", title2, (unsigned)_countof(title2));
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", title1);
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", title2);
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
					if (MessageBox(hwnd, _T("���Ƿ�Ҫ����ע����"), _T("ע��ȷ��"), MB_OKCANCEL) == 1) {//1ȷ����2ȡ��
						system("shutdown /l");
					}
				} else if (cmd[3] == "false") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", "");
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", "");
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
				} else {
					cout << "��������\n";
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
	//����������
	//cout << argc << " arguments:" << endl;
	//ѭ����ӡ���в���
	/*for (int i = 0; i < argc; i++) {
	cout << argv[i] << endl;
	}*/
	//system(".\\SeewoKiller.exe qwert asdfg");
	system("title Seewo Killer Starting");
	poweron(skipcheck, fastboot);
	cls
	//S(10);
	//if (MessageBox(NULL, _T("����ﰥ��"), _T("����"), MB_OKCANCEL) == 2) {
	//	return 0;
	//}//����IDOK,IDCANCEL
	//thread�÷�
	//thread t(taskbarprocess,TBPF_INDETERMINATE,0);
	//t.detach();������������
	//t.join();����������
	system("title ϣ�ֿ���");
	lc.lcmain();
	ReleaseTaskbarInterface();
	return 0;
}
