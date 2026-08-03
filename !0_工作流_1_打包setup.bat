@echo off
cd ".\BuildTools\Inno Setup 编译器\"
set stat1=0 & set stat2=0 & set stat3=0 & set stat4=0 & set stat5=0 & set stat6=0
set sigstat1=0 & set sigstat2=0 & set sigstat3=0 & set sigstat4=0 & set sigstat5=0 & set sigstat6=0
set zipstat1=0 & set zipstat2=0 & set zipstat3=0 & set zipstat4=0

echo 启动编译 main-custom.exe
call iscc "E:\devc++\DEV\SeewoKiller\installer\main-custom.iss"
if errorlevel 1 SET stat1=1

timeout /t 3 /nobreak
echo 启动编译 main-with-gui-and-freeze.exe
call iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-gui-and-freeze.iss"
if errorlevel 1 SET stat2=1

timeout /t 3 /nobreak
echo 启动编译 main-with-gui-and-ai.exe
call iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-gui-and-ai.iss"
if errorlevel 1 SET stat3=1

timeout /t 3 /nobreak
echo 启动编译 main-with-freeze.exe
call iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-freeze.iss"
if errorlevel 1 SET stat4=1

timeout /t 3 /nobreak
echo 启动编译 main-with-gui.exe
call iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-gui.iss"
if errorlevel 1 SET stat5=1

timeout /t 3 /nobreak
echo 启动编译 main-with-nothing.exe
call iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-nothing.iss"
if errorlevel 1 SET stat6=1


echo 正在签名
cd "E:\devc++\DEV\SeewoKiller\BuildTools\signtool"
set PFX=E:\devc++\DEV\SeewoKiller\BuildTools\signtool\sign-password114514.pfx
set PASS=114514
set TIMESTAMP=http://timestamp.digicert.com
set ALG=SHA512
set INSTALLER_DIR=E:\devc++\DEV\SeewoKiller\installer
.\signtool.exe sign /f "%PFX%" /p %PASS% /t %TIMESTAMP% /fd %ALG% /v "%INSTALLER_DIR%\SeewoKiller_custom.exe"
if errorlevel 1 SET sigstat1=1
.\signtool.exe sign /f "%PFX%" /p %PASS% /t %TIMESTAMP% /fd %ALG% /v "%INSTALLER_DIR%\SeewoKiller_with_gui.exe"
if errorlevel 1 SET sigstat2=1
.\signtool.exe sign /f "%PFX%" /p %PASS% /t %TIMESTAMP% /fd %ALG% /v "%INSTALLER_DIR%\SeewoKiller_with_gui_and_ai.exe"
if errorlevel 1 SET sigstat3=1
.\signtool.exe sign /f "%PFX%" /p %PASS% /t %TIMESTAMP% /fd %ALG% /v "%INSTALLER_DIR%\SeewoKiller_with_gui_and_freeze.exe"
if errorlevel 1 SET sigstat4=1
.\signtool.exe sign /f "%PFX%" /p %PASS% /t %TIMESTAMP% /fd %ALG% /v "%INSTALLER_DIR%\SeewoKiller_with_freeze.exe"
if errorlevel 1 SET sigstat5=1
.\signtool.exe sign /f "%PFX%" /p %PASS% /t %TIMESTAMP% /fd %ALG% /v "%INSTALLER_DIR%\SeewoKiller_with_nothing.exe"
if errorlevel 1 SET sigstat6=1

cd "E:\devc++\DEV\SeewoKiller\installer\"

echo 打包main-with-nothing.tar.gz
call "E:\devc++\DEV\SeewoKiller\!0_工作流_1a_7za打包targz.bat" SeewoKiller_with_nothing "E:\devc++\DEV\SeewoKiller\installer\pack_file_list_main-with-nothing.txt"
if errorlevel 1 SET zipstat1=1

:echo 打包main-with-gui.tar.gz
:call "E:\devc++\DEV\SeewoKiller\!0_工作流_1a_7za打包targz.bat" SeewoKiller_with_gui "E:\devc++\DEV\SeewoKiller\installer\pack_file_list_main-with-gui.txt"
:if errorlevel 1 SET zipstat2=1

:echo 打包main-with-gui-and-freeze.tar.gz
:call "E:\devc++\DEV\SeewoKiller\!0_工作流_1a_7za打包targz.bat" SeewoKiller_with_gui_and_freeze "E:\devc++\DEV\SeewoKiller\installer\pack_file_list_main-with-gui-and-freeze.txt"
:if errorlevel 1 SET zipstat3=1

:echo 打包main-with-freeze.tar.gz
:call "E:\devc++\DEV\SeewoKiller\!0_工作流_1a_7za打包targz.bat" SeewoKiller_with_freeze "E:\devc++\DEV\SeewoKiller\installer\pack_file_list_main-with-freeze.txt"
:if errorlevel 1 SET zipstat4=1

REM 输出结果
call :PrintStatus %stat1% "编译 SeewoKiller_custom.exe" DarkYellow
call :PrintStatus %stat2% "编译 SeewoKiller_with_gui_and_freeze.exe" DarkYellow
call :PrintStatus %stat3% "编译 SeewoKiller_with_gui_and_ai.exe" DarkYellow
call :PrintStatus %stat4% "编译 SeewoKiller_with_freeze.exe" DarkYellow
call :PrintStatus %stat5% "编译 SeewoKiller_with_gui.exe" DarkYellow
call :PrintStatus %stat6% "编译 SeewoKiller_with_gui_and_nothing.exe" DarkYellow
echo ==============
call :PrintStatus %sigstat1% "签名 SeewoKiller_custom.exe" Yellow
call :PrintStatus %sigstat2% "签名 SeewoKiller_with_gui_and_freeze.exe" Yellow
call :PrintStatus %sigstat3% "签名 SeewoKiller_with_gui_and_ai.exe" Yellow
call :PrintStatus %sigstat4% "签名 SeewoKiller_with_freeze.exe" Yellow
call :PrintStatus %sigstat5% "签名 SeewoKiller_with_gui.exe" Yellow
call :PrintStatus %sigstat6% "签名 SeewoKiller_with_gui_and_nothing.exe" Yellow
echo ==============
call :PrintStatus %zipstat1% "压缩 SeewoKiller_with_nothing.tar.gz" DarkCyan
:call :PrintStatus %zipstat2% "压缩 SeewoKiller_with_gui.tar.gz" DarkCyan
:call :PrintStatus %zipstat3% "压缩 SeewoKiller_with_gui_and_freeze.tar.gz" DarkCyan
:call :PrintStatus %zipstat4% "压缩 SeewoKiller_with_freeze.tar.gz" DarkCyan

set ALL_SUCCESS=1
if not %stat1%==0 set ALL_SUCCESS=0
if not %stat2%==0 set ALL_SUCCESS=0
if not %stat3%==0 set ALL_SUCCESS=0
if not %stat4%==0 set ALL_SUCCESS=0
if not %stat5%==0 set ALL_SUCCESS=0
if not %stat6%==0 set ALL_SUCCESS=0
if not %sigstat1%==0 set ALL_SUCCESS=0
if not %sigstat2%==0 set ALL_SUCCESS=0
if not %sigstat3%==0 set ALL_SUCCESS=0
if not %sigstat4%==0 set ALL_SUCCESS=0
if not %sigstat5%==0 set ALL_SUCCESS=0
if not %sigstat6%==0 set ALL_SUCCESS=0
if not %zipstat1%==0 set ALL_SUCCESS=0
:if not %zipstat2%==0 set ALL_SUCCESS=0
:if not %zipstat3%==0 set ALL_SUCCESS=0
:if not %zipstat4%==0 set ALL_SUCCESS=0
if %ALL_SUCCESS%==1 (
    powershell -command "Write-Host 'Everything' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Done' -ForegroundColor Black -BackgroundColor Green"
) else (
    powershell -command "Write-Host 'Something' -ForegroundColor DarkYellow -NoNewline; Write-Host ' ' -NoNewline; Write-Host 'Error' -ForegroundColor Black -BackgroundColor Red"
)

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