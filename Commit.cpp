#include<bits/stdc++.h>
#include <filesystem>
#define fs std::filesystem
class Commit{
    private:
        
    public:
        std::string path;
        std::vector<std::string> IGNORE = {".","..",".git"};
        Commit(std::string path);
        ~Commit();
        void listFilesinDir(std::string path);
    };

Commit::Commit(std::string path)
{
    this->path = path;
}

Commit::~Commit()
{
}

void Commit::listFilesinDir(std::string path){
    for (const auto & entry : fs::directory_iterator(path)){
        // TODO: This is the function of .gitignore
        if(entry.path().generic_string().find(".git") != std::string::npos || entry.path().generic_string().find(".yeet") != std::string::npos || entry.path().generic_string().find(".vscode") != std::string::npos){
            continue;
        }
        std::cout << entry.path() << std::endl;
        if(entry.is_directory()) {
            listFilesinDir(entry.path());
        } 
    }
}
