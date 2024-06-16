#include"controllers.hpp"
#include<bits/stdc++.h>
#include<filesystem>
void YeetStatus(){

}

void YeetInit(){
    try
    {
        std::filesystem::create_directory(".git");
        std::filesystem::create_directory(".git/objects");
        std::filesystem::create_directory(".git/refs");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        // return EXIT_FAILURE;
    }
    
}

void YeetAdd(){

}