#ifndef FILES_H
#define FILES_H

#include "./main.h"
using namespace std;
extern bool fileExist(const string& filename);
extern int SearchForAddress(const vector<string>& value, const string& goal);
extern string read_config(string PATH);
extern void write_config(string PATH, string config);
extern void check_config_avaliable(string PATH,string config[],int config_n,string default_config);
extern void change_word(const string& string_class, int address, bool config = false, const string& PATH = "NULL", const string& name = "NULL");
extern void GetSubFolders(const std::string& rootPath, std::vector<std::string>& outFolders);
extern void GetFileName(const std::wstring& rootPath, std::vector<std::wstring>& outFiles);

namespace PLUGIN{
	void PluginMain();
}

#endif
