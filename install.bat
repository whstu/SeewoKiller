@echo on
cd /d %~dp0
set VDISK_TOOL_PATH=..\SeewoDriverService\SWAce\user
set BROOK_TOOL_PATH=..\SeewoDriverService\SWBrook\user
xcopy "%~dp0%VDISK_TOOL_PATH%\*" "%~dp0" /S /Y
xcopy "%~dp0%BROOK_TOOL_PATH%\*" "%~dp0" /S /Y
InstallPackageTool.exe install