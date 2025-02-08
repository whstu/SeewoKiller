echo Building PyGUI-DebugVersion
pyinstaller -F -i SeewoKiller.ico .\pygui\ai.py
copy ".\dist\ai.exe" ".\ai.exe"
rd /s /q .\build
rd /s /q .\dist
del gui.spec
del ai.spec
del SeewoKiller.spec
pause