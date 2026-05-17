#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
SeewoKiller GUI - CustomTkinter 版本
新增功能：
- 完整的外观板块：深浅色模式、主题色切换
- 字体设置：字体族 (微软雅黑/宋体) + 全局字体缩放比例 (0.8~1.5)
- 所有控件（包括帮助弹窗中的文本框）均应用缩放字体
- 设置页面顶部横幅图片自适应宽度
- 两种启动画面（文字版、图片版）
"""

import os
import sys
import subprocess
import threading
from pathlib import Path

import customtkinter
import customtkinter as ctk
from tkinter import messagebox
from PIL import Image  # 需要安装 Pillow 库

# ============================================================
# 全局外观配置 (将从配置文件读取)
# ============================================================
ctk.set_appearance_mode("System")
ctk.set_default_color_theme("blue")

# ============================================================
# 界面文字资源
# ============================================================
TEXT = {
    "window_title": "希沃克星 - SeewoKiller",
    "dev_title_suffix": " - 开发者模式已开启",
    "nav": {
        "常用": "🏠 常用",
        "核心功能": "⚙️ 核心功能",
        "附加功能": "🎁 附加功能",
        "设置": "⚙️ 设置",
    },
    "section_title": {
        "常用": "常用功能",
        "核心功能": "核心功能",
        "附加功能": "附加功能",
        "设置": "设置",
    },
    "card_desc": {
        "一键解希沃锁屏": "终止希沃屏幕锁定进程",
        "晚自习制裁模式": "强力终止监控进程，防止被控屏",
        "连点器 (可防屏保)": "自动点击，防止屏幕保护",
        "小游戏>>>": "迷你游戏合集",
        "循环清任务 (上课防屏保)": "循环终止希沃进程，防屏保",
        "一键卸载": "卸载希沃相关软件",
        "录制视频": "屏幕录制 (需支持)",
        "恶搞>>>": "整蛊工具子菜单",
        "注册表>>>": "注册表修改子菜单",
        "冰点还原破解": "破解冰点还原软件",
        "AI": "内置 AI 助手 (需 ai.exe)",
        "计算π": "计算圆周率",
    },
    "submenu_desc": {
        "数字炸弹": "猜数字游戏",
        "五子棋": "双人五子棋",
        "飞机大战": "像素飞机射击",
        "恶魔轮盘赌": "运气对决",
        "杀 WPS+希沃白板 + 希沃视频展台": "一键终止教学软件",
        "提取 U 盘文件": "自动复制 U 盘内容",
        "禁用任务栏菜单": "禁用右键任务栏菜单",
        "启用任务栏菜单": "恢复任务栏菜单",
        "禁用快捷键": "禁用 Win 键等快捷键",
        "启用快捷键": "恢复系统快捷键",
        "启用显示登录详细信息": "登录界面显示详细信息",
        "禁用显示登录详细信息": "恢复默认登录界面",
        "登录时显示提示": "登录前显示自定义提示",
        "取消登录时显示提示": "移除登录前的提示",
    },
    "settings": {
        "常规组": "常规",
        "启动模式": "启动模式：",
        "启动模式_选项": ["总是询问", "总是新 UI", "总是旧 UI"],
        "日志开关": "在晚自习制裁/循环清任务时启用日志",
        "关闭按钮开关": "允许使用「关闭」按钮",
        "操作组": "操作",
        "打开日志文件夹": "打开日志文件夹",
        "高级组": "高级",
        "冰点还原疑难解答": "冰点还原疑难解答",
        "命令行帮助": "命令行帮助",
        "重启 fastboot": "重启到 fastboot 模式",
        "关于组": "关于",
        "关于文本": "SeewoKiller 2.1 (Expelliarmus)\nby WHSTU\n卓然第三帝国 https://whstu.dpdns.org/\n代码仓库: https://github.com/whstu/SeewoKiller/\nQQ群: 664929698\n新版界面基于 CustomTkinter",
        "开发者组": "开发者选项",
        "释放 COM 接口": "释放进度条 COM 接口",
        "关闭开发者模式": "关闭开发者模式",
        "冰点帮助消息": "请尝试执行以下操作：\n\n1. 打开 SeewoFreeze 文件夹属性\n2. 打开“安全”\n3. 点击“高级”\n4. 禁用继承\n5. 删除所有继承权限\n6. 添加当前用户完全控制权限\n7. 应用于子文件和子文件夹",
        "命令行帮助消息": "常用命令示例:\n{exe} wanzixi          # 晚自习制裁模式\n{exe} taskkill         # 循环清任务\n{exe} liandianqi       # 连点器\n{exe} seewofreeze      # 冰点还原破解\n{exe} uninstall        # 一键卸载\n{exe} game -wzq        # 五子棋\n{exe} joke -killapp    # 恶搞杀进程\n{exe} regedit -NoWinKeys true   # 禁用快捷键",
        "日志文件夹不存在": "日志文件夹尚不存在，请先执行需要记录日志的功能",
        "重启确认": "将以 fastboot 模式启动 SeewoKiller，当前 GUI 将关闭。确认吗？",
        "重启提示": "已启动 fastboot 模式",
        "COM 接口提示": "COM 接口释放操作已完成（模拟）",
        "开发者模式开启": "开发者模式已开启，请重新进入设置页查看开发者选项。",
        "开发者模式关闭": "开发者模式已关闭，设置页将恢复正常。",
        # 外观板块文本
        "外观组": "外观",
        "深色模式": "深色模式",
        "主题色": "主题色",
        "外观_浅色": "浅色",
        "外观_深色": "深色",
        "外观_跟随系统": "跟随系统",
        "主题色_blue": "蓝色 (Blue)",
        "主题色_dark-blue": "深蓝 (Dark Blue)",
        "主题色_green": "绿色 (Green)",
        # 字体设置
        "字体组": "字体",
        "字体设置": "字体族",
        "字体缩放": "字体缩放比例",
        "字体信息": "当前字体：{font}   缩放：{scale:.2f}\n重启应用后完全生效",
        "字体_微软雅黑": "微软雅黑",
        "字体_黑体": "黑体",
        "字体_system":"system",
    },
    "error_no_exe": "找不到 {exe}\n请确保 SeewoKiller.exe 与本程序在同一目录",
    "error_exec_failed": "执行失败：{error}",
    "info_started": "已启动：{name}",
    "info_unconfigured": "「{name}」尚未配置执行参数",
    "cmd_help_title": "命令行帮助",
    "freeze_help_title": "冰点还原疑难解答",
    "dev_mode_title": "开发者模式",
    "com_interface_title": "COM 接口",
}

# ============================================================
# 功能数据结构定义
# ============================================================
SECTIONS = ["常用", "核心功能", "附加功能", "设置"]

SECTION_ITEMS = {
    "常用": [
        "一键解希沃锁屏",
        "晚自习制裁模式",
        "连点器 (可防屏保)",
        "小游戏>>>",
    ],
    "核心功能": [
        "循环清任务 (上课防屏保)",
        "一键卸载",
        "录制视频",
        "恶搞>>>",
        "注册表>>>",
    ],
    "附加功能": [
        "冰点还原破解",
        "AI",
        "计算π",
    ],
}

SUB_MENUS = {
    "小游戏>>>": {
        "显示标题": "小游戏",
        "items": {
            "数字炸弹":   ["game", "numberdamn"],
            "五子棋":     ["game", "wzq"],
            "飞机大战":   ["game", "fjdz"],
            "恶魔轮盘赌": ["game", "emlpd"],
        }
    },
    "恶搞>>>": {
        "显示标题": "恶搞",
        "items": {
            "杀 WPS+希沃白板 + 希沃视频展台": ["joke", "-killapp"],
            "提取 U 盘文件":                ["joke", "-copyfile"],
        }
    },
    "注册表>>>": {
        "显示标题": "注册表",
        "items": {
            "禁用任务栏菜单":     ["regedit", "-NoTrayContextMenu", "true"],
            "启用任务栏菜单":     ["regedit", "-NoTrayContextMenu", "false"],
            "禁用快捷键":         ["regedit", "-NoWinKeys", "true"],
            "启用快捷键":         ["regedit", "-NoWinKeys", "false"],
            "启用显示登录详细信息": ["regedit", "-VerboseStatus", "true"],
            "禁用显示登录详细信息": ["regedit", "-VerboseStatus", "false"],
            "登录时显示提示":       ["regedit", "-legalnotice", "true"],
            "取消登录时显示提示":   ["regedit", "-legalnotice", "false"],
        }
    }
}

ACTION_ARGS = {
    "一键解希沃锁屏":       ["taskkill"],
    "晚自习制裁模式":       ["wanzixi"],
    "连点器 (可防屏保)":   ["liandianqi"],
    "循环清任务 (上课防屏保)": ["taskkill"],
    "一键卸载":            ["uninstall"],
    "录制视频":            ["camrec"],
    "冰点还原破解":        ["seewofreeze"],
    "AI":                 ["ai"],
    "计算π":              ["pi"],
}

# ============================================================
# 配置文件管理 (增加主题色和字体配置)
# ============================================================
SETTINGS_DIR = Path(".") / "settings"

class ConfigManager:
    @staticmethod
    def ensure_dir():
        SETTINGS_DIR.mkdir(exist_ok=True)

    @staticmethod
    def read_config(path: str) -> str:
        try:
            with open(path, "r", encoding="utf-8") as f:
                return f.readline().strip()
        except Exception:
            return "NULL"

    @staticmethod
    def write_config(path: str, config: str):
        ConfigManager.ensure_dir()
        with open(path, "w", encoding="utf-8") as f:
            f.write(config)

    @staticmethod
    def check_config_available(path: str, valid_values: list, default: str):
        value = ConfigManager.read_config(path)
        if value not in valid_values:
            ConfigManager.write_config(path, default)

    @staticmethod
    def read_bool_config(path: str, default: str = "false") -> bool:
        val = ConfigManager.read_config(path)
        return val == "true" if val in ("true", "false") else (default == "true")

    @staticmethod
    def read_float_config(path: str, default: float = 1.0) -> float:
        """读取浮点配置，无效时返回默认值"""
        try:
            val = ConfigManager.read_config(path)
            return float(val)
        except Exception:
            return default

    @staticmethod
    def write_float_config(path: str, value: float):
        ConfigManager.write_config(path, f"{value:.2f}")

# ============================================================
# 字体管理 (全局缩放 + 字体族)
# ============================================================
def load_font_settings():
    """加载字体族和缩放比例，返回 (family, scale)"""
    font_file = SETTINGS_DIR / "font.seewokiller"
    valid_fonts = ["微软雅黑", "黑体","system"]
    ConfigManager.check_config_available(str(font_file), valid_fonts, "微软雅黑")
    family = ConfigManager.read_config(str(font_file))

    scale_file = SETTINGS_DIR / "font_scale.seewokiller"
    # 有效范围 0.8 ~ 1.5
    scale = ConfigManager.read_float_config(str(scale_file), 1.2)
    scale = max(0.5, min(2, scale))
    return family, scale

# 全局字体变量
GLOBAL_FONT_FAMILY, GLOBAL_FONT_SCALE = load_font_settings()

def scaled_font(base_size: int, weight: str = "normal") -> ctk.CTkFont:
    """返回应用了全局字体族和缩放比例的 CTkFont"""
    actual_size = int(base_size * GLOBAL_FONT_SCALE)
    return ctk.CTkFont(family=GLOBAL_FONT_FAMILY, size=actual_size, weight=weight)

# 预设基础尺寸（像素）
FONT_SIZES = {
    "title": 22,      # 页面大标题
    "subtitle": 16,   # 分组标题
    "body": 12,       # 普通文本
    "card_title": 14, # 卡片标题
    "card_desc": 11,  # 卡片描述
    "button": 12,     # 按钮文字
    "help": 12,       # 帮助文本框
}

def get_scaled_font(role: str, weight: str = "normal") -> ctk.CTkFont:
    """通过角色名称获取缩放字体"""
    base = FONT_SIZES.get(role, 12)
    return scaled_font(base, weight)

# ============================================================
# 外观和主题加载
# ============================================================
def load_appearance_settings():
    """加载外观设置并应用到全局"""
    # 深浅色模式
    appearance_file = SETTINGS_DIR / "appearance.seewokiller"
    valid_modes = ["System", "Light", "Dark"]
    ConfigManager.check_config_available(str(appearance_file), valid_modes, "System")
    mode = ConfigManager.read_config(str(appearance_file))
    ctk.set_appearance_mode(mode)

    # 主题色 - 使用完整主题色系统
    theme_file = SETTINGS_DIR / "theme.seewokiller"
    valid_themes = ["blue", "dark-blue", "green"]
    ConfigManager.check_config_available(str(theme_file), valid_themes, "blue")
    theme = ConfigManager.read_config(str(theme_file))
    ctk.set_default_color_theme(theme)

# ============================================================
# 命令执行器
# ============================================================
EXE_PATH = Path(".") / "SeewoKiller.exe"

def run_seewokiller(args: list, parent_widget=None, success_name=None):
    if not EXE_PATH.is_file():
        messagebox.showerror("错误", TEXT["error_no_exe"].format(exe=EXE_PATH))
        return

    def target():
        try:
            subprocess.Popen([str(EXE_PATH)] + args, creationflags=subprocess.CREATE_NEW_CONSOLE)
            if success_name and parent_widget:
                parent_widget.after(0, lambda: messagebox.showinfo("提示", TEXT["info_started"].format(name=success_name)))
        except Exception as e:
            if parent_widget:
                parent_widget.after(0, lambda: messagebox.showerror("错误", TEXT["error_exec_failed"].format(error=e)))

    threading.Thread(target=target, daemon=True).start()

# ============================================================
# 帮助弹窗 (可复制文本框，字体随全局缩放)
# ============================================================
class HelpPopup(ctk.CTkToplevel):
    def __init__(self, title: str, content: str):
        super().__init__()
        self.title(title)
        self.geometry("500x400")
        self.resizable(True, True)
        self.grab_set()

        # 使用缩放后的字体
        self.textbox = ctk.CTkTextbox(self, wrap="word", font=get_scaled_font("help"))
        self.textbox.pack(fill="both", expand=True, padx=10, pady=10)
        self.textbox.insert("0.0", content)
        self.textbox.configure(state="normal")

        btn_frame = ctk.CTkFrame(self, fg_color="transparent")
        btn_frame.pack(fill="x", padx=10, pady=(0, 10))
        close_btn = ctk.CTkButton(btn_frame, text="关闭", command=self.destroy, width=100, font=get_scaled_font("button"))
        close_btn.pack(side="right", padx=5)
        copy_btn = ctk.CTkButton(btn_frame, text="复制到剪贴板", command=self._copy_to_clipboard, width=120, font=get_scaled_font("button"))
        copy_btn.pack(side="right", padx=5)

    def _copy_to_clipboard(self):
        self.clipboard_clear()
        self.clipboard_append(self.textbox.get("0.0", "end-1c"))
        messagebox.showinfo("提示", "已复制到剪贴板", parent=self)

# ============================================================
# 启动画面 (SplashScreen)
# ============================================================
class SplashScreen:
    """文字版启动画面 - 居中显示"""
    def __init__(self, text="正在加载...", duration=1500):
        self.root = ctk.CTk()
        self.root.overrideredirect(True)
        self.root.configure(fg_color=("#1E1E1E", "#2D2D2D"))

        screen_width = self.root.winfo_screenwidth()
        screen_height = self.root.winfo_screenheight()
        width, height = 400, 200
        x = (screen_width - width) // 2
        y = (screen_height - height) // 2
        self.root.geometry(f"{width}x{height}+{x}+{y}")

        self.label = ctk.CTkLabel(
            self.root,
            text=text,
            font=customtkinter.CTkFont(family="Tengwar Annatar",size=50),
            text_color=("white", "white")
        )
        self.label.pack(expand=True)

        self.progress = ctk.CTkProgressBar(self.root, width=300)
        self.progress.pack(pady=(0, 30))
        self.progress.set(0)

        self.root.after(duration, self.close)

    def run(self):
        self.root.mainloop()

    def close(self):
        self.root.destroy()

class ImageSplashScreen:
    """图片版启动画面 - 显示图片"""
    def __init__(self, image_path, duration=1500):
        self.root = ctk.CTk()
        self.root.overrideredirect(True)
        self.root.configure(fg_color=("#1E1E1E", "#2D2D2D"))

        screen_width = self.root.winfo_screenwidth()
        screen_height = self.root.winfo_screenheight()

        if os.path.exists(image_path):
            try:
                pil_image = Image.open(image_path)
                max_width, max_height = 600, 400
                img_width, img_height = pil_image.size
                if img_width > max_width or img_height > max_height:
                    ratio = min(max_width / img_width, max_height / img_height)
                    new_width = int(img_width * ratio)
                    new_height = int(img_height * ratio)
                    pil_image = pil_image.resize((new_width, new_height), Image.Resampling.LANCZOS)
                    img_width, img_height = new_width, new_height
                self.ctk_image = ctk.CTkImage(light_image=pil_image, dark_image=pil_image, size=(img_width, img_height))
                width, height = img_width + 40, img_height + 80
            except Exception as e:
                print(f"图片加载失败：{e}")
                width, height = 400, 200
                self.ctk_image = None
        else:
            print(f"图片不存在：{image_path}")
            width, height = 400, 200
            self.ctk_image = None

        x = (screen_width - width) // 2
        y = (screen_height - height) // 2
        self.root.geometry(f"{width}x{height}+{x}+{y}")

        if self.ctk_image:
            image_label = ctk.CTkLabel(self.root, image=self.ctk_image, text="")
            image_label.pack(expand=True, padx=20, pady=(20, 10))
        else:
            label = ctk.CTkLabel(
                self.root,
                text="SeewoKiller",
                font=scaled_font(28, "bold")
            )
            label.pack(expand=True)

        loading_label = ctk.CTkLabel(
            self.root,
            text="正在加载...",
            font=scaled_font(12),
            text_color=("gray70", "gray70")
        )
        loading_label.pack(pady=(0, 20))

        self.root.after(duration, self.close)

    def run(self):
        self.root.mainloop()

    def close(self):
        self.root.destroy()

# ============================================================
# 主应用程序
# ============================================================
class SeewoKillerApp(ctk.CTk):
    def __init__(self):
        # 先加载外观和字体设置
        load_appearance_settings()
        global GLOBAL_FONT_FAMILY, GLOBAL_FONT_SCALE
        GLOBAL_FONT_FAMILY, GLOBAL_FONT_SCALE = load_font_settings()

        super().__init__()
        self.title(TEXT["window_title"])
        self.geometry("900x600")
        self.minsize(800, 500)

        self.dev_mode = False
        self.about_click_count = 0
        self.current_section = "常用"

        # 横幅图片 (CTkImage)
        self.banner_image = None
        self.banner_label = None
        self._load_banner_image()

        self._create_layout()
        self._init_configs()
        self._show_section("常用")

    def _load_banner_image(self):
        banner_path = Path(".") / "SeewoKiller.png"
        if banner_path.exists():
            try:
                pil_image = Image.open(banner_path)
                self.banner_image = ctk.CTkImage(light_image=pil_image, dark_image=pil_image, size=(1, 1))
            except Exception as e:
                print(f"横幅图片加载失败：{e}")
                self.banner_image = None
        else:
            print(f"横幅图片不存在：{banner_path}")
            self.banner_image = None

    def _update_banner_size(self, event=None):
        if self.banner_image and self.banner_label:
            width = self.content_frame.winfo_width() - 40
            if width > 10:
                pil_image = self.banner_image._light_image
                if pil_image:
                    orig_width, orig_height = pil_image.size
                    new_height = int(orig_height * width / orig_width)
                    self.banner_image.configure(size=(width, new_height))
                    self.banner_label.configure(image=self.banner_image)

    # ---------------------------- 布局 ----------------------------
    def _create_layout(self):
        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=0)
        self.grid_columnconfigure(1, weight=1)

        self.nav_frame = ctk.CTkFrame(self, width=180, corner_radius=0)
        self.nav_frame.grid(row=0, column=0, sticky="nsew")
        self.nav_frame.grid_propagate(False)

        self.content_frame = ctk.CTkScrollableFrame(self, corner_radius=0)
        self.content_frame.grid(row=0, column=1, sticky="nsew")
        self.content_frame.grid_columnconfigure(0, weight=1)
        self.content_frame.grid_columnconfigure(1, weight=1)

        self.bind("<Configure>", self._on_window_resize)

        self.nav_buttons = {}
        for section in SECTIONS:
            btn = ctk.CTkButton(
                self.nav_frame,
                text=TEXT["nav"][section],
                command=lambda s=section: self._show_section(s),
                corner_radius=8,
                height=40,
                anchor="w",
                fg_color="transparent",
                text_color=("gray10", "gray90"),
                hover_color=("gray80", "gray30"),
                font=get_scaled_font("button")
            )
            btn.pack(fill="x", padx=10, pady=5)
            self.nav_buttons[section] = btn

        ctk.CTkLabel(self.nav_frame, text="").pack(expand=True)

    def _on_window_resize(self, event):
        if event.widget == self:
            self._update_banner_size()

    def _clear_content(self):
        for widget in self.content_frame.winfo_children():
            widget.destroy()

    # ---------------------------- 配置 ----------------------------
    def _init_configs(self):
        ConfigManager.ensure_dir()
        ConfigManager.check_config_available(
            str(SETTINGS_DIR / "start.seewokiller"),
            TEXT["settings"]["启动模式_选项"],
            "总是询问"
        )
        ConfigManager.check_config_available(
            str(SETTINGS_DIR / "write-log-when-killapp.seewokiller"),
            ["true", "false"], "false"
        )
        ConfigManager.check_config_available(
            str(SETTINGS_DIR / "enable-close-window-button.seewokiller"),
            ["true", "false"], "false"
        )

    # ---------------------------- 页面切换 ----------------------------
    def _show_section(self, section_name: str):
        self._clear_content()
        self.current_section = section_name

        for sec, btn in self.nav_buttons.items():
            if sec == section_name:
                btn.configure(fg_color=("gray70", "gray40"), text_color=("white", "white"))
            else:
                btn.configure(fg_color="transparent", text_color=("gray10", "gray90"))

        if section_name == "设置":
            self._build_settings_page()
        else:
            self._build_section_page(section_name)

    def _build_section_page(self, section_name: str):
        row_start = 0
        if self.banner_image and self.banner_label is None:
            self.banner_label = ctk.CTkLabel(self.content_frame, text="", image=self.banner_image)
            self.banner_label.grid(row=0, column=0, columnspan=2, sticky="ew", padx=20, pady=(10, 5))
            self.after(100, self._update_banner_size)
            row_start = 1

        title = ctk.CTkLabel(
            self.content_frame,
            text=TEXT["section_title"][section_name],
            font=get_scaled_font("title", "bold")
        )
        title.grid(row=row_start, column=0, columnspan=2, sticky="w", padx=20, pady=(20, 10))

        items = SECTION_ITEMS.get(section_name, [])
        row = row_start + 1
        col = 0
        for item in items:
            desc = TEXT["card_desc"].get(item, "")
            if item in SUB_MENUS:
                desc = SUB_MENUS[item]["显示标题"] + " 子菜单"
            card = self._create_function_card(item, desc)
            card.grid(row=row, column=col, padx=15, pady=10, sticky="nsew")
            col += 1
            if col >= 2:
                col = 0
                row += 1
        self.content_frame.grid_rowconfigure(row, weight=1)

    def _create_function_card(self, name: str, desc: str):
        card = ctk.CTkFrame(self.content_frame, corner_radius=12, border_width=1, border_color=("gray70", "gray30"))
        card.grid_columnconfigure(0, weight=1)
        card.bind("<Button-1>", lambda e: self._on_card_click(name))
        card.bind("<Enter>", lambda e: card.configure(border_color=("blue", "lightblue"), fg_color=("gray90", "gray20")))
        card.bind("<Leave>", lambda e: card.configure(border_color=("gray70", "gray30"), fg_color=("gray90", "gray20")))

        title_lbl = ctk.CTkLabel(
            card,
            text=name,
            font=get_scaled_font("card_title", "bold"),
            anchor="w"
        )
        title_lbl.grid(row=0, column=0, sticky="w", padx=15, pady=(15, 5))
        title_lbl.bind("<Button-1>", lambda e: self._on_card_click(name))

        desc_lbl = ctk.CTkLabel(
            card,
            text=desc,
            font=get_scaled_font("card_desc"),
            anchor="w",
            text_color=("gray30", "gray70")
        )
        desc_lbl.grid(row=1, column=0, sticky="w", padx=15, pady=(0, 15))
        desc_lbl.bind("<Button-1>", lambda e: self._on_card_click(name))

        return card

    def _on_card_click(self, name: str):
        if name in SUB_MENUS:
            self._show_submenu(name)
            return
        args = ACTION_ARGS.get(name)
        if args is None:
            messagebox.showinfo("提示", TEXT["info_unconfigured"].format(name=name))
            return
        run_seewokiller(args, self, success_name=name)

    # ---------------------------- 子菜单 ----------------------------
    def _show_submenu(self, parent_name: str):
        sub_info = SUB_MENUS.get(parent_name)
        if not sub_info:
            return

        self._clear_content()

        row_start = 0
        if self.banner_image and self.banner_label is None:
            self.banner_label = ctk.CTkLabel(self.content_frame, text="", image=self.banner_image)
            self.banner_label.grid(row=0, column=0, columnspan=2, sticky="ew", padx=20, pady=(10, 5))
            self.after(100, self._update_banner_size)
            row_start = 1

        top_frame = ctk.CTkFrame(self.content_frame, fg_color="transparent")
        top_frame.grid(row=row_start, column=0, columnspan=2, sticky="ew", padx=20, pady=(20, 10))
        back_btn = ctk.CTkButton(
            top_frame,
            text="← 返回",
            width=80,
            command=self._back_to_section,
            fg_color="transparent",
            text_color=("blue", "lightblue"),
            hover_color=("gray80", "gray30"),
            font=get_scaled_font("button")
        )
        back_btn.pack(side="left")

        title = ctk.CTkLabel(
            top_frame,
            text=sub_info["显示标题"],
            font=get_scaled_font("title", "bold")
        )
        title.pack(side="left", padx=20)

        items = sub_info["items"]
        row = row_start + 1
        col = 0
        for item_name, args in items.items():
            desc = TEXT["submenu_desc"].get(item_name, "")
            card = self._create_submenu_card(item_name, desc, args)
            card.grid(row=row, column=col, padx=15, pady=10, sticky="nsew")
            col += 1
            if col >= 2:
                col = 0
                row += 1
        self.content_frame.grid_rowconfigure(row, weight=1)

    def _create_submenu_card(self, name: str, desc: str, args):
        card = ctk.CTkFrame(self.content_frame, corner_radius=12, border_width=1, border_color=("gray70", "gray30"))
        card.grid_columnconfigure(0, weight=1)
        card.bind("<Button-1>", lambda e: run_seewokiller(args, self, success_name=name))
        card.bind("<Enter>", lambda e: card.configure(border_color=("blue", "lightblue"), fg_color=("gray90", "gray20")))
        card.bind("<Leave>", lambda e: card.configure(border_color=("gray70", "gray30"), fg_color=("gray90", "gray20")))

        title_lbl = ctk.CTkLabel(
            card,
            text=name,
            font=get_scaled_font("card_title", "bold"),
            anchor="w"
        )
        title_lbl.grid(row=0, column=0, sticky="w", padx=15, pady=(15, 5))
        title_lbl.bind("<Button-1>", lambda e: run_seewokiller(args, self, success_name=name))

        desc_lbl = ctk.CTkLabel(
            card,
            text=desc,
            font=get_scaled_font("card_desc"),
            anchor="w",
            text_color=("gray30", "gray70")
        )
        desc_lbl.grid(row=1, column=0, sticky="w", padx=15, pady=(0, 15))
        desc_lbl.bind("<Button-1>", lambda e: run_seewokiller(args, self, success_name=name))

        return card

    def _back_to_section(self):
        self._show_section(self.current_section)

    # ---------------------------- 设置页面 ----------------------------
    def _build_settings_page(self):
        row_num = 0

        if self.banner_image and self.banner_label is None:
            self.banner_label = ctk.CTkLabel(self.content_frame, text="", image=self.banner_image)
            self.banner_label.grid(row=0, column=0, columnspan=2, sticky="ew", padx=20, pady=(10, 5))
            self.after(100, self._update_banner_size)
            row_num = 1

        # 标题
        title = ctk.CTkLabel(
            self.content_frame,
            text=TEXT["section_title"]["设置"],
            font=get_scaled_font("title", "bold")
        )
        title.grid(row=row_num, column=0, columnspan=2, sticky="w", padx=20, pady=(20, 10))
        row_num += 1

        # ---- 外观设置组 ----
        appearance_group = ctk.CTkFrame(self.content_frame, corner_radius=12)
        appearance_group.grid(row=row_num, column=0, columnspan=2, sticky="ew", padx=20, pady=10)
        row_num += 1
        ctk.CTkLabel(
            appearance_group,
            text=TEXT["settings"]["外观组"],
            font=get_scaled_font("subtitle", "bold")
        ).pack(anchor="w", padx=15, pady=(15, 10))

        # 深浅色模式
        mode_frame = ctk.CTkFrame(appearance_group, fg_color="transparent")
        mode_frame.pack(fill="x", padx=15, pady=5)
        ctk.CTkLabel(
            mode_frame,
            text=TEXT["settings"]["深色模式"],
            width=120,
            anchor="w",
            font=get_scaled_font("body")
        ).pack(side="left")
        mode_option = ctk.CTkOptionMenu(
            mode_frame,
            values=["System", "Light", "Dark"],
            command=self._change_appearance_mode,
            width=150,
            font=get_scaled_font("button")
        )
        current_mode = ConfigManager.read_config(str(SETTINGS_DIR / "appearance.seewokiller"))
        if current_mode not in ["System", "Light", "Dark"]:
            current_mode = "System"
        mode_option.set(current_mode)
        mode_option.pack(side="right")

        # 主题色
        theme_frame = ctk.CTkFrame(appearance_group, fg_color="transparent")
        theme_frame.pack(fill="x", padx=15, pady=(5, 15))
        ctk.CTkLabel(
            theme_frame,
            text=TEXT["settings"]["主题色"],
            width=120,
            anchor="w",
            font=get_scaled_font("body")
        ).pack(side="left")
        theme_option = ctk.CTkOptionMenu(
            theme_frame,
            values=["blue", "dark-blue", "green"],
            command=self._change_theme,
            width=150,
            font=get_scaled_font("button")
        )
        current_theme = ConfigManager.read_config(str(SETTINGS_DIR / "theme.seewokiller"))
        if current_theme not in ["blue", "dark-blue", "green"]:
            current_theme = "blue"
        theme_option.set(current_theme)
        theme_option.pack(side="right")

        # ---- 字体设置组 ----
        font_group = ctk.CTkFrame(self.content_frame, corner_radius=12)
        font_group.grid(row=row_num, column=0, columnspan=2, sticky="ew", padx=20, pady=10)
        row_num += 1
        ctk.CTkLabel(
            font_group,
            text=TEXT["settings"]["字体组"],
            font=get_scaled_font("subtitle", "bold")
        ).pack(anchor="w", padx=15, pady=(15, 10))

        # 字体族选择
        family_frame = ctk.CTkFrame(font_group, fg_color="transparent")
        family_frame.pack(fill="x", padx=15, pady=5)
        ctk.CTkLabel(
            family_frame,
            text=TEXT["settings"]["字体设置"],
            width=120,
            anchor="w",
            font=get_scaled_font("body")
        ).pack(side="left")
        font_option = ctk.CTkOptionMenu(
            family_frame,
            values=["微软雅黑", "黑体","system"],
            command=self._change_font_family,
            width=150,
            font=get_scaled_font("button")
        )
        current_font = ConfigManager.read_config(str(SETTINGS_DIR / "font.seewokiller"))
        if current_font not in ["微软雅黑", "黑体","system"]:
            current_font = "微软雅黑"
        font_option.set(current_font)
        font_option.pack(side="right")

        # 字体缩放滑块
        scale_frame = ctk.CTkFrame(font_group, fg_color="transparent")
        scale_frame.pack(fill="x", padx=15, pady=(10, 5))
        ctk.CTkLabel(
            scale_frame,
            text=TEXT["settings"]["字体缩放"],
            width=120,
            anchor="w",
            font=get_scaled_font("body")
        ).pack(side="left")

        current_scale = ConfigManager.read_float_config(str(SETTINGS_DIR / "font_scale.seewokiller"), 1.0)
        current_scale = max(0.8, min(1.5, current_scale))
        scale_var = ctk.DoubleVar(value=current_scale)
        scale_slider = ctk.CTkSlider(
            scale_frame,
            from_=0.8, to=1.5,
            number_of_steps=14,
            variable=scale_var,
            command=self._on_scale_changed,
            width=200
        )
        scale_slider.pack(side="left", padx=(10, 10))
        scale_label = ctk.CTkLabel(
            scale_frame,
            text=f"{current_scale:.2f}",
            width=50,
            font=get_scaled_font("body")
        )
        scale_label.pack(side="left")
        # 保存缩放比例的函数（滑块拖动时实时保存）
        def save_scale(value):
            scale_label.configure(text=f"{float(value):.2f}")
            ConfigManager.write_float_config(str(SETTINGS_DIR / "font_scale.seewokiller"), float(value))
        scale_slider.configure(command=save_scale)

        # 提示信息
        info_label = ctk.CTkLabel(
            font_group,
            text=TEXT["settings"]["字体信息"].format(font=current_font, scale=current_scale),
            font=get_scaled_font("body"),
            text_color=("gray40", "gray60")
        )
        info_label.pack(anchor="w", padx=15, pady=(5, 15))

        # ---- 常规设置组 ----
        group1 = ctk.CTkFrame(self.content_frame, corner_radius=12)
        group1.grid(row=row_num, column=0, columnspan=2, sticky="ew", padx=20, pady=10)
        row_num += 1
        ctk.CTkLabel(
            group1,
            text=TEXT["settings"]["常规组"],
            font=get_scaled_font("subtitle", "bold")
        ).pack(anchor="w", padx=15, pady=(15, 10))

        start_path = SETTINGS_DIR / "start.seewokiller"
        start_frame = ctk.CTkFrame(group1, fg_color="transparent")
        start_frame.pack(fill="x", padx=15, pady=5)
        ctk.CTkLabel(
            start_frame,
            text=TEXT["settings"]["启动模式"],
            width=150,
            anchor="w",
            font=get_scaled_font("body")
        ).pack(side="left")
        start_option = ctk.CTkOptionMenu(
            start_frame,
            values=TEXT["settings"]["启动模式_选项"],
            command=lambda v: ConfigManager.write_config(str(start_path), v),
            width=150,
            font=get_scaled_font("button")
        )
        start_option.set(ConfigManager.read_config(str(start_path)))
        start_option.pack(side="right")

        log_path = SETTINGS_DIR / "write-log-when-killapp.seewokiller"
        log_switch = ctk.CTkSwitch(
            group1,
            text=TEXT["settings"]["日志开关"],
            command=lambda: ConfigManager.write_config(str(log_path), "true" if log_switch.get() else "false"),
            font=get_scaled_font("body")
        )
        log_switch.select() if ConfigManager.read_bool_config(str(log_path)) else log_switch.deselect()
        log_switch.pack(anchor="w", padx=15, pady=5)

        close_path = SETTINGS_DIR / "enable-close-window-button.seewokiller"
        close_switch = ctk.CTkSwitch(
            group1,
            text=TEXT["settings"]["关闭按钮开关"],
            command=lambda: ConfigManager.write_config(str(close_path), "true" if close_switch.get() else "false"),
            font=get_scaled_font("body")
        )
        close_switch.select() if ConfigManager.read_bool_config(str(close_path)) else close_switch.deselect()
        close_switch.pack(anchor="w", padx=15, pady=(5, 15))

        # ---- 操作组 ----
        group2 = ctk.CTkFrame(self.content_frame, corner_radius=12)
        group2.grid(row=row_num, column=0, columnspan=2, sticky="ew", padx=20, pady=10)
        row_num += 1
        ctk.CTkLabel(
            group2,
            text=TEXT["settings"]["操作组"],
            font=get_scaled_font("subtitle", "bold")
        ).pack(anchor="w", padx=15, pady=(15, 10))

        log_btn = ctk.CTkButton(
            group2,
            text=TEXT["settings"]["打开日志文件夹"],
            command=self._open_log_folder,
            width=200,
            font=get_scaled_font("button")
        )
        log_btn.pack(anchor="w", padx=15, pady=(0, 15))

        # ---- 高级组 ----
        group3 = ctk.CTkFrame(self.content_frame, corner_radius=12)
        group3.grid(row=row_num, column=0, columnspan=2, sticky="ew", padx=20, pady=10)
        row_num += 1
        ctk.CTkLabel(
            group3,
            text=TEXT["settings"]["高级组"],
            font=get_scaled_font("subtitle", "bold")
        ).pack(anchor="w", padx=15, pady=(15, 10))

        freeze_btn = ctk.CTkButton(
            group3,
            text=TEXT["settings"]["冰点还原疑难解答"],
            command=self._show_freeze_popup,
            width=200,
            font=get_scaled_font("button")
        )
        freeze_btn.pack(anchor="w", padx=15, pady=5)

        cmd_btn = ctk.CTkButton(
            group3,
            text=TEXT["settings"]["命令行帮助"],
            command=self._show_cmd_popup,
            width=200,
            font=get_scaled_font("button")
        )
        cmd_btn.pack(anchor="w", padx=15, pady=5)

        fastboot_btn = ctk.CTkButton(
            group3,
            text=TEXT["settings"]["重启 fastboot"],
            command=self._restart_fastboot,
            width=200,
            font=get_scaled_font("button")
        )
        fastboot_btn.pack(anchor="w", padx=15, pady=(5, 15))

        # ---- 开发者选项 ----
        if self.dev_mode:
            dev_group = ctk.CTkFrame(self.content_frame, corner_radius=12)
            dev_group.grid(row=row_num, column=0, columnspan=2, sticky="ew", padx=20, pady=10)
            row_num += 1
            ctk.CTkLabel(
                dev_group,
                text=TEXT["settings"]["开发者组"],
                font=get_scaled_font("subtitle", "bold")
            ).pack(anchor="w", padx=15, pady=(15, 10))

            com_btn = ctk.CTkButton(
                dev_group,
                text=TEXT["settings"]["释放 COM 接口"],
                command=self._release_com_interface,
                width=200,
                font=get_scaled_font("button")
            )
            com_btn.pack(anchor="w", padx=15, pady=5)

            close_dev_btn = ctk.CTkButton(
                dev_group,
                text=TEXT["settings"]["关闭开发者模式"],
                command=self._disable_dev_mode,
                width=200,
                font=get_scaled_font("button")
            )
            close_dev_btn.pack(anchor="w", padx=15, pady=(5, 15))

        # ---- 关于组 (彩蛋区) ----
        group4 = ctk.CTkFrame(self.content_frame, corner_radius=12)
        group4.grid(row=row_num, column=0, columnspan=2, sticky="ew", padx=20, pady=10)
        row_num += 1
        ctk.CTkLabel(
            group4,
            text=TEXT["settings"]["关于组"],
            font=get_scaled_font("subtitle", "bold")
        ).pack(anchor="w", padx=15, pady=(15, 10))

        about_lbl = ctk.CTkLabel(
            group4,
            text=TEXT["settings"]["关于文本"],
            justify="left",
            anchor="w",
            font=get_scaled_font("body")
        )
        about_lbl.pack(anchor="w", padx=15, pady=(0, 15))
        about_lbl.bind("<Button-1>", self._on_about_click)

        # 底部留空
        ctk.CTkLabel(self.content_frame, text="", height=20).grid(row=row_num, column=0, columnspan=2)

    # ---------------------------- 外观设置回调 ----------------------------
    def _change_appearance_mode(self, mode: str):
        ctk.set_appearance_mode(mode)
        ConfigManager.write_config(str(SETTINGS_DIR / "appearance.seewokiller"), mode)

    def _change_theme(self, theme: str):
        ctk.set_default_color_theme(theme)
        ConfigManager.write_config(str(SETTINGS_DIR / "theme.seewokiller"), theme)

    def _change_font_family(self, font_family: str):
        ConfigManager.write_config(str(SETTINGS_DIR / "font.seewokiller"), font_family)
        messagebox.showinfo("提示", TEXT["settings"]["字体信息"].format(font=font_family, scale=GLOBAL_FONT_SCALE))

    def _on_scale_changed(self, value):
        # 实时保存，但提示重启生效
        pass  # 实际保存已在 slider 的 command 中完成

    # ---------------------------- 帮助弹窗 ----------------------------
    def _show_freeze_popup(self):
        content = TEXT["settings"]["冰点帮助消息"]
        HelpPopup(TEXT["freeze_help_title"], content)

    def _show_cmd_popup(self):
        exe = os.path.abspath("SeewoKiller.exe")
        content = TEXT["settings"]["命令行帮助消息"].format(exe=exe)
        HelpPopup(TEXT["cmd_help_title"], content)

    # ---------------------------- 其他设置功能 ----------------------------
    def _open_log_folder(self):
        log_dir = Path(".") / "log"
        if log_dir.exists():
            os.startfile(str(log_dir))
        else:
            messagebox.showinfo("提示", TEXT["settings"]["日志文件夹不存在"])

    def _restart_fastboot(self):
        if messagebox.askyesno("重启", TEXT["settings"]["重启确认"]):
            run_seewokiller(["run", "fastboot"], self, success_name=TEXT["settings"]["重启提示"])
            self.quit()

    def _release_com_interface(self):
        messagebox.showinfo(TEXT["com_interface_title"], TEXT["settings"]["COM 接口提示"])

    def _disable_dev_mode(self):
        self.dev_mode = False
        self.about_click_count = 0
        self.title(TEXT["window_title"])
        messagebox.showinfo(TEXT["dev_mode_title"], TEXT["settings"]["开发者模式关闭"])
        self._show_section("设置")

    # ---------------------------- 彩蛋逻辑 ----------------------------
    def _on_about_click(self, event):
        self.about_click_count += 1
        remain = 10 - self.about_click_count
        if remain > 0:
            self.title(f"{TEXT['window_title']} - 再点击 {remain} 次开启开发者模式")
            if remain == 1:
                self.title(f"{TEXT['window_title']} - 最后一次点击开启开发者模式")
        else:
            if not self.dev_mode:
                self.dev_mode = True
                self.title(TEXT["window_title"] + TEXT["dev_title_suffix"])
                messagebox.showinfo(TEXT["dev_mode_title"], TEXT["settings"]["开发者模式开启"])
                self._show_section("设置")

# ============================================================
# 程序入口
# ============================================================
def main():
    if getattr(sys, 'frozen', False):
        os.chdir(os.path.dirname(sys.executable))
    else:
        os.chdir(os.path.dirname(os.path.abspath(__file__)))

    # 可选：显示启动画面 (文字版)
    splash = SplashScreen(text="SeewoKiller", duration=1500)
    splash.run()

    # 图片版启动画面（需要准备 ./splash.png）
    # splash = ImageSplashScreen("./splash.png", duration=1500)
    # splash.run()

    app = SeewoKillerApp()
    app.mainloop()

if __name__ == "__main__":
    main()

#pyinstaller --noconfirm --onedir --windowed --add-data "E:\python\APP\Python3.11\Lib\site-packages\customtkinter;customtkinter/" "./gui-tkinter.py"