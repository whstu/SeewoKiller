#ifndef FILES_H
#define FILES_H

#include "./main.h"
using namespace std;
extern bool fileExist(const string& filename);
extern bool dirExist(const string& path);
extern int SearchForAddress(const vector<string>& value, const string& goal, bool exactMatch = false);
extern string read_config(string PATH);
extern bool read_Lines(const string& PATH, vector<string>& lines);
extern void write_config(string PATH, string config);
extern void check_config_avaliable(string PATH,string config[],int config_n,string default_config);
extern void change_word(vector<string>& StringClass, int address, bool IsConfig, const string& PATH="NULL", const string& name="NULL");
extern void GetSubFolders(const string& rootPath, vector<string>& outFolders);
extern void GetFileName(const wstring& rootPath, vector<wstring>& outFiles);
extern string UTF8ToGBK(const string& utf8Str);
extern string GBKToUTF8(const string& gbkStr);

#define PLUGIN_INSTALL 1
namespace PLUGIN{
	void PluginMain();
	void PluginSystem();
}

#endif
