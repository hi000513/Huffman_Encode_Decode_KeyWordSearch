#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include"KeyWordSearch.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 3) {
		cout << "用法: " << argv[0] << "<压缩文件路径> <关键词>" << endl;
		return 1;
	}
	string filename = argv[1];
	string keyword = argv[2];

	FILE* file = fopen(filename.c_str(), "rb");
	if (!file) {
		cerr << "无法打开文件：" << filename << endl;
		return 1;
	}
	//调用搜索函数
	int count = commonKeyWordSearch(keyword, file);
	if (count == 0) {
		cout << "抱歉，未检索到关键词:" << keyword << endl;
	}
	else {
		cout << "综上，共检索到关键词：" << keyword << " " << count << "次" << endl;
	}
	
	fclose(file);
	return 0;

}