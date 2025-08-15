#pragma once

#include"libs.hpp"
#include <sha.h>
#include <filters.h>
#include <hex.h>

std::string calculateSHA1Hex(const std::string& content);
std::string timeToString(time_t currtime);