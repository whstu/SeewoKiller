#ifndef FILES_H
#define FILES_H

#include "./main.h"
using namespace std;
using namespace libconfig;
extern bool fileExist(const string& filename);
extern bool dirExist(const string& path);
extern int SearchForAddress(const vector<string>& value, const string& goal, bool exactMatch = false);
extern string read_config(string PATH);
extern bool read_Lines(const string& PATH, vector<string>& lines);
extern void write_config(string PATH, string config);
extern void check_config_avaliable(string PATH,string config[],int config_n,string default_config);
extern void change_word(vector<string>& StringClass, int address, bool IsConfig, const string& PATH="NULL", const string& name="NULL");
namespace ConfigNext{
	bool read_cfg_file(Config& cfg);
	int WriteConfigValue(Config& cfg, const string& CfgName, auto value);
	void check_cfg_valid(Config& cfg);
}
extern void GetSubFolders(const string& rootPath, vector<string>& outFolders);
extern void GetFileName(const wstring& rootPath, vector<wstring>& outFiles);
string OpenFileDialogModern(const vector<pair<wstring, wstring>>& filters = {{L"所有文件", L"*.*"}},const wstring& defaultExtension = L"",const wstring& title = L"选择文件");
extern void unzip(const string& input, const string& output);
extern string UTF8ToGBK(const string& utf8Str);
extern string GBKToUTF8(const string& gbkStr);

#define PLUGIN_INSTALL 1
#define PLUGIN_UNINSTALL 2
#define PLUGIN_DISABLE 3
#define PLUGIN_ENABLE 4
#define PLUGIN_GETINFO 5
namespace PLUGIN{
	void PluginMain();
	//void PluginSystem(unsigned int OperationType, string str = "NULL");
	void PluginManagerUI();
}

#endif
