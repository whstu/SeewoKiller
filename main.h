#ifndef MAIN_H
#define MAIN_H

#include <bits/stdc++.h>
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <string>
//任务栏进度条
#define INITGUID
#include <shobjidl.h>
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Ole32.lib")
inline const GUID IID_ITaskbarList3 = {// 手动定义IID_ITaskbarList3
	0xea1afb91, 0x9e28, 0x4b86,
{0x90, 0xe9, 0x9e, 0x9f, 0x8a, 0x5e, 0xef, 0xaf}
};
//重启explorer.exe
#include <tlhelp32.h>
//注册表修改
#include <tchar.h>
//游戏
#define N 25
#define Forij(x) for(int i=1;i<=x;i++)for(int j=1;j<=x;j++)
//获取管理员权限所需
#include <tchar.h>
#include <shellapi.h>
//cin错误去除
#include <limits>
//文件控制
#include <fstream>

#define S(i) Sleep(i)
#define cls system("cls");
#define ei else if
typedef long long LL;

inline int box = 1/*板块*/, boxn = 4/*板块总数*/;
inline struct Word {
	std::string box[5]={"NULL", "常用", "核心功能", "附加功能", "设置"};
	int recentn = 4;
	std::string recent[5] = {"NULL", "一键解希沃锁屏", "晚自习制裁模式", "连点器(可防屏保)", "小游戏>>>"};
	int alln = 8;
	std::string all[9] = {"NULL", "循环清任务(上课防屏保)", "一键卸载", "晚自习制裁模式", "连点器(可防屏保)", "一键解希沃锁屏", "小游戏>>>", "恶搞>>>", "注册表>>>"};
	int moren = 3;
	std::string more[4] = {"NULL", "冰点还原破解", "AI", "计算π"};
	int settingn = 11;
	std::string setting[13] = {"NULL", "退出", "在晚自习制裁/循环清任务时启用日志", "打开日志文件夹", "允许使用“关闭”按钮", "启动设置", "冰点还原疑难解答", "启动初学者引导", "命令行帮助", "使用新版界面", "重启到fastboot(真的fast!)", "关于", "开发者选项>>>"};
	int gamen = 5;
	std::string game[6] = {"NULL", "返回", "数字炸弹", "五子棋", "飞机大战", "恶魔轮盘赌"};
	int joken = 3;
	std::string joke[4] = {"NULL", "返回", "杀WPS+希沃白板+希沃视频展台", "提取U盘文件"};
	int regn = 9;
	std::string reg[10] = {"NULL", "返回", "禁用任务栏菜单", "启用任务栏菜单", "禁用快捷键", "启用快捷键", "启用显示登录详细信息", "禁用显示登录详细信息", "登录时显示提示", "取消登录时显示提示"};
	int devn = 3;
	std::string dev[4] = {"NULL", "返回", "释放进度条COM接口", "关闭开发者模式"};
} word;

inline struct DefaultWord {
	const int settingn = 11;
	const std::string setting[13] = {"NULL", "退出", "在晚自习制裁/循环清任务时启用日志", "打开日志文件夹", "允许使用“关闭”按钮", "启动设置", "冰点还原疑难解答", "启动初学者引导", "命令行帮助", "使用新版界面", "重启到fastboot(真的fast!)", "关于", "开发者选项>>>"};
	const int devn = 3;
	const std::string dev[4] = {"NULL", "返回", "释放进度条COM接口", "关闭开发者模式"};
} def_word;

#endif
