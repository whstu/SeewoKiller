import sys
import ctypes
from PyQt5.QtWidgets import QApplication, QMainWindow, QTabWidget, QWidget, QPushButton, QVBoxLayout, QListWidget, QListWidgetItem, QDialog
from PyQt5.QtCore import Qt

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)

import os

'''管理员'''
def is_admin():
    try:
        return ctypes.windll.shell32.IsUserAnAdmin()
    except:
        return False
if not is_admin():
    ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, __file__, None, 1)
    sys.exit()

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("希沃克星")
        self.setGeometry(100, 100, 600, 400)

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
        self.list_items_all = ["循环清任务", "一键卸载", "冰点解冻", "晚自习制裁模式", "一键防屏保", "小游戏", "恶搞","注册表"]
        self.list_items_settings = ["项 1", "项 2", "项 3", "项 4", "项 5", "项 6", "项 7"]

        # 创建“所有”选项卡
        self.tab_all = QWidget()
        self.tab_widget.addTab(self.tab_all, "所有")
        self.setup_all_tab()

        # 创建“设置”选项卡
        self.tab_settings = QWidget()
        self.tab_widget.addTab(self.tab_settings, "设置")
        self.setup_settings_tab()

    def setup_common_tab(self):
        layout = QVBoxLayout()

        # 使用数组中的按钮名称创建按钮
        for button_name in self.common_buttons:
            button = QPushButton(button_name)
            button_number = self.common_buttons.index(button_name) + 1  # 获取按钮编号（从 1 开始）
            button.clicked.connect(lambda _, x=button_number: self.on_common_button_clicked(x))
            layout.addWidget(button)
        self.tab_common.setLayout(layout)

    def setup_all_tab(self):
        layout = QVBoxLayout()

        # 创建列表并添加项（使用数组中的项名称）
        self.list_widget = QListWidget()
        for item_name in self.list_items_all:
            item = QListWidgetItem(item_name)
            if item_name == "小游戏":
                item.setData(Qt.UserRole, "open_new_window_game")
            if item_name == "恶搞":
                item.setData(Qt.UserRole,"open_new_window_joke")
            if item_name == "注册表":
                item.setData(Qt.UserRole,"open_new_window_regedit")
            self.list_widget.addItem(item)

        self.list_widget.itemClicked.connect(self.on_all_list_item_clicked)

        layout.addWidget(self.list_widget)
        self.tab_all.setLayout(layout)

    def setup_settings_tab(self):
        layout = QVBoxLayout()

        # 创建列表并添加项（与“所有”选项卡相同，使用数组中的项名称）
        self.settings_list_widget = QListWidget()
        for item_name in self.list_items_settings:
            # 这里不设置特殊项的数据，因为设置选项卡不需要打开新窗口的功能
            self.settings_list_widget.addItem(item_name)

        self.settings_list_widget.itemClicked.connect(self.on_settings_list_item_clicked)

        layout.addWidget(self.settings_list_widget)
        self.tab_settings.setLayout(layout)

    def on_common_button_clicked(self, button_number):
        if button_number==1:
            os.system(".\SeewoKiller.exe wanzixi")
        if button_number==2:
            os.system(".\SeewoKiller.exe pingbao")
        if button_number==3:
            self.open_new_window_game()
        '''print("123", button_number)'''

    def on_all_list_item_clicked(self, item):
        # 检查项文本或数据以确定是否打开新窗口
        if item.text() == "小游戏" or item.data(Qt.UserRole) == "open_new_window_game":
            self.open_new_window_game()
        if item.text()== "恶搞" or item.data(Qt.UserRole) == "open_new_window_joke":
            self.open_new_window_joke()
        if item.text()== "注册表" or item.data(Qt.UserRole) == "open_new_window_regedit":
            self.open_new_window_regedit()
        if item.text() == "循环清任务":
            '''os.system("SeewoKiller.exe")'''
        if item.text() == "一键卸载":
            os.system(".\SeewoKiller.exe uninstall")
        if item.text() == "冰点解冻":
            os.system(".\SeewoKiller.exe seewofreeze")
        if item.text() == "晚自习制裁模式":
            os.system(".\SeewoKiller.exe wanzixi")
        if item.text() == "一键防屏保":
            os.system(".\SeewoKiller.exe pingbao")
            #print("456", item.text().split(' ')[1])
            print("456")
            # 打印项文本（去掉前面的“项”字和编号后的空格）

    def on_settings_list_item_clicked(self, item):
        # 打印项文本（与“所有”选项卡不同，这里不需要特殊处理）
        print("Settings:", item.text())

    def open_new_window_game(self):
        self.new_window_game = NewWindow_game()
        self.new_window_game.show()
    def open_new_window_joke(self):
        self.new_window_joke = NewWindow_joke()
        self.new_window_joke.show()
    def open_new_window_regedit(self):
        self.new_window_regedit = NewWindow_regedit()
        self.new_window_regedit.show()

class NewWindow_game(QDialog):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("小游戏")
        self.setGeometry(100, 100, 300, 200)

        layout = QVBoxLayout()

        # 新窗口的列表项名称数组
        self.new_list_items = ["数字炸弹", "五子棋"]

        # 创建列表并添加项（使用数组中的项名称）
        self.new_list_widget = QListWidget()
        for item_name in self.new_list_items:
            self.new_list_widget.addItem(item_name)
        self.settings_list_widget.itemClicked.connect(self.on_list_item_clicked)
        layout.addWidget(self.new_list_widget)
        self.setLayout(layout)
    def on_list_item_clicked(self, item):
        if item.text() == "五子棋":
            os.system(".\SeewoKiller.exe pingbao")
            #print("456", item.text().split(' ')[1])
            print("456")
class NewWindow_regedit(QDialog):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("注册表")
        self.setGeometry(100, 100, 300, 200)

        layout = QVBoxLayout()

        # 新窗口的列表项名称数组
        self.new_list_items = ["禁用任务栏菜单", "启用任务栏菜单", "禁用快捷键", "启用快捷键", "启用显示登录详细信息", "禁用显示登录详细信息","登录时显示提示","取消登录时显示提示"]

        # 创建列表并添加项（使用数组中的项名称）
        self.new_list_widget = QListWidget()
        for item_name in self.new_list_items:
            self.new_list_widget.addItem(item_name)

        layout.addWidget(self.new_list_widget)
        self.setLayout(layout)
class NewWindow_joke(QDialog):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("恶搞")
        self.setGeometry(100, 100, 300, 200)

        layout = QVBoxLayout()

        # 新窗口的列表项名称数组
        self.new_list_items = ["杀死所有有用的进程"]

        # 创建列表并添加项（使用数组中的项名称）
        self.new_list_widget = QListWidget()
        for item_name in self.new_list_items:
            self.new_list_widget.addItem(item_name)

        layout.addWidget(self.new_list_widget)
        self.setLayout(layout)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
#pyinstaller -F -w -i SeewoKiller.ico gui.py