@echo off
pyinstaller -F --onefile -i E:\devc++\DEV\SeewoKiller\label_friends_diligentsnake.ico .\pygui\ai.py
copy ".\dist\ai.exe" ".\ai.exe"
timeout /t 1
rd /s /q .\build
rd /s /q .\dist
del .\ai.spec
pause