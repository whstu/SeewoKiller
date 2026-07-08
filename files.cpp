#include "./files.h"
#include "./cmdCtrl.h"

//检查文件是否存在
bool fileExist(const string& filename) {
	ifstream file(filename);
	return file.good();
}
bool dirExist(const string& path) {
	DWORD attrib = GetFileAttributes(path.c_str());
	return (attrib != INVALID_FILE_ATTRIBUTES &&
	        (attrib & FILE_ATTRIBUTE_DIRECTORY));
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
bool read_Lines(const string& PATH, vector<string>& lines) {
	ifstream file(PATH);
	if (!file.is_open()) {
		return false;          // 打开失败
	}
	string line;
	while (getline(file, line)) {
		lines.push_back(line); // 保留空行（push_back 空字符串）
	}
	file.close();
	return true;
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

void change_word(vector<string>& StringClass, int address, bool IsConfig, const string& PATH, const string& name) {
	string tmp;
	if (IsConfig) {
		tmp = def_word.setting[address];
	} else {
		tmp = StringClass[address];
	}
//如果是config，则不会考虑name
	if (IsConfig == true) {
		tmp = tmp + "-当前: " + read_config(PATH);
		StringClass[address] = tmp;
		return;
	} else {
		tmp = name;
		StringClass[address] = tmp;
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

namespace PLUGIN {
	bool ReadPluginList() {
		plugin.errorpath.clear();
		plugin.plugin.clear();
		plugin.pluginName.clear();
		plugin.pluginExec.clear();
		//plugin.pluginList.clear();
		plugin.pluginType.clear();
		bool success = true;
		GetSubFolders("./plugin", plugin.plugin);
		for (const auto& ID : plugin.plugin) {
			vector<string> checkPATH = {
				".\\plugin\\" + ID + "\\name.config",
				".\\plugin\\" + ID + "\\type.config",
				".\\plugin\\" + ID + "\\exec.config"
			};
			//string PATH=".\\plugin\\"+ID+"\\name.config";
			//cout<<PATH<<endl;
			gotoxy(15, 18);
			cout << "正在扫描文件夹: .\\plugin\\" << ID << "\\                 ";
			S(10);
			for (const auto& PATH : checkPATH) {
				gotoxy(15, 19);
				cout << "正在扫描文件: " << PATH << "                          ";
				if (!fileExist(PATH)) {//检验存在
					auto del = find(plugin.plugin.begin(), plugin.plugin.end(), ID);
					if (del != plugin.plugin.end()) {
						plugin.plugin.erase(del);
						plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\");
						success = false;
					}
				} else { //检验内容
					if (PATH.find("name.config") != string::npos) {
						string content = read_config(PATH);
						if (content != "") {
							plugin.pluginName.push_back(content);
						} else {
							plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\");
							success = false;
							plugin.pluginName.push_back("[插件名称未找到]");
						}
					}
					if (PATH.find("type.config") != string::npos) {
						string content = read_config(PATH);
						if (content == "list" or content == "exec") {
							plugin.pluginType.push_back(content);
						} else {
							plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\");
							success = false;
							plugin.pluginType.push_back("NULL");
						}
					}
					if (PATH.find("exec.config") != string::npos) {
						string content = read_config(PATH);
						if (content != "") {
							plugin.pluginExec.push_back(content);
						} else {
							plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\");
							success = false;
							plugin.pluginExec.push_back("NULL");
						}
					}
				}
				S(10);
			}
		}

		//清除重复
		std::sort(plugin.errorpath.begin(), plugin.errorpath.end());
		auto last = std::unique(plugin.errorpath.begin(), plugin.errorpath.end());
		plugin.errorpath.erase(last, plugin.errorpath.end());
		return success;
	}
	/*void ReadPluginCfg() {
		for (const auto& ID : plugin.plugin) {
			string PATH = ".\\plugin\\" + ID + "\\name.config";
			plugin.pluginName.push_back(read_config(PATH));
		}
	}*/
	void PluginMain() {
		if (ReadPluginList() == false) {
			word.recent.push_back("[*]有插件加载失败>>>");
		}
		plugin.plugin.insert(plugin.plugin.begin(), "NULL");
		plugin.pluginName.insert(plugin.pluginName.begin(), "NULL");
		plugin.pluginExec.insert(plugin.pluginExec.begin(), "NULL");
		plugin.pluginType.insert(plugin.pluginType.begin(), "NULL");
		/*cout<<"\nFind "<<plugin.pluginn<<" plugin IDs:\n";
		for(const auto& ID:plugin.plugin){
			cout<<"   "<<ID<<"\n";
		}*/
		//ReadPluginCfg();
		/*cout<<"\n\nFind "<<plugin.pluginn<<" plugins:\n";
		for(const auto& name:plugin.pluginName){
			cout<<"   "<<name<<"\n";
		}*/
	}
}
