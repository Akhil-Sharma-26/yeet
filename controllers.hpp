#include<string>
#include<vector>
#include<filesystem>
void YeetAdd();

void YeetStatus();

void YeetInit(std::string path);

class Commit{
    private:
        
    public:
        std::string path;
        std::vector<std::string> IGNORE = {".","..",".git"};
        Commit(std::string path);
        void listFilesinDir(std::string path);
        std::string readFile(std::filesystem::path path);
    };

// class Blob{
//     public:
//         std::string oid;
//         std::string data;
//         Blob(std::string data);
//         std::string type();

// };