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

int SearchForAddress(const vector<string>& value, const string& goal, bool exactMatch) {
	for (size_t i = 0; i < value.size(); ++i) {
		if (value[i] == goal) {
			return static_cast<int>(i);
		}
		if (value[i].find(goal) != string::npos and exactMatch == false) {
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
		value=UTF8ToGBK(value);
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
		line=UTF8ToGBK(line);
		lines.push_back(line); // 保留空行（push_back 空字符串）
	}
	file.close();
	return true;
}

void write_config(string PATH, string config) {
	ofstream file(PATH);
	config=GBKToUTF8(config);
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

void GetSubFolders(const string& rootPath, vector<string>& outFolders) {
	// 清空目标容器
	outFolders.clear();

	string searchPath = rootPath + "\\*";
	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		cerr << "无法打开目录: " << rootPath << " (错误码: " << GetLastError() << ")" << endl;
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
void GetFileName(const wstring& rootPath, vector<wstring>& outFiles) {
	outFiles.clear();

	wstring searchPath = rootPath + L"\\*";
	WIN32_FIND_DATAW findData;
	HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		wcerr << "无法打开目录: " << rootPath.c_str() << " (错误码: " << GetLastError() << L")" << endl;
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

string UTF8ToGBK(const string& utf8Str) {
	// 1. UTF-8 -> UTF-16 (宽字符)
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
	wchar_t* wbuf = new wchar_t[wlen];
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, wbuf, wlen);
	
	// 2. UTF-16 -> GBK
	int glen = WideCharToMultiByte(CP_ACP, 0, wbuf, -1, NULL, 0, NULL, NULL);
	char* gbuf = new char[glen];
	WideCharToMultiByte(CP_ACP, 0, wbuf, -1, gbuf, glen, NULL, NULL);
	
	std::string result(gbuf);
	delete[] wbuf;
	delete[] gbuf;
	return result;
}
string GBKToUTF8(const string& gbkStr) {
	// 1. GBK -> UTF-16 (宽字符)
	int wlen = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, NULL, 0);
	wchar_t* wbuf = new wchar_t[wlen];
	MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, wbuf, wlen);
	
	// 2. UTF-16 -> UTF-8
	int ulen = WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, NULL, 0, NULL, NULL);
	char* ubuf = new char[ulen];
	WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, ubuf, ulen, NULL, NULL);
	
	std::string result(ubuf);
	delete[] wbuf;
	delete[] ubuf;
	return result;
}

namespace PLUGIN {
	bool ReadPluginList() {
		plugin.errorpath.clear();
		plugin.plugin.clear();
		plugin.pluginName.clear();
		plugin.pluginExec.clear();
		plugin.pluginList.clear();
		plugin.pluginType.clear();
		plugin.pluginIsCls.clear();

		bool success = true;
		vector<string> allIDs;
		GetSubFolders("./plugin", allIDs);

		for (const auto& ID : allIDs) {
			string basePath = ".\\plugin\\" + ID + "\\";
			string pathName = basePath + "name.config";
			string pathType = basePath + "type.config";
			string pathExec = basePath + "exec.config";
			string pathList = basePath + "list.config";
			string pathIsCls = basePath + "IsCls.config";
			// 临时存储读取的内容
			string nameContent, typeContent, isClsContent;
			vector<string> execContent, listContent;
			execContent.clear();
			listContent.clear();
			bool valid = true;

			// 辅助 lambda：读取文件内容，若文件不存在或内容为空则返回 false
			auto readIfValid = [&](const string & path, string & out, bool allowEmpty = false, bool allowNonexist = false) -> bool {
				if (!fileExist(path) && !allowNonexist) return false;
				out = read_config(path);
				if (!allowEmpty && out.empty()) return false;
				return true;
			};
			auto readIfValidVector = [&](const string & path, vector<string>& out, bool allowEmpty = false, bool allowNonexist = false) -> bool {
				if (!fileExist(path) && !allowNonexist) return false;
				read_Lines(path, out);
				if (!allowEmpty && out.empty()) return false;
				return true;
			};

			gotoxy(15, 18);
			cout << "正在扫描文件夹: .\\plugin\\" << ID << "\\                 ";
			S(10);

			// 检查 name.config
			gotoxy(15, 19);
			cout << "正在扫描文件: " << pathName << "                          ";
			if (!readIfValid(pathName, nameContent)) {
				valid = false;
				plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\  - 缺少文件或内容为空: name.config");
			}
			S(10);

			// 检查 type.config
			gotoxy(15, 19);
			cout << "正在扫描文件: " << pathType << "                          ";
			if (!readIfValid(pathType, typeContent) || (typeContent != "list" && typeContent != "exec")) {
				valid = false;
				plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\  - 缺少文件或类型无效: type.config");
			}
			S(10);

			// 检查 exec.config与list.config
			gotoxy(15, 19);
			cout << "正在扫描文件: " << pathExec << "                          ";
			if (!readIfValidVector(pathExec, execContent, true, false)) {
				valid = false;
				plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\  - 缺少文件或内容为空: exec.config");
			}
			S(10);

			gotoxy(15, 19);
			cout << "正在扫描文件: " << pathList << "                          ";
			if (!readIfValidVector(pathList, listContent, true, true)) {
				//虽然不可能有这种情况
				valid = false;
				plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\  - 缺少文件或内容为空: list.config");
			}
			S(10);

			//检查list与exec
			if (typeContent == "list" and execContent.size() != listContent.size()) {
				plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\  - 功能列表与运行列表没有对齐: exec.config, list.config");
				valid = false;
			}



			// 检查 IsCls.config
			gotoxy(15, 19);
			cout << "正在扫描文件: " << pathIsCls << "                          ";
			if (!readIfValid(pathIsCls, isClsContent, true, true)) {
				isClsContent = "false";   // 默认值
			}
			S(10);

			if (valid) {
				//size_t count=plugin.plugin.size()-1;
				plugin.plugin.push_back(ID);
				plugin.pluginName.push_back(nameContent);
				plugin.pluginType.push_back(typeContent);
				plugin.pluginExec.push_back(execContent);
				plugin.pluginList.push_back(listContent);
				plugin.pluginIsCls.push_back(isClsContent == "true" ? true : false);
			} else {
				success = false;
			}
		}

		//pluginName去重
		unordered_set<string> seen;
		seen.reserve(plugin.pluginName.size());
		for (const auto& s : plugin.pluginName) {
			if (!seen.insert(s).second) {
				plugin.errorpath.push_back("重复的插件名称: " + s);
				success = false;
			}
		}
		// 去重
		sort(plugin.errorpath.begin(), plugin.errorpath.end());
		auto last = unique(plugin.errorpath.begin(), plugin.errorpath.end());
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
		word.more=def_word.more;
		if(plugin.plugin.size()<=1){
			return;
		}
		word.more.insert(word.more.end(), "---插件---");
		word.more.insert(word.more.end(), plugin.pluginName.begin(), plugin.pluginName.end());

		plugin.plugin.insert(plugin.plugin.begin(), "NULL");
		plugin.pluginName.insert(plugin.pluginName.begin(), "NULL");
		for (size_t i = 0; i < plugin.pluginExec.size(); i++) {
			plugin.pluginExec[i].insert(plugin.pluginExec[i].begin(), "NULL");
		}
		plugin.pluginExec.insert(plugin.pluginExec.begin(), {"NULL"});
		for (size_t i = 0; i < plugin.pluginList.size(); i++) {
			plugin.pluginList[i].insert(plugin.pluginList[i].begin(), "NULL");
			plugin.pluginList[i].insert(plugin.pluginList[i].end(), "返回");
		}
		plugin.pluginList.insert(plugin.pluginList.begin(), {"NULL"});
		plugin.pluginType.insert(plugin.pluginType.begin(), "NULL");
		plugin.pluginIsCls.insert(plugin.pluginIsCls.begin(), false);
		/*cout<<"\nFind "<<plugin.pluginn<<" plugin IDs:\n";
		for(const auto& ID:plugin.plugin){
			cout<<"   "<<ID<<"\n";
		}*/
	}
	void PluginSystem(unsigned int OperationType){
	}
}
