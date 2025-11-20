#pragma once

#include"libs.hpp"
#include "vendor/picosha2.h"

std::string calculateSHA256Hex(const std::string& content);
std::string timeToString(time_t currtime);