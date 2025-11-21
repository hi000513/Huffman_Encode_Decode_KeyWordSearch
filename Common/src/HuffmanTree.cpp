#include "HuffmanTree.h"
#include <queue>
#include <algorithm>

namespace Common {

HuffmanTree::HuffmanTree() : root_(nullptr) {}

HuffmanTree::~HuffmanTree() {}

std::map<char, int> HuffmanTree::CalculateFrequencies(const std::string& text) {
    std::map<char, int> frequencies;
    for (char c : text) {
        frequencies[c]++;
    }
    return frequencies;
}

void HuffmanTree::BuildTree(const std::map<char, int>& frequencies) {
    // Priority queue comparator (min-heap based on frequency)
    auto compare = [](const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
        return a->frequency > b->frequency;
    };
    
    std::priority_queue<
        std::shared_ptr<HuffmanNode>,
        std::vector<std::shared_ptr<HuffmanNode>>,
        decltype(compare)
    > pq(compare);
    
    // Create leaf nodes for each character
    for (const auto& pair : frequencies) {
        pq.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
    }
    
    // Build tree
    while (pq.size() > 1) {
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();
        
        auto parent = std::make_shared<HuffmanNode>(
            left->frequency + right->frequency,
            left,
            right
        );
        pq.push(parent);
    }
    
    root_ = pq.top();
}

void HuffmanTree::GenerateCodesHelper(std::shared_ptr<HuffmanNode> node, const std::string& code) {
    if (!node) return;
    
    if (node->IsLeaf()) {
        codes_[node->character] = code.empty() ? "0" : code;
        return;
    }
    
    GenerateCodesHelper(node->left, code + "0");
    GenerateCodesHelper(node->right, code + "1");
}

std::map<char, std::string> HuffmanTree::GenerateCodes() {
    codes_.clear();
    GenerateCodesHelper(root_, "");
    return codes_;
}

std::string HuffmanTree::Encode(const std::string& text) {
    if (codes_.empty()) {
        GenerateCodes();
    }
    
    std::string encoded;
    for (char c : text) {
        encoded += codes_[c];
    }
    return encoded;
}

std::string HuffmanTree::Decode(const std::string& encodedText) {
    if (!root_) {
        return "";
    }
    
    std::string decoded;
    auto current = root_;
    
    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        
        if (current->IsLeaf()) {
            decoded += current->character;
            current = root_;
        }
    }
    
    return decoded;
}

} // namespace Common
