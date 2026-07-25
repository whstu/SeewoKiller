cd ".\BuildTools\Inno Setup 编译器\"

echo 启动编译 main-custom.exe
start iscc "E:\devc++\DEV\SeewoKiller\installer\main-custom.iss"

timeout /t 3 /nobreak
echo 启动编译 main-with-gui-and-freeze.exe
start iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-gui-and-freeze.iss"

timeout /t 3 /nobreak
echo 启动编译 main-with-gui-and-ai.exe
start iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-gui-and-ai.iss"

timeout /t 3 /nobreak
echo 启动编译 main-with-gui.exe
start iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-freeze.iss"

timeout /t 3 /nobreak
echo 启动编译 main-with-gui.exe
start iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-gui.iss"

timeout /t 3 /nobreak
echo 启动编译 main-with-nothing.exe
iscc "E:\devc++\DEV\SeewoKiller\installer\main-with-nothing.iss"

pause

echo 正在签名
cd "E:\devc++\DEV\SeewoKiller\BuildTools\signtool"
call "E:\devc++\DEV\SeewoKiller\BuildTools\signtool\签名-安装包.bat"

cd "E:\devc++\DEV\SeewoKiller\BuildTools\"

echo 打包main-with-nothing.tar.gz
E:\devc++\DEV\SeewoKiller\!0_工作流_1a_7za打包targz.bat main-with-nothing "E:\devc++\DEV\SeewoKiller\installer\pack_file_list_main-with-nothing.txt"

cd E:\devc++\DEV\SeewoKiller
pause