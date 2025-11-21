#pragma once
#ifdef _WIN32
    #ifdef _WINNT_
    #undef _WINNT_
    #endif
    #include<windows.h>
    #include<intrin.h>
#endif
#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include<filesystem>
#include<sstream>
#include<fstream>
#include<map>

namespace fs = std::filesystem; // in header files, I have to do this only. "#define fs std::gdgsd" wil not worl
