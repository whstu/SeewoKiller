@echo off
cd /d %~dp0
PsExec.exe -sid "%~dp0\SWAceUser.exe" LINK 1 1 Z:
echo Linked Log vdisk to Z: volume.
echo Press any to un-link Z: volume.
pause
PsExec.exe -sid "%~dp0\SWAceUser.exe" ULINK Z: