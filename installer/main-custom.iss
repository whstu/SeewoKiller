; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "希沃克星"
#define MyAppVersion "2.2.1"
#define MyAppProductVersion "2.2.1"
#define MyAppPublisher "WHSTU Studio"
#define MyAppURL "https://whstu.dpdns.org/"
#define MyAppExeName "SeewoKiller.exe"
#define MyAppCopyright "Copyright 2020-2025 WHSTU Studio"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{13340AD7-C6F1-46D7-8B03-4C5C78F1AD9F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
VersionInfoVersion={#MyAppProductVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoProductName={#MyAppName}
VersionInfoProductVersion={#MyAppProductVersion}
AppCopyright={#MyAppCopyright}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={commonpf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=E:\devc++\DEV\SeewoKiller\!Licence.txt
OutputDir=E:\devc++\DEV\SeewoKiller\installer
OutputBaseFilename=SeewoKiller_custom
SetupIconFile=E:\devc++\DEV\SeewoKiller\app.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern dynamic windows11
WizardSmallImageFile="E:\devc++\DEV\SeewoKiller\icon\seewokiller-icon-20260725.bmp"

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "programicon"; Description: "创建“开始”菜单快捷方式"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce;
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce;
Name: "fbicon"; Description: "创建“启动到Fastboot”快捷方式"; GroupDescription: "用于破解希沃锁屏的快速软件启动方案:"; Flags: unchecked;

[Components]
Name: "main"; Description:"主程序（核心功能）"; Types: full compact custom; Flags:fixed
Name: "GUI"; Description:"新版界面"; Types: full custom
Name: "AI"; Description:"AI（附加功能）"; Types: full custom
Name: "SeewoFreeze"; Description:"冰点还原破解（附加功能）"; Types: full custom
Name: "pai"; Description:"计算π（附加功能）"; Types: full custom

[Files]
Source: "E:\devc++\DEV\SeewoKiller\SeewoKiller.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\gui.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: GUI
Source: "E:\devc++\DEV\SeewoKiller\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\libconfig++.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\libcurl-x64.dll"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\app.ico"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\seewokiller2.png"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\RunAsFastboot.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\ai.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: AI
Source: "E:\devc++\DEV\SeewoKiller\SeewoFreeze\*"; DestDir: "{app}\SeewoFreeze"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: SeewoFreeze
Source: "E:\devc++\DEV\SeewoKiller\pai.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: pai
Source: "E:\devc++\DEV\SeewoKiller\7za.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
Source: "E:\devc++\DEV\SeewoKiller\procgov.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: main
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: programicon
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{commondesktop}\{#MyAppName}-fastboot模式"; Filename: "{app}\RunAsFastboot.exe"; Tasks: fbicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
{ 卸载时：先执行 SeewoKiller.exe uninstall，再询问是否清除所有数据 }
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  ResultCode: Integer;
  ExePath: String;
begin
  if CurUninstallStep = usUninstall then
  begin
    ExePath := ExpandConstant('{app}\{#MyAppExeName}');

    { 只在可执行文件存在时才执行 }
    if FileExists(ExePath) then
    begin
      { SeewoKiller.exe uninstall }
      ShellExec('runas', ExpandConstant('{app}\{#MyAppExeName}'),
                'run recovery uninstall', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);

      { 弹窗询问 }
      { 静默卸载 (/SILENT) 时不弹窗，直接清除数据 }
      if (UninstallSilent) or
         (MsgBox('是否清除所有数据？' + #13#10 + #13#10 +
                 '选择“是”：将' +
                 '清除所有配置与用户数据（日志、配置文件等），此操作不可恢复。' + #13#10 +
                 '选择“否”：保留现有数据，仅卸载程序。',
                 mbConfirmation, MB_YESNO) = IDYES) then
      begin
        ShellExec('runas', ExpandConstant('{app}\{#MyAppExeName}'),
                  'run recovery factoryreset', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      end;
    end;
  end;
end;