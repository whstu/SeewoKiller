@echo off

echo 签名：SeewoKiller_custom.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\installer\SeewoKiller_custom.exe"

echo 签名：SeewoKiller_with_gui.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\installer\SeewoKiller_with_gui.exe"

echo 签名：SeewoKiller_with_gui_and_ai.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\installer\SeewoKiller_with_gui_and_ai.exe"

echo 签名：SeewoKiller_with_gui_and_freeze.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\installer\SeewoKiller_with_gui_and_freeze.exe"

echo 签名：SeewoKiller_with_nothing.exe
.\signtool.exe sign /f "E:\devc++\DEV\SeewoKiller\signtool\sign-password114514.pfx" /p 114514 /t http://timestamp.digicert.com /fd SHA512 /v "E:\devc++\DEV\SeewoKiller\installer\SeewoKiller_with_nothing.exe"

pause
