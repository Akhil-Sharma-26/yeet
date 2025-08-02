#include "utils/file_utils.hpp"
#include <fstream>
#include <iostream>

void ListFiles(std::string path, std::vector<std::filesystem::path>& FilePath) {
    // Copy implementation from controllers.cpp
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        
        // TODO: Make a file and get names from there
        // My .yeet-ignore
        const bool IGNORE = entry.path().generic_string().find(".git") != std::string::npos ||
        entry.path().generic_string().find(".yeet") != std::string::npos || 
                          entry.path().generic_string().find(".vscode") != std::string::npos || 
                          entry.path().generic_string().find(".xmake") != std::string::npos || 
                          entry.path().generic_string().find(".cmake") != std::string::npos || 
                          entry.path().generic_string().find("/build") != std::string::npos;

        if(IGNORE){
            continue; // SKip that
        }

        if(entry.is_directory()){
            ListFiles(entry.path(), FilePath); // Recurse into that directory
        }
        else {
            FilePath.push_back(entry);
        }
    }
}

std::string readFile(std::filesystem::path path) {
    // Copy implementation from controllers.cpp
    std::ifstream f(path, std::ios::in);
    const auto sz = std::filesystem::file_size(path);
    std::string result(sz, '\0');
    f.read(result.data(), sz);
    return result;
}

// Implement other functions...




