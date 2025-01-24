@echo off
pyinstaller -F -w -i SeewoKiller.ico .\pygui\gui.py
copy ".\dist\gui.exe" ".\gui.exe"
rd /s /q .\build
rd /s /q .\dist
del gui.spec
pause