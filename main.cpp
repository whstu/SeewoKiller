#include <bits/stdc++.h>
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <string>
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
bool closeapp = false;

int box = 1/*���*/, boxn = 4/*�������*/;
struct Word {
	string box[5] {"NULL", "����", "���Ĺ���", "���ӹ���", "����"};
	int recentn = 3;
	string recent[4] = {"NULL", "����ϰ�Ʋ�ģʽ", "һ��������", "С��Ϸ>>>"};
	int alln = 7;
	string all[8] = {"NULL", "ѭ��������", "һ��ж��", "����ϰ�Ʋ�ģʽ", "һ��������", "С��Ϸ>>>", "���>>>", "ע���>>>"};
	int moren = 3;
	string more[4] = {"NULL", "���㻹ԭ�ƽ�", "AI","�����"};
	int settingn = 5;
	string setting[6] = {"NULL", "�˳�", "������ϰ�Ʋ�/ѭ��������ʱ������־", "ʹ���°����", "������ѧ������", "����"};
	int gamen = 3;
	string game[4] = {"NULL", "����", "����ը��", "������"};
	int joken = 2;
	string joke[3] = {"NULL", "����", "ɱWPS+ϣ�ְװ�+ϣ����Ƶչ̨"};
	int regn = 11;
	string reg[12] = {"NULL", "����", "һ������(�ݲ�����)", "һ������(�ݲ�����)", "�����������˵�", "�����������˵�", "���ÿ�ݼ�", "���ÿ�ݼ�", "������ʾ��¼��ϸ��Ϣ", "������ʾ��¼��ϸ��Ϣ", "��¼ʱ��ʾ��ʾ", "ȡ����¼ʱ��ʾ��ʾ"};
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
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~(WS_MINIMIZEBOX);
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	ShowWindow(hwnd, SW_MAXIMIZE);//���
	DestroyMenu(hmenu);
	ReleaseDC(hwnd, NULL);
}

void quickstart() {
	int step = 1;
	cls
	while (step <= 4) {
		cls
		gotoxy(0, 6);
		cout << "\n\n\n���ǳ�ѧ���������򡣰�a���أ���d��������s��������\n\n";
		cout << "��" << step << "������4��\n\n";
		switch (step) {
			case 1: {
				cout << "��wasd������������";
				break;
			}
			case 2: {
				cout << "���ո��ȷ��";
				break;
			}
			case 3: {
				cout << "����\">>>\"��ѡ���������Ŀ�����԰��ո����";
				break;
			}
			case 4: {
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
				if (MessageBox(NULL, _T("��ȷʵҪ������\n���������򽫻ᱣ�������ð����"), _T("����"), MB_YESNO) == IDYES) {
					cls
					return;
				}
			}
		}
	}
	cls
}
void poweron(bool SkipCheckWinVer) {
	//1������2���̣�3���࣬4��죬5���ϣ�6��ƣ�7�Ұף�Ĭ�ϣ���8���
	//9ǳ����10ǳ�̣�11ǳ�࣬12ǳ�죬13ǳ�ϣ�14ǳ�ƣ�15��ɫ��0��ɫ
	connot_close_button();
	setfont(30);
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
	gotoxy(10, 16);
	cout << "[=                   ]";
	S(500);
	//����ı��ֵ�ٴγ�ʼ��
	word.setting[2] = "������ϰ�Ʋ�/ѭ��������ʱ������־";
	//---
	if (fileExist(".\\settings\\write-log-when-killapp.seewokiller") == false) {
		ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
		file << "false";
		file.close();
	} else {
		ifstream file(".\\settings\\write-log-when-killapp.seewokiller");
		string value;
		getline(file, value);
		file.close();
		if (value == "true") {
			word.setting[2] = word.setting[2] + "-��ǰ:true";
		} else if (value == "false") {
			word.setting[2] = word.setting[2] + "-��ǰ:false";
		} else {
			ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
			file << "false";
			file.close();
			word.setting[2] = word.setting[2] + "-��ǰ:false";
		}
	}
	//-----
	gotoxy(16, 14);
	cout << "������֤ϵͳ�汾(2/4) ";
	gotoxy(10, 16);
	cout << "[=====               ]";
	S(200);
	//���Windows�汾
	typedef void(__stdcall * NTPROC)(DWORD*, DWORD*, DWORD*);
	HINSTANCE hinst = LoadLibrary(TEXT("ntdll.dll"));//����DLL
	NTPROC GetNtVersionNumbers = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers");//��ȡ������ַ
	DWORD dwMajor, dwMinor, dwBuildNumber;
	GetNtVersionNumbers(&dwMajor, &dwMinor, &dwBuildNumber);
	gotoxy(15, 14);
	printf("Windows�汾: %d.%d", dwMajor, dwMinor);
	cout << "(3/4)      ";
	gotoxy(10, 16);
	cout << "[=======             ]";
	S(400);
	dwMajorInt = static_cast<int>(dwMajor);
	dwMinorInt = static_cast<int>(dwMinor);
	float version = dwMajorInt + dwMinorInt * 0.1;
	if (SkipCheckWinVer == false) {
		if (version >= 6.1) {
			if (MessageBox(NULL, _T("��⵽���ϵͳΪWindows 7+��\n�Ƿ�ʹ��ȫ��UI��"), _T("��ʾ"), MB_OKCANCEL) == 1) {
				string guipath = executable_path + "\\gui.exe";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(guipath.c_str()));//��Ȩ�޵Ķ����Դ�
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
				DWORD dwExitCode;
				if (fSuccess) {
					gotoxy(10, 16);
					cout << "[=============       ]";
					closeapp = true;
					S(10);
					gotoxy(10, 16);
					cout << "[====================]";
					S(200);
					return;
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
	gotoxy(10, 16);
	cout << "[===========         ]";
	S(400);
	CreateDirectory("./settings", NULL);
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
	gotoxy(10, 16);
	cout << "[===============     ]";
	S(10);
	gotoxy(10, 16);
	cout << "[====================]";
	S(100);
	setfont(30);
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
	cout << "\nϣ�ֿ���\n";
	cout << "\n׿Ȼ�����۹�https://whstu.us.kg/�ṩ����֧��";
	cout << "\n�������UI����SlytherinOS���";
	cout << "\n�°�������PyQt5\n";
	cout << "��b+�س�����\n";
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
		system("taskkill /f /t /im taskmgr.exe");
		cout << "���ڽ������̣���¼��\n";
		cout << "TASKKILL /F /IM EasiRecorder.exe\n";
		system("TASKKILL /F /IM EasiRecorder.exe");
		if (KillSeewoService == true) {
			cout << "���ڽ������̣�ϣ�ֹܼ�\n";
			cout << "TASKKILL /F /IM SeewoServiceAssistant.exe\n";
			system("TASKKILL /F /IM SeewoServiceAssistant.exe");
			cout << "TASKKILL /F /IM SeewoAbility.exe\n";
			system("TASKKILL /F /IM SeewoAbility.exe");
			cout << "TASKKILL /F /IM SeewoCore.exe\n";
			system("TASKKILL /F /IM SeewoCore.exe");
		}
		if (Wanzixi == true) {
			cout << "���ڽ������̣�����\n";
			cout << "TASKKILL /F /IM SystemSettings.exe\n";
			system("TASKKILL /F /IM SystemSettings.exe");
			cout << "���ڽ������̣��������\n";
			cout << "TASKKILL /F /FI \"WINDOWTITLE eq ��������\"\n";
			system("taskkill /f /fi \"WINDOWTITLE eq ��������\"");
			system("taskkill /f /fi \"WINDOWTITLE eq �������\\����� Internet\\��������\"");
			cout << "���ڽ������̣�Edge\n";
			cout << "TASKKILL /F /IM msedge.exe\n";
			system("TASKKILL /F /IM msedge.exe");
			cout << "���ڽ������̣�IE\n";
			cout << "TASKKILL /F /IM iexplore.exe\n";
			system("TASKKILL /F /IM iexplore.exe");
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

void pingbaoservice() {
	SetColorAndBackground(4, 6);
	cout << "���棺�������������ϿΣ�\n";
	SetColorAndBackground(0, 7);
	cout << "ÿ100������Ļһ�Σ��뽫����ƶ�������λ��\n";
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
} game;

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
		system("title ϣ�ֿ���");
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
			//��ҳ��
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
			}
			if (s == "ѭ��������") {
				taskkill(true, false);
			}
			if (s == "һ��ж��û�õ����") {
				uninstall();
				s = "-1";
				continue;
			}
			if (s == "AI") {
				setfont(20);
				string aipath = executable_path + "\\ai.exe";
				system(aipath.c_str());
				system("pause");
				setfont(30);
				s = "-1";
				continue;
			}
			if(s=="�����"){
				setfont(20);
				string aipath = executable_path + "\\pai.exe";
				system(aipath.c_str());
				setfont(30);
				s = "-1";
				continue;
			}
			if (s == "���㻹ԭ�ƽ�") {
				system("title ���㻹ԭ");
				cout << "\n���ȹرձ��㴰�ں��ټ�������ϣ�ֿ��ǡ�\n";
				string unfreezepath = executable_path + "\\SeewoFreeze\\SeewoFreezeUI.exe --startup-with-main-window";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//��Ȩ�޵Ķ����Դ�
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
				DWORD dwExitCode;
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
			}
			if (s == "����ϰ�Ʋ�ģʽ") {
				system("title �Ʋ�����ϰ");
				taskkill(true, true);
			}
			if (s == "һ��������") {
				pingbaoservice();
			}
			if (s == "�˳�") {
				return;
			}
			if (s == "����") {
				about();
				s = "-1";
				continue;
			}
			if (s == "������ѧ������") {
				quickstart();
				s = "-1";
				continue;
			}
			if (s == "������ϰ�Ʋ�/ѭ��������ʱ������־-��ǰ:false" or s == "������ϰ�Ʋ�/ѭ��������ʱ������־-��ǰ:true") {
				ifstream file(".\\settings\\write-log-when-killapp.seewokiller");
				string value;
				getline(file, value);
				value = "��ǰ:" + value + "\n��Ҫ�������ø���Ϊʲô��\n���\"��\"����Ϊtrue�����\"��\"����Ϊfalse�����\"ȡ��\"�����޸�";
				int ans = MessageBox(NULL, value.c_str(), _T("�޸ı���"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "true";
						file.close();
						if (MessageBox(NULL, _T("�޸���ɣ��Ƿ��������¼��������ļ���"), _T("��ʾ"), MB_YESNO) == IDYES) {
							cls
							poweron(true);
						}
						break;
					}
					case IDNO: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "false";
						file.close();
						if (MessageBox(NULL, _T("�޸���ɣ��Ƿ��������¼��������ļ���"), _T("��ʾ"), MB_YESNO) == IDYES) {
							cls
							poweron(true);
						}
						break;
					}
				}
				s = "-1";
				continue;
			}
			if (s == "С��Ϸ") {
				head();
				string d = listname(false, false, word.game, word.gamen);
				if (d == "����") {
					s = "-1";
					continue;
				}
				if (d == "����ը��") {
					game.numberdamn();
					d = "����";
				}
				if (d == "������") {
					setfont(20);
					game.wzq.wzqmain();
					setfont(30);
					d = "����";
				}
			}
			if (s == "���") {
				head();
				string d = listname(false, false, word.joke, word.joken);
				if (d == "����") {
					s = "-1";
					continue;
				}
				if (d == "ɱWPS+ϣ�ְװ�+ϣ����Ƶչ̨") {
					joke.kill();
				}
			}
			if (s == "ע���") {
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
					d = "����";
				}
				if (d == "�����������˵�") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
					regedit("HKEY_CURRENT_USER", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoTrayContextMenu", "REG_DWORD", "0");
					restartexp();
					d = "����";
				}
				if (d == "���ÿ�ݼ�") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "1");
					restartexp();
					d = "����";
				}
				if (d == "���ÿ�ݼ�") {
					regedit("HKEY_CURRENT_USER", "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\", "NoWinKeys", "REG_DWORD", "0");
					restartexp();
					d = "����";
				}
				if (d == "������ʾ��¼��ϸ��Ϣ") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "1");
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
					if (MessageBox(NULL, _T("ע��ȷ��(Beta)"), _T("���Ƿ�Ҫ����ע����"), MB_OKCANCEL) == 1) {//1ȷ����2ȡ��
						system("shutdown /l");
					}
					system("pause");
					d = "����";
				}
				if (d == "������ʾ��¼��ϸ��Ϣ") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "VerboseStatus", "REG_DWORD", "0");
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
					system("pause");
					d = "����";
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
					if (MessageBox(NULL, _T("ע��ȷ��(Beta)"), _T("���Ƿ�Ҫ����ע����"), MB_OKCANCEL) == 1) {//1ȷ����2ȡ��
						system("shutdown /l");
					}
					system("pause");
					d = "����";
				}
				if (d == "ȡ����¼ʱ��ʾ��ʾ") {
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticecaption", "REG_SZ", "");
					regedit("HKEY_LOCAL_MACHINE", "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", "legalnoticetext", "REG_SZ", "");
					cout << "�޸���ɣ���ע���Լ���Ƿ��޸ĳɹ���\n";
					system("pause");
					d = "����";
				}
			}
			//end
		}
	}
} lc;

int main(int argc, char *argv[]) {
	system("title ���ڳ�ʼ��");
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
		string cmd[100];
		for (int i = 0; i < argc; i++) {
			cmd[i] = argv[i];
		}
		//����Ҫadmin
		if (cmd[1] == "about") {
			about();
			return 0;
		}
		if (cmd[1] == "game") { //��Ϸ
			if (argc <= 2) {
				cout << "����ȱʧ�������Զ��˳�\n����game��ʹ�÷���\n";
				cout << "-wzq������\n-numberdamn����ը��\n";
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
		//��Ҫadmin
		if (IsUserAnAdmin() == false) {
			cout << "������δȡ�ù���ԱȨ�ޣ������޷����С�\n��ʹ�ù���ԱȨ�������նˡ�";
			return 0;
		}
		if (cmd[1] == "run") { //����
			if (argc <= 2) {
				cout << "����ȱʧ�������Զ��˳�\n����run��ʹ�÷���\n";
				cout << "-newuiǿ����UI\n-olduiǿ�ƾ�UI\n";
				return 0;
			}
			if (cmd[2] == "-skipcheck") {
				skipcheck = true;
			}
			if (cmd[2] == "-oldui") {
				string unfreezepath = executable_path + "\\SeewoKiller.exe run -skipcheck";
				STARTUPINFO si = { sizeof(si) };//0
				PROCESS_INFORMATION pi;
				LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//��Ȩ�޵Ķ����Դ�
				BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
				DWORD dwExitCode;
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
			system("title �Ʋ�����ϰ");
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
			system("title һ��������");
			pingbaoservice();
			return 0;
		}
		if (cmd[1] == "seewofreeze") {
			cout << "\n���ȹرձ��㴰�ں��ټ���������\n";
			string unfreezepath = executable_path + "\\SeewoFreeze\\SeewoFreezeUI.exe --startup-with-main-window";
			STARTUPINFO si = { sizeof(si) };//0
			PROCESS_INFORMATION pi;
			LPTSTR szCommandLine = _tcsdup(TEXT(unfreezepath.c_str()));//��Ȩ�޵Ķ����Դ�
			BOOL fSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);//��������
			DWORD dwExitCode;
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
				int ans = MessageBox(NULL, value.c_str(), _T("�޸ı���"), MB_YESNOCANCEL);
				switch (ans) {
					case IDYES: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "true";
						file.close();
						MessageBox(NULL, _T("�������"), _T("�޸ı���"), MB_OK);
						break;
					}
					case IDNO: {
						ofstream file(".\\settings\\write-log-when-killapp.seewokiller");
						file << "false";
						file.close();
						MessageBox(NULL, _T("�������"), _T("�޸ı���"), MB_OK);
						break;
					}
				}
			}
			return 0;
		}
		if (cmd[1] == "joke") { //���
			if (argc <= 2) {
				cout << "����ȱʧ�������Զ��˳�\n����joke��ʹ�÷���\n";
				cout << "-killappɱ�Ͽ������\n";
			}
			if (cmd[2] == "-killapp") {
				joke.kill();
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
					if (MessageBox(NULL, _T("ע��ȷ��(Beta)"), _T("���Ƿ�Ҫ����ע����"), MB_OKCANCEL) == 1) {//1ȷ����2ȡ��
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
					if (MessageBox(NULL, _T("ע��ȷ��(Beta)"), _T("���Ƿ�Ҫ����ע����"), MB_OKCANCEL) == 1) {//1ȷ����2ȡ��
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
	poweron(skipcheck);
	if (closeapp == true) {
		return 0;
	}
	cls
	//S(10);
	//if (MessageBox(NULL, _T("����ﰥ��"), _T("����"), MB_OKCANCEL) == 2) {
	//	return 0;
	//}//����1ȷ����2ȡ��
	//��ȡ����·��
	system("title ϣ�ֿ���");
	lc.lcmain();
	return 0;
}
