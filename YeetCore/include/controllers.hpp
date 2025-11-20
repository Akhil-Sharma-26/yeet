#pragma once
#include<vector>
#include<filesystem>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip> // for input/output manipulators
#include<algorithm>
#include<unordered_set>
#include <iomanip>
#include<filesystem>
#include <iostream>  
#include <vector>  
#include <string>  
#include <algorithm>  
#include <unordered_map>  
#include <fstream>  
#include <sstream>  
#include <iomanip>  
#include <filesystem>  
#include <regex>  
#include <stdexcept>  
#include <ctime>  
#include <cstdlib>  
#include <iterator>  
#include "commit.hpp"
#include "tree.hpp"
// Structures:




// Helper Functions
void writeStoreinDB(std::map<std::string, std::string> Store);
void ListFiles(std::string path,std::vector<std::filesystem::path>&FilePath);
std::string Inflate(std::string path);
std::vector<std::string> splitIntoLines(const std::string& str);
int Shortest_Edit_Search(const std::vector<std::string>& a, const std::vector<std::string>& b, 
                         std::vector<std::vector<int>>& trace);
void Backtrack(std::string a, std::string b, std::vector<std::vector<int>> trace);
std::vector<Edit> diff(const std::vector<std::string> &a, const std::vector<std::string> &b, const std::vector<std::vector<int>> &trace, int d);
void storeDiff(const std::vector<Edit>& edits);



std::string Directory_name_Helper(std::string Objpath);
std::string File_name_Helper(std::string Objpath);




class Index{
    public:
        std::vector<std::filesystem::path> Entries;
};

class Diffs{
    public:
        std::string path;
        std::string diffs;
        
};

// For Diffs





namespace CommitHelper{
    void YeetStatus(std::string path, std::vector<std::filesystem::path> &FilePath);
}