@echo off
SET Error=0

cd E:\devc++\DEV\SeewoKiller\
E:\python\APP\Python3.11\Scripts\python.exe E:\devc++\DEV\SeewoKiller\!0_工作流_0b_SeewoKiller_gui.rc-build.py
if errorlevel 1 powershell -command "Write-Host 'Python运行错误。' -ForegroundColor Red" & SET Error=1

echo Compiling .rc file...
.\BuildTools\ResourceHacker\rh.exe -open .\SeewoKiller_gui.rc -save .\SeewoKiller_gui.res -action compile -log NUL
if errorlevel 1 powershell -command "Write-Host 'RH res编译错误。' -ForegroundColor Red" & SET Error=1

echo Building exe file...
.\BuildTools\ResourceHacker\rh.exe -open gui.exe -save gui.exe -action add -res .\SeewoKiller_gui.res -mask VERSIONINFO
if errorlevel 1 powershell -command "Write-Host 'RH exe注入错误。' -ForegroundColor Red" & SET Error=1

powershell -command "Write-Host 'RC-Process' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline"

if %Error%==1 powershell -command "Write-Host 'Error.' -ForegroundColor Black -BackgroundColor Red" & exit /b 1

powershell -command "Write-Host 'Done.' -ForegroundColor Black -BackgroundColor Green"
exit /b 0

pause

:: :error
:: powershell -command "Write-Host '发生错误。' -ForegroundColor Red"