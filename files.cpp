#include "./files.h"
#include "./main.h"
#include "./cmdCtrl.h"
#include <shobjidl.h>

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
		value = UTF8ToGBK(value);
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
		line = UTF8ToGBK(line);
		lines.push_back(line); // 保留空行（push_back 空字符串）
	}
	file.close();
	return true;
}

void write_config(string PATH, string config) {
	ofstream file(PATH);
	config = GBKToUTF8(config);
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

namespace ConfigNext {
	bool read_cfg_file(Config& cfg) {
		//构建配置文件路径
		string config_dir = ".\\settings";
		string config_file = config_dir + "\\settings.cfg";
		//检查配置文件是否存在
		if (!fileExist(config_file)) {
			// 创建默认配置
			try {
				// 获取根设置
				Setting& root = cfg.getRoot();
				// for poweron()
				Setting& start = root.add("start", Setting::TypeGroup);
				start.add("StartOption", Setting::TypeString) = "ask";
				start.add("EnableCloseButtonOnConsole", Setting::TypeBoolean) = false;

				// logger
				Setting& logging = root.add("logging", Setting::TypeGroup);
				logging.add("WriteLogWhenKillApps", Setting::TypeBoolean) = true;

				// 写入配置文件
				cfg.writeFile(config_file.c_str());

			} catch (const SettingException& e) {
				gotoxy(15, 18);
				cll;
				cerr << "创建默认配置时发生错误: " << e.what() << endl;
				return false;
			} catch (const FileIOException& e) {
				gotoxy(15, 18);
				cll;
				cerr << "写入配置文件时发生I/O错误" << endl;
				return false;
			}
			return true;
		}

		// 读取现有配置文件
		try {
			cfg.readFile(config_file.c_str());
			gotoxy(15, 18);
			cll;
			cout << "成功读取配置文件: " << config_file << endl;
			return true;
		} catch (const FileIOException& fioex) {
			cls;
			gotoxy(0, 10);
			cerr << "读取配置文件时发生I/O错误: " << config_file << endl;
			cout << "可能为配置文件格式错误\n";
			cout << "按空格键重置配置, 或按其它键退出\n";
			while (true) {
				if (_kbhit()) {
					char ch = getch();
					if (ch == ' ') {
						system("del .\\settings\\settings.cfg");
					}
					exit(1);
				}
			}
			return false;
		} catch (const ParseException& pex) {
			cls;
			gotoxy(0, 10);
			cerr << "解析配置文件错误: " << pex.getFile()
			     << " 第 " << pex.getLine() << " 行 - " << pex.getError() << endl;
			cout << "按空格键重置配置, 或按其它键退出\n";
			while (true) {
				if (_kbhit()) {
					char ch = getch();
					if (ch == ' ') {
						system("del .\\settings\\settings.cfg");
					}
					exit(1);
				}
			}
		}
	}

	void WriteToCfgFile(Config& cfg) {
		string config_file = ".\\settings\\settings.cfg";
		try {
			cfg.writeFile(config_file.c_str());
			//cout << "配置已保存到 " << config_file << endl;
		} catch (const FileIOException& e) {
			cls;
			gotoxy(0, 10);
			cerr << "保存配置文件失败: " << e.what() << endl;
			system("pause");
		}
	}
	int WriteConfigValue(Config& cfg, const string& CfgName, auto value) {
		try {
			cfg.lookup(CfgName) = value;
		} catch (const SettingNotFoundException& e) {
			cerr << "配置项不存在: " << CfgName << endl;
			return 1;
		} catch (const SettingTypeException& e) {
			cerr << "配置项类型错误: " << CfgName << endl;
			return 1;
		}
		// 写回文件
		WriteToCfgFile(cfg);
		return 0;
	}

	namespace ChangeWord {
		void ChangeWord(vector<string>& StringClass, int address, const string& str) {
			StringClass[address] = str;
			return;
		}
		void ChangeConfigWordFromString(vector<string>& StringClass, int address, const string& str) {
			string tmp;
			tmp = def_word.setting[address];
			tmp = tmp + "-当前: " + str;
			StringClass[address] = tmp;
			return;
		}
		void ChangeConfigWordFromConfig(vector<string>& StringClass, int address, const string& CfgPATH) {
			string tmp;
			tmp = def_word.setting[address];
			auto& root = cfg.getRoot();
			auto& config = root.lookup(CfgPATH);
			string cfgtmp;
			if (config.getType() == Setting::TypeBoolean) {
				cfgtmp = config ? "true" : "false";
			} else if (config.getType() == Setting::TypeInt or config.getType() == Setting::TypeInt64) {
				LL number = config;
				cfgtmp = to_string(number);
			} else if (config.getType() == Setting::TypeFloat) {
				float number = config;
				cfgtmp = to_string(number);
			} else {
				string str = config;
				cfgtmp = str;
			}
			tmp = tmp + "-当前: " + cfgtmp;
			StringClass[address] = tmp;
			return;
		}
	}

	void check_cfg_valid(Config& cfg) {
		//poweron--------------------------------------------------------
		Setting& setting = cfg.getRoot();
		gotoxy(15, 18);
		cll;
		cout << "检查配置: start";
		if (!setting.exists("start")) {//check group "start"
			setting.add("start", Setting::TypeGroup);
		}
		if (setting["start"].getType() != Setting::TypeGroup) {
			setting.remove("StartOption");
			setting.add("StartOption", Setting::TypeGroup);
		}
		auto& start = setting["start"];
		S(1);
		while (true) { //StartOption(string)
			gotoxy(15, 18);
			cll;
			cout << "检查配置: start.StartOption";
			//check exist & type
			if (!start.exists("StartOption")) {
				start.add("StartOption", Setting::TypeString);
				WriteConfigValue(cfg, "start.StartOption", "old");
			} else if (start["StartOption"].getType() != Setting::TypeString) {
				start.remove("StartOption");
				start.add("StartOption", Setting::TypeString);
				WriteConfigValue(cfg, "start.StartOption", "old");
			}
			//check valid
			string StartOption = start.lookup("StartOption");
			vector<string> def = {"ask", "old", "new"};
			bool found = false;
			for (int i = 0; i < def.size(); i++) {
				if (StartOption == def[i]) {
					found = true;
					break;
				}
			}
			if (!found) {
				WriteConfigValue(cfg, "start.StartOption", "ask");
				StartOption = "ask";
			}
			if (StartOption == "new") {
				StartOption = "总是新UI";
			} else if (StartOption == "old") {
				StartOption = "总是旧UI";
			} else if (StartOption == "ask") {
				StartOption = "每次询问";
			}
			ChangeWord::ChangeConfigWordFromString(word.setting, SearchForAddress(def_word.setting, "启动设置"), StartOption);
			break;
		}
		S(1);
		//string StartOption = cfg.lookup("start.StartOption");
		//cout << StartOption;
		while (true) {//check start.EnableCloseButton(bool)
			gotoxy(15, 18);
			cll;
			cout << "检查配置: start.EnableCloseButtonOnConsole";
			//check exist & type
			if (!start.exists("EnableCloseButtonOnConsole")) {
				start.add("EnableCloseButtonOnConsole", Setting::TypeBoolean);
				WriteConfigValue(cfg, "start.EnableCloseButtonOnConsole", false);
			} else if (start["EnableCloseButtonOnConsole"].getType() != Setting::TypeBoolean) {
				start.remove("EnableCloseButtonOnConsole");
				start.add("EnableCloseButtonOnConsole", Setting::TypeBoolean);
				WriteConfigValue(cfg, "start.EnableCloseButtonOnConsole", false);
			}
			auto& config = start.lookup("EnableCloseButtonOnConsole");
			string value = config ? "true" : "false";
			ChangeWord::ChangeConfigWordFromString(word.setting, SearchForAddress(def_word.setting, "允许使用“关闭”按钮"), value);
			break;
		}
		S(1);


		//logging--------------------------------------------------------
		gotoxy(15, 18);
		ClearLine(true);
		cout << "检查配置: logging";
		if (!setting.exists("logging")) {//check group "start"
			setting.add("logging", Setting::TypeGroup);
		}
		if (setting["logging"].getType() != Setting::TypeGroup) {
			setting.remove("logging");
			setting.add("logging", Setting::TypeGroup);
		}
		auto& logging = setting["logging"];
		S(1);
		while (true) {//check logging.WriteLogWhenKillApps(bool)
			gotoxy(15, 18);
			cll;
			cout << "检查配置: logging.WriteLogWhenKillApps";
			//check exist & type
			if (!logging.exists("WriteLogWhenKillApps")) {
				logging.add("WriteLogWhenKillApps", Setting::TypeBoolean);
				WriteConfigValue(cfg, "logging.WriteLogWhenKillApps", false);
			} else if (logging["WriteLogWhenKillApps"].getType() != Setting::TypeBoolean) {
				logging.remove("WriteLogWhenKillApps");
				logging.add("WriteLogWhenKillApps", Setting::TypeBoolean);
				WriteConfigValue(cfg, "logging.WriteLogWhenKillApps", false);
			}
			auto& config = logging.lookup("WriteLogWhenKillApps");
			string value = config ? "true" : "false";
			ChangeWord::ChangeConfigWordFromString(word.setting, SearchForAddress(def_word.setting, "在晚自习制裁/循环清任务时启用日志"), value);
			break;
		}
		S(1);
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

string OpenFileDialogModern(
    const vector<pair<wstring, wstring>>& filters,
    const wstring& defaultExtension, const wstring& title) {
	CoInitialize(NULL);
	string result;
	IFileOpenDialog* pFileOpen = NULL;
	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL,
	                               CLSCTX_ALL, IID_IFileOpenDialog,
	                               (void * *)&pFileOpen))) {
		// 构建过滤器数组
		vector<COMDLG_FILTERSPEC> filterSpecs;
		filterSpecs.reserve(filters.size());
		for (const auto& [name, spec] : filters) {
			filterSpecs.push_back({name.c_str(), spec.c_str()});
		}
		if (!filterSpecs.empty()) {
			pFileOpen->SetFileTypes(static_cast<UINT>(filterSpecs.size()), filterSpecs.data());
			pFileOpen->SetFileTypeIndex(1);
		}
		if (!defaultExtension.empty()) {
			pFileOpen->SetDefaultExtension(defaultExtension.c_str());
		}
		if (!title.empty()) {
			pFileOpen->SetTitle(title.c_str());
		}
		if (SUCCEEDED(pFileOpen->Show(NULL))) {
			IShellItem* pItem;
			if (SUCCEEDED(pFileOpen->GetResult(&pItem))) {
				PWSTR pszFilePath;
				if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
					int len = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, NULL, 0, NULL, NULL);
					result.resize(len);
					WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, &result[0], len, NULL, NULL);
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}
	CoUninitialize();
	while (!result.empty() && result.back() == '\0') {
		result.pop_back();
	}
	return result;
}

void unzip(string& input, const string& output) {
	string command = ".\\7za.exe x \"" + input + "\" -o\"" + output + "\" -y";
	//cout << command.c_str();
	system(command.c_str());
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
		plugin.pluginIsEnabled.clear();
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
			ClearLine(true);
			cout << "正在扫描文件夹: .\\plugin\\" << ID << "\\                 ";
			S(10);

			// 检查 name.config
			gotoxy(15, 19);
			ClearLine(true);
			cout << "正在扫描文件: " << pathName << "                          ";
			if (!readIfValid(pathName, nameContent)) {
				valid = false;
				plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\  - 缺少文件或内容为空: name.config");
			}
			S(10);

			// 检查 type.config
			gotoxy(15, 19);
			ClearLine(true);
			cout << "正在扫描文件: " << pathType << "                          ";
			if (!readIfValid(pathType, typeContent) || (typeContent != "list" && typeContent != "exec")) {
				valid = false;
				plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\  - 缺少文件或类型无效: type.config");
			}
			S(10);

			// 检查 exec.config与list.config
			gotoxy(15, 19);
			ClearLine(true);
			cout << "正在扫描文件: " << pathExec << "                          ";
			if (!readIfValidVector(pathExec, execContent, true, false)) {
				valid = false;
				plugin.errorpath.push_back(executable_path + "\\plugin\\" + ID + "\\  - 缺少文件或内容为空: exec.config");
			}
			S(10);

			gotoxy(15, 19);
			ClearLine(true);
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
			ClearLine(true);
			cout << "正在扫描文件: " << pathIsCls << "                          ";
			if (!readIfValid(pathIsCls, isClsContent, true, true)) {
				isClsContent = "false";   // 默认值
			}
			S(10);

			if (valid) {
				//size_t count=plugin.plugin.size()-1;
				plugin.pluginIsEnabled.push_back(!fileExist(basePath + ".disabled"));
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
		word.more = def_word.more;
		if (plugin.plugin.empty()) {
			return;
		}
		vector<string> enabledNames;
		for (size_t i = 0; i < plugin.pluginName.size(); ++i) {
			if (plugin.pluginIsEnabled[i]) {
				enabledNames.push_back(plugin.pluginName[i]);
			}
		}
		if (enabledNames.empty()) {
			return;
		}
		word.more.insert(word.more.end(), "---插件---");
		word.more.insert(word.more.end(), enabledNames.begin(), enabledNames.end());

		plugin.plugin.insert(plugin.plugin.begin(), "NULL");
		plugin.pluginName.insert(plugin.pluginName.begin(), "NULL");
		plugin.pluginIsEnabled.insert(plugin.pluginIsEnabled.begin(), false);
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
	void PluginSystem(unsigned int OperationType, string str) {
		switch (OperationType) {
			case PLUGIN_INSTALL: {
				if (!fileExist(str)) {
					cout << "\n错误: 找不到指定的文件。" << str << "\n";
					break;
				}
				cout << "Copying zip file to temp...\n";
				unzip(str, executable_path + "\\temp\\plugin\\");
				string pluginID = read_config(executable_path + "\\temp\\plugin\\id.config");
				if (SearchForAddress(plugin.plugin, pluginID, true) != -1) { //更新
					cout << "\n正在更新: " << plugin.pluginName[SearchForAddress(plugin.plugin, pluginID, true)] << endl;
					if (fileExist(executable_path + "\\temp\\plugin\\version.config") or fileExist(executable_path + "\\plugin\\" + pluginID + "\\version.config")) {
						cout << "From Version " << read_config(executable_path + "\\plugin\\" + pluginID + "\\version.config");
						cout << " to Version " << read_config(executable_path + "\\temp\\plugin\\version.config");
						cout << "\n";
					}
					//update.bat
					int statbat;
					if (fileExist(executable_path + "\\temp\\plugin\\update.bat")) {
						cout << "\n正在运行: update.bat\n";
						string execpath = executable_path + "\\temp\\plugin\\";
						string cmd = "cd /d \"" + execpath + "\" && \".\\update.bat\"";
						statbat = system(cmd.c_str());
					}

					cout << "\n正在复制文件。\n";
					string rm_command = "rmdir /s /q \"" + executable_path + "\\plugin\\" + pluginID + "\\\"";
					int stat1 = system(rm_command.c_str());
					string cp_command = "xcopy \"" + executable_path + "\\temp\\plugin\\*\" \"" + executable_path + "\\plugin\\" + pluginID + "\" /E /I /H /R /Y";
					int stat2 = system(cp_command.c_str());
					cout << "\n";
					S(100);
					prints("运行update.bat", statbat);
					cout << "\n";
					S(100);
					prints("清理文件夹", stat1);
					cout << "\n";
					S(100);
					prints("复制文件", stat2);
					cout << "\n";
					S(100);
					SetColorAndBackground(7, 0);
					cout << "\n更新 " << plugin.pluginName[SearchForAddress(plugin.plugin, pluginID, true)] << " - ";
					if (stat1 == 0 and stat2 == 0 and statbat == 0) {
						SetColorAndBackground(10, 0);
						cout << "操作已完成。";
					} else {
						SetColorAndBackground(0, 12);
						cout << "发生错误。请重试。";
					}
					SetColorAndBackground(7, 0);
					cout << "\n\n按任意键重载插件和配置文件。\n";
					_getch();
				} else { //安装
					string Name = read_config(executable_path + "\\temp\\plugin\\name.config");
					if (fileExist(executable_path + "\\temp\\plugin\\name.config") == false or Name.empty()) {
						cout << "\n错误: 插件结构不完整。\n";
						system("pause");
						return;
					}
					//pluginName去重
					for (const string& s : plugin.pluginName) {
						if (Name == s) {
							cout << "错误: 重复的插件名称。\n安装已取消。";
							system("pause");
							return;
						}
					}

					cout << "\n正在安装: " << Name;
					if (fileExist(executable_path + "\\temp\\plugin\\version.config")) {
						cout << " - Version " << read_config(executable_path + "\\temp\\plugin\\version.config");
					}
					//install.bat
					int statbat;
					if (fileExist(executable_path + "\\temp\\plugin\\install.bat")) {
						cout << "\n\n正在运行: install.bat\n";
						string execpath = executable_path + "\\temp\\plugin\\";
						string cmd = "cd /d \"" + execpath + "\" && \".\\install.bat\"";
						statbat = system(cmd.c_str());
					}

					cout << "\n\n";
					string cp_command = "xcopy \"" + executable_path + "\\temp\\plugin\\*\" \"" + executable_path + "\\plugin\\" + pluginID + "\" /E /I /H /R /Y";
					int stat = system(cp_command.c_str());
					S(100);
					prints("运行update.bat", statbat);
					cout << "\n";
					S(100);
					prints("复制文件", stat);
					cout << "\n";
					S(100);
					SetColorAndBackground(7, 0);
					cout << "\n安装 " << Name << " - ";
					if (stat == 0 and statbat == 0) {
						SetColorAndBackground(10, 0);
						cout << "操作已完成。";
					} else {
						SetColorAndBackground(0, 12);
						cout << "发生错误。请重试。";
					}
					SetColorAndBackground(7, 0);
					cout << "\n\n按任意键重载插件和配置文件。\n";
					_getch();
				}
				break;
			}
			case PLUGIN_UNINSTALL: {//卸载
				string text = "确实要卸载 \"" + str + "\"吗?\n";
				if (MessageBox(hwnd, text.c_str(), _T("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO) {
					cout << "\n操作已取消。\n";
					system("pause");
					return;
				}
				cout << "\n正在卸载: " << str;
				//uninstall.bat
				int statbat;
				if (fileExist(executable_path + "\\temp\\plugin\\update.bat")) {
					cout << "\n\n正在运行: uninstall.bat\n";
					string execpath = executable_path + "\\temp\\plugin\\";
					string cmd = "cd /d \"" + execpath + "\" && \".\\uninstall.bat\"";
					statbat = system(cmd.c_str());
				}

				string pluginID = plugin.plugin[SearchForAddress(plugin.pluginName, str, true)];
				string rm_command = "rmdir /s /q \"" + executable_path + "\\plugin\\" + pluginID + "\\\"";
				int stat = system(rm_command.c_str());
				S(100);
				prints("运行uninstall.bat", statbat);
				cout << "\n";
				S(100);
				prints("删除文件", stat);
				cout << "\n";
				S(100);
				SetColorAndBackground(7, 0);
				cout << "\n卸载 " << str << " - ";
				if (stat == 0 and statbat == 0) {
					SetColorAndBackground(10, 0);
					cout << "操作已完成。";
				} else {
					SetColorAndBackground(0, 12);
					cout << "发生错误。请重试。";
				}
				SetColorAndBackground(7, 0);
				cout << "\n\n按任意键重载插件和配置文件。\n";
				_getch();
				break;
			}
			case PLUGIN_DISABLE: {
				string path = executable_path + "\\plugin\\" + plugin.plugin[SearchForAddress(plugin.pluginName, str, true)] + "\\.disabled";
				write_config(path, "");
				break;
			}
			case PLUGIN_ENABLE: {
				string cmd = "del \"" + executable_path + "\\plugin\\" + plugin.plugin[SearchForAddress(plugin.pluginName, str, true)] + "\\.disabled\"";
				system(cmd.c_str());
				break;
			}
		}
		return;
	}
	void PluginManagerUI() {
startpluginui:
		// 复制当前启用状态
		vector<bool> tempState = plugin.pluginIsEnabled;   // 包含索引0占位
		// 获取插件列表（忽略索引0的"NULL"）
		const vector<string>& names = plugin.pluginName;
		int n = names.size() - 1;  // 实际插件数量
		if (n <= 0) {
			gotoxy(0, 3);
			SetColorAndBackground(0, 7);
			cout << "[暂无插件]\n";
			SetColorAndBackground(7, 0);
			system("pause");
			return;
		}
		int current = 1;

		bool exitFlag = false;
		bool saveFlag = false;

		HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
		DWORD eventsRead;
		INPUT_RECORD ir;

		while (!exitFlag) {
			n = names.size() - 1;
			gotoxy(0, 0);
			cout << " ===插件管理===\n";
			SetColorAndBackground(7, 0);
			cout << "按 ";
			SetColorAndBackground(0, 7);
			cout << "w/s/W/S";
			SetColorAndBackground(7, 0);
			cout << " 或 ";
			SetColorAndBackground(0, 7);
			cout << "上下方向键";
			SetColorAndBackground(7, 0);
			cout << " 调整方向, 按";
			SetColorAndBackground(0, 7);
			cout << "z/Z";
			SetColorAndBackground(7, 0);
			cout << "保存并退出, 按";
			SetColorAndBackground(0, 7);
			cout << "c/C";
			SetColorAndBackground(7, 0);
			cout << "取消更改并退出。\n按r/R安装新插件, 按f卸载高亮显示的插件。不受保存/取消的影响。";
			gotoxy(0, 3);
			for (int i = 0; i < n; ++i) {
				cout << "                                                \n";
			}
			gotoxy(0, 0);
			cout << " ";
			gotoxy(0, 3);

			// 显示所有插件（带状态标记）
			for (int i = 1; i <= n; ++i) {
				if (i == current) {
					SetColorAndBackground(0, 7);   // 高亮当前项
				} else {
					SetColorAndBackground(7, 0);
				}
				// 显示状态：启用为 "[*]"，禁用为 "[ ]"
				string mark = tempState[i] ? "[*]" : "[ ]";
				cout << mark << " " << names[i] << "\n";
			}
			SetColorAndBackground(7, 0);

			// 读取键盘事件
			ReadConsoleInput(hInput, &ir, 1, &eventsRead);
			if (eventsRead == 0 || ir.EventType != KEY_EVENT || !ir.Event.KeyEvent.bKeyDown)
				continue;

			WORD vk = ir.Event.KeyEvent.wVirtualKeyCode;
			char ch = ir.Event.KeyEvent.uChar.AsciiChar;

			// 处理方向键 / wasd
			if (vk == VK_UP || ch == 'w' || ch == 'W') {
				if (current > 1) current--;
			} else if (vk == VK_DOWN || ch == 's' || ch == 'S') {
				if (current < n) current++;
			} else if (vk == VK_RETURN || ch == ' ') {
				// 切换当前插件的启用状态
				tempState[current] = !tempState[current];
			} else if (ch == 'z' || ch == 'Z') {
				saveFlag = true;
				exitFlag = true;
			} else if (ch == 'c' || ch == 'C') {
				saveFlag = false;
				exitFlag = true;
			} else if (ch == 'r' || ch == 'R') {
				while (true) {
					gotoxy(0, 3);
					for (int i = 0; i < n; ++i) {
						cout << "                                                \n";
					}
					gotoxy(0, 0);
					gotoxy(0, 3);
					cout << "请选择插件包路径: ";
					string path = OpenFileDialogModern({
						{L"压缩文件", L"*.zip"}//,//使用分号隔开
						//{L"所有文件", L"*.*"}
					}, L"压缩文件", L"*.zip");
					if (path.empty()) {
						cout << "\n没有获取到路径。\n手动输入路径, 或键入\"b\"返回, 或键入\"r\"重试\n";
						cout << "\n请键入: \n你也可以把zip文件直接拖入此窗口。";
						gotoxy(4, 7);
						cin >> path;
						if (path == "b") {
							cls;
							break;
						}
						if (path == "r") {
							continue;
						}
					} else {
						cout << path << endl;
					}
					PLUGIN::PluginSystem(PLUGIN_INSTALL, path);
					cls;
					poweron(true);
					cls;
					goto startpluginui;
				}
			} else if (ch == 'f' || ch == 'F') {
				PLUGIN::PluginSystem(PLUGIN_UNINSTALL, names[current]);
				cls;
				poweron(true);
				cls;
				continue;
			}
		}

		// 处理保存操作
		if (saveFlag) {
			for (int i = 1; i <= n; ++i) {
				if (tempState[i] != plugin.pluginIsEnabled[i]) {
					// 调用 PluginSystem 启用或禁用
					if (tempState[i]) {
						PLUGIN::PluginSystem(PLUGIN_ENABLE, names[i]);
					} else {
						PLUGIN::PluginSystem(PLUGIN_DISABLE, names[i]);
					}
					// 更新内存中的状态
					plugin.pluginIsEnabled[i] = tempState[i];
				}
			}
			// 提示保存成功
			gotoxy(0, 3);
			SetColorAndBackground(10, 0);
			cout << "状态已保存。     \n";
			SetColorAndBackground(7, 0);
			system("pause");
		} else {
			// 取消操作，不做任何修改
			gotoxy(0, 3);
			SetColorAndBackground(7, 0);
			cout << "已取消修改。     \n";
			system("pause");
		}
	}
}
