#include "KeyWordSearch.h"

int getTextBits(FILE* file)
{
	//获取正文比特数
	fseek(file, 1, SEEK_SET);

	unsigned int textBits;
	fread(&textBits, sizeof(unsigned int), 1, file);

	return textBits;
}

void seekToTextStart(FILE* f)
{
	unsigned int textBits = getTextBits(f);
	//2.计算正文字节数
	int textBytes = (textBits + 7) / 8;

	//3.跳转到正文比特流起始位置
	fseek(f, -textBytes, SEEK_END);

}
// 检查后续字符是否匹配
bool isMatch(string& word, FILE* file, Node* root,
	unsigned char buffer, int bitCount) {
	long pos = ftell(file);  // 记录当前位置（字节偏移量）
    Node* curNode = root;
	// 从第二个字符开始匹配
	for (size_t i = 1; i < word.size(); i++) {
		char curCh = word[i]; // 待匹配的字符


		// 不断解码得到下一个字符
		while (curNode->left || curNode->right) {
			int bit = readBit(file, buffer, bitCount);
			if (bit == EOF) { // EOF 检查
				fseek(file, pos, SEEK_SET);
				return false;
			}
			curNode = (bit == 0) ? curNode->left : curNode->right;
		}

		char textCh = curNode->name;
        curNode = root;
		if (textCh != curCh) {
			fseek(file, pos, SEEK_SET); // 回到之前的位置
			return false;
		}
	}

	fseek(file, pos, SEEK_SET); // 回到之前的位置
	return true;
}

int commonKeyWordSearch(string& word, FILE* file) {
    if (!file) {
        printf("文件未打开\n");
        return -1;
    }

    unsigned int textBits = 0;
    //构建码表
    codeTable t = regetCodeTable(file, textBits);
    //构建哈夫曼树
    Node* root = rebuildHuffmanTree(t);

    unsigned int curChar = -1;//第一个字符从0开始
    unsigned int wordCnt = 0;
    Node* curNode = root;

    unsigned char buffer = 0;

    int bitCount = 8;
    
    // 边界问题：根节点也是叶子节点
    if (!root->left && !root->right) {
        if (word.size() == 1 && root->name == word[0]) {
            printf("整个文件都是字符 '%c'，关键词匹配成功。\n", root->name);
            return 1; // 出现次数可以直接认为是 1（或文件长度）
        }
        else {
            printf("整个文件只有字符 '%c'，无法匹配关键词 \"%s\"。\n",
                root->name, word.c_str());
            return 0;
        }
    }


    for (unsigned int i = 0; i < textBits; i++) {

        int bit = readBit(file, buffer, bitCount);
        if (bit == EOF) break; // EOF 检查

        curNode = (bit == 0) ? curNode->left : curNode->right;

        if (!curNode->left && !curNode->right) {
            if(curNode->name!=13) curChar++; //忽略回车符，大多数软件都是这样
            // 叶子节点
            if (curNode->name == word[0]) {
                // 检查字符串是否匹配
                if (isMatch(word, file, root, buffer, bitCount)) {
                    wordCnt++;
                    printf("查找到，第%d个%s出现在   %d-%d  字符处\n",
                        wordCnt, word.c_str(), curChar,curChar+word.size());
                }
            }
            curNode = root; // 回到根节点
        }
    }

    return wordCnt;
}

