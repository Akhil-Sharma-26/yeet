#include"controllers.hpp"
#include<bits/stdc++.h>
#include<filesystem>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
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
// Commit Class:
// TODO: Add a check that you can only list files if a .yeet dir is present/ initialized.
void Commit::CommitMain(std::string path){
    for (const auto & entry : fs::directory_iterator(path)){
        // TODO: This is my .gitignore
        const bool IGNORE = entry.path().generic_string().find(".git") != std::string::npos || entry.path().generic_string().find(".yeet") != std::string::npos || entry.path().generic_string().find(".vscode") != std::string::npos || entry.path().generic_string().find(".xmake") != std::string::npos;

        if(IGNORE){
            continue;
        }
        if(entry.is_directory()) {
            CommitMain(entry.path());
        } 
        if(entry.is_directory()) {
            continue;;
        }
        // std::cout << entry.path() << std::endl;

        std::string data = readFile(entry.path());
        Blob newBlobObject(data);
        Database DbObj(Commit::path+"/.yeet/objects");
        DbObj.storeContentInDB(newBlobObject);
    }
}
Commit::Commit(std::string path){
    this->path = path;
}

/**
*  @param: `path` is of type fs::path. It needs the path to the file and then it reads all the content of it.
*/
std::string Commit::readFile(fs::path path){
    // Open the stream to 'lock' the file.
    std::ifstream f(path, std::ios::in);

    // Obtain the size of the file.
    const auto sz = fs::file_size(path);
    std::string result(sz, '\0');

    // Read the whole file into the buffer.
    f.read(result.data(), sz);

    return result;
}

// Blob Class
Blob::Blob(std::string newdata){
    this->data = newdata;
}
std::string Blob::type(){
    return "blob";
}

// Database Class
Database::Database(std::filesystem::path path){
    this->path = path;
}

// Creating Hash
std::string calculateSHA1Hex(const std::string& content) { // used some copilot
    CryptoPP::SHA1 sha1;
    std::string hash;
    // Create a filter that calculates the SHA1 hash and then encodes it as hexadecimal
    CryptoPP::StringSource(content, true, new CryptoPP::HashFilter(sha1, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash),false)));

    return hash;
}

void Database::storeContentInDB(Blob object){
    std::string Data = object.data;
    std::string content = object.type() + " " + std::to_string(Data.size()) + "\0" + Data; // The null character is included just to use when we itterate over it.
    object.oid = calculateSHA1Hex(content);
    // std::cout<<object.oid<<std::endl; // Hashes are coming out.
    write_object(object.oid,content); // Writing/ making directories of the commit object/blob
}


// Helper Functions:
std::string Directory_name_Helper(std::string Objpath){
    std::string ans="";
    ans+=Objpath[Objpath.size()-36];
    ans+=Objpath[Objpath.size()-37];
    return ans;
}

std::string File_name_Helper(std::string Objpath){
    std::string ans="";
    for(int i=Objpath.size()-1;i>=0;i--){
        if(Objpath[i]=='/'){
            break;
        }
        ans+=Objpath[i];
    }
    std::string act_ans="";
    for(int i=ans.size()-1;i>=0;i--){
        act_ans+=ans[i];
    }
    return act_ans;
}

std::string Compressing_using_zlib(std::string content){
    z_stream stream;
    stream.zalloc = nullptr;
    stream.zfree = nullptr;
    stream.opaque = nullptr;
    stream.avail_in = content.size(); // input size
    stream.avail_out = content.size() * 1LL*3; //assuming the compressed can become 3 times of og
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(content.data())); // new for me, used copilot for this line; // input fil
    unsigned long compressed_size = content.size()*3;
    Bytef* compressed_data = new Bytef[compressed_size];
    stream.next_out = reinterpret_cast<unsigned char*>(compressed_data); // The output file type

    // intializing compression
    deflateInit(&stream, Z_DEFAULT_COMPRESSION); //The compression level must be Z_DEFAULT_COMPRESSION, or between 0 and 9: 1 gives best speed, 9 gives best compression, 0 gives no compression at all (the input data is simply copied a block at a time). Z_DEFAULT_COMPRESSION requests a default compromise between speed and compression (currently equivalent to level 6).  // from zlib manual.

    // compress:
    deflate(&stream, Z_BEST_SPEED);

    compressed_size = stream.total_out;

    deflateEnd(&stream);
    std::string compressed = reinterpret_cast<const char*>(compressed_data);
    delete[] compressed_data; // releasing the memory of the buffer holding the compressed data
    return compressed;
}