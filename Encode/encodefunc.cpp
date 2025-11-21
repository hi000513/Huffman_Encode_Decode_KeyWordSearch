#define _CRT_SECURE_NO_WARNINGS
#include "encodefunc.h"
#include<iostream>
using namespace std;

/*-------------------------------------------------------------------编码部分------------------------------------------------------------------------*/

int cntWords(FILE* file, dictionary& d)
{
	if (file == NULL) {
		printf("文件未打开或初始化失败\n");
		return -1;
	}
	char ch;
	int word[256];//定义ASCII全集，统计字符
	memset(word, 0, 256 * sizeof(int));
	while ((ch = fgetc(file)) != EOF) {
		word[(unsigned char)ch]++;
	}
	//把统计的字符用最小堆进行存储
	for (int i = 0; i < 128; i++) {
		if (word[i] != 0) {
			Node* n = new Node();
			n->name = (char)i;
			n->freq = word[i];
			n->left = nullptr;
			n->right = nullptr;
			d.push(n);   // 存的是堆分配的指针
		}
	}
	return 0;
}

Node* creatHuffManTree(dictionary& d) {
	while (!d.empty()) {
		Node* leftNode = d.top(); d.pop();
		if (d.empty()) return leftNode; // 最后一个就是根
		Node* rightNode = d.top(); d.pop();
		Node* newNode = new Node;
		newNode->name = -1;//新节点不维护名字，每个字符只出现在叶子节点
		newNode->left = leftNode;
		newNode->right = rightNode;
		newNode->freq = leftNode->freq + rightNode->freq;
		d.push(newNode);
	}
	return nullptr; // 空堆情况
}

//辅助构建码表
static void traverseTree(Node* curNode, codeTable& table, string path) {

	if (!curNode) return;
	if (curNode->left == NULL && curNode->right == NULL) {
		//说明是叶子节点
		table[curNode->name] = path;
		return;
	}
	//向左搜索
	traverseTree(curNode->left, table, path + "0");
	//向右搜索
	traverseTree(curNode->right, table, path + "1");
}

//构建码表
codeTable creatCodeTable(Node* root) {
	codeTable table;
	traverseTree(root, table, "");
	return table;
}

//按位写入比特流
static void writeBit(FILE* dirfile, unsigned char& buffer, int& bitCount, bool bit)
{
	buffer = (buffer << 1) | bit;//左移并写入新位
	bitCount++;
	if (bitCount == 8) {
		fwrite(&buffer, sizeof(unsigned char), 1, dirfile);
		bitCount = 0;
		buffer = 0;
	}
}

/*
	压缩文件结构：
	[1字节] 字符总数 tot_char
	[4字节] 正文比特流大小
	[2 * tot_char字节] 每个字符 + 码长
	[tot_code字节] 所有码字拼接成的比特流    如果剩余码字不足一个字节，用0补齐
	[正文编码比特流]
*/
void compressFile(FILE* dirfile, FILE* srcfile, codeTable& t)
{
	// Step 1: 计算码表编码部分总比特数
	int tot_code_bits = 0;
	for (auto& tt : t) {
		tot_code_bits += tt.second.size(); // 每个码字的比特数
	}

	// Step 2: 写入字符总数（1 字节）
	unsigned char buf[4];
	unsigned char tot_char = t.size();
	fwrite(&tot_char, 1, 1, dirfile);

	// Step 3: 预留正文比特流总长度（4 字节，占位符）
	unsigned int text_bits_placeholder = 0;
	fwrite(&text_bits_placeholder, sizeof(unsigned int), 1, dirfile);

	// Step 4: 写入每个字符及其码长（每项 2 字节）
	for (auto& tt : t) {
		buf[0] = tt.first;                  // 字符
		buf[1] = tt.second.size();          // 码长（单位：bit）
		fwrite(buf, 1, 2, dirfile);
	}

	// Step 5: 写入码表编码部分（所有码字拼接成的比特流）
	unsigned char buffer = 0;
	int bitCount = 0;
	for (auto& tt : t) {
		string binarych = tt.second;
		for (char bitChar : binarych) {
			bool bit = (bitChar == '1');
			writeBit(dirfile, buffer, bitCount, bit);
		}
	}

	// 补齐码表最后不足 8 位的部分
	if (bitCount > 0) {
		buffer <<= (8 - bitCount);
		fwrite(&buffer, 1, 1, dirfile);
	}

	// Step 6: 写入正文压缩内容（重新开始 buffer/bitCount）
	buffer = 0;
	bitCount = 0;
	unsigned int text_bits = 0; // 统计正文比特数
	char ch;
	while ((ch = fgetc(srcfile)) != EOF) {
		string binarych = t[ch];
		for (char bitChar : binarych) {
			bool bit = (bitChar == '1');
			writeBit(dirfile, buffer, bitCount, bit);
			text_bits++; // 每写一位，计数+1
		}
	}

	// 补齐正文最后不足 8 位的部分
	if (bitCount > 0) {
		buffer <<= (8 - bitCount);
		fwrite(&buffer, 1, 1, dirfile);
	}

	// Step 7: 回填正文比特流总长度
	fseek(dirfile, 1, SEEK_SET); // 跳到 tot_char 后的位置
	fwrite(&text_bits, sizeof(unsigned int), 1, dirfile);

	// 最后文件指针会停在正文末尾
}

void enCode(const char* srcfilename,const char* dirfilename)
{
	FILE* srcfile = fopen(srcfilename, "rb");
	if (!srcfile) {
		fprintf(stderr, "无法打开源文件: %s\n", srcfilename);
		return;
	}

	// 首先需要统计文档中各字符出现的次数
	dictionary d;
	if (cntWords(srcfile, d) != 0) {
		fclose(srcfile);
		return;
	}

	// cntWords 读到了 EOF，若后续需再次读取，rewind 回到文件开头
	rewind(srcfile);

	// 使用目标文件名打开目标文件（写入模式），压缩文件应以二进制写入
	FILE* dirfile = fopen(dirfilename, "wb");
	if (!dirfile) {
		fprintf(stderr, "无法创建目标文件: %s\n", dirfilename);
		fclose(srcfile);
		return;
	}

	// 构建哈夫曼树与码表
	Node* huffmanroot = creatHuffManTree(d);
	codeTable table = creatCodeTable(huffmanroot);

	// 压缩到目标文件（注意传入正确的文件指针）
	compressFile(dirfile, srcfile, table);

	fclose(srcfile);
	fclose(dirfile);
	printf("恭喜！成功将文件:%s压缩到文件:%s\n", srcfilename, dirfilename);
}

double getActualCompressionRates(const char* srcfilename, const char* dirfilename)
{
	FILE* src = fopen(srcfilename, "r");
	FILE* dir = fopen(dirfilename, "rb");
	
	fseek(src, 0, SEEK_END);  //文件指针移动到末尾
	long src_size = ftell(src);  //ftell会返回返回当前文件指针相对于文件开头的字节偏移量

	fseek(dir, 0, SEEK_END);  //文件指针移动到末尾
	long dir_size = ftell(dir);

	double actualrate = (double)dir_size / src_size;

	fclose(src);
	fclose(dir);
	
	return actualrate;
}
