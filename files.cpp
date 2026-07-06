#include "./files.h"

//检查文件是否存在
bool fileExist(const string& filename) {
	ifstream file(filename);
	return file.good();
}

int SearchForAddress(const vector<string>& value, const string& goal) {
	for (size_t i = 0; i < value.size(); ++i) {
		if (value[i].find(goal) != string::npos) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

string read_config(string PATH) {
	string value = "NULL";
	if (fileExist(PATH)) {
		ifstream file(PATH);
		getline(file, value);
		file.close();
	}
	return value;
}

void write_config(string PATH, string config) {
	ofstream file(PATH);
	file << config;
	file.close();
	return;
}

//检查配置
void check_config_avaliable(string PATH, string config[], int config_n, string default_config) {
	string config_value = read_config(PATH);
	for (int i = 0; i < config_n; i++) {
		if (config_value == config[i]) {
			return;
		}
	}
	write_config(PATH, default_config);
	return;
}

void change_word(const string& string_class, int address, bool config, const string& PATH, const string& name) {
	string tmp;
	if (string_class == "设置") {
		tmp = def_word.setting[address];
	}
//如果是config，则不会考虑name
	
	if (config == true) {
		tmp = tmp + "-当前: " + read_config(PATH);
		
		if (string_class == "设置") {
			word.setting[address] = tmp;
		}
		return;
	} else {
		tmp = name;
		
		if (string_class == "设置") {
			word.setting[address] = tmp;
		}
		return;
	}
}

void GetSubFolders(const std::string& rootPath, std::vector<std::string>& outFolders) {
	// 清空目标容器
	outFolders.clear();
	
	std::string searchPath = rootPath + "\\*";
	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		std::cerr << "无法打开目录: " << rootPath << " (错误码: " << GetLastError() << ")" << std::endl;
		return;
	}
	do {
		if (strcmp(findData.cFileName, ".") == 0 || 
			strcmp(findData.cFileName, "..") == 0) {
			continue;
		}
		// 只存储目录
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			outFolders.push_back(findData.cFileName);
		}
	} while (FindNextFileA(hFind, &findData));
	FindClose(hFind);
}
void GetFileName(const std::wstring& rootPath, std::vector<std::wstring>& outFiles) {
	outFiles.clear();
	
	std::wstring searchPath = rootPath + L"\\*";
	WIN32_FIND_DATAW findData;
	HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		std::wcerr << "无法打开目录: " << rootPath.c_str() << " (错误码: " << GetLastError() << L")" << std::endl;
		return;
	}
	do {
		if (wcscmp(findData.cFileName, L".") == 0 || 
			wcscmp(findData.cFileName, L"..") == 0) {
			continue;
		}
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			outFiles.push_back(findData.cFileName);
		}
	} while (FindNextFileW(hFind, &findData));
	FindClose(hFind);
}


namespace PLUGIN{
	void ReadPluginList(){
		GetSubFolders("./plugin",plugin.plugin);
		for(const auto& ID:plugin.plugin){
			string PATH=".\\plugin\\"+ID+"\\name.config";
			//cout<<PATH<<endl;
			if(!fileExist(PATH)){
				auto del=find(plugin.plugin.begin(),plugin.plugin.end(),ID);
				if (del!=plugin.plugin.end()){
					plugin.plugin.erase(del);
				}
			}
		}
		plugin.pluginn=plugin.plugin.size();
	}
	void ReadPluginName(){
		for(const auto& ID:plugin.plugin){
			string PATH=".\\plugin\\"+ID+"\\name.config";
			plugin.pluginName.push_back(read_config(PATH));
		}
	}
	void PluginMain(){
		ReadPluginList();
		cout<<"\nFind "<<plugin.pluginn<<" plugin IDs:\n";
		for(const auto& ID:plugin.plugin){
			cout<<"   "<<ID<<"\n";
		}
		ReadPluginName();
		cout<<"\n\nFind "<<plugin.pluginn<<" plugins:\n";
		for(const auto& name:plugin.pluginName){
			cout<<"   "<<name<<"\n";
		}
	}
}
