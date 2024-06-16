#include"controllers.hpp"
#include<bits/stdc++.h>
#include<filesystem>
void YeetStatus(){

}


/**
 * Initializes a .git directory with default configs
 *
 * @param path absolute path of the directory should be entered or .
 * @return Nothing for now
 */
void YeetInit(std::string path){
    try
    {
        // std::cout<<path;
        // TODO: 
        // Make unit test for the testing of making .git director.
        std::string pwd = std::filesystem::current_path();
        std::string _actualPath=path+".git";
        if(path.back()=='.') _actualPath=pwd+"/.git";  
        if(std::filesystem::exists(_actualPath)){
            throw std::runtime_error("A git folder already exists in this directory. \n");
        }
        std::cout<<_actualPath<<std::endl;
        std::filesystem::create_directory(_actualPath);
        std::filesystem::create_directory(_actualPath+"/objects");
        std::filesystem::create_directory(_actualPath+"/refs");

        // TODO
        // Make HEAD file.
        // std::ofstream headFile(".git/HEAD");
        //     if (headFile.is_open()) {
        //         headFile << "ref: refs/heads/main\n";
        //         headFile.close();
        //     } else {
        //         throw std::runtime_error("Failed to create .git/HEAD file.\n");
        //     }
    
            std::cout << "Initialized git directory\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        // return EXIT_FAILURE;
    }
    
}

void YeetAdd(){

}