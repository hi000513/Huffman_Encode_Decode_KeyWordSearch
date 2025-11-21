#define _CRT_SECURE_NO_WARNINGS
#include "huffman.h"

// 辅助函数：按位读取
int readBit(FILE* srcfile, unsigned char& buffer, int& bitCount)
{
	if (bitCount == 8) {
		//从源文件读取下一个字节
		if (fread(&buffer, 1, 1, srcfile) != 1) {
			fprintf(stderr, "按位读取失败\n");
			return -1;
		}
		bitCount = 0;
	}
	int bit = (buffer >> (7 - bitCount)) & 1;
	++bitCount;
	return bit;
}

// 辅助函数：按位写入
void writeBit(FILE* dirfile, unsigned char& buffer, int& bitCount, bool bit)
{
	buffer = (buffer << 1) | bit;//左移并写入新位
	bitCount++;
	if (bitCount == 8) {
		fwrite(&buffer, sizeof(unsigned char), 1, dirfile);
		bitCount = 0;
		buffer = 0;
	}
}

// 销毁哈夫曼树
void deleteTree(Node* node)
{
	if (!node) return;
	deleteTree(node->left);
	deleteTree(node->right);
	delete node;
}