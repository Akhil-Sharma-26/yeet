#pragma once

#ifdef _WIN32
    #ifdef _WINNT_
    #undef _WINNT_
    #endif
    #include<windows.h>
    #include<intrin.h>
#endif
#include"../../YeetUtils/include/libs.hpp"
#include"../../YeetUtils/include/file_utils.hpp"
#include"../../YeetUtils/include/diff_utils.hpp"
#include"db.hpp"

#include<unordered_map>


void YeetInit(std::string path=".");
void YeetStatus(std::string path);
void YeetAdd();