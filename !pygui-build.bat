@echo off
pyinstaller -F -w -i E:\devc++\DEV\SeewoKiller\app.ico .\pygui\gui.py
copy ".\dist\gui.exe" ".\gui.exe"
timeout /t 2
rd /s /q .\build
rd /s /q .\dist
del .\gui.spec
pause