#pragma once
#include <stdio.h>
#include <queue>
#include <vector>
#include <functional>
#include<unordered_map>
#include<string>
#include<stdbool.h>
#include <cstring>
using namespace std;

struct Node {
	char name;
	int freq;
	Node* left;
	Node* right;
	bool isLeaf;
	Node() : name(0), freq(0), left(nullptr), right(nullptr), isLeaf(false) {}
};

struct Compare {
	bool operator()(Node* a, Node* b) const {
		return a->freq > b->freq;  // 频率小的优先
	}
};

typedef priority_queue<Node*, vector<Node*>, Compare > dictionary;
typedef unordered_map<char, string> codeTable;

// ============ 文件格式说明 ============
/*
	压缩文件结构：
	[1字节] 字符总数 tot_char
	[4字节] 正文比特流大小
	[2 * tot_char字节] 每个字符 + 码长
	[tot_code字节] 所有码字拼接成的比特流    如果剩余码字不足一个字节，用0补齐
	[正文编码比特流]
*/

// 从bit流中读取一位比特
int readBit(FILE* srcfile, unsigned char& buffer, int& bitCount);

// 按位写入比特流
void writeBit(FILE* dirfile, unsigned char& buffer, int& bitCount, bool bit);

// 销毁哈夫曼树
void deleteTree(Node* node);
