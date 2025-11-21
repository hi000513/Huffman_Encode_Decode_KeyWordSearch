#define _CRT_SECURE_NO_WARNINGS
#include "encodefunc.h"
#include <stdio.h>

/*
  用法: encode_main.exe <源文件> <压缩文件>
*/
int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("用法: %s <源文件> <压缩文件>\n", argv[0]);
		return 1;
	}

	const char* src = argv[1];
	const char* dst = argv[2];

	// 调用编码接口（内部会做文件打开/关闭与错误输出）
	enCode(src, dst);

	//计算文件压缩率
	printf("文件压缩率约为:%.2lf%%", getActualCompressionRates(src, dst)*100);

	return 0;
}