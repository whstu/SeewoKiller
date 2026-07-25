import re,sys
print("Building SeewoKiller_gui.rc...")
data=open('SeewoKiller_private.rc','rb').read()
lines=data.splitlines(keepends=True)
out=[l for l in lines if not re.match(rb'^\s*#include\s*<.*>',l) and not re.match(rb'^\s*A\s+ICON\s+\"app\.ico\"\s*[\r\n]*$',l)]

# 将所有 "SeewoKiller.exe" 替换为 "gui.exe"
out = [l.replace(b'SeewoKiller.exe', b'gui.exe') for l in out]

open('SeewoKiller_gui.rc','wb').writelines(out)
print("Build SeewoKiller_gui.rc done.")