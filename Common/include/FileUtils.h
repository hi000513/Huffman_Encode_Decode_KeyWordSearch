#pragma once

#include <string>
#include <vector>

namespace Common {

class FileUtils {
public:
    // Read entire file contents into a string
    static std::string ReadFile(const std::string& filename);
    
    // Write string content to a file
    static bool WriteFile(const std::string& filename, const std::string& content);
    
    // Read binary file into byte vector
    static std::vector<unsigned char> ReadBinaryFile(const std::string& filename);
    
    // Write byte vector to binary file
    static bool WriteBinaryFile(const std::string& filename, const std::vector<unsigned char>& data);
    
    // Check if file exists
    static bool FileExists(const std::string& filename);
    
    // Get file size in bytes
    static size_t GetFileSize(const std::string& filename);
};

} // namespace Common
