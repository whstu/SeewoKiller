@echo off

E:\python\APP\Python3.11\Scripts\python.exe -m PyInstaller --upx-dir "E:\devc++\DEV\SeewoKiller\pygui\upx-5.1.0" SeewoKiller.spec

copy ".\dist\SeewoKiller.exe" "E:\devc++\DEV\SeewoKiller\gui.exe"
timeout /t 2

rd /s /q .\build
pause