#pragma once
#include "../common/huffman.h"
#include "../DECODE/decodefunc.h"

/*
	压缩文件结构：
	[1字节] 字符总数 tot_char
	[4字节] 正文比特流大小
	[2 * tot_char字节] 每个字符 + 码长
	[tot_code字节] 所有码字拼接成的比特流    如果剩余码字不足一个字节，用0补齐
	[正文编码比特流]
*/
//获取正文比特数
int getTextBits(FILE* file);
//这个函数会将文件指针跳转到正文比特流的起始位置
void seekToTextStart(FILE* f);
//朴素搜索函数,返回检索到单词的次数
int commonKeyWordSearch(string& word, FILE* file);
//检查当前解码的字符与单词是否匹配(从正文比特流中不断解码，直到出现不匹配的字符，或完全匹配成功)
bool isMatch(string &word, FILE* file, Node* root, unsigned char buffer, int bitCount);
