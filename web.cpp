#include "./web.h"

// 回调函数：将接收到的数据追加到 string 中
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t totalSize = size * nmemb;
	((string*)userp)->append((char*)contents, totalSize);
	return totalSize;
}

void ReadWebFileVector(string url, vector<string>& lines, long long TimeLimit) {
	lines.clear();  // 清空输出容器
	
	CURL *curl = curl_easy_init();
	if (!curl) return;
	
	string response;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	
	// 设置超时时间（毫秒）
	if (TimeLimit > 0) {
		curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, TimeLimit);
	}
	
	// 若为 HTTPS 且证书验证不便，可忽略
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	
	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	
	if (res != CURLE_OK) return;  // 请求失败，lines 保持为空
	
	// 按行分割响应内容
	istringstream iss(response);
	string line;
	while (getline(iss, line)) {
		line=UTF8ToGBK(line);
		lines.push_back(line);
	}
}
