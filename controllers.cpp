#include"controllers.hpp"
#include<bits/stdc++.h>
#include<filesystem>
void YeetStatus(){

}


/**
 * Initializes a .git directory with default configs
 *
 * @param path write the name of the directory should be entered according to pwd, see README for more on it or `.` The default is `.`
 * @attention For now, You can only make git init in the pwd, You can not do git init /home/abc/some/folder. This will not work. It will be executed but will give to rise complications.
 * @return Nothing for now
 * @author Akhil Sharma
 */
void YeetInit(std::string path="."){
    try
    {
        // std::cout<<path;
        // TODO: Consider all cases the user can enter here
        // He can enter . --> init in pwd ------> Will Workd
        // He can enter ebc --> init in pwd/ebc -------->
        // He can enter ebc/ --> init in pwd/ebc only not pwd/ebc/ ------->
        // He can enter full path from root --> init at that path ---------> will not work
        // Solution: For now, You can only make git init in the pwd, You can not do git init /home/abc/some/folder. This will not work. It will be executed but will give rise complications.
        std::string pwd = std::filesystem::current_path();
        std::string temp_pwd = pwd;
        std::string _actualPath=pwd+'/'+path+".git";
        if(path.back()!='/'&& path.back()!='.'){ _actualPath=pwd+'/'+path+"/.git"; temp_pwd=pwd+'/'+path;}
        if(path.back()=='.') _actualPath=pwd+"/.git"; 
        if(std::filesystem::exists(temp_pwd)){
            throw std::runtime_error("A git folder already exists in this directory. \n");
        }
        std::system("ls -l ");
        std::system("tree .");
        std::cout<<_actualPath<<std::endl;
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
        std::cerr << e.what() << '\n';
        // return EXIT_FAILURE;
    }
    
}

void YeetAdd(){

}