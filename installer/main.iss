; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "ϣ�ֿ���"
#define MyAppVersion "2.-10.-9.-8"
#define MyAppPublisher "WHSTU Studio"
#define MyAppURL "https://whstu.us.kg/"
#define MyAppExeName "SeewoKiller.exe"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
AppId={{8FC0FE31-92C3-48EE-8DFA-6C72015C01A4}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=E:\devc++\DEV\SeewoKiller\!Licence.txt
InfoAfterFile=E:\devc++\DEV\SeewoKiller\!install.txt
OutputDir=E:\devc++\DEV\SeewoKiller\installer
OutputBaseFilename=setup
SetupIconFile=E:\devc++\DEV\SeewoKiller\SeewoKiller.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"
Name: "english"; MessagesFile: "compiler:Languages\English.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "E:\devc++\DEV\SeewoKiller\SeewoKiller.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\devc++\DEV\SeewoKiller\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\devc++\DEV\SeewoKiller\SeewoKiller.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\devc++\DEV\SeewoKiller\SeewoKiller.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\devc++\DEV\SeewoKiller\SeewoFreeze\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "E:\devc++\DEV\SeewoKiller\gui.exe"; DestDir: "{app}"; Flags: ignoreversion
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

