@echo off
SET Error0a=0

echo 正在注入版本信息 (RC-Process.bat)...
E:
cd E:\devc++\DEV\SeewoKiller\
call "E:\devc++\DEV\SeewoKiller\!0_工作流_0a_RC-Process.bat"

if errorlevel 1 SET Error0a=1

echo.
echo.
echo.
echo 正在签名 (signtool/签名-软件.bat)
cd "E:\devc++\DEV\SeewoKiller\BuildTools\signtool"
call "E:\devc++\DEV\SeewoKiller\BuildTools\signtool\签名-软件.bat"
set "ret=%errorlevel%"

REM 检查返回值
REM 逐位检查（第1~4位）skmain,gui,ai,pai
set /a bit1=%ret% %% 2
set /a bit2=(%ret%-bit1)/10 %% 2
set /a bit3=(%ret%-bit1-bit2*10) %% 2
set /a bit4=(%ret%-bit1-bit2*10-bit3*100) %% 2
echo %bit1% %bit2% %bit3% %bit4%

REM 输出返回状态
powershell -command "Write-Host 'RC-Process' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline"
if %Error0a% == 1 (
    powershell -command "Write-Host 'Error.' -ForegroundColor Black -BackgroundColor Red"
) else (
    powershell -command "Write-Host 'Done.' -ForegroundColor Black -BackgroundColor Green"
)


if %bit1% == 1 (
    powershell -command "Write-Host 'Sign SeewoKiller.exe' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Error.' -ForegroundColor Black -BackgroundColor Red"
) else (
    powershell -command "Write-Host 'Sign SeewoKiller.exe' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Done.' -ForegroundColor Black -BackgroundColor Green"
)
if %bit2% == 1 (
    powershell -command "Write-Host 'Sign gui.exe' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Error.' -ForegroundColor Black -BackgroundColor Red"
) else (
    powershell -command "Write-Host 'Sign gui.exe' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Done.' -ForegroundColor Black -BackgroundColor Green"
)
if %bit3% == 1 (
    powershell -command "Write-Host 'Sign ai.exe' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Error.' -ForegroundColor Black -BackgroundColor Red"
) else (
    powershell -command "Write-Host 'Sign ai.exe' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Done.' -ForegroundColor Black -BackgroundColor Green"
)
if %bit4% == 1 (
    powershell -command "Write-Host 'Sign pai.exe' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Error.' -ForegroundColor Black -BackgroundColor Red"
) else (
    powershell -command "Write-Host 'Sign pai.exe' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Done.' -ForegroundColor Black -BackgroundColor Green"
)

cd E:\devc++\DEV\SeewoKiller
pause