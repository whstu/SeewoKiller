# -*- mode: python ; coding: utf-8 -*-

a = Analysis(
    ['gui-fluent.py'],  # 您的主程序文件名，请根据实际情况修改
    pathex=['E:\\devc++\\DEV\\SeewoKiller\\pygui'],
    binaries=[],
    datas=[
        ('E:\\devc++\\DEV\\SeewoKiller\\pygui\\tengwar.png', '.'),  # 添加图片文件
        ('E:\\devc++\\DEV\\SeewoKiller\\pygui\\app.ico', '.'),  # 添加图片文件
    ],
    hiddenimports=[
        'PyQt5',  # PyQt5核心模块
        'PyQt5.QtCore',
        'PyQt5.QtGui',
        'PyQt5.QtWidgets',
    ],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[
        # 测试框架
        'unittest', 'pydoc', 'doctest', 'pdb', 'test',
        'setuptools', 'distutils',
        # 大型库（如果未使用）
        'numpy', 'scipy', 'pandas', 'matplotlib',
        # 网络相关（如果不需要）
        'http.server', 'smtplib', 'poplib', 'imaplib',
        'turtle', 'curses',  # 不常用的库
        'xmlrpc', 'json.tool',
        # 开发工具
        'venv', 'pip', 'ensurepip',
    ],
    noarchive=False,
)

pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name='SeewoKiller',  # 软件名称
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,  # 启用UPX压缩
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,  # GUI程序，不显示控制台
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon='.\\app.ico',  # 应用程序图标
)