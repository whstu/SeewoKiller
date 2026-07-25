@echo off

set "ARCHIVE_NAME=%~1"

set "LIST_FILE=%~2"

if exist "%ARCHIVE_NAME%.tar.gz" del "%ARCHIVE_NAME%.tar.gz"

call "E:\devc++\DEV\SeewoKiller\BuildTools\7za.exe" a -ttar "%ARCHIVE_NAME%.tar" -i@"%LIST_FILE%" -mx=0
if errorlevel 1 goto :error

call "E:\devc++\DEV\SeewoKiller\BuildTools\7za.exe" a -tgzip "%ARCHIVE_NAME%.tar.gz" "%ARCHIVE_NAME%.tar"
if errorlevel 1 goto :error

del "%ARCHIVE_NAME%.tar"
echo —πÀı≥…π¶£∫%ARCHIVE_NAME%.tar.gz
exit /b 0

:error
echo —πÀı ß∞‹£° & exit /b 1