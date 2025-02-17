; 脚本由 Inno Setup 脚本向导 生成！

; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "希沃克星"
#define MyAppVersion "2.-10.-8.-10"
#define MyAppPublisher "WHSTU Studio"
#define MyAppURL "https://whstu.us.kg/"
#define MyAppExeName "SeewoKiller.exe"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
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
Source: "E:\devc++\DEV\SeewoKiller\seewokiller2.png"; DestDir: "{app}"; Flags: ignoreversion
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

