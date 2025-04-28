; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "ϣ�ֿ���"
#define MyAppVersion "2.0.0.0"
#define MyAppProductVersion "2.0.0.0"
#define MyAppPublisher "WHSTU Studio"
#define MyAppURL "https://whstu.dpdns.org/"
#define MyAppExeName "SeewoKiller.exe"
#define MyAppCopyright "Copyright 2020-2025 WHSTU Studio"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
AppId={{13340AD7-C6F1-46D7-8B03-4C5C78F1AD9F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
VersionInfoVersion={#MyAppProductVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoProductName={#MyAppName}
VersionInfoProductVersion={#MyAppProductVersion};AppVerName={#MyAppName} {#MyAppVersion}
AppCopyright={#MyAppCopyright}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=E:\devc++\DEV\SeewoKiller\!Licence.txt
OutputDir=E:\devc++\DEV\SeewoKiller\installer
OutputBaseFilename=setup-custom
SetupIconFile=E:\devc++\DEV\SeewoKiller\label_friends_diligentsnake.ico
Compression=lzma
SolidCompression=yes
WizardStyle=classic
WizardSmallImageFile="E:\devc++\DEV\SeewoKiller\installer\label_friends_diligentsnake.bmp"

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "programicon"; Description: "��������ʼ���˵���ݷ�ʽ"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce;
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce;

[Components]
Name: "main"; Description:"�����򣨺��Ĺ��ܣ�"; Types: full compact custom; Flags:fixed
Name: "AI"; Description:"AI�����ӹ��ܣ�"; Types: full custom
Name: "SeewoFreeze"; Description:"���㻹ԭ�ƽ⣨���ӹ��ܣ�"; Types: full custom
Name: "pai"; Description:"����У����ӹ��ܣ�"; Types: full custom

[Files]
Source: "E:\devc++\DEV\SeewoKiller\SeewoKiller.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\gui.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\app.ico"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\seewokiller2.png"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\ai.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: AI
Source: "E:\devc++\DEV\SeewoKiller\SeewoFreeze\*"; DestDir: "{app}\SeewoFreeze"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: SeewoFreeze
Source: "E:\devc++\DEV\SeewoKiller\pai.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: pai
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: programicon
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

