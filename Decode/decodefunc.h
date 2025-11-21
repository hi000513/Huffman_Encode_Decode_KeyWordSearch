#pragma once
#include "../common/huffman.h"

//从bit流中读取一位比特
// static int readBit(FILE* srcfile, unsigned char& buffer, int& bitCount);//要读取的源文件 缓存一个要读取的字节  已经处理的位

//根据目标文件获得码表
codeTable regetCodeTable(FILE* srcfile, unsigned int& textBits);

//根据码表重建哈夫曼树
Node* rebuildHuffmanTree(codeTable& t);

//根据生成的哈夫曼树解压缩
void deCompressFile(FILE* dirfile, FILE* srcfile, Node* root, unsigned int textBits);

//解码函数
void deCode(const char* srcfile, const char* dirfile);
