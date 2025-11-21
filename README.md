项目名称：--Huffman--  
数据结构课程实验：哈夫曼编码压缩、解压缩与关键词检索  
⚠️ 注意事项  
本项目仅支持处理 ASCII 编码的英文文本文件  

编码算法采用朴素哈夫曼编码（非标准哈夫曼树）

关键词搜索使用朴素字符串匹配算法（未使用 KMP 或 Boyer-Moore）

🧠 项目简介
本项目实现了一个完整的哈夫曼编码系统，支持以下功能：

✅ 对任意英文 .txt 文档进行哈夫曼编码压缩

✅ 对压缩后的文件进行解码还原

✅ 在压缩文件中进行关键词检索（无需完全解压）

该项目采用 C++ 编写，模块化设计，适合教学演示、算法学习和功能扩展。

🧱 项目结构
plaintext  
--Huffman--/  
│
├── common/           # 公共静态库模块，包含通用结构体与工具函数  
│   ├── common.h  
│   └── common.cpp  
│  
├── Encode/           # 编码模块，实现哈夫曼压缩  
│   ├── encode.h  
│   └── encode.cpp  
│  
├── Decode/           # 解码模块，实现哈夫曼解压  
│   ├── decode.h  
│   └── decode.cpp  
│  
├── KeyWordSearch/    # 关键词检索模块  
│   ├── search.h  
│   └── search.cpp  
│  
├── .gitignore        # 忽略编译产物与缓存文件  
├── LICENSE           # MIT 开源协议  
└── README.md         # 项目说明文件（你正在看）  
🛠️ 编译说明
推荐环境：
编译器：Visual Studio 2022

语言标准：C++17 或以上

平台：Windows x64

编译步骤：
打开根目录下的 Huffman.sln 解决方案文件。

编译顺序建议：

先编译 common 模块为静态库（生成 common.lib）

再编译 Encode、Decode、KeyWordSearch 模块，自动链接 common.lib

所有模块均可独立运行，也可集成为一个主程序。

📂 输入输出说明
输入文件：
任意英文 .txt 文档，建议 ASCII 编码  

放置于项目根目录或指定路径

输出文件：
.huff：压缩后的二进制文件

.txt：解码还原后的文本

.log 或控制台输出：关键词检索结果

🔍 使用示例
bash
# 编码
Encode.exe input.txt output.huff

# 解码
Decode.exe output.huff restored.txt

# 关键词搜索
KeyWordSearch.exe output.huff "keyword"

