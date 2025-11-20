#pragma once

#include"libs.hpp"
#include "vendor/picosha2.h"

std::string calculateSHA256Hex(const std::string& content);
std::string calculateFileSHA256Hex(const std::filesystem::path& path);
std::string timeToString(time_t currtime);