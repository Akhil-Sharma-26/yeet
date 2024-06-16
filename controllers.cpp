#include"controllers.hpp"
#include<bits/stdc++.h>
#include<filesystem>
void YeetStatus(){

}


/**
 * Initializes a .git directory with default configs
 *
 * @param path absolute path of the directory should be entered or `.` The default is `.`
 * @attention If You put `path` as `~/dev/abc/` then it will create .git in `~/dev/abc/`. If you want to put .git in your pwd then put `~/dev/abc/.` or just `.` .
 * @return Nothing for now
 * @author Akhil Sharma
 */
void YeetInit(std::string path="."){
    try
    {
        // std::cout<<path;
        // TODO: Consider all cases the user can enter here
        // He can enter . --> init in pwd
        // He can enter ebc --> init in pwd/ebc
        // He can enter full path from root --> init at that path
        // He can enter ebc/ --> init in pwd/ebc only not pwd/abc/
        std::string pwd = std::filesystem::current_path();
        std::string _actualPath=path+".git";
        if(path.back()!='/'&& path.back()!='.') _actualPath=path+"/.git";
        if(path.back()=='.') _actualPath=pwd+"/.git";  
        if(std::filesystem::exists(_actualPath)){
            throw std::runtime_error("A git folder already exists in this directory. \n");
        }
        // std::cout<<_actualPath<<std::endl;
        std::filesystem::create_directory(_actualPath);
        std::filesystem::create_directory(_actualPath+"/objects");
        std::filesystem::create_directory(_actualPath+"/refs");
        std::filesystem::create_directory(_actualPath+"/refs/heads");
        std::filesystem::create_directory(_actualPath+"/refs/tags");

        // Make HEAD file.
        std::ofstream headFile(_actualPath+".git/HEAD");
            if (headFile.is_open()) {
                headFile << "ref: refs/heads/main\n";
                headFile.close();
            } else {
                throw std::runtime_error("Failed to create .git/HEAD file.\n");
            }

        // Making Description file.
        std::ofstream descFile(_actualPath+".git/description");
            if(descFile.is_open()){
                descFile<<"Demo Description. This file contains the info and descriptio about the repository.\n";
                descFile.close();
            }
            else {
                throw std::runtime_error("Failed to create .git/description file.\n");
            }
        
        // Making config file
        std::ofstream configFile(_actualPath+".git/config");
            if(configFile.is_open()){
                // TODO: Find a configparser for C++. and replace the content of this configFile.
                configFile<<"Demo Config\n";
                configFile.close();
            }
            else {
                throw std::runtime_error("Failed to create .git/config file.\n");
            }
        std::cout << "Initialized git directory\n";
    }
    catch(const std::exception& e)
    {
        std::cout<<"Hello";
        std::cerr << e.what() << '\n';
        // return EXIT_FAILURE;
    }
    
}

void YeetAdd(){

}