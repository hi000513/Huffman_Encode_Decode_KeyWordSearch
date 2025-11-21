#define _CRT_SECURE_NO_WARNINGS
#include"decodefunc.h"

/*==========================================================================解码部分=====================================================================================*/

/*
	压缩文件结构：
	[1字节] 字符总数 tot_char
	[4字节] 正文比特流大小
	[2 * tot_char字节] 每个字符 + 码长
	[tot_code字节] 所有码字拼接成的比特流    如果剩余码字不足一个字节，用0补齐
	[正文编码比特流]
*/

codeTable regetCodeTable(FILE* srcfile, unsigned int& textBits)
{
	// 读取总字符数
	unsigned char tot_char = fgetc(srcfile);

	// 读取正文比特流长度（4 字节，unsigned int，小端序）
	textBits = 0;
	if (fread(&textBits, sizeof(unsigned int), 1, srcfile) != 1) {
		fprintf(stderr, "读取 textBits 失败\n");
		return codeTable();
	}

	// 读取每个字符和码长
	vector<char> ch;
	vector<unsigned char> codeLens;
	for (int i = 0; i < tot_char; i++) {
		char curch = fgetc(srcfile);
		unsigned char len = fgetc(srcfile);
		ch.push_back(curch);
		codeLens.push_back(len);
	}

	// 读取码字比特流
	codeTable table;
	unsigned char buffer = 0;
	int bitCount = 8;//强制第一次调用时读取一个字节 
	for (int i = 0; i < tot_char; i++) {
		string binarych;
		for (int j = 0; j < codeLens[i]; j++) {
			int b = readBit(srcfile, buffer, bitCount);
			if (b == -1) {
				fprintf(stderr, "码字读取失败\n");
				return codeTable();
			}
			binarych.push_back(b ? '1' : '0');
		}
		table[ch[i]] = binarych;
	}
	return table;
}

//重构哈夫曼树
Node* rebuildHuffmanTree(codeTable& t)
{
	//遍历每个字符，每个字符相当于一个叶子，我们把所有到叶子的路径构建出来，就获得了整棵树
	Node* root = new Node();

	for (auto& kv : t) {
		char curch = kv.first;
		const string& code = kv.second;
		Node* cur = root;
		for (char bit : code) {
			if (bit == '0') {
				if (!cur->left) cur->left = new Node();
				cur = cur->left;
			}
			else {
				if (!cur->right) cur->right = new Node();
				cur = cur->right;
			}
		}
		// 到达叶子节点
		cur->name = curch;
		cur->isLeaf = true;
	}
	return root;
}

//依据哈夫曼树进行解码
void deCompressFile(FILE* dirfile, FILE* srcfile, Node* root, unsigned int textBits)
{
	unsigned char buffer = 0;
	int bitCount = 8; // 强制第一次调用时触发 fread
	Node* curpos = root;

	for (unsigned int i = 0; i < textBits; i++) {
		int bit = readBit(srcfile, buffer, bitCount);
		if (bit == -1) {
			fprintf(stderr, "解码时读取比特失败\n");
			return;
		}
		curpos = (bit == 0) ? curpos->left : curpos->right;
		// 到达叶子节点
		if (!curpos->left  && !curpos->right) {
			fputc(curpos->name, dirfile); // 写入字符
			curpos = root;              // 回到根节点
		}
	}
}
//把源文件（二进制文件） 解压成目标文件（文本文件）
void deCode(const char* srcfilename, const char* dirfilename)
{
	FILE* srcfile = fopen(srcfilename,"rb");//以二进制读的形式打开源文件
	FILE* dirfile = fopen(dirfilename, "w");//以文本写的形式打开目标文件
	//获取码表以及正文比特流长度
	unsigned int textBits = 0;
	codeTable table = regetCodeTable(srcfile, textBits);

	//重建哈夫曼树
	Node* root = rebuildHuffmanTree(table);

	//解压
	deCompressFile(dirfile, srcfile, root, textBits);
	
	// 清理
	fclose(srcfile);
	fclose(dirfile);
	deleteTree(root);

	printf("成功将文件:%s解压到文件:%s\n", srcfilename, dirfilename);

}
