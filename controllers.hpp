#include<string>
#include<vector>
#include<filesystem>
#include<zlib.h> // for database class
#include<cryptopp/sha.h> // for database class
#include<iostream>
#include<fstream>

void YeetAdd();

void YeetStatus();

void YeetInit(std::string path);

class Blob{
    public:
        std::string oid;
        std::string data;
        Blob(std::string data);
        std::string type();
};

class Commit{
    public:
        std::string path;
        std::vector<std::string> IGNORE = {".","..",".git"};
        Commit(std::string path);
        void CommitMain(std::string path);
        std::string readFile(std::filesystem::path path);
    };

std::string Directory_name_Helper(std::string Objpath);
std::string File_name_Helper(std::string Objpath);
/**
 * The below code is setup of zlib compression
    https://www.zlib.net/manual.html#Basic
    @param the content of be compressed
    @return The compressed string
 */
std::string Compressing_using_zlib(std::string content);
class Database{
    private:
        /**
         *  writing the object to disk
         *  @param `oid`: The object id that is the hexadecimal hash of file content.
         *  @param `content`: The content of the file
         */
        void write_object(std::string oid,std::string content){
            try
            {
                std::string obj_path = this->path.generic_string() + "/" + oid.substr(0,2) + "/" + oid.substr(2,oid.size()-1);
                // std::cout<<obj_path<<std::endl;
                std::string Dir_name = Directory_name_Helper(obj_path);
                std::cout<<"Hello, I am the directory: "<<Dir_name<<std::endl;
                std::filesystem::create_directory(this->path.generic_string()+"/"+Dir_name);
                int res = std::system(("touch " + this->path.generic_string()+"/"+Dir_name+"/"+File_name_Helper(obj_path)).c_str());
                if(res != 0) std::runtime_error("touch is not working \n");
                std::cout<<"Hello, I am the File: "<<this->path.generic_string()+"/"+Dir_name+"/"+File_name_Helper(obj_path).c_str()<<std::endl;
                // Compressing the content
                std::string compressed_data = Compressing_using_zlib(content);
                std::ofstream f(this->path.generic_string()+"/"+Dir_name+"/"+File_name_Helper(obj_path),std::ios::out | std::ios::binary);
                if(f.is_open()){
                    f.write(compressed_data.c_str(),compressed_data.size());
                    f.close();
                }
                else{
                    std::runtime_error("Unable to write in Commit Blob Files!!");
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    public:
        std::filesystem::path path;
        Database(std::filesystem::path path);
        void storeContentInDB(Blob object);
};