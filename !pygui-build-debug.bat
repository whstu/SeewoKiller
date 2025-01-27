echo Building PyGUI-DebugVersion
pyinstaller -F -i SeewoKiller.ico .\pygui\gui.py
copy ".\dist\gui.exe" ".\gui.exe"
rd /s /q .\build
rd /s /q .\dist
del gui.spec
del SeewoKiller.spec
pause