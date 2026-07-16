#ifndef MAIN_H
#define MAIN_H

#include <bits/stdc++.h>
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <string>
#include <curl/curl.h>
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
//std::string EXECPATH;
inline std::string executable_path;

inline int box = 1/*板块*/, boxn = 4/*板块总数*/;
inline struct Word {
	std::vector<std::string> box = {"NULL", "常用", "核心功能", "附加功能", "设置"};
	std::vector<std::string> recent = {"NULL", "一键解希沃锁屏", "晚自习制裁模式", "连点器(可防屏保)", "小游戏>>>"};
	std::vector<std::string> all = {"NULL", "循环清任务(上课防屏保)", "一键卸载", "晚自习制裁模式", "连点器(可防屏保)", "一键解希沃锁屏", "录制视频", "小游戏>>>", "恶搞>>>", "注册表>>>"};
	std::vector<std::string> more = {"NULL", "冰点还原破解", "AI", "计算π"};
	std::vector<std::string> setting = {
		"NULL", "退出",
		"---配置文件---",
		"在晚自习制裁/循环清任务时启用日志",
		"-打开制裁日志文件夹",
		"允许使用“关闭”按钮",
		"启动设置",
		"---帮助---",
		"启动初学者引导",
		"冰点还原疑难解答",
		"命令行帮助",
		"---高级---",
		"使用新版界面",
		"重载插件与配置文件",
		"重启到fastboot(真的fast!)",
		"---其它---",
		"关于"
	};
	std::vector<std::string> game = {"NULL", "返回", "数字炸弹", "五子棋", "飞机大战", "恶魔轮盘赌"};
	std::vector<std::string> joke = {"NULL", "返回", "杀WPS+希沃白板+希沃视频展台", "提取U盘文件"};
	std::vector<std::string> reg = {"NULL", "返回", "禁用任务栏菜单", "启用任务栏菜单", "禁用快捷键", "启用快捷键", "启用显示登录详细信息", "禁用显示登录详细信息", "登录时显示提示", "取消登录时显示提示"};
	std::vector<std::string> dev = {"NULL", "返回", "释放进度条COM接口", "关闭开发者模式"};
} word;

inline struct DefaultWord {
	const std::vector<std::string> more = {"NULL", "冰点还原破解", "AI", "计算π"};
	const std::vector<std::string> setting = {
		"NULL", "退出",
		"---配置文件---",
		"在晚自习制裁/循环清任务时启用日志",
		"-打开制裁日志文件夹",
		"允许使用“关闭”按钮",
		"启动设置",
		"---帮助---",
		"启动初学者引导",
		"冰点还原疑难解答",
		"命令行帮助",
		"---高级---",
		"使用新版界面",
		"重载插件与配置文件",
		"重启到fastboot(真的fast!)",
		"---其它---",
		"关于"
	};
	const std::vector<std::string> dev = {"NULL", "返回", "释放进度条COM接口", "关闭开发者模式"};
} def_word;

inline struct Plugin {
	std::vector<std::string> pluginName;
	std::vector<std::string> plugin;//ID
	std::vector<std::string> pluginType;
	std::vector<std::vector<std::string>> pluginExec;
	//std::vector<std::string> pluginExec;//type=exec
	std::vector<std::vector<std::string>> pluginList;//type=list
	std::vector<bool> pluginIsCls;
	std::vector<std::string> errorpath;
} plugin;

#endif
