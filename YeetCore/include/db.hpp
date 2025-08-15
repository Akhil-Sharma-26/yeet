#pragma once

#include"../../YeetUtils/include/libs.hpp"
#include"../../YeetUtils/include/file_utils.hpp"
#include"../../YeetUtils/include/compression_utils.hpp"
#include"../../YeetUtils/include/hash_utils.hpp"
#include"tree.hpp"
#include"commit.hpp"

// Forward declarations
class Commit;

class Blob{
    public:
        std::string oid;
        std::string data;
        // Will not work, As I have to make a custom hash function for this
        // std::unordered_set<std::pair<std::string, std::string>> BlobStore; 
        Blob(std::string data);
        std::string type();
};

class Database{
    private:
        /**
         *  writing the object to disk
         *  @param `oid`: The object id that is the hexadecimal hash of file content.
         *  @param `content`: The content of the file
         */
        void write_object(std::string oid,std::string content);
    public:
        std::filesystem::path path;
        std::map<std::string, std::string> Store;
        Database(std::filesystem::path path);
        // Its storing the blob store also. ie the pair of file path and its oid.
        void storeContentInDB(Blob& object,const std::string &path);
        void storeContentInDB(Tree& object);
        void storeContentInDB(Commit& object);
};