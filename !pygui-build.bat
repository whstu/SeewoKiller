@echo off
pyinstaller -F -w -i E:\devc++\DEV\SeewoKiller\SeewoKiller.ico .\pygui\gui.py
copy ".\dist\gui.exe" ".\gui.exe"
timeout /t 1
rd /s /q .\build
rd /s /q .\dist
del .\gui.spec
pause