@echo off

set PFX=E:\devc++\DEV\SeewoKiller\BuildTools\signtool\sign-password114514.pfx

set PASS=114514

set TIMESTAMP=http://timestamp.digicert.com

set ALG=SHA512

set INSTALLER_DIR=E:\devc++\DEV\SeewoKiller\installer

for %%f in (
    "SeewoKiller_custom.exe"
    "SeewoKiller_with_gui.exe"
    "SeewoKiller_with_gui_and_ai.exe"
    "SeewoKiller_with_gui_and_freeze.exe"
    "SeewoKiller_with_freeze.exe"
    "SeewoKiller_with_nothing.exe"
) do (
    echo Sign %%~f
    .\signtool.exe sign /f "%PFX%" /p %PASS% /t %TIMESTAMP% /fd %ALG% /v "%INSTALLER_DIR%\%%~f"
)
pause