import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QTabWidget, QWidget, QPushButton, QVBoxLayout, QListWidget, QListWidgetItem, QDialog
from PyQt5.QtCore import Qt

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)

import os

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("希沃克星")
        self.setGeometry(100, 100, 600, 400)

        # 创建选项卡组件
        self.tab_widget = QTabWidget()
        self.setCentralWidget(self.tab_widget)

        # 按钮名称数组
        self.common_buttons = ["晚自习制裁模式", "按钮 2", "按钮 3"]
        # 创建“常用”选项卡
        self.tab_common = QWidget()
        self.tab_widget.addTab(self.tab_common, "常用")
        self.setup_common_tab()

        # 列表项名称数组（所有和设置选项卡共用）
        self.list_items_all = ["项 1", "项 2", "项 3", "项 4", "项 5", "项 6", "打开新窗口（项 7）"]
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
            if item_name == "打开新窗口（项 7）":
                item.setData(Qt.UserRole, "open_new_window")
            self.list_widget.addItem(item)

        self.list_widget.itemClicked.connect(self.on_list_item_clicked)

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
        print("123", button_number)

    def on_list_item_clicked(self, item):
        # 检查项文本或数据以确定是否打开新窗口
        if item.text() == "打开新窗口（项 7）" or item.data(Qt.UserRole) == "open_new_window":
            self.open_new_window()
        else:
            # 打印项文本（去掉前面的“项”字和编号后的空格）
            print("456", item.text().split(' ')[1])

    def on_settings_list_item_clicked(self, item):
        # 打印项文本（与“所有”选项卡不同，这里不需要特殊处理）
        print("Settings:", item.text())

    def open_new_window(self):
        new_window = NewWindow()
        new_window.show()

class NewWindow(QDialog):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("注册表")
        self.setGeometry(100, 100, 300, 200)

        layout = QVBoxLayout()

        # 新窗口的列表项名称数组
        self.new_list_items = ["新项 1", "新项 2", "新项 3", "新项 4", "新项 5", "新项 6"]

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