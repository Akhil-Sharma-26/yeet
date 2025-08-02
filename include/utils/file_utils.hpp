#pragma once
#include "core/libs.hpp"
#include <string>
#include <vector>
#include <filesystem>

// File utility functions
void ListFiles(std::string path, std::vector<std::filesystem::path>& FilePath);
std::string readFile(std::filesystem::path path);
std::vector<std::string> splitIntoLines(const std::string& str);
void writeStoreinDB(std::map<std::string, std::string> Store);