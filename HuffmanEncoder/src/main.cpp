#include <iostream>
#include <string>
#include "FileUtils.h"
#include "HuffmanTree.h"

using namespace Common;

int main(int argc, char* argv[]) {
    std::cout << "Huffman Encoder" << std::endl;
    std::cout << "===============" << std::endl << std::endl;
    
    if (argc < 3) {
        std::cout << "Usage: HuffmanEncoder <input_file> <output_file>" << std::endl;
        std::cout << "Example: HuffmanEncoder input.txt output.huf" << std::endl;
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    
    try {
        // Read input file
        std::cout << "Reading file: " << inputFile << std::endl;
        std::string text = FileUtils::ReadFile(inputFile);
        std::cout << "File size: " << text.size() << " bytes" << std::endl;
        
        // Calculate frequencies
        auto frequencies = HuffmanTree::CalculateFrequencies(text);
        std::cout << "Unique characters: " << frequencies.size() << std::endl;
        
        // Build Huffman tree
        HuffmanTree tree;
        tree.BuildTree(frequencies);
        
        // Generate codes
        auto codes = tree.GenerateCodes();
        
        // Encode text
        std::cout << "Encoding..." << std::endl;
        std::string encoded = tree.Encode(text);
        
        // Save encoded data (simplified - saving as binary string for now)
        // In a real implementation, you would pack bits into bytes
        FileUtils::WriteFile(outputFile, encoded);
        
        std::cout << "Encoded size: " << encoded.size() << " bits" << std::endl;
        std::cout << "Compression ratio: " << (1.0 - (encoded.size() / 8.0) / text.size()) * 100 << "%" << std::endl;
        std::cout << "Output saved to: " << outputFile << std::endl;
        
        // Display some sample codes
        std::cout << "\nSample Huffman codes:" << std::endl;
        int count = 0;
        for (const auto& pair : codes) {
            if (count++ >= 10) break;
            if (pair.first == '\n') {
                std::cout << "  '\\n' -> " << pair.second << std::endl;
            } else if (pair.first == ' ') {
                std::cout << "  ' ' -> " << pair.second << std::endl;
            } else {
                std::cout << "  '" << pair.first << "' -> " << pair.second << std::endl;
            }
        }
        
        std::cout << "\nEncoding completed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
