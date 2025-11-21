#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>

namespace Common {

// Node structure for Huffman tree
struct HuffmanNode {
    char character;
    int frequency;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(char ch, int freq)
        : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
    
    HuffmanNode(int freq, std::shared_ptr<HuffmanNode> l, std::shared_ptr<HuffmanNode> r)
        : character('\0'), frequency(freq), left(l), right(r) {}
    
    bool IsLeaf() const { return left == nullptr && right == nullptr; }
};

class HuffmanTree {
public:
    HuffmanTree();
    ~HuffmanTree();
    
    // Build tree from frequency map
    void BuildTree(const std::map<char, int>& frequencies);
    
    // Generate Huffman codes for each character
    std::map<char, std::string> GenerateCodes();
    
    // Encode text using Huffman codes
    std::string Encode(const std::string& text);
    
    // Decode binary string using Huffman tree
    std::string Decode(const std::string& encodedText);
    
    // Get root node
    std::shared_ptr<HuffmanNode> GetRoot() const { return root_; }
    
    // Calculate frequency map from text
    static std::map<char, int> CalculateFrequencies(const std::string& text);

private:
    std::shared_ptr<HuffmanNode> root_;
    std::map<char, std::string> codes_;
    
    void GenerateCodesHelper(std::shared_ptr<HuffmanNode> node, const std::string& code);
};

} // namespace Common
