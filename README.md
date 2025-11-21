# Huffman Encoding Project

A C++ implementation of Huffman encoding for text compression, decompression, and keyword search operations.

## Project Description

This is a data structure course project that implements Huffman encoding to compress English text documents in .txt format. The project supports compression, decompression, and keyword search operations on the files.

数据结构课程实验，要求使用哈夫曼编码对任意一篇.txt格式的英文文档进行压缩。且支持对压缩后的文件进行解压缩以及关键字搜索的操作。

## Project Structure

The project consists of three main executable modules and one shared static library:

```
HuffmanProject/
├── Common/                  # Static library with shared utilities
│   ├── include/
│   │   ├── FileUtils.h     # File I/O operations
│   │   └── HuffmanTree.h   # Huffman tree implementation
│   ├── src/
│   │   ├── FileUtils.cpp
│   │   └── HuffmanTree.cpp
│   └── Common.vcxproj
├── HuffmanEncoder/         # Encoder executable
│   ├── src/
│   │   └── main.cpp
│   └── HuffmanEncoder.vcxproj
├── HuffmanDecoder/         # Decoder executable
│   ├── src/
│   │   └── main.cpp
│   └── HuffmanDecoder.vcxproj
├── KeywordSearch/          # Keyword search tool
│   ├── src/
│   │   └── main.cpp
│   └── KeywordSearch.vcxproj
└── HuffmanProject.sln      # Visual Studio 2022 solution
```

## Features

### Common Library
- **FileUtils**: Utility functions for file I/O operations
  - Read/write text files
  - Read/write binary files
  - File existence checking
  - File size calculation

- **HuffmanTree**: Core Huffman encoding/decoding functionality
  - Calculate character frequencies
  - Build Huffman tree from frequencies
  - Generate Huffman codes
  - Encode text to binary representation
  - Decode binary back to text

### HuffmanEncoder
- Compress text files using Huffman encoding
- Display compression statistics
- Show sample Huffman codes for characters

### HuffmanDecoder
- Decompress Huffman encoded files
- Starter implementation ready for extension

### KeywordSearch
- Search for keywords in text files
- Display search results with context
- Show line numbers and positions
- Count total matches

## Building the Project

### Requirements
- Visual Studio 2022 or later
- Windows SDK 10.0
- C++17 support

### Build Steps

1. Open the solution file:
   ```
   HuffmanProject.sln
   ```

2. Select build configuration:
   - Debug (for development)
   - Release (for optimized builds)

3. Build the solution:
   - Press `Ctrl+Shift+B` or
   - Menu: Build → Build Solution

4. Output executables will be in:
   ```
   bin/Debug/    or    bin/Release/
   ```

### Build from Command Line

Using MSBuild:
```batch
msbuild HuffmanProject.sln /p:Configuration=Release /p:Platform=x64
```

## Usage

### HuffmanEncoder
```batch
HuffmanEncoder.exe <input_file> <output_file>

Example:
HuffmanEncoder.exe document.txt compressed.huf
```

### HuffmanDecoder
```batch
HuffmanDecoder.exe <input_file> <output_file>

Example:
HuffmanDecoder.exe compressed.huf decompressed.txt
```

### KeywordSearch
```batch
KeywordSearch.exe <file> <keyword>

Example:
KeywordSearch.exe document.txt "search term"
```

## Implementation Notes

### Current Status
- ✅ Project structure set up
- ✅ Common library with file utilities
- ✅ Huffman tree implementation
- ✅ Basic encoder functionality
- ✅ Keyword search tool
- ⚠️ Decoder requires full implementation (see below)

### Decoder Implementation
The decoder module is provided as a starter implementation. To complete it, you need to:

1. **Store frequency table in encoded file**: Modify the encoder to save the character frequency map at the beginning of the output file
2. **Read frequency table**: Update the decoder to read the frequency table from the file header
3. **Rebuild Huffman tree**: Use the frequency table to reconstruct the same Huffman tree used during encoding
4. **Decode bit stream**: Use the tree to decode the binary data back to text

### Potential Enhancements
- Bit packing: Currently stores binary as ASCII '0' and '1' characters; should pack into actual bits
- File format: Define a proper file format with header containing metadata
- Error handling: Add more robust error checking and validation
- Compression statistics: Track and display more detailed statistics
- Support for binary files: Extend to handle non-text files

## Technical Details

### Huffman Encoding Algorithm
1. Calculate frequency of each character in input text
2. Build a binary tree where:
   - Each leaf node represents a character
   - Node weights are character frequencies
   - Tree is built bottom-up using a priority queue
3. Generate binary codes by traversing from root to each leaf:
   - Left edge = '0', Right edge = '1'
4. Replace each character with its Huffman code

### Dependencies
All three executable projects depend on the Common static library:
- HuffmanEncoder → Common.lib
- HuffmanDecoder → Common.lib
- KeywordSearch → Common.lib

## License

See LICENSE file for details.

## Contributing

This is a course project. Feel free to fork and extend for educational purposes.
