import ctypes
import os
import sys
import threading
import urllib.request
import warnings
from threading import Thread
from uuid import uuid1

from bs4 import BeautifulSoup
from os import system
from PyQt5 import QtWidgets, QtCore
from PyQt5.QtCore import Qt, QProcess
from PyQt5.QtGui import QFont, QPixmap, QIcon
from PyQt5.QtWidgets import QApplication, QMainWindow, QTabWidget, QWidget, QPushButton, QVBoxLayout, QListWidget, \
    QListWidgetItem, QMessageBox, QFrame, QHBoxLayout, QLabel, QStackedWidget

localversion1, localversion2, localversion3, localversion4 = 2, 1, 0, 0
warnings.filterwarnings("ignore", category=DeprecationWarning)

from qfluentwidgets import NavigationItemPosition, FluentWindow, SubtitleLabel, setFont, MSFluentWindow, CardWidget, \
    IconWidget, BodyLabel, CaptionLabel, PushButton, InfoBar, InfoBarPosition, ImageLabel, ElevatedCardWidget, \
    FlowLayout, BreadcrumbBar, PrimaryToolButton, LineEdit, ScrollArea, InfoBarIcon, PrimaryPushButton
from qfluentwidgets import FluentIcon as FIF

#管理员
def is_admin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False
if not is_admin():
    ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, __file__, None, 1)
    sys.exit()

def QhGetScreenResolution():
    # 获取屏幕分辨率（显示&物理）
    import tkinter as tk
    import pyautogui
    # 创建一个隐藏的 Tkinter 根窗口
    QhRoot = tk.Tk()
    QhRoot.withdraw()  # 隐藏根窗口
    # 获取屏幕显示分辨率
    QhXSwidth = QhRoot.winfo_screenwidth()
    QhXSheight = QhRoot.winfo_screenheight()
    # 关闭 Tkinter 根窗口
    QhRoot.destroy()
    print(f"屏幕显示分辨率 QH: {QhXSwidth}x{QhXSheight}")
    # 获取屏幕物理分辨率
    size = pyautogui.size()
    QhWLwidth = size.width
    QhWLheight = size.height
    print(f"屏幕物理分辨率 QH: {QhWLwidth}x{QhWLheight}")
    return QhXSwidth, QhXSheight, QhWLwidth, QhWLheight

class SeewoKiller_run():
    class Wanzixi(threading.Thread):
        def run(self):
            system(".\\SeewoKiller.exe wanzixi")
    class Pingbao(threading.Thread):
        def run(self):
            system(".\\SeewoKiller.exe liandianqi")
    class AI(threading.Thread):
        def run(self):
            system(".\\ai.exe")
    class pai(threading.Thread):
        def run(self):
            system(".\\pai.exe")
    class Taskkill(threading.Thread):
        def run(self):
            system(".\\SeewoKiller.exe taskkill")
    class Uninstall(threading.Thread):
        def run(self):
            system(".\\SeewoKiller.exe uninstall")
    class SeewoFreeze(threading.Thread):
        def run(self):
            system(".\\SeewoKiller.exe seewofreeze")
    class SeewoLock(threading.Thread):
        def run(self):
            system(".\\SeewoKiller.exe wanzixi")
    class OldUI(threading.Thread):
        def run(self):
            system(".\\SeewoKiller.exe ui -oldui")
    class Run():
        class Fastboot(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe run fastboot")
    class Setvalue():
        class Log(threading.Thread):
            def run(self):
                system(".\\Seewokiller.exe setvalue -log")
        class Button(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe setvalue -button")
        class Start(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe setvalue -start")
    class Regedit():
        class NoTrayContextMenu_True(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe regedit -NoTrayContextMenu true")
        class NoTrayContextMenu_False(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe regedit -NoTrayContextMenu false")
        class NoWinKeys_True(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe regedit -NoWinKeys true")
        class NoWinKeys_False(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe regedit -NoWinKeys false")
        class VerboseStatus_True(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe regedit -VerboseStatus true")
        class VerboseStatus_False(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe regedit -VerboseStatus false")
        class legalnotice_True(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe regedit -legalnotice true")
        class legalnotice_False(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe regedit -legalnotice false")
    class Joke():
        class Killapp(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe joke -killapp")
        class Copyfile(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe joke -copyfile")
    class Game():
        class Wzq(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe game -wzq")
        class NumberDamn(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe game -numberdamn")
        class Fjdz(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe game -fjdz")
        class Emlpd(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe game -emlpd")

class Widget(QFrame):
    def __init__(self, text: str, parent=None):
        super().__init__(parent=parent)
        self.label = SubtitleLabel(text, self)
        self.hBoxLayout = QHBoxLayout(self)

        setFont(self.label, 24)
        self.label.setAlignment(Qt.AlignCenter)
        self.hBoxLayout.addWidget(self.label, 1, Qt.AlignCenter)
        self.setObjectName(text.replace(' ', '-'))

class show_update_dialog(QWidget):
    # 简化的更新检查，实际使用时需要完整的更新检查逻辑
    def __init__(self):
        super().__init__()
        self.layout = QVBoxLayout()
        self.layout.setSpacing(20)
        self.setWindowTitle("检查更新")
        self.setGeometry(200, 200, 350, 30)
        self.button = PrimaryPushButton("点此检查更新")
        self.button.setMinimumHeight(50)
        self.button.setFont(QFont("微软雅黑", 10))
        self.button.clicked.connect(self.check_update)
        self.layout.addWidget(self.button)  # 正确添加按钮到布局
        self.setLayout(self.layout)  # 布局设置给窗口而非按钮

    def message_true(self):
        print("有可用更新")
        self.reply = QMessageBox.question(self, "提示",
                                          f"有可用更新{self.version1}.{self.version2}.{self.version3}.{self.version4}\n是否前往网页下载？",
                                          QMessageBox.Yes | QMessageBox.No, QMessageBox.Yes)
        print(self.reply)
        if self.reply == QMessageBox.Yes:
            import webbrowser
            webbrowser.open("https://whstu.dpdns.org/download/seewokiller/")

    def message_false(self):
        print("暂无更新")
        self.msg_box = QMessageBox(QMessageBox.Information, "提示", "暂无可用更新。")
        self.msg_box.exec_()

    def check_update(self):
        response = urllib.request.urlopen("https://seewokiller.whstu.dpdns.org/installer/index.html")
        html = response.read()
        soup = BeautifulSoup(html, 'html.parser')
        p_tags = soup.find_all('p')
        self.version1, self.version2, self.version3, self.version4 = -1, -1, -1, -1
        if len(p_tags) >= 4:
            for i, p_tag in enumerate(p_tags[:4]):
                text = p_tag.get_text().strip()
                try:
                    number = int(text)
                    if i == 0:
                        self.version1 = number
                    elif i == 1:
                        self.version2 = number
                    elif i == 2:
                        self.version3 = number
                    elif i == 3:
                        self.version4 = number
                except ValueError:
                    continue
        else:
            print("Error:<p>Not Found.")
        print(f"Version Check Website: {self.version1}.{self.version2}.{self.version3}.{self.version4}")
        print(f"Local Version: {localversion1}.{localversion2}.{localversion3}.{localversion4}")
        if localversion1 < self.version1:
            self.message_true()
        elif localversion1 == self.version1:
            if localversion2 < self.version2:
                self.message_true()
            elif localversion2 == self.version2:
                if localversion3 < self.version3:
                    self.message_true()
                elif localversion3 == self.version3:
                    if localversion4 < self.version4:
                        self.message_true()
                    else:
                        self.message_false()
                else:
                    self.message_false()
            else:
                self.message_false()
        else:
            self.message_false()

class Card(CardWidget):
    def __init__(self, icon, title, content, parent=None, is_navigation=False):
        super().__init__()
        self.icon = IconWidget(icon)
        self.title = BodyLabel(title, self)
        self.content = CaptionLabel(content, self)
        self.is_navigation = is_navigation

        if is_navigation:
            self.openButton = PrimaryToolButton(FIF.CHEVRON_RIGHT, self)
            self.openButton.setFixedSize(34, 34)
        else:
            self.openButton = PushButton('启动', self)
            self.openButton.setFixedWidth(120)

        self.hlayout = QHBoxLayout(self)
        self.vlayout = QVBoxLayout()
        self.setFixedHeight(73)
        self.icon.setFixedSize(48, 48)
        self.content.setTextColor("#606060", "#d2d2d2")
        self.hlayout.setContentsMargins(20, 11, 11, 11)
        self.hlayout.setSpacing(15)
        self.hlayout.addWidget(self.icon)
        self.vlayout.setContentsMargins(0, 0, 0, 0)
        self.vlayout.setSpacing(0)
        self.vlayout.addWidget(self.title, 0, Qt.AlignVCenter)
        self.vlayout.addWidget(self.content, 0, Qt.AlignVCenter)
        self.vlayout.setAlignment(Qt.AlignVCenter)
        self.hlayout.addLayout(self.vlayout)
        self.hlayout.addStretch(1)
        self.hlayout.addWidget(self.openButton, 0, Qt.AlignRight)

        self.openButton.clicked.connect(self.on_card_clicked)

    def on_card_clicked(self):
        title = self.title.text()
        if hasattr(self.parent(), 'handle_card_click'):
            self.parent().handle_card_click(title)


class bigCard(ElevatedCardWidget):
    def __init__(self, iconPath: str, name: str, parent=None):
        super().__init__(parent)
        self.iconWidget = ImageLabel(iconPath, self)
        self.label = CaptionLabel(name, self)
        self.iconWidget.scaledToHeight(68)
        self.vBoxLayout = QVBoxLayout(self)
        self.vBoxLayout.setAlignment(Qt.AlignCenter)
        self.vBoxLayout.addStretch(1)
        self.vBoxLayout.addWidget(self.iconWidget, 0, Qt.AlignCenter)
        self.vBoxLayout.addStretch(1)
        self.vBoxLayout.addWidget(self.label, 0, Qt.AlignHCenter | Qt.AlignBottom)
        self.setFixedSize(168, 176)

        self.clicked.connect(self.on_card_clicked)

    def on_card_clicked(self):
        name = self.label.text()
        if hasattr(self.parent(), 'handle_bigcard_click'):
            self.parent().handle_bigcard_click(name)


class Window(MSFluentWindow):
    def __init__(self):
        super().__init__()

        self.homeInterface = Home(self)
        self.coreInterface = CoreInterface(self)
        self.moreInterface = MoreInterface(self)
        self.settingInterface = SettingInterface(self)

        self.addSubInterface(self.homeInterface, FIF.HOME, '主页')
        self.addSubInterface(self.coreInterface, FIF.LAYOUT, '核心功能')
        self.addSubInterface(self.moreInterface, FIF.EMOJI_TAB_SYMBOLS, '附加功能')
        self.addSubInterface(self.settingInterface, FIF.SETTING, '设置', position=NavigationItemPosition.BOTTOM)

        self.initWindow()

    def initWindow(self):
        self.resize(800, 300)
        self.setWindowIcon(QIcon('./app.ico'))
        self.setWindowTitle('SeewoKiller')


class Home(QFrame):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setObjectName("主页")

        self.layout = QVBoxLayout(self)

        self.SeewoLock = bigCard("./app.ico", "一键解希沃锁屏", self)
        self.Wanzixi = bigCard("./app.ico", "晚自习制裁模式", self)
        self.Ldq = bigCard("./app.ico", "一键防屏保", self)
        self.Game = bigCard("./app.ico", "小游戏", self)

        # 创建水平布局来放置卡片
        self.cardLayout = QHBoxLayout()
        self.cardLayout.addWidget(self.SeewoLock)
        self.cardLayout.addWidget(self.Wanzixi)
        self.cardLayout.addWidget(self.Ldq)
        self.cardLayout.addWidget(self.Game)
        self.cardLayout.addStretch()

        self.layout.addLayout(self.cardLayout)
        self.layout.addStretch()

    def handle_bigcard_click(self, name):
        if name == "一键解希沃锁屏":
            self.seewolock = SeewoKiller_run.SeewoLock()
            self.seewolock.start()
        elif name == "晚自习制裁模式":
            self.wanzixi = SeewoKiller_run.Wanzixi()
            self.wanzixi.start()
        elif name == "一键防屏保":
            self.pingbao = SeewoKiller_run.Pingbao()
            self.pingbao.start()
        elif name == "小游戏":
            parent_window = self.window()
            if hasattr(parent_window, 'coreInterface'):
                # 先切换到核心功能界面
                parent_window.switchTo(parent_window.coreInterface)
                # 使用 QTimer 延迟执行，确保界面切换完成
                QtCore.QTimer.singleShot(100, parent_window.coreInterface.show_game_interface)


class CoreInterface(QFrame):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setObjectName("核心功能")

        self.breadcrumbBar = BreadcrumbBar(self)
        self.stackedWidget = QStackedWidget(self)
        self.vBoxLayout = QVBoxLayout(self)

        setFont(self.breadcrumbBar, 20)
        self.breadcrumbBar.setSpacing(15)
        self.breadcrumbBar.currentItemChanged.connect(self.switchInterface)

        self.vBoxLayout.setContentsMargins(20, 20, 20, 20)
        self.vBoxLayout.addWidget(self.breadcrumbBar)
        self.vBoxLayout.addWidget(self.stackedWidget)

        # 创建主界面
        self.create_main_interface()

    def create_main_interface(self):
        main_widget = QFrame()

        # 创建滚动区域
        scroll_area = ScrollArea()
        scroll_area.setWidgetResizable(True)
        scroll_area.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        scroll_area.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        scroll_area.setStyleSheet("QScrollArea{background: transparent; border: none}")
        # 创建内容 widget
        content_widget = QFrame()
        main_layout = QVBoxLayout(content_widget)

        # 核心功能卡片 - 注意最后三个卡片使用导航样式
        self.card1 = Card(icon=FIF.DELETE, title="循环清任务 (可解屏保)", content="清理系统任务进程",
                          parent=content_widget)
        self.card2 = Card(icon=FIF.CANCEL, title="一键卸载", content="卸载希沃相关软件", parent=content_widget)
        self.card3 = Card(icon=FIF.POWER_BUTTON, title="晚自习制裁模式", content="限制系统功能", parent=content_widget)
        self.card4 = Card(icon=FIF.PLAY, title="连点器 (自习课防屏保)", content="自动点击防屏保", parent=content_widget)
        self.card5 = Card(icon=FIF.SETTING, title="一键解希沃锁屏", content="解除屏幕锁定", parent=content_widget)
        self.card6 = Card(icon=FIF.GAME, title="小游戏", content="内置小游戏集合", parent=content_widget,
                          is_navigation=True)
        self.card7 = Card(icon=FIF.SETTING, title="恶搞", content="恶搞功能", parent=content_widget, is_navigation=True)
        self.card8 = Card(icon=FIF.SETTING, title="注册表", content="注册表相关设置", parent=content_widget,
                          is_navigation=True)

        main_layout.addWidget(self.card1)
        main_layout.addWidget(self.card2)
        main_layout.addWidget(self.card3)
        main_layout.addWidget(self.card4)
        main_layout.addWidget(self.card5)
        main_layout.addWidget(self.card6)
        main_layout.addWidget(self.card7)
        main_layout.addWidget(self.card8)
        main_layout.addStretch()

        # 设置滚动区域
        scroll_area.setWidget(content_widget)
        scroll_area.enableTransparentBackground()

        # 主 widget 布局
        main_widget_layout = QVBoxLayout(main_widget)
        main_widget_layout.setContentsMargins(0, 0, 0, 0)
        main_widget_layout.addWidget(scroll_area)

        content_widget.setObjectName("main")
        content_widget.handle_card_click = self.handle_card_click  # 手动绑定处理函数
        main_widget.setObjectName("main")
        main_widget.handle_card_click = self.handle_card_click  # 也为主 widget 绑定
        self.stackedWidget.addWidget(main_widget)

        # 初始化面包屑导航
        self.breadcrumbBar.clear()
        self.breadcrumbBar.addItem("main", "核心功能")

    def handle_card_click(self, title):
        if title == "循环清任务 (可解屏保)":
            self.taskkill = SeewoKiller_run.Taskkill()
            self.taskkill.start()
        elif title == "一键卸载":
            self.uninstall = SeewoKiller_run.Uninstall()
            self.uninstall.start()
        elif title == "晚自习制裁模式":
            self.wanzixi = SeewoKiller_run.Wanzixi()
            self.wanzixi.start()
        elif title == "连点器 (自习课防屏保)":
            self.pingbao = SeewoKiller_run.Pingbao()
            self.pingbao.start()
        elif title == "一键解希沃锁屏":
            self.seewolock = SeewoKiller_run.SeewoLock()
            self.seewolock.start()
        elif title == "小游戏":
            self.show_game_interface()
        elif title == "恶搞":
            self.show_joke_interface()
        elif title == "注册表":
            self.show_regedit_interface()

    def _breadcrumb_has_item(self, item_name):
        """检查面包屑导航中是否已有指定项目"""
        try:
            # 尝试获取面包屑导航的所有项目
            for i in range(self.breadcrumbBar.count() if hasattr(self.breadcrumbBar, 'count') else 0):
                if hasattr(self.breadcrumbBar, 'itemAt'):
                    item = self.breadcrumbBar.itemAt(i)
                    if item and hasattr(item, 'routeKey') and item.routeKey == item_name:
                        return True
        except:
            pass
        return False

    def show_game_interface(self):
        # 检查是否已经存在游戏界面
        game_widget = None
        for i in range(self.stackedWidget.count()):
            widget = self.stackedWidget.widget(i)
            if widget.objectName() == "game":
                game_widget = widget
                break

        if game_widget is None:
            # 创建新的游戏界面
            game_widget = QFrame()
            game_layout = QVBoxLayout(game_widget)

            self.game_card1 = Card(icon=FIF.GAME, title="数字炸弹", content="数字猜谜游戏", parent=game_widget)
            self.game_card2 = Card(icon=FIF.GAME, title="五子棋", content="经典五子棋游戏", parent=game_widget)
            self.game_card3 = Card(icon=FIF.GAME, title="飞机大战", content="飞行射击游戏", parent=game_widget)
            self.game_card4 = Card(icon=FIF.GAME, title="恶魔轮盘赌", content="刺激轮盘游戏", parent=game_widget)

            game_layout.addWidget(self.game_card1)
            game_layout.addWidget(self.game_card2)
            game_layout.addWidget(self.game_card3)
            game_layout.addWidget(self.game_card4)
            game_layout.addStretch()

            game_widget.setObjectName("game")
            game_widget.handle_card_click = self.handle_game_click  # 手动绑定处理函数
            self.stackedWidget.addWidget(game_widget)

        # 重置面包屑导航到正确的路径
        self.breadcrumbBar.clear()
        self.breadcrumbBar.addItem("main", "核心功能")
        self.breadcrumbBar.addItem("game", "小游戏")

        self.stackedWidget.setCurrentWidget(game_widget)

    def show_joke_interface(self):
        # 检查是否已经存在恶搞界面
        joke_widget = None
        for i in range(self.stackedWidget.count()):
            widget = self.stackedWidget.widget(i)
            if widget.objectName() == "joke":
                joke_widget = widget
                break

        if joke_widget is None:
            # 创建新的恶搞界面
            joke_widget = QFrame()
            joke_layout = QVBoxLayout(joke_widget)

            self.joke_card1 = Card(icon=FIF.DELETE, title="杀死所有有用的进程", content="终止系统进程",
                                   parent=joke_widget)
            self.joke_card2 = Card(icon=FIF.COPY, title="自动复制 U 盘文件", content="复制移动设备文件",
                                   parent=joke_widget)

            joke_layout.addWidget(self.joke_card1)
            joke_layout.addWidget(self.joke_card2)
            joke_layout.addStretch()

            joke_widget.setObjectName("joke")
            joke_widget.handle_card_click = self.handle_joke_click  # 手动绑定处理函数
            self.stackedWidget.addWidget(joke_widget)

        # 重置面包屑导航到正确的路径
        self.breadcrumbBar.clear()
        self.breadcrumbBar.addItem("main", "核心功能")
        self.breadcrumbBar.addItem("joke", "恶搞")

        self.stackedWidget.setCurrentWidget(joke_widget)

    def show_regedit_interface(self):
        # 检查是否已经存在注册表界面
        regedit_widget = None
        for i in range(self.stackedWidget.count()):
            widget = self.stackedWidget.widget(i)
            if widget.objectName() == "regedit":
                regedit_widget = widget
                break

        if regedit_widget is None:
            # 创建新的注册表界面
            from PyQt5.QtWidgets import QScrollArea
            regedit_widget = QFrame()

            # 创建滚动区域
            scroll_area = ScrollArea()
            scroll_area.setWidgetResizable(True)
            scroll_area.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
            scroll_area.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
            scroll_area.setStyleSheet("QScrollArea{background: transparent; border: none}")
            # 创建内容 widget
            content_widget = QFrame()
            regedit_layout = QVBoxLayout(content_widget)

            cards_data = [
                ("禁用任务栏菜单", "禁用系统任务栏右键菜单"),
                ("启用任务栏菜单", "恢复系统任务栏右键菜单"),
                ("禁用快捷键", "禁用 Windows 快捷键"),
                ("启用快捷键", "恢复 Windows 快捷键"),
                ("启用显示登录详细信息", "显示登录过程详细信息"),
                ("禁用显示登录详细信息", "隐藏登录过程详细信息"),
                ("登录时显示提示", "登录时显示自定义提示"),
                ("取消登录时显示提示", "取消登录提示显示")
            ]

            self.regedit_cards = []
            for title, content in cards_data:
                card = Card(icon=FIF.SETTING, title=title, content=content, parent=content_widget)
                self.regedit_cards.append(card)
                regedit_layout.addWidget(card)

            regedit_layout.addStretch()

            # 设置滚动区域
            scroll_area.setWidget(content_widget)
            scroll_area.enableTransparentBackground()
            # 主 widget 布局
            regedit_widget_layout = QVBoxLayout(regedit_widget)
            regedit_widget_layout.setContentsMargins(0, 0, 0, 0)
            regedit_widget_layout.addWidget(scroll_area)

            content_widget.handle_card_click = self.handle_regedit_click  # 手动绑定处理函数
            regedit_widget.handle_card_click = self.handle_regedit_click  # 也为主 widget 绑定
            regedit_widget.setObjectName("regedit")
            self.stackedWidget.addWidget(regedit_widget)

        # 重置面包屑导航到正确的路径
        self.breadcrumbBar.clear()
        self.breadcrumbBar.addItem("main", "核心功能")
        self.breadcrumbBar.addItem("regedit", "注册表")

        self.stackedWidget.setCurrentWidget(regedit_widget)

    def switchInterface(self, objectName):
        # 查找对应的 widget
        target_widget = None
        for i in range(self.stackedWidget.count()):
            widget = self.stackedWidget.widget(i)
            if widget.objectName() == objectName:
                target_widget = widget
                break

        if target_widget:
            self.stackedWidget.setCurrentWidget(target_widget)

    def handle_game_click(self, title):
        if title == "五子棋":
            self.wzq = SeewoKiller_run.Game.Wzq()
            self.wzq.start()
        elif title == "数字炸弹":
            self.numberdamn = SeewoKiller_run.Game.NumberDamn()
            self.numberdamn.start()
        elif title == "飞机大战":
            self.fjdz = SeewoKiller_run.Game.Fjdz()
            self.fjdz.start()
        elif title == "恶魔轮盘赌":
            self.emlpd = SeewoKiller_run.Game.Emlpd()
            self.emlpd.start()

    def handle_joke_click(self, title):
        if title == "杀死所有有用的进程":
            self.killapp = SeewoKiller_run.Joke.Killapp()
            self.killapp.start()
        elif title == "自动复制 U 盘文件":
            self.copyfile = SeewoKiller_run.Joke.Copyfile()
            self.copyfile.start()

    def handle_regedit_click(self, title):
        if title == "禁用任务栏菜单":
            self.regedit = SeewoKiller_run.Regedit.NoTrayContextMenu_True()
            self.regedit.start()
        elif title == "启用任务栏菜单":
            self.regedit = SeewoKiller_run.Regedit.NoTrayContextMenu_False()
            self.regedit.start()
        elif title == "禁用快捷键":
            self.regedit = SeewoKiller_run.Regedit.NoWinKeys_True()
            self.regedit.start()
        elif title == "启用快捷键":
            self.regedit = SeewoKiller_run.Regedit.NoWinKeys_False()
            self.regedit.start()
        elif title == "启用显示登录详细信息":
            self.regedit = SeewoKiller_run.Regedit.VerboseStatus_True()
            self.regedit.start()
        elif title == "禁用显示登录详细信息":
            self.regedit = SeewoKiller_run.Regedit.VerboseStatus_False()
            self.regedit.start()
        elif title == "登录时显示提示":
            self.regedit = SeewoKiller_run.Regedit.legalnotice_True()
            self.regedit.start()
        elif title == "取消登录时显示提示":
            self.regedit = SeewoKiller_run.Regedit.legalnotice_False()
            self.regedit.start()


class MoreInterface(QFrame):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setObjectName("附加功能")

        self.layout = QVBoxLayout(self)

        self.card1 = Card(icon=FIF.CLOUD, title="冰点还原破解", content="破解冰点还原软件")
        self.card2 = Card(icon=FIF.ROBOT, title="AI", content="人工智能助手")
        self.card3 = Card(icon=FIF.SETTING, title="计算π", content="高精度π值计算")

        self.layout.addWidget(self.card1)
        self.layout.addWidget(self.card2)
        self.layout.addWidget(self.card3)
        InfoBar.warning(
            title='提示',
            content="新 UI 对冰点破解界面适配不佳。请使用旧版界面启动。",
            orient=Qt.Horizontal,
            isClosable=True,
            position=InfoBarPosition.BOTTOM,
            duration=-1,  # 永不消失
            parent=self
        )
        self.layout.addStretch()

    def handle_card_click(self, title):
        if title == "冰点还原破解":
            self.seewofreeze = SeewoKiller_run.SeewoFreeze()
            self.seewofreeze.start()
        elif title == "AI":
            self.ai = SeewoKiller_run.AI()
            self.ai.start()
        elif title == "计算π":
            self.pai = SeewoKiller_run.pai()
            self.pai.start()


class SettingInterface(QFrame):
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setObjectName("设置")

        self.layout = QVBoxLayout(self)

        settings_data = [
            ("在晚自习制裁/循环清任务时启用日志", "启用详细日志记录"),
            ("允许使用\"关闭\"按钮 (仅适用旧 UI)", "允许关闭按钮功能"),
            ("启动设置", "配置程序启动选项"),
            ("检查更新", "检查软件更新"),
            ("使用经典界面", "切换到经典用户界面"),
            ("重启到 fastboot", "重启到快速启动模式"),
            ("关于", "查看软件信息")
        ]

        self.setting_cards = []
        for title, content in settings_data:
            card = Card(icon=FIF.SETTING, title=title, content=content)
            card.openButton.clicked.connect(lambda checked, t=title: self.handle_setting_click(t))
            self.setting_cards.append(card)
            self.layout.addWidget(card)

        self.layout.addStretch()

    def handle_setting_click(self, title):
        if title == "在晚自习制裁/循环清任务时启用日志":
            self.setvalue_log = SeewoKiller_run.Setvalue.Log()
            self.setvalue_log.start()
        elif title == "允许使用\"关闭\"按钮 (仅适用旧 UI)":
            self.setvale_button = SeewoKiller_run.Setvalue.Button()
            self.setvale_button.start()
        elif title == "启动设置":
            self.setvale_start = SeewoKiller_run.Setvalue.Start()
            self.setvale_start.start()
        elif title == "重启到 fastboot":
            self.fastboot = SeewoKiller_run.Run.Fastboot()
            self.fastboot.start()
            system("TASKKILL /F /IM gui.exe")
        elif title == "检查更新":
            self.window=show_update_dialog()
            self.window.show()
        elif title == "关于":
            InfoBar.info(
                title='提示',
                content="请使用旧 UI 开启。",
                orient=Qt.Horizontal,
                isClosable=True,
                position=InfoBarPosition.TOP_RIGHT,
                duration=5000,  # 永不消失
                parent=self
            )
        elif title == "使用经典界面":
            self.oldui = SeewoKiller_run.OldUI()
            self.oldui.start()


if __name__ == '__main__':
    QhXSwidth, QhXSheight, QhWLwidth, QhWLheight = QhGetScreenResolution()
    if QhWLwidth > 1920:
        os.environ["QT_SCALE_FACTOR"] = "1"
        QApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling, True)  # 适应高 DPI 设备
        QApplication.setAttribute(QtCore.Qt.AA_UseHighDpiPixmaps, True)
    else:
        os.environ["QT_SCALE_FACTOR"] = "0.75"
        QApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling, True)
        QApplication.setAttribute(QtCore.Qt.AA_UseHighDpiPixmaps, True)
        pass
    app = QApplication(sys.argv)
    w = Window()
    w.setGeometry(100, 30, 800, 500)
    w.show()
    app.exec()