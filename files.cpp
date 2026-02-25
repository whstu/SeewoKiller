#include "./files.h"

//检查文件是否存在
bool fileExist(const string& filename) {
	ifstream file(filename);
	return file.good();
}

int SearchForAddress(string value[], string goal) {
	int i = 0;
	// 假设数组以空字符串 "" 作为结束标志
	while (!value[i].empty()) {
		if (value[i].find(goal) != string::npos) {
			return i;   // 返回第一个匹配的位置
		}
		i++;
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

void change_word(string string_class, int address, bool config, string PATH, string name) {
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
