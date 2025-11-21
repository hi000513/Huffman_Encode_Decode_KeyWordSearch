#pragma once
#include "../common/huffman.h"

// ============ 编码相关函数 ============

// 统计文件中各字符出现的频率
int cntWords(FILE* file, dictionary& d);

// 根据频率构建哈夫曼树
Node* creatHuffManTree(dictionary& d);

// 生成码表
codeTable creatCodeTable(Node* root);

// 压缩文件到目标文件
void compressFile(FILE* dirfile, FILE* srcfile, codeTable& t);

// 编码主函数
void enCode(const char* srcfilename, const char* dirfilename);

//获得实际压缩率
double getActualCompressionRates(const char* srcfilename, const char* dirfilename);
