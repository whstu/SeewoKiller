import os
import sys
from PyQt5.QtCore import Qt, QProcess
from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QHBoxLayout
from qfluentwidgets import (NavigationItemPosition, MessageBox,
                            BodyLabel, SubtitleLabel, PrimaryPushButton, ComboBox,
                            ScrollArea, CardWidget, FluentWindow, TitleLabel,
                            TextEdit, FluentIcon as FIF, BreadcrumbBar)

# ==============================================================================
# 1. 核心思想架构：解耦的界面架构与触发器 (Box 与 Word 思想)
# ==============================================================================
BOX_WORDS = {
    "最近使用": [
        {"text": "循环清任务 (上课防屏保)", "cmd": "taskkill", "callback": None},
        {"text": "晚自习制裁模式", "cmd": "wanzixi", "callback": None},
    ],
    "所有功能": [
        {"text": "循环清任务 (上课防屏保)", "cmd": "taskkill", "callback": None},
        {"text": "晚自习制裁模式", "cmd": "wanzixi", "callback": None},
        {"text": "连点器 (可防屏保)", "cmd": "liandianqi", "callback": None},
        {"text": "冰点还原破解", "cmd": "seewofreeze", "callback": None},
        {"text": "一键解希沃锁屏", "cmd": "wanzixi", "callback": None},
    ],
    "更多模块": [
        {"text": "一键卸载", "cmd": "uninstall", "callback": None},
        {"text": "录制视频", "cmd": "video", "callback": None},
    ],
    "设置中心": [
        {"text": "启动初学者引导", "cmd": "quickstart", "callback": None}
    ]
}

EXE_NAME = "SeewoKiller.exe"


# ==============================================================================
# 2. 借鉴 C++逻辑的通用低级配置文件读写及下拉框同步函数
# ==============================================================================
def file_exist(filename):
    return os.path.exists(filename)


def read_config(path):
    if file_exist(path):
        try:
            with open(path, "r", encoding="utf-8") as f:
                return f.readline().strip()
        except:
            pass
    return "NULL"


def write_config(path, config_str):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as f:
        f.write(config_str)


def check_and_sync_combobox(path, combo_widget, default_value):
    config_value = read_config(path)
    allowed_configs = [combo_widget.itemText(i) for i in range(combo_widget.count())]

    if config_value in allowed_configs:
        combo_widget.setCurrentText(config_value)
    else:
        write_config(path, default_value)
        combo_widget.setCurrentText(default_value)


# ==============================================================================
# 3. 功能执行器组件 (基于 QProcess)
# ==============================================================================
class ProcessLauncher:
    def __init__(self):
        self.process = None

    def launch(self, parent_widget, cmd_arg):
        if self.process and self.process.state() == QProcess.Running:
            MessageBox("提示", f"已有一个 {EXE_NAME} 任务正在后台运行中，请勿重复开启！", parent_widget).exec_()
            return

        if not file_exist(EXE_NAME):
            MessageBox("未找到核心核心程序", f"当前目录下未检测到核心后端 {EXE_NAME}，请确保它们在同一根目录下。",
                       parent_widget).exec_()
            return

        self.process = QProcess()
        self.process.start(EXE_NAME, [cmd_arg] if cmd_arg else [])


# ==============================================================================
# 4. 基础子页面基类 (修复 ValueError 关键就在这里)
# ==============================================================================
class BasePage(ScrollArea):
    def __init__(self, page_title, parent=None):
        super().__init__(parent=parent)
        self.page_title = page_title

        # 【核心修复】显式设置 objectName，禁止为空字符串，否则 qfluentwidgets 的导航系统会崩
        self.setObjectName(page_title.replace(" ", "_"))

        self.setWidgetResizable(True)
        self.scroll_widget = QWidget()
        self.layout = QVBoxLayout(self.scroll_widget)
        self.layout.setAlignment(Qt.AlignTop)
        self.layout.setContentsMargins(36, 20, 36, 20)
        self.layout.setSpacing(15)

        # 面包屑导航 BreadcrumbBar
        self.breadcrumb = BreadcrumbBar(self)
        self.breadcrumb.addItem("主页", "home")
        self.breadcrumb.addItem(page_title, "current_page")
        self.layout.addWidget(self.breadcrumb)

        # 页面主标题
        self.title_label = TitleLabel(page_title, self)
        self.layout.addWidget(self.title_label)

        self.setWidget(self.scroll_widget)
        self.setStyleSheet("QScrollArea { border: none; background: transparent; }")


# ==============================================================================
# 5. 动态常规模块渲染页 (最近使用/所有功能/更多模块)
# ==============================================================================
class DynamicFuncPage(BasePage):
    def __init__(self, box_name, launcher, parent=None):
        # 将 box_name 传给父类，父类会用它自动建立 objectName
        super().__init__(box_name, parent)
        self.launcher = launcher
        self.box_name = box_name
        self.render_words()

    def render_words(self):
        words = BOX_WORDS.get(self.box_name, [])
        for word in words:
            card = CardWidget(self)
            card_layout = QHBoxLayout(card)
            card_layout.setContentsMargins(15, 12, 15, 12)

            label = SubtitleLabel(word["text"], card)
            btn = PrimaryPushButton("立即启动", card)

            btn.clicked.connect(lambda checked, w=word: self.on_word_clicked(w))

            card_layout.addWidget(label)
            card_layout.addStretch(1)
            card_layout.addWidget(btn)
            self.layout.addWidget(card)

    def on_word_clicked(self, word_info):
        if word_info["cmd"]:
            self.launcher.launch(self, word_info["cmd"])
        elif word_info["callback"]:
            word_info["callback"]()


# ==============================================================================
# 6. 设置与高级/关于/开发者选项 核心功能页面
# ==============================================================================
class SettingPage(BasePage):
    def __init__(self, launcher, parent=None):
        super().__init__("设置中心", parent)
        self.launcher = launcher
        self.click_count = 0
        self.dev_group = None
        self.setup_ui()

    def setup_ui(self):
        # ---------------- 板块 1: 常规设置 ----------------
        self.layout.addWidget(BodyLabel("启动 UI 设置", self))
        card_ui = CardWidget(self)
        lay_ui = QHBoxLayout(card_ui)
        lay_ui.addWidget(BodyLabel("当检测到系统为 Win7+时的默认行为", card_ui))
        lay_ui.addStretch(1)

        self.combo_start = ComboBox(card_ui)
        self.combo_start.addItems(["总是询问", "总是旧 UI", "总是新 UI"])
        self.combo_start.currentTextChanged.connect(self.save_ui_config)
        lay_ui.addWidget(self.combo_start)
        self.layout.addWidget(card_ui)
        check_and_sync_combobox(".\\settings\\start.seewokiller", self.combo_start, "总是询问")

        card_log = CardWidget(self)
        lay_log = QHBoxLayout(card_log)
        lay_log.addWidget(BodyLabel("在晚自习制裁/循环清任务时启用日志", card_log))
        lay_log.addStretch(1)
        self.combo_log = ComboBox(card_log)
        self.combo_log.addItems(["true", "false"])
        self.combo_log.currentTextChanged.connect(self.save_log_config)
        lay_log.addWidget(self.combo_log)
        self.layout.addWidget(card_log)
        check_and_sync_combobox(".\\settings\\write-log-when-killapp.seewokiller", self.combo_log, "false")

        # ---------------- 板块 2: 高级疑难解答 ----------------
        self.layout.addWidget(BodyLabel("高级疑难解答", self))

        card_freeze = CardWidget(self)
        lay_fz = QHBoxLayout(card_freeze)
        lay_fz.addWidget(BodyLabel("冰点还原疑难解答指南 (如果您确认已安装模块却无法调用时使用)", card_freeze))
        lay_fz.addStretch(1)
        btn_fz = PrimaryPushButton("查看指南", card_freeze)
        btn_fz.clicked.connect(self.show_freeze_help)
        lay_fz.addWidget(btn_fz)
        self.layout.addWidget(card_freeze)

        card_cmd_help = CardWidget(self)
        lay_ch = QHBoxLayout(card_cmd_help)
        lay_ch.addWidget(BodyLabel("希沃克星支持通过外接控制台传入参数执行后台脱机静默运行", card_cmd_help))
        lay_ch.addStretch(1)
        btn_ch = PrimaryPushButton("查看指令集", card_cmd_help)
        btn_ch.clicked.connect(self.show_cmd_help)
        lay_ch.addWidget(btn_ch)
        self.layout.addWidget(card_cmd_help)

        card_fastboot = CardWidget(self)
        lay_fb = QHBoxLayout(card_fastboot)
        lay_fb.addWidget(BodyLabel("重启系统到精简 Fastboot 安全脱机状态", card_fastboot))
        lay_fb.addStretch(1)
        btn_fb = PrimaryPushButton("立即重启", card_fastboot)
        btn_fb.clicked.connect(lambda: self.launcher.launch(self, "fastboot"))
        lay_fb.addWidget(btn_fb)
        self.layout.addWidget(card_fastboot)

        # ---------------- 板块 3: 关于软件 ----------------
        self.layout.addWidget(BodyLabel("关于", self))
        self.about_text = TextEdit(self)
        self.about_text.setReadOnly(True)
        self.about_text.setText(
            "Seewo Killer 2.1 (Expelliarmus)\n\n"
            "Powered by WHSTU Studio\n"
            "卓然第三帝国 提供技术支持\n"
            "新版图形化界面基于 PyQt5 和 QFluentWidgets 精心打造。\n\n"
            "【点击本文字控制台区域满 10 次可临时召唤魔法开发者控制台】"
        )
        self.about_text.setMaximumHeight(150)
        self.about_text.mousePressEvent = self.on_about_clicked
        self.layout.addWidget(self.about_text)

    def save_ui_config(self, val):
        write_config(".\\settings\\start.seewokiller", val)

    def save_log_config(self, val):
        write_config(".\\settings\\write-log-when-killapp.seewokiller", val)

    def show_freeze_help(self):
        msg = (
            "【执行以下操作修复冰点权限】\n\n"
            "1. 右键主程序下的 SeewoFreeze 文件夹打开属性。\n"
            "2. 选择 [安全] -> [高级] -> 点击 [禁用继承]。\n"
            "3. 选择 [从此对象中删除所有已继承的权限]。\n"
            "4. 回到高级界面点击 [添加] -> [选择主体] -> 输入您当前的账户名。\n"
            "5. 勾选 [完全控制] 并勾选 [替换所有子对象权限继承]。确定保存即可。"
        )
        MessageBox("冰点还原疑难解答指南", msg, self).exec_()

    def show_cmd_help(self):
        msg = (
            "希沃克星允许通过命令行参数直接脱机拉起指定任务：\n\n"
            "• 晚自习制裁模式：SeewoKiller.exe wanzixi\n"
            "• 循环结束核心服务：SeewoKiller.exe taskkill\n"
            "• 自动高频连点器：SeewoKiller.exe liandianqi\n"
            "• 独立冰点破解窗口：SeewoKiller.exe seewofreeze"
        )
        MessageBox("命令行外部调用帮助系统", msg, self).exec_()

    def on_about_clicked(self, event):
        self.click_count += 1
        if self.click_count >= 10 and not self.dev_group:
            self.dev_group = CardWidget(self)
            dev_layout = QHBoxLayout(self.dev_group)
            dev_layout.addWidget(BodyLabel("⚠️ 开发者临时模式已开启：当前运行参数 word.settingn = 12", self.dev_group))
            dev_layout.addStretch(1)
            btn_kill_gui = PrimaryPushButton("脱机完全卸载 GUI 壳", self.dev_group)
            dev_layout.addWidget(btn_kill_gui)

            self.layout.addWidget(self.dev_group)
            MessageBox("魔法降临", "开发者权限认证成功！开发者选项已动态加载在当前页面底部，重启软件后即消失。",
                       self).exec_()

        super(TextEdit, self.about_text).mousePressEvent(event)


# ==============================================================================
# 7. 主控 FluentWindow 导航中心
# ==============================================================================
class MainWindow(FluentWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("希沃克星 SeewoKiller - 全新图形化界面")
        self.resize(850, 600)

        self.launcher = ProcessLauncher()
        self.init_navigation()

    def init_navigation(self):
        self.recent_page = DynamicFuncPage("最近使用", self.launcher, self)
        self.all_page = DynamicFuncPage("所有功能", self.launcher, self)
        self.more_page = DynamicFuncPage("更多模块", self.launcher, self)
        self.setting_page = SettingPage(self.launcher, self)

        # 此时各个子页面均有了唯一的 objectName，addSubInterface 不再抛出异常
        self.addSubInterface(self.recent_page, FIF.HISTORY, "最近使用")
        self.addSubInterface(self.all_page, FIF.APPLICATION, "所有功能")
        self.addSubInterface(self.more_page, FIF.MORE, "更多模块")
        self.addSubInterface(self.setting_page, FIF.SETTING, "设置中心", position=NavigationItemPosition.BOTTOM)

        self.setWindowIcon(QIcon())


if __name__ == "__main__":
    QApplication.setHighDpiScaleFactorRoundingPolicy(Qt.HighDpiScaleFactorRoundingPolicy.PassThrough)
    QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
    QApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)

    app = QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec_())