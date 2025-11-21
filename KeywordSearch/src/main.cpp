#include <iostream>
#include <string>
#include <algorithm>
#include "FileUtils.h"

using namespace Common;

// Simple keyword search implementation
void SearchKeyword(const std::string& text, const std::string& keyword) {
    size_t pos = 0;
    int count = 0;
    
    std::cout << "Searching for keyword: \"" << keyword << "\"" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    
    while ((pos = text.find(keyword, pos)) != std::string::npos) {
        count++;
        
        // Find line number
        int lineNum = 1 + std::count(text.begin(), text.begin() + pos, '\n');
        
        // Extract context (50 chars before and after)
        size_t contextStart = (pos > 50) ? pos - 50 : 0;
        size_t contextEnd = std::min(pos + keyword.length() + 50, text.length());
        std::string context = text.substr(contextStart, contextEnd - contextStart);
        
        // Replace newlines with spaces for display
        std::replace(context.begin(), context.end(), '\n', ' ');
        
        std::cout << "Match #" << count << " at line " << lineNum << ", position " << pos << std::endl;
        std::cout << "  Context: ..." << context << "..." << std::endl << std::endl;
        
        pos += keyword.length();
    }
    
    if (count == 0) {
        std::cout << "Keyword not found." << std::endl;
    } else {
        std::cout << "Total matches found: " << count << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Keyword Search Tool" << std::endl;
    std::cout << "===================" << std::endl << std::endl;
    
    if (argc < 3) {
        std::cout << "Usage: KeywordSearch <file> <keyword>" << std::endl;
        std::cout << "Example: KeywordSearch document.txt \"search term\"" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    std::string keyword = argv[2];
    
    try {
        // Check if file exists
        if (!FileUtils::FileExists(filename)) {
            std::cerr << "Error: File not found: " << filename << std::endl;
            return 1;
        }
        
        // Read file
        std::cout << "Reading file: " << filename << std::endl;
        std::string text = FileUtils::ReadFile(filename);
        std::cout << "File size: " << text.size() << " bytes" << std::endl;
        std::cout << std::endl;
        
        // Search for keyword
        SearchKeyword(text, keyword);
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
