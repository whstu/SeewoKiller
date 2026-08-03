@echo off
cd ".\BuildTools\Inno Setup 编译器\"
set stat4=0
set sigstat4=0

echo 启动编译 main-with-freeze.exe
call iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-freeze.iss"
if errorlevel 1 SET stat4=1

echo 正在签名
cd "E:\devc++\DEV\SeewoKiller\BuildTools\signtool"
set PFX=E:\devc++\DEV\SeewoKiller\BuildTools\signtool\sign-password114514.pfx
set PASS=114514
set TIMESTAMP=http://timestamp.digicert.com
set ALG=SHA512
set INSTALLER_DIR=E:\devc++\DEV\SeewoKiller\installer
.\signtool.exe sign /f "%PFX%" /p %PASS% /t %TIMESTAMP% /fd %ALG% /v "%INSTALLER_DIR%\SeewoKiller_with_gui_and_freeze.exe"
if errorlevel 1 SET sigstat4=1

call :PrintStatus %stat4% "编译 SeewoKiller_with_freeze.exe" DarkYellow
call :PrintStatus %sigstat4% "签名 SeewoKiller_with_freeze.exe" Yellow

cd E:\devc++\DEV\SeewoKiller
pause
exit /b 0

:PrintStatus
REM 接收两个参数：%1=flag, %2=task
set "flag=%~1"
set "task=%~2"
set "TaskColorForeG=%~3"
REM set "TaskColorBackG=%~4"
REM if %TaskColorForeG% == "" set TaskColorForeG=DarkYellow
REM if %TaskColorBackG% == "" set TaskColorBackG=Black
if "%flag%"=="0" (
    powershell -command "Write-Host '%task%' -ForegroundColor %TaskColorForeG% -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Done' -ForegroundColor Black -BackgroundColor Green"
) else (
    powershell -command "Write-Host '%task%' -ForegroundColor %TaskColorForeG% -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Error' -ForegroundColor Black -BackgroundColor Red"
)
exit /b 0