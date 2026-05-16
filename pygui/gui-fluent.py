# coding: utf-8
"""
SeewoKiller GUI - 基于 PyQt5 + qfluentwidgets 的新版界面
架构说明：
  - MENU_STRUCTURE: 定义所有板块和功能，仅需在此处增减条目
  - action_handlers: 定义每个功能对应的处理函数，与界面解耦
  - ConfigManager: 仿照 C++ files.cpp 的配置文件读写逻辑
  - SettingCard / ComboSettingCard: 设置页的下拉框组件，不使用 QConfig
"""

import os
import sys
import subprocess
from pathlib import Path

from PyQt5.QtCore import Qt, QThread, pyqtSignal, QTimer
from PyQt5.QtGui import QIcon, QFont
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout,
    QStackedWidget, QLabel, QSizePolicy, QFrame, QSplitter
)

from qfluentwidgets import (
    FluentWindow, NavigationItemPosition,
    ScrollArea, PushButton, PrimaryPushButton, CardWidget,
    TitleLabel, SubtitleLabel, BodyLabel, CaptionLabel,
    ComboBox, SwitchButton, InfoBar, InfoBarPosition,
    FluentIcon as FIF, IconWidget, setTheme, Theme,
    SmoothScrollArea
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
        """
        检查配置值是否合法，不合法则写入默认值
        对应 C++ 的 check_config_avaliable()
        """
        value = ConfigManager.read_config(path)
        if value not in valid_values:
            ConfigManager.write_config(path, default)


# ============================================================
# 功能菜单结构定义
# 对应 C++ 中的 Word 结构体 + n 变量
# 要增加功能：在对应 box 的 items 里加一项，再在 action_handlers 里加对应处理。
# 要隐藏末尾功能：减小 visible_count 的值。
# ============================================================
MENU_STRUCTURE = {
    "常用": {
        "icon": FIF.HOME,
        "visible_count": 4,   # 控制显示条目数（对应 C++ 的 recentn）
        "items": [
            {"name": "一键解希沃锁屏",   "icon": FIF.CLOSE,       "desc": "立即终止希沃相关屏幕锁定进程"},
            {"name": "晚自习制裁模式",   "icon": FIF.CLOSE,      "desc": "终止监控进程，防止被控屏"},
            {"name": "连点器 (可防屏保)", "icon": FIF.GAME,       "desc": "自动点击，防止屏幕保护程序启动"},
            {"name": "小游戏>>>",        "icon": FIF.CAFE,       "desc": "迷你游戏合集"},
        ]
    },
    "核心功能": {
        "icon": FIF.CODE,
        "visible_count": 9,   # 对应 C++ 的 alln
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
        "visible_count": 3,   # 对应 C++ 的 moren
        "items": [
            {"name": "冰点还原破解", "icon": FIF.BROOM,    "desc": "破解冰点还原软件"},
            {"name": "AI",           "icon": FIF.EDUCATION,     "desc": "内置 AI 助手"},
            {"name": "计算π",        "icon": FIF.BOOK_SHELF,   "desc": "计算圆周率"},
        ]
    },
    "设置": {
        "icon": FIF.SETTING,
        "visible_count": None,  # None 表示显示全部（设置页单独处理）
        "items": []  # 设置页用专用 SettingPage，不走通用卡片
    },
}

# 游戏子菜单
GAME_ITEMS = [
    {"name": "数字炸弹", "icon": FIF.GAME,    "desc": "猜数字游戏"},
    {"name": "五子棋",   "icon": FIF.CHECKBOX,    "desc": "双人五子棋"},
    {"name": "飞机大战", "icon": FIF.SEND,    "desc": "像素飞机射击游戏"},
    {"name": "恶魔轮盘赌", "icon": FIF.FLAG, "desc": "运气对决"},
]

# 恶搞子菜单
JOKE_ITEMS = [
    {"name": "杀 WPS+希沃白板 + 希沃视频展台", "icon": FIF.BROOM,  "desc": "一键终止教学软件进程"},
    {"name": "提取 U 盘文件",                  "icon": FIF.DOWNLOAD,  "desc": "自动检测并复制 U 盘内容"},
]

# 注册表子菜单
REG_ITEMS = [
    {"name": "禁用任务栏菜单",       "icon": FIF.CLOSE,  "desc": "禁用右键任务栏上下文菜单"},
    {"name": "启用任务栏菜单",       "icon": FIF.ADD,    "desc": "恢复右键任务栏上下文菜单"},
    {"name": "禁用快捷键",           "icon": FIF.CLOSE,  "desc": "禁用 Win 键等系统快捷键"},
    {"name": "启用快捷键",           "icon": FIF.ADD,    "desc": "恢复系统快捷键"},
    {"name": "启用显示登录详细信息", "icon": FIF.INFO,   "desc": "登录界面显示详细系统信息"},
    {"name": "禁用显示登录详细信息", "icon": FIF.CLOSE,  "desc": "恢复默认登录界面"},
    {"name": "登录时显示提示",       "icon": FIF.CHAT,"desc": "在登录前显示自定义提示"},
    {"name": "取消登录时显示提示",   "icon": FIF.CLOSE,  "desc": "移除登录前的自定义提示"},
]


# ============================================================
# 功能触发器（Action Handlers）
# 界面架构与功能逻辑完全分离
# ============================================================
EXE_PATH = Path(".") / "SeewoKiller.exe"

def run_sk(args: list):
    """调用 SeewoKiller.exe 并传入参数"""
    cmd = [str(EXE_PATH)] + args
    try:
        subprocess.Popen(cmd, creationflags=subprocess.CREATE_NEW_CONSOLE)
    except FileNotFoundError:
        pass  # exe 不存在时静默失败，界面仍可预览

def action_handlers(name: str, parent_widget: QWidget):
    """
    根据功能名称分发对应动作。
    要增加功能：在 MENU_STRUCTURE 的对应 items 里加条目后，
    再在此函数末尾的 dispatch 字典里加对应 lambda 即可。
    """
    dispatch = {
        # 常用 / 核心功能
        "一键解希沃锁屏":           lambda: run_sk(["run", "unlock"]),
        "晚自习制裁模式":           lambda: run_sk(["run", "wanzixi"]),
        "连点器 (可防屏保)":         lambda: run_sk(["run", "clicker"]),
        "循环清任务 (上课防屏保)":   lambda: run_sk(["run", "loop"]),
        "一键卸载":                 lambda: run_sk(["run", "uninstall"]),
        "录制视频":                 lambda: run_sk(["run", "camrec"]),
        # 小游戏
        "数字炸弹":                 lambda: run_sk(["game", "bomb"]),
        "五子棋":                   lambda: run_sk(["game", "gomoku"]),
        "飞机大战":                 lambda: run_sk(["game", "plane"]),
        "恶魔轮盘赌":               lambda: run_sk(["game", "roulette"]),
        # 恶搞
        "杀 WPS+希沃白板 + 希沃视频展台": lambda: run_sk(["joke", "-killapp"]),
        "提取 U 盘文件":              lambda: run_sk(["joke", "-copyfile"]),
        # 注册表
        "禁用任务栏菜单":           lambda: run_sk(["regedit", "-NoTrayContextMenu", "true"]),
        "启用任务栏菜单":           lambda: run_sk(["regedit", "-NoTrayContextMenu", "false"]),
        "禁用快捷键":               lambda: run_sk(["regedit", "-NoWinKeys", "true"]),
        "启用快捷键":               lambda: run_sk(["regedit", "-NoWinKeys", "false"]),
        "启用显示登录详细信息":     lambda: run_sk(["regedit", "-VerboseStatus", "true"]),
        "禁用显示登录详细信息":     lambda: run_sk(["regedit", "-VerboseStatus", "false"]),
        "登录时显示提示":           lambda: run_sk(["regedit", "-legalnotice", "true"]),
        "取消登录时显示提示":       lambda: run_sk(["regedit", "-legalnotice", "false"]),
        # 附加功能
        "冰点还原破解":             lambda: run_sk(["run", "freeze"]),
        "AI":                       lambda: run_sk(["run", "ai"]),
        "计算π":                    lambda: run_sk(["run", "pi"]),
    }

    handler = dispatch.get(name)
    if handler:
        handler()
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
    """单个功能卡片，点击后触发对应 action"""

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
            action_handlers(self.name, self.parent_window)
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


# ============================================================
# 功能列表页（通用板块页面）
# ============================================================
class FunctionListPage(SmoothScrollArea):
    """
    通用功能列表页。
    传入 box_key（板块名称），自动从 MENU_STRUCTURE 读取并渲染对应条目。
    """

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

            # 子菜单入口
            if name.endswith(">>>"):
                card = SubMenuCard(name, icon, desc)
                card.clicked.connect(self._open_submenu)
            else:
                card = FunctionCard(name, icon, desc, self.main_window)
            layout.addWidget(card)

        layout.addStretch()

    def _open_submenu(self, name: str):
        submenu_map = {
            "小游戏>>>":  ("小游戏", GAME_ITEMS),
            "恶搞>>>":    ("恶搞",   JOKE_ITEMS),
            "注册表>>>":  ("注册表", REG_ITEMS),
        }
        if name in submenu_map:
            title, items = submenu_map[name]
            self.main_window.show_submenu(title, items)


# ============================================================
# 子菜单页（游戏、恶搞、注册表）
# ============================================================
class SubMenuPage(SmoothScrollArea):
    def __init__(self, title: str, items: list, main_window, parent=None):
        super().__init__(parent)
        self.main_window = main_window

        container = QWidget()
        self.setWidget(container)
        self.setWidgetResizable(True)

        layout = QVBoxLayout(container)
        layout.setContentsMargins(36, 24, 36, 24)
        layout.setSpacing(10)

        # 返回按钮
        back_btn = PushButton(FIF.PAGE_LEFT, "返回")
        back_btn.clicked.connect(main_window.go_back)
        layout.addWidget(back_btn, alignment=Qt.AlignLeft)
        layout.addSpacing(4)

        title_label = TitleLabel(title)
        layout.addWidget(title_label)
        layout.addSpacing(8)

        for item in items:
            card = FunctionCard(item["name"], item["icon"], item["desc"], main_window)
            layout.addWidget(card)

        layout.addStretch()


# ============================================================
# 设置页 —— 自定义下拉框设置组件（不使用 QConfig）
# ============================================================
class ComboSettingCard(CardWidget):
    """
    带下拉选择框的设置卡片。
    仿照 C++ 的 check_config_avaliable + change_word 逻辑，
    读取/写入指定配置文件，并即时更新显示。
    """

    def __init__(self, title: str, desc: str, config_path: str,
                 options: list, default: str, icon=None, parent=None):
        super().__init__(parent)
        self.config_path = config_path
        self.options = options
        self.default = default

        # 初始化配置
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
    """
    带开关的设置卡片，读写 'true'/'false' 格式的配置文件。
    """

    def __init__(self, title: str, desc: str, config_path: str,
                 icon=None, parent=None):
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
    """
    设置页面，包含所有可配置项。
    使用自定义的 ComboSettingCard 和 SwitchSettingCardCustom，
    不依赖 QConfig。
    """

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setObjectName("设置")

        container = QWidget()
        self.setWidget(container)
        self.setWidgetResizable(True)

        layout = QVBoxLayout(container)
        layout.setContentsMargins(36, 24, 36, 36)
        layout.setSpacing(16)

        layout.addWidget(TitleLabel("设置"))
        layout.addSpacing(4)

        # ---- 常规设置组 ----
        layout.addWidget(SubtitleLabel("常规"))
        layout.addWidget(ComboSettingCard(
            title="启动模式",
            desc="设置软件启动时的行为",
            config_path=str(SETTINGS_DIR / "start.seewokiller"),
            options=["总是询问", "总是新 UI", "总是旧 UI"],
            default="总是询问",
            icon=FIF.PLAY,
        ))
        layout.addWidget(SwitchSettingCardCustom(
            title="在晚自习制裁/循环清任务时启用日志",
            desc="杀进程时记录日志到 log 文件夹",
            config_path=str(SETTINGS_DIR / "write-log-when-killapp.seewokiller"),
            icon=FIF.DOCUMENT,
        ))
        layout.addWidget(SwitchSettingCardCustom(
            title='允许使用"关闭"按钮',
            desc="允许通过窗口关闭按钮退出程序（默认禁用以防误触）",
            config_path=str(SETTINGS_DIR / "enable-close-window-button.seewokiller"),
            icon=FIF.CLOSE,
        ))

        layout.addSpacing(8)
        # ---- 操作按钮组 ----
        layout.addWidget(SubtitleLabel("操作"))

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
        layout.addWidget(open_log_btn_card)

        layout.addSpacing(8)
        # ---- 关于 ----
        layout.addWidget(SubtitleLabel("关于"))
        about_card = CardWidget()
        about_layout = QVBoxLayout(about_card)
        about_layout.setContentsMargins(20, 16, 20, 16)
        about_layout.setSpacing(4)
        about_layout.addWidget(BodyLabel("SeewoKiller"))
        about_layout.addWidget(CaptionLabel("Version 2.1 (Expelliarmus)  ·  by WHSTU"))
        about_layout.addWidget(CaptionLabel("卓然第三帝国  https://whstu.dpdns.org/"))
        about_layout.addWidget(CaptionLabel("代码仓库：https://github.com/whstu/SeewoKiller/"))
        about_layout.addWidget(CaptionLabel("SeewoKiller QQ 群：664929698"))
        about_layout.addWidget(CaptionLabel("新版界面基于 PyQt5 和 qfluentwidgets"))
        layout.addWidget(about_card)

        layout.addStretch()


# ============================================================
# 主窗口
# ============================================================
class MainWindow(FluentWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("希沃克星 SeewoKiller")
        self.setMinimumSize(800, 560)
        self.resize(960, 640)

        # 页面栈（用于子菜单导航）
        self._page_history = []

        # 创建各板块页面
        self._pages = {}
        for box_key, box_data in MENU_STRUCTURE.items():
            if box_key == "设置":
                page = SettingPage(self)
            else:
                page = FunctionListPage(box_key, self, self)
            self._pages[box_key] = page
            self.addSubInterface(
                interface=page,
                icon=box_data["icon"],
                text=box_key,
                position=NavigationItemPosition.SCROLL,
            )

        # 子菜单容器（动态替换）
        self._submenu_page = None

        # 主题
        setTheme(Theme.AUTO)

    def show_submenu(self, title: str, items: list):
        """显示子菜单页，记录当前页面以便返回"""
        if self._submenu_page:
            self.stackedWidget.removeWidget(self._submenu_page)

        self._submenu_page = SubMenuPage(title, items, self, self)
        self.stackedWidget.addWidget(self._submenu_page)

        # 记录历史，切换到子菜单
        current = self.stackedWidget.currentWidget()
        self._page_history.append(current)
        self.stackedWidget.setCurrentWidget(self._submenu_page)

    def go_back(self):
        """返回上一个页面"""
        if self._page_history:
            prev = self._page_history.pop()
            self.stackedWidget.setCurrentWidget(prev)


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