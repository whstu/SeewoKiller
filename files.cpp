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
