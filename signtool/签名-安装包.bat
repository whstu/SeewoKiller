@echo off
echo 签名：setup.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\installer\setup.exe"
echo 签名：setup-small.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\installer\setup-small.exe"
echo 签名：setup-ai.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\installer\setup-ai.exe"
pause