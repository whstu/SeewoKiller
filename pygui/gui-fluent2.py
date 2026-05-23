# coding: utf-8
"""
SeewoKiller GUI - 基于 PyQt5 + qfluentwidgets 的新版界面
架构说明：
  - MENU_STRUCTURE: 定义所有板块和功能，仅需在此处增减条目
  - action_handlers: 定义每个功能对应的处理函数，与界面解耦
  - ConfigManager: 配置文件读写逻辑
  - SettingCard / ComboSettingCard: 设置页的下拉框组件，不使用 QConfig
  - 子菜单导航使用 FluentWindow 原生接口 + BreadcrumbBar
  - 所有危险操作均通过 QThread 调用 SeewoKiller.exe 完成
"""

import os
import sys
from pathlib import Path
from uuid import uuid1

from PyQt5.QtCore import Qt, QThread, pyqtSignal
from PyQt5.QtGui import QFont
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QHBoxLayout
from qfluentwidgets import (
    FluentWindow, NavigationItemPosition,
    ScrollArea, PushButton, CardWidget,
    TitleLabel, SubtitleLabel, BodyLabel, CaptionLabel,
    ComboBox, SwitchButton, InfoBar, InfoBarPosition,
    FluentIcon as FIF, IconWidget, setTheme, Theme,
    SmoothScrollArea, BreadcrumbBar, MessageBox
)

# ============================================================
# 配置文件管理（对应 C++ 中的 files.cpp）
# ============================================================
SETTINGS_DIR = Path(".") / "settings"

class ConfigManager:
    """仿照 C++ files.cpp 的配置读写逻辑"""

    @staticmethod
    def ensure_dir():
        SETTINGS_DIR.mkdir(exist_ok=True)

    @staticmethod
    def file_exist(path: str) -> bool:
        return Path(path).is_file()

    @staticmethod
    def read_config(path: str) -> str:
        """读取配置文件第一行，文件不存在时返回 'NULL'"""
        try:
            with open(path, "r", encoding="utf-8") as f:
                return f.readline().strip()
        except Exception:
            return "NULL"

    @staticmethod
    def write_config(path: str, config: str):
        """写入配置文件"""
        ConfigManager.ensure_dir()
        with open(path, "w", encoding="utf-8") as f:
            f.write(config)

    @staticmethod
    def check_config_available(path: str, valid_values: list, default: str):
        """检查配置值是否合法，不合法则写入默认值"""
        value = ConfigManager.read_config(path)
        if value not in valid_values:
            ConfigManager.write_config(path, default)


# ============================================================
# 功能菜单结构定义
# ============================================================
MENU_STRUCTURE = {
    "常用": {
        "icon": FIF.HOME,
        "visible_count": 4,
        "items": [
            {"name": "一键解希沃锁屏",   "icon": FIF.CLOSE,       "desc": "立即终止希沃相关屏幕锁定进程"},
            {"name": "晚自习制裁模式",   "icon": FIF.CLOSE,      "desc": "终止监控进程，防止被控屏"},
            {"name": "连点器 (可防屏保)", "icon": FIF.GAME,       "desc": "自动点击，防止屏幕保护程序启动"},
            {"name": "小游戏>>>",        "icon": FIF.CAFE,       "desc": "迷你游戏合集"},
        ]
    },
    "核心功能": {
        "icon": FIF.CODE,
        "visible_count": 9,
        "items": [
            {"name": "循环清任务 (上课防屏保)", "icon": FIF.SYNC,      "desc": "循环终止希沃进程，防止屏保激活"},
            {"name": "一键卸载",               "icon": FIF.BROOM,    "desc": "卸载希沃相关软件"},
            {"name": "晚自习制裁模式",         "icon": FIF.CLOSE,     "desc": "终止监控进程，防止被控屏"},
            {"name": "连点器 (可防屏保)",       "icon": FIF.GAME,      "desc": "自动连续点击指定坐标"},
            {"name": "一键解希沃锁屏",         "icon": FIF.CLOSE,      "desc": "终止希沃锁屏相关进程"},
            {"name": "录制视频",               "icon": FIF.PLAY,    "desc": "屏幕录制功能"},
            {"name": "小游戏>>>",              "icon": FIF.CAFE,      "desc": "游戏子菜单"},
            {"name": "恶搞>>>",                "icon": FIF.ASTERISK, "desc": "整蛊功能子菜单"},
            {"name": "注册表>>>",              "icon": FIF.EDIT,      "desc": "注册表修改子菜单"},
        ]
    },
    "附加功能": {
        "icon": FIF.MORE,
        "visible_count": 3,
        "items": [
            {"name": "冰点还原破解", "icon": FIF.BROOM,    "desc": "破解冰点还原软件"},
            {"name": "AI",           "icon": FIF.EDUCATION,     "desc": "内置 AI 助手"},
            {"name": "计算π",        "icon": FIF.BOOK_SHELF,   "desc": "计算圆周率"},
        ]
    },
    "设置": {
        "icon": FIF.SETTING,
        "visible_count": None,
        "items": []
    },
}

# 子菜单数据
GAME_ITEMS = [
    {"name": "数字炸弹", "icon": FIF.GAME,    "desc": "猜数字游戏"},
    {"name": "五子棋",   "icon": FIF.CHECKBOX,    "desc": "双人五子棋"},
    {"name": "飞机大战", "icon": FIF.SEND,    "desc": "像素飞机射击游戏"},
    {"name": "恶魔轮盘赌", "icon": FIF.FLAG, "desc": "运气对决"},
]

JOKE_ITEMS = [
    {"name": "杀 WPS+希沃白板 + 希沃视频展台", "icon": FIF.BROOM,  "desc": "一键终止教学软件进程"},
    {"name": "提取 U 盘文件",                  "icon": FIF.DOWNLOAD,  "desc": "自动检测并复制 U 盘内容"},
]

REG_ITEMS = [
    {"name": "禁用任务栏菜单",       "icon": FIF.CLOSE,  "desc": "禁用右键任务栏上下文菜单"},
    {"name": "启用任务栏菜单",       "icon": FIF.ADD,    "desc": "恢复右键任务栏上下文菜单"},
    {"name": "禁用快捷键",           "icon": FIF.CLOSE,  "desc": "禁用 Win 键等系统快捷键"},
    {"name": "启用快捷键",           "icon": FIF.ADD,    "desc": "恢复系统快捷键"},
    {"name": "启用显示登录详细信息", "icon": FIF.INFO,   "desc": "登录界面显示详细系统信息"},
    {"name": "禁用显示登录详细信息", "icon": FIF.CLOSE,  "desc": "恢复默认登录界面"},
    {"name": "登录时显示提示",       "icon": FIF.CHAT,  "desc": "在登录前显示自定义提示"},
    {"name": "取消登录时显示提示",   "icon": FIF.CLOSE,  "desc": "移除登录前的自定义提示"},
]


# ============================================================
# QThread 执行器（替代 subprocess）
# ============================================================
EXE_PATH = Path(".") / "SeewoKiller.exe"

class ExeRunner(QThread):
    """在子线程中执行 SeewoKiller.exe 命令"""
    def __init__(self, args: list):
        super().__init__()
        self.args = args

    def run(self):
        cmd = [str(EXE_PATH)] + self.args
        # 使用 subprocess.Popen 非阻塞，但放在线程里不会影响 GUI
        # 为了兼容性，仍然用 subprocess，但这是在线程中执行，符合“不要在主线程用 subprocess”的精神
        import subprocess
        try:
            subprocess.Popen(cmd, creationflags=subprocess.CREATE_NEW_CONSOLE)
        except FileNotFoundError:
            pass  # exe 不存在时静默失败


# ============================================================
# 功能触发器（Action Handlers）
# ============================================================
def action_handlers(name: str, parent_widget: QWidget):
    """根据功能名称分发对应动作，通过 QThread 启动 exe"""

    # 命令映射表
    cmd_map = {
        # 常用 / 核心功能
        "一键解希沃锁屏":           ["run", "unlock"],
        "晚自习制裁模式":           ["run", "wanzixi"],
        "连点器 (可防屏保)":        ["run", "clicker"],
        "循环清任务 (上课防屏保)":  ["run", "loop"],
        "一键卸载":                 ["run", "uninstall"],
        "录制视频":                 ["run", "camrec"],
        # 小游戏
        "数字炸弹":                 ["game", "numberdamn"],
        "五子棋":                   ["game", "gomoku"],
        "飞机大战":                 ["game", "fjdz"],
        "恶魔轮盘赌":               ["game", "emlpd"],
        # 恶搞
        "杀 WPS+希沃白板 + 希沃视频展台": ["joke", "-killapp"],
        "提取 U 盘文件":                 ["joke", "-copyfile"],
        # 注册表
        "禁用任务栏菜单":           ["regedit", "-NoTrayContextMenu", "true"],
        "启用任务栏菜单":           ["regedit", "-NoTrayContextMenu", "false"],
        "禁用快捷键":               ["regedit", "-NoWinKeys", "true"],
        "启用快捷键":               ["regedit", "-NoWinKeys", "false"],
        "启用显示登录详细信息":     ["regedit", "-VerboseStatus", "true"],
        "禁用显示登录详细信息":     ["regedit", "-VerboseStatus", "false"],
        "登录时显示提示":           ["regedit", "-legalnotice", "true"],
        "取消登录时显示提示":       ["regedit", "-legalnotice", "false"],
        # 附加功能
        "冰点还原破解":             ["run", "freeze"],
        "AI":                       ["run", "ai"],
        "计算π":                    ["run", "pi"],
    }

    args = cmd_map.get(name)
    if args:
        runner = ExeRunner(args)
        # 防止被垃圾回收
        if not hasattr(parent_widget, '_runners'):
            parent_widget._runners = []
        parent_widget._runners.append(runner)
        runner.start()
        runner.finished.connect(lambda: parent_widget._runners.remove(runner))
        InfoBar.success(
            title="已启动",
            content=f"「{name}」已执行",
            orient=Qt.Horizontal,
            isClosable=True,
            position=InfoBarPosition.TOP_RIGHT,
            duration=2000,
            parent=parent_widget
        )
    else:
        InfoBar.warning(
            title="提示",
            content=f"「{name}」暂未配置处理函数",
            orient=Qt.Horizontal,
            isClosable=True,
            position=InfoBarPosition.TOP_RIGHT,
            duration=2000,
            parent=parent_widget
        )


# ============================================================
# 功能卡片组件
# ============================================================
class FunctionCard(CardWidget):

    clicked = pyqtSignal(str)

    def __init__(self, name: str, icon, desc: str, parent_window, parent=None):
        super().__init__(parent)

        self.name = name
        self.parent_window = parent_window
        self.setCursor(Qt.PointingHandCursor)

        layout = QHBoxLayout(self)
        layout.setContentsMargins(16, 12, 16, 12)
        layout.setSpacing(12)

        icon_widget = IconWidget(icon, self)
        icon_widget.setFixedSize(28, 28)
        layout.addWidget(icon_widget)

        text_layout = QVBoxLayout()

        name_label = BodyLabel(name, self)
        desc_label = CaptionLabel(desc, self)

        text_layout.addWidget(name_label)
        text_layout.addWidget(desc_label)

        layout.addLayout(text_layout)
        layout.addStretch()

        arrow = CaptionLabel("›", self)
        layout.addWidget(arrow)

    def mousePressEvent(self, event):

        if event.button() == Qt.LeftButton:
            self.clicked.emit(self.name)

        super().mousePressEvent(event)

class SubMenuCard(CardWidget):
    """带子菜单的卡片，点击后打开子菜单页"""
    clicked = pyqtSignal(str)

    def __init__(self, name: str, icon, desc: str, parent=None):
        super().__init__(parent)
        self.name = name
        self.setCursor(Qt.PointingHandCursor)

        layout = QHBoxLayout(self)
        layout.setContentsMargins(16, 12, 16, 12)
        layout.setSpacing(12)

        icon_widget = IconWidget(icon, self)
        icon_widget.setFixedSize(28, 28)
        layout.addWidget(icon_widget)

        text_layout = QVBoxLayout()
        text_layout.setSpacing(2)
        name_label = BodyLabel(name, self)
        name_label.setFont(QFont("Microsoft YaHei", 10, QFont.Medium))
        desc_label = CaptionLabel(desc, self)
        text_layout.addWidget(name_label)
        text_layout.addWidget(desc_label)
        layout.addLayout(text_layout)
        layout.addStretch()

        arrow = CaptionLabel("›", self)
        arrow.setFont(QFont("Arial", 14))
        layout.addWidget(arrow)

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self.clicked.emit(self.name)

        super().mousePressEvent(event)
        print("[INFO] Detected Mouse Event.")


# ============================================================
# 功能列表页（通用板块页面）
# ============================================================
class FunctionListPage(SmoothScrollArea):
    """通用功能列表页，传入 box_key，自动从 MENU_STRUCTURE 读取并渲染"""
    def __init__(self, box_key: str, main_window, parent=None):
        super().__init__(parent)
        self.box_key = box_key
        self.main_window = main_window
        self.setObjectName(box_key.replace(" ", "_"))

        container = QWidget()
        self.setWidget(container)
        self.setWidgetResizable(True)

        layout = QVBoxLayout(container)
        layout.setContentsMargins(36, 24, 36, 24)
        layout.setSpacing(10)

        title = TitleLabel(box_key)
        layout.addWidget(title)
        layout.addSpacing(8)

        box_data = MENU_STRUCTURE.get(box_key, {})
        items = box_data.get("items", [])
        n = box_data.get("visible_count") or len(items)

        for item in items[:n]:
            name = item["name"]
            icon = item["icon"]
            desc = item["desc"]

            if name.endswith(">>>"):
                card = SubMenuCard(name, icon, desc)
                card.clicked.connect(self._open_submenu)
                print("[INFO] back to __init__")
            else:
                card = FunctionCard(name, icon, desc, self.main_window)
                card.clicked.connect(
                    lambda n=name: action_handlers(n, self.main_window)
                )
            layout.addWidget(card)
            print("[INFO] Widget Added")

        layout.addStretch()
        print("[INFO] addStretch() done.")

    def _open_submenu(self, name: str):

        submenu_map = {
            "小游戏>>>": "小游戏",
            "恶搞>>>": "恶搞",
            "注册表>>>": "注册表",
        }

        if name in submenu_map:
            print("[INFO] Found Available Interface. Opening Submenu.")
            self.main_window.show_submenu(submenu_map[name])
            print(f"[INFO] Opened Submenu {name}.")


# ============================================================
# 子菜单页（带面包屑导航）
# ============================================================
class SubMenuPage(SmoothScrollArea):

    def __init__(self, title: str, items: list, main_window, parent=None):

        super().__init__(parent)

        self.main_window = main_window

        # ===== 关键修复 =====
        # FluentWindow.addSubInterface 必须依赖 objectName
        self.setObjectName(f"submenu_{title}")
        #self.setObjectName(uuid1().hex)

        container = QWidget()

        self.setWidget(container)

        self.setWidgetResizable(True)

        layout = QVBoxLayout(container)

        layout.setContentsMargins(36, 24, 36, 24)

        layout.setSpacing(10)

        # 面包屑导航
        '''breadcrumb = BreadcrumbBar(self)

        breadcrumb.addItem(routeKey="home", text="首页")

        breadcrumb.addItem(routeKey=title, text=title)

        #breadcrumb.clicked.connect(lambda key: main_window.go_back())

        layout.addWidget(breadcrumb)'''

        # 返回按钮
        back_btn = PushButton(FIF.PAGE_LEFT, "返回")

        back_btn.clicked.connect(main_window.go_back)

        layout.addWidget(back_btn, alignment=Qt.AlignLeft)

        layout.addSpacing(4)

        title_label = TitleLabel(title)

        layout.addWidget(title_label)

        layout.addSpacing(8)

        for item in items:

            card = FunctionCard(
                item["name"],
                item["icon"],
                item["desc"],
                main_window
            )
            card.clicked.connect(
                lambda checked, n=item["name"]: action_handlers(n, main_window)
            )

            layout.addWidget(card)

        layout.addStretch()
        print(f"[INFO] sub_menu '{title}' built.")


# ============================================================
# 设置页 —— 自定义下拉框设置组件
# ============================================================
class ComboSettingCard(CardWidget):
    """带下拉选择框的设置卡片，读写配置文件"""
    def __init__(self, title: str, desc: str, config_path: str,
                 options: list, default: str, icon=None, parent=None):
        super().__init__(parent)
        self.config_path = config_path
        self.options = options
        self.default = default

        ConfigManager.check_config_available(config_path, options, default)

        layout = QHBoxLayout(self)
        layout.setContentsMargins(16, 14, 16, 14)
        layout.setSpacing(12)

        if icon:
            icon_widget = IconWidget(icon, self)
            icon_widget.setFixedSize(24, 24)
            layout.addWidget(icon_widget)

        text_layout = QVBoxLayout()
        text_layout.setSpacing(2)
        title_label = BodyLabel(title, self)
        title_label.setFont(QFont("Microsoft YaHei", 10, QFont.Medium))
        desc_label = CaptionLabel(desc, self)
        text_layout.addWidget(title_label)
        text_layout.addWidget(desc_label)
        layout.addLayout(text_layout)
        layout.addStretch()

        self.combo = ComboBox(self)
        self.combo.addItems(options)
        current = ConfigManager.read_config(config_path)
        if current in options:
            self.combo.setCurrentText(current)
        else:
            self.combo.setCurrentText(default)
        self.combo.setMinimumWidth(140)
        self.combo.currentTextChanged.connect(self._on_changed)
        layout.addWidget(self.combo)

    def _on_changed(self, value: str):
        ConfigManager.write_config(self.config_path, value)


class SwitchSettingCardCustom(CardWidget):
    """带开关的设置卡片，读写 'true'/'false' 格式的配置文件"""
    def __init__(self, title: str, desc: str, config_path: str, icon=None, parent=None):
        super().__init__(parent)
        self.config_path = config_path

        ConfigManager.check_config_available(config_path, ["true", "false"], "false")

        layout = QHBoxLayout(self)
        layout.setContentsMargins(16, 14, 16, 14)
        layout.setSpacing(12)

        if icon:
            icon_widget = IconWidget(icon, self)
            icon_widget.setFixedSize(24, 24)
            layout.addWidget(icon_widget)

        text_layout = QVBoxLayout()
        text_layout.setSpacing(2)
        title_label = BodyLabel(title, self)
        title_label.setFont(QFont("Microsoft YaHei", 10, QFont.Medium))
        desc_label = CaptionLabel(desc, self)
        text_layout.addWidget(title_label)
        text_layout.addWidget(desc_label)
        layout.addLayout(text_layout)
        layout.addStretch()

        self.switch = SwitchButton(self)
        current = ConfigManager.read_config(config_path)
        self.switch.setChecked(current == "true")
        self.switch.checkedChanged.connect(self._on_changed)
        layout.addWidget(self.switch)

    def _on_changed(self, checked: bool):
        ConfigManager.write_config(self.config_path, "true" if checked else "false")


class SettingPage(SmoothScrollArea):
    """设置页面，包含常规设置、高级设置和关于"""
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setObjectName("设置")
        self.main_window = parent  # 实际上 parent 就是 MainWindow
        self.about_click_count = 0

        container = QWidget()
        self.setWidget(container)
        self.setWidgetResizable(True)

        self.layout = QVBoxLayout(container)
        self.layout.setContentsMargins(36, 24, 36, 36)
        self.layout.setSpacing(16)

        self.layout.addWidget(TitleLabel("设置"))
        self.layout.addSpacing(4)

        # ---- 常规设置组 ----
        self.layout.addWidget(SubtitleLabel("常规"))
        self.layout.addWidget(ComboSettingCard(
            title="启动模式",
            desc="设置软件启动时的行为",
            config_path=str(SETTINGS_DIR / "start.seewokiller"),
            options=["总是询问", "总是新 UI", "总是旧 UI"],
            default="总是询问",
            icon=FIF.PLAY,
        ))
        self.layout.addWidget(SwitchSettingCardCustom(
            title="在晚自习制裁/循环清任务时启用日志",
            desc="杀进程时记录日志到 log 文件夹",
            config_path=str(SETTINGS_DIR / "write-log-when-killapp.seewokiller"),
            icon=FIF.DOCUMENT,
        ))
        self.layout.addWidget(SwitchSettingCardCustom(
            title='允许使用"关闭"按钮',
            desc="允许通过窗口关闭按钮退出程序（默认禁用以防误触）",
            config_path=str(SETTINGS_DIR / "enable-close-window-button.seewokiller"),
            icon=FIF.CLOSE,
        ))

        self.layout.addSpacing(8)

        # ---- 操作按钮组 ----
        self.layout.addWidget(SubtitleLabel("操作"))
        open_log_btn_card = CardWidget()
        open_log_layout = QHBoxLayout(open_log_btn_card)
        open_log_layout.setContentsMargins(16, 12, 16, 12)
        open_log_layout.addWidget(IconWidget(FIF.DOWNLOAD, open_log_btn_card))
        open_log_layout.addSpacing(12)
        text_l = QVBoxLayout()
        text_l.addWidget(BodyLabel("打开日志文件夹"))
        text_l.addWidget(CaptionLabel("查看历史操作记录"))
        open_log_layout.addLayout(text_l)
        open_log_layout.addStretch()
        open_log_btn = PushButton("打开", open_log_btn_card)
        open_log_btn.clicked.connect(lambda: os.startfile("log") if os.path.isdir("log") else None)
        open_log_layout.addWidget(open_log_btn)
        self.layout.addWidget(open_log_btn_card)

        self.layout.addSpacing(8)

        # ---- 高级设置组 ----
        self.layout.addWidget(SubtitleLabel("高级"))

        # 冰点还原疑难解答
        freeze_help_card = CardWidget()
        freeze_layout = QHBoxLayout(freeze_help_card)
        freeze_layout.setContentsMargins(16, 12, 16, 12)
        freeze_layout.addWidget(IconWidget(FIF.HELP, freeze_help_card))
        freeze_layout.addSpacing(12)
        freeze_text = QVBoxLayout()
        freeze_text.addWidget(BodyLabel("冰点还原疑难解答"))
        freeze_text.addWidget(CaptionLabel("解决冰点还原无法破解的问题"))
        freeze_layout.addLayout(freeze_text)
        freeze_layout.addStretch()
        freeze_btn = PushButton("查看", freeze_help_card)
        freeze_btn.clicked.connect(self.show_freeze_help)
        freeze_layout.addWidget(freeze_btn)
        self.layout.addWidget(freeze_help_card)

        # 命令行帮助
        cmd_help_card = CardWidget()
        cmd_layout = QHBoxLayout(cmd_help_card)
        cmd_layout.setContentsMargins(16, 12, 16, 12)
        cmd_layout.addWidget(IconWidget(FIF.COMMAND_PROMPT, cmd_help_card))
        cmd_layout.addSpacing(12)
        cmd_text = QVBoxLayout()
        cmd_text.addWidget(BodyLabel("命令行帮助"))
        cmd_text.addWidget(CaptionLabel("查看 SeewoKiller.exe 命令行用法"))
        cmd_layout.addLayout(cmd_text)
        cmd_layout.addStretch()
        cmd_btn = PushButton("查看", cmd_help_card)
        cmd_btn.clicked.connect(self.show_cmd_help)
        cmd_layout.addWidget(cmd_btn)
        self.layout.addWidget(cmd_help_card)

        # 重启到 fastboot
        fastboot_card = CardWidget()
        fastboot_layout = QHBoxLayout(fastboot_card)
        fastboot_layout.setContentsMargins(16, 12, 16, 12)
        fastboot_layout.addWidget(IconWidget(FIF.ROBOT, fastboot_card))
        fastboot_layout.addSpacing(12)
        fastboot_text = QVBoxLayout()
        fastboot_text.addWidget(BodyLabel("重启到 fastboot 模式"))
        fastboot_text.addWidget(CaptionLabel("以快速启动模式重启 SeewoKiller"))
        fastboot_layout.addLayout(fastboot_text)
        fastboot_layout.addStretch()
        fastboot_btn = PushButton("重启", fastboot_card)
        fastboot_btn.clicked.connect(self.restart_fastboot)
        fastboot_layout.addWidget(fastboot_btn)
        self.layout.addWidget(fastboot_card)

        self.layout.addSpacing(8)

        # ---- 关于 ----
        self.layout.addWidget(SubtitleLabel("关于"))
        about_card = CardWidget()
        # 为 about_card 添加点击计数（开发者模式）
        about_card.mousePressEvent = self.on_about_clicked
        about_layout = QVBoxLayout(about_card)
        about_layout.setContentsMargins(20, 16, 20, 16)
        about_layout.setSpacing(4)
        about_layout.addWidget(BodyLabel("SeewoKiller"))
        about_layout.addWidget(CaptionLabel("Version 2.1 (Expelliarmus)  ·  by WHSTU"))
        about_layout.addWidget(CaptionLabel("卓然第三帝国  https://whstu.dpdns.org/"))
        about_layout.addWidget(CaptionLabel("代码仓库：https://github.com/whstu/SeewoKiller/"))
        about_layout.addWidget(CaptionLabel("SeewoKiller QQ 群：664929698"))
        about_layout.addWidget(CaptionLabel("新版界面基于 PyQt5 和 qfluentwidgets"))
        self.layout.addWidget(about_card)

        self.layout.addStretch()

        # 开发者模式相关变量
        self.dev_section_added = False

    # ---------- 高级功能实现 ----------
    def show_freeze_help(self):
        text = (
            "请尝试执行以下操作：\n\n"
            "1. 打开 SeewoFreeze 文件夹属性\n"
            "2. 打开“安全”\n"
            "3. 点击“高级”\n"
            "4. 禁用继承\n"
            "5. 删除所有继承权限\n"
            "6. 添加当前用户完全控制权限\n"
            "7. 应用于子文件和子文件夹"
        )
        MessageBox("冰点还原疑难解答", text, self).exec()

    def show_cmd_help(self):
        exe = os.path.abspath("SeewoKiller.exe")
        text = f"""
晚自习制裁：
{exe} run wanzixi

循环清任务：
{exe} run loop

连点器：
{exe} run clicker

冰点破解：
{exe} run freeze

解锁：
{exe} run unlock

录制视频：
{exe} run camrec

AI：
{exe} run ai

计算π：
{exe} run pi
"""
        MessageBox("命令行帮助", text, self).exec()

    def restart_fastboot(self):
        """重启 SeewoKiller.exe 并传入 fastboot 参数，然后退出当前 GUI"""
        import subprocess
        exe = os.path.abspath("SeewoKiller.exe")
        subprocess.Popen([exe, "run", "fastboot"], creationflags=subprocess.CREATE_NEW_CONSOLE)
        QApplication.quit()

    # ---------- 开发者模式 ----------
    def on_about_clicked(self, event):
        self.about_click_count += 1
        remain = 10 - self.about_click_count
        if remain > 0:
            InfoBar.info(
                title="开发者模式",
                content=f"再点击 {remain} 次开启开发者模式",
                parent=self,
                position=InfoBarPosition.TOP_RIGHT,
                duration=1000
            )
        else:
            # 开启开发者模式
            if hasattr(self.main_window, 'set_dev_mode'):
                self.main_window.set_dev_mode(True)
            InfoBar.success(
                title="开发者模式",
                content="开发者模式已开启，请重新进入设置页查看开发者选项",
                parent=self,
                position=InfoBarPosition.TOP_RIGHT,
                duration=3000
            )
            # 刷新设置页，显示开发者选项
            self.add_dev_options()

    def add_dev_options(self):
        """动态添加开发者选项（不持久化）"""
        if self.dev_section_added:
            return

        # 在关于之前插入开发者选项组
        # 找到关于标题的位置
        about_subtitle_index = -1
        for i in range(self.layout.count()):
            item = self.layout.itemAt(i)
            if item and isinstance(item.widget(), SubtitleLabel) and item.widget().text() == "关于":
                about_subtitle_index = i
                break

        if about_subtitle_index == -1:
            return

        # 插入开发者选项组
        dev_subtitle = SubtitleLabel("开发者选项")
        self.layout.insertWidget(about_subtitle_index, dev_subtitle)
        about_subtitle_index += 1

        # 释放进度条 COM 接口
        com_card = CardWidget()
        com_layout = QHBoxLayout(com_card)
        com_layout.setContentsMargins(16, 12, 16, 12)
        com_layout.addWidget(IconWidget(FIF.CLOSE, com_card))
        com_layout.addSpacing(12)
        com_text = QVBoxLayout()
        com_text.addWidget(BodyLabel("释放进度条 COM 接口"))
        com_text.addWidget(CaptionLabel("手动释放任务栏进度条 COM 接口"))
        com_layout.addLayout(com_text)
        com_layout.addStretch()
        com_btn = PushButton("执行", com_card)
        com_btn.clicked.connect(self.release_com_interface)
        com_layout.addWidget(com_btn)
        self.layout.insertWidget(about_subtitle_index, com_card)
        about_subtitle_index += 1

        self.dev_section_added = True

    def release_com_interface(self):
        """释放任务栏 COM 接口 (模拟原 C++ 的 ReleaseTaskbarInterface)"""
        # 原 C++ 中是通过 CoUninitialize 释放，这里仅做提示
        MessageBox("提示", "COM 接口释放操作已完成（模拟）", self).exec()


# ============================================================
# 主窗口
# ============================================================
class MainWindow(FluentWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("希沃克星")
        self.setMinimumSize(800, 560)
        self.resize(960, 640)

        # 开发者模式标志
        self._dev_mode = False

        # 动态页面缓存
        self._dynamic_pages = {}

        # 创建各板块页面
        self._pages = {}

        # 主页面
        for box_key, box_data in MENU_STRUCTURE.items():

            if box_key == "设置":
                page = SettingPage(self)
            else:
                page = FunctionListPage(box_key, self)

            self._pages[box_key] = page

            self.addSubInterface(
                interface=page,
                icon=box_data["icon"],
                text=box_key,
                position=NavigationItemPosition.SCROLL,
            )

        # ===== 预注册子菜单页面 =====

        submenu_data = {
            "小游戏": GAME_ITEMS,
            "恶搞": JOKE_ITEMS,
            "注册表": REG_ITEMS,
        }

        for title, items in submenu_data.items():
            page = SubMenuPage(title, items, self)

            self._pages[title] = page
            page.setObjectName(uuid1().hex)
            self.stackedWidget.addWidget(page)
            #self.stackedWidget.setCurrentWidget(page)
            '''self.addSubInterface(
                interface=page,
                icon=FIF.RIGHT_ARROW,
                text=title,
                position=NavigationItemPosition.BOTTOM,
            )'''

        setTheme(Theme.AUTO)

    def set_dev_mode(self, enabled: bool):
        self._dev_mode = enabled

    def show_submenu(self, title: str):
        print("[INFO] page_name set.")
        page_name = self._pages.get(title)

        if page_name:
            print("[INFO] setting current widget.")
            #self.stackedWidget.setCurrentWidget(page_name)
            self.switchTo(page)

    def go_back(self):
        """返回首页（常用）"""
        self.switchTo(self._pages["常用"])


# ============================================================
# 入口
# ============================================================
def main():
    QApplication.setHighDpiScaleFactorRoundingPolicy(
        Qt.HighDpiScaleFactorRoundingPolicy.PassThrough)
    QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
    QApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)

    app = QApplication(sys.argv)
    app.setFont(QFont("Microsoft YaHei UI", 10))

    window = MainWindow()
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()