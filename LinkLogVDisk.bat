@echo off
cd /d %~dp0
PsExec.exe -sid "%~dp0\SWAceUser.exe" LINK 0 1 Y:
echo Linked Log vdisk to Y: volume.
echo Press any to un-link Y: volume.
pause
PsExec.exe -sid "%~dp0\SWAceUser.exe" ULINK Y: