@echo off
pyinstaller -F --onefile -i E:\devc++\DEV\SeewoKiller\label_friends_diligentsnake.ico .\pygui\pai.py
copy ".\dist\pai.exe" ".\pai.exe"
timeout /t 1
rd /s /q .\build
rd /s /q .\dist
del .\pai.spec
pause