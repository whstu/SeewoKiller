VersionName="Frodo Baggins"
AppName_en,AppName_cn="SeewoKiller","希沃克星"
home={}
more={
    "循环清任务":"上课防屏保",
    "一键卸载":"",
    "晚自习制裁模式":"",
    "连点器":"可防屏保",
    "一键解希沃锁屏":"",
    "录制视频":""
}

import os
import sys
from pathlib import Path
from uuid import uuid1

from PyQt5.QtCore import Qt, QThread, pyqtSignal, QSize
from PyQt5.QtGui import QFont, QIcon
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QHBoxLayout, QFrame
from qfluentwidgets import (
    FluentWindow, NavigationItemPosition,
    ScrollArea, PushButton, CardWidget,
    TitleLabel, SubtitleLabel, BodyLabel, CaptionLabel,
    ComboBox, SwitchButton, InfoBar, InfoBarPosition,
    FluentIcon as FIF, IconWidget, setTheme, Theme,
    SmoothScrollArea, BreadcrumbBar, MessageBox, SplashScreen, SingleDirectionScrollArea, PushSettingCard, FluentIcon,
    QConfig, ConfigItem, BoolValidator, OptionsValidator, qconfig, ComboBoxSettingCard, OptionsConfigItem
)

def resource_path(relative_path):
    """获取打包后资源的绝对路径"""
    try:
        # PyInstaller 创建临时文件夹，将路径存储在_MEIPASS 中
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)
def printi(text:str):
    print(f"[info] {text}")

class More(QFrame):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setObjectName("more")

        self.vBoxLayout = QVBoxLayout(self)
        self.scroll = SingleDirectionScrollArea(orient=Qt.Vertical)

        self.scroll.setWidgetResizable(True)

        self.view = QWidget()
        self.scrollLayout = QVBoxLayout(self.view)
        self.scrollLayout.setContentsMargins(0, 0, 0, 0)
        self.scrollLayout.setSpacing(10)

        for title, content in more.items():
            self.card = PushSettingCard(
                text="启动",
                icon=FluentIcon.CODE,
                title=title,
                content=content
            )
            self.card.clicked.connect(lambda checked=False, t=title: HandleRun(t))
            self.scrollLayout.addWidget(self.card)

        self.scrollLayout.addStretch(1)
        self.scroll.setWidget(self.view)
        self.scroll.enableTransparentBackground()
        self.vBoxLayout.addWidget(self.scroll)

class Setting(QFrame):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setObjectName("settings")

        self.vBoxLayout = QVBoxLayout(self)
        self.scroll = SingleDirectionScrollArea(orient=Qt.Vertical)

        self.scroll.setWidgetResizable(True)

        self.view = QWidget()
        self.scrollLayout = QVBoxLayout(self.view)
        self.scrollLayout.setContentsMargins(0, 0, 0, 0)
        self.scrollLayout.setSpacing(10)

        self.card1=ComboBoxSettingCard(
            configItem=cfg.StartOption,
            icon=FluentIcon.COMPLETED,
            title="启动选项",
            content=None,
            texts=["总是询问","总是旧UI","总是新UI"]
        )
        cfg.StartOption.valueChanged.connect(print)

        #self.card.clicked.connect(lambda checked=False, t=title: HandleRun(t))

        self.scrollLayout.addWidget(self.card1)

        self.scrollLayout.addStretch(1)
        self.scroll.setWidget(self.view)
        self.scroll.enableTransparentBackground()
        self.vBoxLayout.addWidget(self.scroll)

def HandleStartOpt(t:str):
    print(t)

class MainWindow(FluentWindow):
    def __init__(self):
        super().__init__()
        #self.resize(400, 500)
        self.setMinimumSize(350, 300)
        self.setGeometry(100, 30, 400, 500)
        self.setWindowIcon(QIcon(resource_path('./app.ico') ) )
        self.setWindowTitle(AppName_en)
        #self.splash=SplashScreen(self.windowIcon(),self)
        self.splash=SplashScreen(QIcon(resource_path('./tengwar.png')),self)
        self.splash.setIconSize(QSize(700,700))
        self.show()

        #self.homeInterface = Home(self)
        #self.coreInterface = CoreInterface(self)
        self.moreInterface = More(self)
        self.settingInterface = Setting(self)

        #self.addSubInterface(self.homeInterface, FIF.HOME, '主页')
        #self.addSubInterface(self.coreInterface, FIF.LAYOUT, '核心功能')
        self.addSubInterface(self.moreInterface, FIF.EMOJI_TAB_SYMBOLS, '功能')
        self.addSubInterface(self.settingInterface, FIF.SETTING, '设置', position=NavigationItemPosition.BOTTOM)

        import time
        time.sleep(1)
        self.splash.finish()


class Config(QConfig):
    LogWhenKillApps=ConfigItem("logging","WriteLogWhenKillApps",False,BoolValidator())
    StartOption=OptionsConfigItem("start","StartOption","ask",OptionsValidator(["ask","old","new"]))
cfg=Config()
qconfig.load('./settings/settings_gui.json',cfg)
printi("Config Loaded.")
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