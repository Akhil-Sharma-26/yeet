#include<string>
#include<vector>
#include<filesystem>
#include<zlib.h> // for database class
#include<cryptopp/sha.h> // for database class


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
        void listFilesinDir(std::string path);
        std::string readFile(std::filesystem::path path);
    };



class Database{
    private:
        void write_object(std::string oid,std::string content);
    public:
        std::filesystem::path path;
        Database(std::filesystem::path path);
        void storeContentInDB(Blob object);
};