#pragma once

#include"../../YeetUtils/include/libs.hpp"
#include"../../YeetUtils/include/file_utils.hpp"
#include"../../YeetUtils/include/hash_utils.hpp"
#include"../../YeetUtils/include/compression_utils.hpp"

class Author{
    public:
        std::string name;
        std::string email;
        time_t time;
        Author(std::string name,std::string email,time_t& time){
            this->email=email;
            this->name=name;
            this->time=time;
        }
        std::string to_stringg();
};


// For History:
class Refs{
    public:
        std::string path;
        std::string commit_oid;
        Refs(std::string path);
        void update_HEAD(std::string commit_oid);
        std::string HEAD_path();
        std::string Read_HEAD();
        std::string currentBranch();
};
