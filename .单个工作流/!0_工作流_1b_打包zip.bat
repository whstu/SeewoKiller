@echo off
cd ".\BuildTools\Inno Setup 编译器\"
set zipstat1=0 & set zipstat2=0 & set zipstat3=0 & set zipstat4=0

cd "E:\devc++\DEV\SeewoKiller\installer\"

echo 打包main-with-nothing.tar.gz
call "E:\devc++\DEV\SeewoKiller\!0_工作流_1a_7za打包targz.bat" SeewoKiller_with_nothing "E:\devc++\DEV\SeewoKiller\installer\pack_file_list_main-with-nothing.txt"
if errorlevel 1 SET zipstat1=1

echo 打包main-with-gui.tar.gz
call "E:\devc++\DEV\SeewoKiller\!0_工作流_1a_7za打包targz.bat" SeewoKiller_with_gui "E:\devc++\DEV\SeewoKiller\installer\pack_file_list_main-with-gui.txt"
if errorlevel 1 SET zipstat2=1

echo 打包main-with-gui-and-freeze.tar.gz
call "E:\devc++\DEV\SeewoKiller\!0_工作流_1a_7za打包targz.bat" SeewoKiller_with_gui_and_freeze "E:\devc++\DEV\SeewoKiller\installer\pack_file_list_main-with-gui-and-freeze.txt"
if errorlevel 1 SET zipstat3=1

echo 打包main-with-freeze.tar.gz
call "E:\devc++\DEV\SeewoKiller\!0_工作流_1a_7za打包targz.bat" SeewoKiller_with_freeze "E:\devc++\DEV\SeewoKiller\installer\pack_file_list_main-with-freeze.txt"
if errorlevel 1 SET zipstat4=1

call :PrintStatus %zipstat1% "压缩 SeewoKiller_with_nothing.tar.gz" DarkCyan
call :PrintStatus %zipstat2% "压缩 SeewoKiller_with_gui.tar.gz" DarkCyan
call :PrintStatus %zipstat3% "压缩 SeewoKiller_with_gui_and_freeze.tar.gz" DarkCyan
call :PrintStatus %zipstat4% "压缩 SeewoKiller_with_freeze.tar.gz" DarkCyan

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