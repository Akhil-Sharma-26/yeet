#pragma once

#include"libs.hpp"
#include<fstream>

#if defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
#endif
/**
 * 
 */
void ListFiles(const std::string& path, std::vector<fs::path>& FilePath);
// TODO: This ListFiles need to check the diffs also to disnguish betweeen new and old files.
// TODO: Add a check that you can only list files if a .yeet dir is present/ initialized.

/**
 * 
 */
std::string readFile(fs::path path);


/**
 * This function removes trailing whitespace characters from the end of a given std::string by erasing characters starting from the first non-whitespace character found when iterating in reverse.
 */
static inline void rtrim(std::string &s) { 
    s.erase(std::find_if(s.rbegin(), s.rend(), // https://en.cppreference.com/w/cpp/algorithm/find.html
                [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

/**
 * 
 */
std::vector<unsigned char> readFile(const std::string& filename);

/**
 * 
 */
std::string File_name_Helper(std::string Objpath);

/**
 * 
 */
namespace Helper{
    std::string readFile(std::string path);
    void update_HEAD(std::string oid, std::string NewBranchPath);
};

/**
 * 
 */
bool isExecutableFile(const std::filesystem::path& path);

/**
 * 
 */
void writeStoreinDB(std::map<std::string, std::string> Store);

bool is_yeet_repo(fs::path pt);