#include<string>
#include<vector>
#include<filesystem>
#include<zlib.h> // for database class
#include<cryptopp/sha.h> // for database class
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip> // for input/output manipulators
#include<algorithm>

void YeetAdd();

void YeetStatus();

void YeetInit(std::string path);

class Tree;
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
        std::string TreeOID;
        std::string AuthorData;
        std::string CommitMessage;
        std::string Writtenlines;
        std::string oid;
        std::string parent;
        std::vector<std::string> IGNORE = {".","..",".git"};
        Commit(std::string path);
        Commit(std::string TreeOid, std::string AuthorData, std::string CommitMessage, std::string parent);
        void CommitMain(std::string path);
        void ListFiles(std::string path,std::vector<std::filesystem::path>& FilePath);
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
std::string Compressing_using_zlib(std::string& content);
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
                // std::cout<<"The obj path"<<obj_path<<std::endl;
                std::string Dir_name = Directory_name_Helper(obj_path);
                /** Actual File Path to the object created. */
                std::string File_Path = (this->path.generic_string()+"/"+Dir_name+"/"+File_name_Helper(obj_path)).c_str();
                if(std::filesystem::exists(File_Path)) return;
                // std::cout<<"Hello, I am the directory: "<<Dir_name<<std::endl;
                std::filesystem::create_directory(this->path.generic_string()+"/"+Dir_name);
                /** res contains the return value of the `touch` command. */
                int res = std::system(("touch " + this->path.generic_string()+"/"+Dir_name+"/"+File_name_Helper(obj_path)).c_str());
                if(res != 0) std::runtime_error("touch is not working \n");
                // std::cout<<"Hello, I am the File: "<<this->path.generic_string()+"/"+Dir_name+"/"+File_name_Helper(obj_path).c_str()<<std::endl;
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
        void storeContentInDB(Blob& object);
        void storeContentInDB(Tree& object);
        void storeContentInDB(Commit& object);
};

/**
 * An TreeEntry is a simple structure that exists to package up the information that Tree needs to
know about its contents: the filename, and the object ID. Tree will also need to know the mode
of each file, but for now all our source code is in non-executable regular files, so we will hard-
code the 100644 mode string that appears in the tree file.
 */
class TreeEntry {
public:
    // operator overloaded
    TreeEntry& operator=(const TreeEntry& other) {
        if (this != &other) { // Protect against self-assignment
            this->name = other.name;
            this->oid = other.oid;
            this->stat = other.stat;
        }
        return *this;
    }
    // TreeEntry(const std::string& name, const std::string& oid) : name(name), oid(oid) {} // same as below
    TreeEntry(std::string name, std::string oid,std::string stat) {
        this->name = name;
        this->oid = oid;
        this->stat = stat;
    }
    
    // copy constructor
    TreeEntry(const TreeEntry& other) : name(other.name), oid(other.oid), stat(other.stat) {}
    std::string name;
    std::string oid;
    // TODO: Customize these codes later for yeet.
    const std::string REGULAR_MODE = "100644";
    const std::string EXE_MODE = "100755";

    std::string stat;
    
};


class Tree{
    // const std::string ENTRY_FORMAT = "Z*H40"; no need
    public:

        std::string oid;
        std::vector<TreeEntry> entries;
        Tree(std::vector<TreeEntry>& entries){
            this->entries = entries;
        }

        std::string Type(){ return "tree"; }
        std::string ReturnS_tring();
};


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
        std::string oid;
        Refs(std::string path);
        void update_HEAD(std::string oid);
        std::string HEAD_path();
        std::string Read_HEAD();
};




        