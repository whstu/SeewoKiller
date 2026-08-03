@echo off
SET Error=0

echo SeewoKiller.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\BuildTools\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\SeewoKiller.exe"
if errorlevel 1 set /a Error=Error+1

echo gui.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\BuildTools\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\gui.exe"
if errorlevel 1 set /a Error=Error+10

echo ai.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\BuildTools\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\ai.exe"
if errorlevel 1 set /a Error=Error+100

echo pai.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\BuildTools\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\pai.exe"
if errorlevel 1 set /a Error=Error+1000

exit /b %Error%

pause