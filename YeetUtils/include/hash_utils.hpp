#pragma once

#include"libs.hpp"
#include <sha.h>
#include <filters.h>
#include <hex.h>

std::string calculateSHA256Hex(const std::string& content);
std::string timeToString(time_t currtime);