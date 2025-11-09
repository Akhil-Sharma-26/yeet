#include"include/refs_auth.hpp"

std::string Author::to_stringg(){
    std::string thecurrTime=timeToString(time);
    return name+" <"+email+"> "+ thecurrTime;
}


// History
// Refs Class:
// Constructor of Ref Class
Refs::Refs(std::string path){
    this->path = path;
}

// @return the path to the HEAD file
std::string Refs::HEAD_path(){
    std::string currBranch = Helper::readFile(fs::path(path+"/.yeet/Branch"));
    return path + "/.yeet/refs/heads/" + currBranch;
}

std::string Refs::currentBranch(){
    std::string currBranch = Helper::readFile(fs::path(path+"/.yeet/Branch"));
    return currBranch;
}

// TODO: Updates the HEAD file to the latest commit according to the branch

void Refs::update_HEAD(std::string oid){
    // std::cout<<path<<std::endl;
    std::ofstream headFile(HEAD_path());
    if (headFile.is_open()) {
        headFile << oid;
        headFile.close();
    } else {
        throw std::runtime_error("ERROR::HEADUPDATE:: Failed to open .yeet/refs/heads/ file.\n");
    }
}

std::string Refs::Read_HEAD(std::string currPath){
    std::ifstream headFile(currPath + "/.yeet/refs/heads/" + currentBranch(),std::ios::binary);
    std::string FileContent; // Becuase I am using string, It will not pick up anything after a space. I need to use getLine() function.
    if(headFile){
        headFile>>FileContent; // All content of the file into the string
    }
    return FileContent;
}

bool Refs::show_history(){

}