import ctypes
import os
import sys
import threading
# 获取更新
import urllib.request
import warnings
from bs4 import BeautifulSoup

from os import system
from PyQt5 import QtWidgets,QtCore
from PyQt5.QtCore import Qt, QProcess
from PyQt5.QtGui import QFont, QPixmap,QIcon
from PyQt5.QtWidgets import QApplication, QMainWindow, QTabWidget, QWidget, QPushButton, QVBoxLayout, QListWidget, \
    QListWidgetItem, QMessageBox

localversion1,localversion2,localversion3,localversion4=2,-10,-7,-5
#QApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling, True)
#QApplication.setAttribute(QtCore.Qt.AA_UseHighDpiPixmaps, True)
warnings.filterwarnings("ignore", category=DeprecationWarning)
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

#管理员
def is_admin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False
if not is_admin():
    ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, __file__, None, 1)
    sys.exit()

class SeewoKiller_run():
    class Wanzixi(threading.Thread):
        def run(self):
            system(".\\SeewoKiller.exe wanzixi")
    class Pingbao(threading.Thread):
        def run(self):
            system(".\\SeewoKiller.exe pingbao")
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
    class OldUI(threading.Thread):
        def run(self):
            system(".\\SeewoKiller.exe run -oldui")
    class setvalue():
        class Log(threading.Thread):
            def run(self):
                system(".\\Seewokiller.exe setvalue -log")
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
                system(".\\SeewoKIller.exe joke -killapp")
    class Game():
        class Wzq(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe game -wzq")
        class NumberDamn(threading.Thread):
            def run(self):
                system(".\\SeewoKiller.exe game -numberdamn")
class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.process = QProcess(self)
        self.setWindowTitle("希沃克星")
        self.setGeometry(100, 500, 600, 400)
        # 创建选项卡组件
        self.tab_widget = QTabWidget()
        self.setCentralWidget(self.tab_widget)

        # 按钮名称数组
        self.common_buttons = ["晚自习制裁模式", "一键防屏保", "小游戏"]
        # 创建“常用”选项卡
        self.tab_common = QWidget()
        self.tab_widget.addTab(self.tab_common, "常用")
        self.setup_common_tab()

        # 列表项名称数组（所有和设置选项卡共用）
        self.list_items_all = ["循环清任务", "一键卸载","晚自习制裁模式", "一键防屏保", "小游戏", "恶搞","注册表"]
        self.list_items_more = ["冰点还原破解","AI","计算π"]
        self.list_items_settings = ["退出","在晚自习制裁/循环清任务时启用日志","检查更新","关于", "使用经典界面"]

        # 创建“所有”选项卡
        self.tab_all = QWidget()
        self.tab_widget.addTab(self.tab_all, "核心功能")
        self.setup_all_tab()

        #创建“更多”选项卡
        self.tab_more=QWidget()
        self.tab_widget.addTab(self.tab_more,"附加功能")
        self.setup_more_tab()

        # 创建“设置”选项卡
        self.tab_settings = QWidget()
        self.tab_widget.addTab(self.tab_settings, "设置")
        self.setup_settings_tab()
    #def run_command(self,command):
        #self.process.start('cmd', ['/c', '.\\SeewoKiller.exe', command])
        #system(command)
    def setup_common_tab(self):
        layout = QVBoxLayout()
        layout.setSpacing(20)
        # 使用数组中的按钮名称创建按钮
        for button_name in self.common_buttons:
            button = QPushButton(button_name)
            button.setMinimumHeight(50)
            button.setFont(QFont("微软雅黑",10))
            button_number = self.common_buttons.index(button_name) + 1  # 获取按钮编号（从 1 开始）
            button.clicked.connect(lambda _, x=button_number: self.on_common_button_clicked(x))
            layout.addWidget(button)
        self.tab_common.setLayout(layout)

    def setup_all_tab(self):
        layout = QVBoxLayout()

        # 创建列表并添加项（使用数组中的项名称）
        self.list_widget = QListWidget()
        self.list_widget.setSpacing(5)
        for item_name in self.list_items_all:
            item = QListWidgetItem(item_name)
            item.setFont(QFont("微软雅黑",10))
            if item_name == "小游戏":
                item.setData(Qt.UserRole, "open_new_window_game")
            if item_name == "恶搞":
                item.setData(Qt.UserRole,"open_new_window_joke")
            if item_name == "注册表":
                item.setData(Qt.UserRole,"open_new_window_regedit")
            self.list_widget.addItem(item)

        self.list_widget.itemClicked.connect(self.on_list_item_clicked)

        layout.addWidget(self.list_widget)
        self.tab_all.setLayout(layout)

    def setup_more_tab(self):
        layout = QVBoxLayout()

        # 创建列表并添加项（与“所有”选项卡相同，使用数组中的项名称）
        self.more_list_widget = QListWidget()
        self.more_list_widget.setSpacing(5)
        for item_name in self.list_items_more:
            item = QListWidgetItem(item_name)
            item.setFont(QFont("微软雅黑", 10))
            # 这里不设置特殊项的数据，因为设置选项卡不需要打开新窗口的功能
            self.more_list_widget.addItem(item)
        self.more_list_widget.itemClicked.connect(self.on_list_item_clicked)
        layout.addWidget(self.more_list_widget)
        self.tab_more.setLayout(layout)

    def setup_settings_tab(self):
        layout = QVBoxLayout()

        # 创建列表并添加项（与“所有”选项卡相同，使用数组中的项名称）
        self.settings_list_widget = QListWidget()
        self.settings_list_widget.setSpacing(5)
        for item_name in self.list_items_settings:
            item=QListWidgetItem(item_name)
            item.setFont(QFont("微软雅黑",10))
            # 这里不设置特殊项的数据，因为设置选项卡不需要打开新窗口的功能
            self.settings_list_widget.addItem(item)

        self.settings_list_widget.itemClicked.connect(self.on_list_item_clicked)

        layout.addWidget(self.settings_list_widget)
        self.tab_settings.setLayout(layout)

    def on_common_button_clicked(self, button_number):
        if button_number==1:
            self.wanzixi=SeewoKiller_run.Wanzixi()
            self.wanzixi.start()
            #system(".\\SeewoKiller.exe wanzixi")
        if button_number==2:
            self.pingbao=SeewoKiller_run.Pingbao()
            self.pingbao.start()
            #system(".\\SeewoKiller.exe pingbao")
        if button_number==3:
            self.open_new_window_game()
        '''print("123", button_number)'''

    def on_list_item_clicked(self, item):
        # 检查项文本或数据以确定是否打开新窗口
        if item.text() == "小游戏" or item.data(Qt.UserRole) == "open_new_window_game":
            self.open_new_window_game()
        if item.text()== "恶搞" or item.data(Qt.UserRole) == "open_new_window_joke":
            self.open_new_window_joke()
        if item.text()== "注册表" or item.data(Qt.UserRole) == "open_new_window_regedit":
            self.open_new_window_regedit()
        if item.text() == "循环清任务":
            self.taskkill=SeewoKiller_run.Taskkill()
            self.taskkill.start()
        if item.text() == "一键卸载":
            self.uninstall=SeewoKiller_run.Uninstall()
            self.uninstall.start()
        if item.text() == "冰点还原破解":
            self.seewofreeze=SeewoKiller_run.SeewoFreeze()
            self.seewofreeze.start()
            #system(".\\SeewoFreeze\\SeewoFreezeUI.exe --startup-with-main-window")
        if item.text()=="AI":
            self.ai=SeewoKiller_run.AI()
            self.ai.start()
        if item.text()=="计算π":
            self.pai=SeewoKiller_run.pai()
            self.pai.start()
        if item.text() == "晚自习制裁模式":
            self.wanzixi=SeewoKiller_run.Wanzixi()
            self.wanzixi.start()
            #system(".\\SeewoKiller.exe wanzixi")
        if item.text() == "一键防屏保":
            self.pingbao=SeewoKiller_run.Pingbao()
            self.pingbao.start()
            #system(".\\SeewoKiller.exe pingbao")
            #print("456", item.text().split(' ')[1])
            # 打印项文本（去掉前面的“项”字和编号后的空格）
        if item.text()=="退出":
            sys.exit(app.exec_())
        if item.text()=="在晚自习制裁/循环清任务时启用日志":
            self.setvalue_log=SeewoKiller_run.setvalue.Log()
            self.setvalue_log.start()
        if item.text()=="检查更新":
            self.new_window_checkupdate=NewWindow_Update()
            self.new_window_checkupdate.show()
        if item.text() == "关于":
            self.new_window_about=NewWindow_About()
            self.new_window_about.show()
        if item.text() == "使用经典界面":
            self.oldui=SeewoKiller_run.OldUI()
            self.oldui.start()

    def open_new_window_game(self):
        self.new_window_game = NewWindow_game()
        self.new_window_game.show()
    def open_new_window_joke(self):
        self.new_window_joke = NewWindow_joke()
        self.new_window_joke.show()
    def open_new_window_regedit(self):
        self.new_window_regedit = NewWindow_regedit()
        self.new_window_regedit.show()

class NewWindow_game(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("小游戏")
        self.setGeometry(750, 500, 300, 200)

        layout = QVBoxLayout()

        # 新窗口的列表项名称数组
        self.new_list_items = ["数字炸弹", "五子棋"]

        # 创建列表并添加项（使用数组中的项名称）
        self.new_list_widget = QListWidget()
        self.new_list_widget.setSpacing(5)
        self.new_list_widget.setMinimumSize(300,200)
        for item_name in self.new_list_items:
            item=QListWidgetItem(item_name)
            item.setFont(QFont("微软雅黑",10))
            self.new_list_widget.addItem(item)
        self.new_list_widget.itemClicked.connect(self.on_list_item_clicked)
        layout.addWidget(self.new_list_widget)
        self.setLayout(layout)
    def on_list_item_clicked(self, item):
        if item.text() == "五子棋":
            self.wzq=SeewoKiller_run.Game.Wzq()
            self.wzq.start()
            #system(".\\SeewoKiller.exe game -wzq")
        if item.text()=="数字炸弹":
            self.numberdamn=SeewoKiller_run.Game.NumberDamn()
            self.numberdamn.start()
            #system(".\\SeewoKiller.exe game -numberdamn")
class NewWindow_regedit(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("注册表")
        self.setGeometry(750, 500, 300, 200)

        layout = QVBoxLayout()

        # 新窗口的列表项名称数组
        self.new_list_items = ["禁用任务栏菜单", "启用任务栏菜单", "禁用快捷键", "启用快捷键", "启用显示登录详细信息", "禁用显示登录详细信息","登录时显示提示","取消登录时显示提示"]

        # 创建列表并添加项（使用数组中的项名称）
        self.new_list_widget = QListWidget()
        self.new_list_widget.setSpacing(5)
        self.new_list_widget.setMinimumSize(300,200)
        for item_name in self.new_list_items:
            item=QListWidgetItem(item_name)
            item.setFont(QFont("微软雅黑",10))
            self.new_list_widget.addItem(item)
        self.new_list_widget.itemClicked.connect(self.on_list_item_clicked)
        layout.addWidget(self.new_list_widget)
        self.setLayout(layout)
    def on_list_item_clicked(self, item):
        if item.text() == "禁用任务栏菜单":
            self.regedit=SeewoKiller_run.Regedit.NoTrayContextMenu_True()
            self.regedit.start()
            #system(".\\SeewoKiller.exe regedit -NoTrayContextMenu true")
        if item.text()=="启用任务栏菜单":
            self.regedit = SeewoKiller_run.Regedit.NoTrayContextMenu_False()
            self.regedit.start()
            #system(".\\SeewoKiller.exe regedit -NoTrayContextMenu false")
        if item.text()=="禁用快捷键":
            self.regedit = SeewoKiller_run.Regedit.NoWinKeys_True()
            self.regedit.start()
            #system(".\\SeewoKiller.exe regedit -NoWinKeys true")
        if item.text()=="启用快捷键":
            self.regedit = SeewoKiller_run.Regedit.NoWinKeys_False()
            self.regedit.start()
            #system(".\\SeewoKiller.exe regedit -NoWinKeys false")
        if item.text()=="启用显示登录详细信息":
            self.regedit = SeewoKiller_run.Regedit.VerboseStatus_True()
            self.regedit.start()
            #system(".\\SeewoKiller.exe regedit -VerboseStatus true")
        if item.text()=="禁用显示登录详细信息":
            self.regedit = SeewoKiller_run.Regedit.VerboseStatus_False()
            self.regedit.start()
            #system(".\\SeewoKiller.exe regedit -VerboseStatus false")
        if item.text()=="登录时显示提示":
            self.regedit=SeewoKiller_run.Regedit.legalnotice_True()
            self.regedit.start()
            #system(".\\SeewoKiller.exe regedit -legalnotice true")
        if item.text()=="取消登录时显示提示":
            self.regedit=SeewoKiller_run.Regedit.legalnotice_False()
            self.regedit.start()
            #system(".\\SeewoKiller.exe regedit -legalnotice false")
class NewWindow_joke(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("恶搞")
        self.setGeometry(750, 500, 300, 200)

        layout = QVBoxLayout()

        # 新窗口的列表项名称数组
        self.new_list_items = ["杀死所有有用的进程"]

        # 创建列表并添加项（使用数组中的项名称）
        self.new_list_widget = QListWidget()
        self.new_list_widget.setSpacing(5)
        self.new_list_widget.setMinimumSize(300,200)
        for item_name in self.new_list_items:
            item=QListWidgetItem(item_name)
            item.setFont(QFont("微软雅黑",10))
            self.new_list_widget.addItem(item)
        self.new_list_widget.itemClicked.connect(self.on_list_item_clicked)
        layout.addWidget(self.new_list_widget)
        self.setLayout(layout)

    def on_list_item_clicked(self, item):
        if item.text() == "杀死所有有用的进程":
            self.killapp=SeewoKiller_run.Joke.Killapp()
            self.killapp.start()
            #system(".\\SeewoKiller.exe joke -killapp")
class NewWindow_About(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("关于")
        self.setGeometry(100, 100, 500, 400)
        layout = QVBoxLayout()
        self.label=QtWidgets.QLabel()
        #self.label.setGeometry(QRect(30,30,81,41))
        self.label.setPixmap(QPixmap('.\\seewokiller2.png'))
        self.label2=QtWidgets.QLabel()
        self.label2.setWordWrap(True)
        self.label2.setFont(QFont("微软雅黑",10))
        self.label2.setText("SeewoKiller 2.0\n希沃克星 2.0\n版本代号：郑子谦\n卓然第三帝国 https://whstu.us.kg/提供技术支持\nSeewoKiller QQ 群：664929698")
        layout.addWidget(self.label)
        layout.addWidget(self.label2)
        self.setLayout(layout)
class NewWindow_Update(QWidget):
    def __init__(self):
        super().__init__()
        self.layout = QVBoxLayout()
        self.layout.setSpacing(20)
        self.setWindowTitle("检查更新")
        self.setGeometry(800, 800, 350, 30)
        self.button = QPushButton("点此检查更新")
        self.button.setMinimumHeight(50)
        self.button.setFont(QFont("微软雅黑", 10))
        self.button.clicked.connect(self.check_update)
        self.layout.addWidget(self.button)  # 正确添加按钮到布局
        self.setLayout(self.layout)  # 布局设置给窗口而非按钮
    def message_true(self):
        print("有可用更新")
        self.reply = QMessageBox.question(NewWindow_Update(), "提示",
             f"有可用更新{self.version1}.{self.version2}.{self.version3}.{self.version4}\n是否前往网页下载？",
             QMessageBox.Yes | QMessageBox.No, QMessageBox.Yes)
        print(self.reply)
        if self.reply == QMessageBox.Yes:
            import webbrowser
            webbrowser.open("https://whstu.us.kg/download/seewokiller/")
    def message_false(self):
        print("暂无更新")
        self.msg_box = QMessageBox(QMessageBox.Information, "提示", "暂无可用更新。")
        self.msg_box.exec_()
    def check_update(self):
        response = urllib.request.urlopen("https://seewokiller.whstu.us.kg/installer/index.html")
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
        if localversion1<self.version1:
            self.message_true()
        elif localversion1==self.version1:
            if localversion2<self.version2:
                self.message_true()
            elif localversion2==self.version2:
                if localversion3<self.version3:
                    self.message_true()
                elif localversion3==self.version3:
                    if localversion4<self.version4:
                        self.message_true()
                    else:
                        self.message_false()
                else:
                    self.message_false()
            else:
                self.message_false()
        else:
            self.message_false()


if __name__ == "__main__":
    QhShiPeiBeiShu = {
        "3840": 2,
    }
    # 获取屏幕分辨率（显示&物理）
    QhXSwidth, QhXSheight, QhWLwidth, QhWLheight = QhGetScreenResolution()
    if QhWLwidth > 1920:
        # 是否启动适配比例
        QhFd = int(QhWLwidth / QhXSwidth)  # 计算缩放比例
        # 设置 QT_SCREEN_SCALE_FACTORS 环境变量
        if QhFd == 1:
            QhFd0 = QhShiPeiBeiShu[str(QhWLwidth)]
            os.environ["QT_SCREEN_SCALE_FACTORS"] = "{}".format(str(QhFd0))
        elif QhFd > 1:
            os.environ["QT_SCREEN_SCALE_FACTORS"] = "{}".format(str(QhFd))
        # QApplication.setHighDpiScaleFactorRoundingPolicy(
        #     Qt.HighDpiScaleFactorRoundingPolicy.PassThrough)
        # QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
        # QApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)
        QApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling, True)  # 适应高 DPI 设备
        QApplication.setAttribute(QtCore.Qt.AA_UseHighDpiPixmaps, True)
    app = QApplication(sys.argv)
    #设置图标
    app.setWindowIcon(QIcon('SeewoKiller.ico'))
    #设置字体
    font = QFont()
    font.setFamily("微软雅黑")
    font.setPointSize(10)
    app.setFont(font)
    #-------
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
#pyinstaller -F -w -i SeewoKiller.ico gui.py