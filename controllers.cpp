#include"controllers.hpp"
#include<bits/stdc++.h>
#include<filesystem>
#define fs std::filesystem
void YeetStatus(){

}


/**
 * Initializes a .yeet directory with default configs
 *
 * @param path write the name of the directory should be entered according to pwd, see README for more on it or `.` The default is `.`
 * @attention For now, You can only init in the directory which has already been made.
 * @return Nothing for now
 * @author Akhil Sharma
 */
void YeetInit(std::string path="."){
    try
    {
        // std::cout<<path;
        // TODO: Consider all cases the user can enter here
        // He can enter . --> init in pwd ------> Will Workd
        // He can enter ebc --> init in pwd/ebc --------> not work, you to mkdir ebc first
        // He can enter ebc/ --> init in pwd/ebc only not pwd/ebc/ -------> same as above
        // He can enter full path from root --> init at that path ---------> will not work
        std::string pwd = std::filesystem::current_path();
        std::string temp_pwd = pwd;
        std::string _actualPath=pwd+'/'+path+".yeet";
        if(path.back()!='/'&& path.back()!='.'){ _actualPath=pwd+'/'+path+"/.yeet"; temp_pwd=pwd+'/'+path;}
        if(path.back()=='.') _actualPath=pwd+"/.yeet"; 
        std::cout<<"temp: "<<temp_pwd<<std::endl;
        if(std::filesystem::exists(temp_pwd+"/.yeet")){
            throw std::runtime_error("A yeet folder already exists in this directory. \n");
        }
        std::system("ls -a");
        std::system("tree .");
        std::cout<<"actual: "<<_actualPath<<std::endl;
        std::filesystem::create_directory(_actualPath);
        std::filesystem::create_directory(_actualPath+"/objects");
        std::filesystem::create_directory(_actualPath+"/refs");
        std::filesystem::create_directory(_actualPath+"/refs/heads");
        std::filesystem::create_directory(_actualPath+"/refs/tags");

        // Make HEAD file.
        std::ofstream headFile(_actualPath+"/HEAD");
            if (headFile.is_open()) {
                headFile << "ref: refs/heads/main\n";
                headFile.close();
            } else {
                throw std::runtime_error("Failed to create .yeet/HEAD file.\n");
            }

        // Making Description file.
        std::ofstream descFile(_actualPath+"/description");
            if(descFile.is_open()){
                descFile<<"Demo Description. This file contains the info and descriptio about the repository.\n";
                descFile.close();
            }
            else {
                throw std::runtime_error("Failed to create .yeet/description file.\n");
            }
        
        // Making config file
        std::ofstream configFile(_actualPath+"/config");
            if(configFile.is_open()){
                // TODO: Find a configparser for C++. and replace the content of this configFile.
                configFile<<"Demo Config\n";
                configFile.close();
            }
            else {
                throw std::runtime_error("Failed to create .yeet/config file.\n");
            }
        std::cout << "Initialized yeet directory\n";
    }
    catch(const std::exception& e)
    {
        
        std::cerr << e.what() << '\n';
        // return EXIT_FAILURE;
    }
    
}

void YeetAdd(){

}

// TODO: Add a check that you can only list files if a .yeet dir is present/ initialized.
void Commit::listFilesinDir(std::string path){
    for (const auto & entry : fs::directory_iterator(path)){
        // TODO: This is my .gitignore
        const bool IGNORE = entry.path().generic_string().find(".git") != std::string::npos || entry.path().generic_string().find(".yeet") != std::string::npos || entry.path().generic_string().find(".vscode") != std::string::npos;

        if(IGNORE){
            continue;
        }
        if(entry.is_directory()) {
            listFilesinDir(entry.path());
        } 
        if(entry.is_directory()) {
            continue;;
        }
        std::cout << entry.path() << std::endl;

        std::string data = readFile(entry.path());
        
    }
}
Commit::Commit(std::string path){
    this->path = path;
}

/**
*  @param: `path` is of type fs::path. It needs the path to the file and then it reads all the content of it.
*/
std::string Commit::readFile(fs::path path){{
    // Open the stream to 'lock' the file.
    std::ifstream f(path, std::ios::in);

    // Obtain the size of the file.
    const auto sz = fs::file_size(path);

    // Create a buffer.
    std::string result(sz, '\0');

    // Read the whole file into the buffer.
    f.read(result.data(), sz);

    return result;
}

}