@echo off
pyinstaller -F -w -i E:\devc++\DEV\SeewoKiller\app.ico .\pygui\gui-fluent.py
copy ".\dist\gui-fluent.exe" ".\gui-fluent.exe"
timeout /t 2
rd /s /q .\build
rd /s /q .\dist
del .\gui-fluent.spec
pause