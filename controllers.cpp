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
/**
 * Lists all the files in the current yeet repo.
 */
void Commit::ListFiles(std::string path,std::vector<std::filesystem::path>&FilePath){
    for (const auto & entry : fs::directory_iterator(path)){
        // TODO: This is my .gitignore
        const bool IGNORE = entry.path().generic_string().find(".git") != std::string::npos || entry.path().generic_string().find(".yeet") != std::string::npos || entry.path().generic_string().find(".vscode") != std::string::npos || entry.path().generic_string().find(".xmake") != std::string::npos;

        if(IGNORE){
            continue;
        }
        if(entry.is_directory()) {
            ListFiles(entry.path(),FilePath);
        } 
        if(entry.is_directory()) {
            continue;;
        }
        FilePath.push_back(entry);
    }
}
void Commit::CommitMain(std::string path){
    try
    {
        std::vector<TreeEntry> TreeEntries;
        Database DbObj(Commit::path+"/.yeet/objects");
        Refs RefObj(Commit::path);
        std::vector<std::filesystem::path>FilePath;
        ListFiles(path,FilePath);
        for (const auto & entry : FilePath){
            std::string data = readFile(entry);
            Blob newBlobObject(data);
            DbObj.storeContentInDB(newBlobObject);
            TreeEntry TreeEntryObj(entry.generic_string(),newBlobObject.oid);
            TreeEntries.push_back(TreeEntryObj);
        }
        if (!TreeEntries.empty()) {
            Tree TreeObject(TreeEntries);
            DbObj.storeContentInDB(TreeObject);
            // std::cout << "My Tree Id is wao: " << TreeObject.oid << std::endl;

            std::string parent = RefObj.Read_HEAD(); // The oid of previous commit
            std::string name = getenv("YEET_AUTHOR_NAME");
            std::string email = getenv("YEET_AUTHOR_EMAIL");
            // std::cout<<"Name: "<<name<<"\nmail: "<<email<<"\n"; // working
            time_t currtime = time(nullptr);
            Author NewAuthorObj(name,email,currtime);
            std::string author = NewAuthorObj.to_stringg();
            std::string message; std::cout<<"\nPlease enter your Commit Message: \n";
            // std::cin>>message; // This doesn't takes any spaces " "
            std::getline(std::cin >> std::ws, message); // ws means white spaces.
            Commit MainCommitObj(TreeObject.oid,author,message,parent);
            DbObj.storeContentInDB(MainCommitObj);
            RefObj.update_HEAD(MainCommitObj.oid); // Updating the HEAD file to new commit

            // std::cout<<"the parent value: "<<parent<<std::endl;
            bool is_RootCommit = false;
            if(parent=="ref:") is_RootCommit=true;
            
            if(is_RootCommit)std::cout<<"\nThis is a root commit"<<std::endl;
            std::cout<<"Your Commit id is: "<<MainCommitObj.oid<<"\nCommit-Message: "<<MainCommitObj.CommitMessage<<"\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << '\n An error occured while commit your latest changes. \nError by e.what(): '  << e.what();
    }
    
    
}

Commit::Commit(std::string path){
    this->path = path;
}

Commit::Commit(std::string TreeOid, std::string AuthorData, std::string CommitMessage,std::string parent){
    this->AuthorData=AuthorData;
    this->TreeOID=TreeOid;
    this->CommitMessage=CommitMessage;
    this->parent=parent;
    this->Writtenlines = "parent: " + parent +  "\ntree: "+TreeOID+"\nauthor: "+AuthorData+"\nCommitedBy: "+AuthorData+"\n\nCommitMessage: "+CommitMessage;
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
    // std::cout<<"The blob oid is: "<<this->oid<<std::endl;
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

void Database::storeContentInDB(Blob& object){
    std::string Data = object.data;
    std::string content = object.type() + " " + std::to_string(Data.size()) + "\0" + Data; // The null character is included just to use when we itterate over it.
    object.oid = calculateSHA1Hex(content);
    // std::cout<<object.oid<<std::endl; // Hashes are coming out.
    write_object(object.oid,content); // Writing/ making directories of the commit object/blob
}

void Database::storeContentInDB(Tree& object){
    std::string Data = object.ReturnS_tring();
    std::string content = object.Type() + " " + std::to_string(Data.size()) + "\0" + Data; // The null character is included just to use when we itterate over it.
    // std::cout<<"the content: "<<content<<std::endl;
    object.oid = calculateSHA1Hex(content);
    // std::cout<<"The hash of the tree object is: "<<object.oid<<std::endl; // Hashes are coming out.
    write_object(object.oid,content); // Writing/ making directories of the commit object/blob
}

void Database::storeContentInDB(Commit& object){
    std::string Data = object.Writtenlines;
    std::string content = "Commit  \nData Size: " + std::to_string(Data.size()) + "\0 \n" + Data; // The null character is included just to use when we itterate over it.
    // TODO: I have added Data size as extra here for readablitliy, but it may create problem later.
    // std::cout<<"the content: "<<content<<std::endl;
    object.oid = calculateSHA1Hex(content);
    // std::cout<<"The hash of the Commit object is: "<<object.oid<<std::endl; // Hashes are coming out.
    write_object(object.oid,content); // Writing/ making directories of the commit object/blob
}

// Tree Class

/**
 * This function converts the vector<TreeEntry> to string
 * @return String, which will be used in storing the tree to Database.
 * The entries contains all the files commit info.
 */
std::string Tree::ReturnS_tring(){
    std::ostringstream result;

    // std::ostringstream result; // The ostringstream stands for output string stream just like ofstream
    // Using the 'result' stream to concatenate strings and numbers
    // result << "The answer to life, the universe, and everything is " << 42 << ".";
    // Sort entries by name

    std::vector<TreeEntry> sortedEntries = entries;
    // for(auto it:sortedEntries){
    //     std::cout<<it.name<<" "<<it.oid<<std::endl;
    // }
    std::sort(sortedEntries.begin(), sortedEntries.end(), [](const TreeEntry& a, const TreeEntry& b) {
        return a.name < b.name;
    });

    // for(auto it:sortedEntries){
    //     std::cout<<it.name<<" "<<it.oid<<std::endl;
    // }

    // Format entries
    for (const auto& entry : sortedEntries) {
        result << Tree::MODE << entry.name << "\0" << entry.oid;
    }

    return result.str();
}



// Helper Functions:
std::string timeToString(time_t currtime) { 
    std::stringstream ss; 
    ss << std::put_time(localtime(&currtime), "%Y-%m-%d %H:%M:%S"); // this is new to me.
    return ss.str();
}


std::string Directory_name_Helper(std::string Objpath){
    std::string ans="";
    ans+=Objpath[Objpath.size()-41];
    ans+=Objpath[Objpath.size()-40];
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

// std::string Compressing_using_zlib(std::string content){
//     z_stream stream;
//     stream.zalloc = nullptr;
//     stream.zfree = nullptr;
//     stream.opaque = nullptr;
//     stream.avail_in = content.size(); // input size
//     uLongf compressed_size = compressBound(content.size());
//     stream.avail_out = compressed_size; //assuming the compressed can become 3 times of og
//     stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(content.data())); // new for me, used copilot for this line; // input fil
//     Bytef* compressed_data = new Bytef[compressed_size];
//     stream.next_out = reinterpret_cast<unsigned char*>(compressed_data); // The output file type

//     // intializing compression
//     deflateInit(&stream, Z_DEFAULT_COMPRESSION); //The compression level must be Z_DEFAULT_COMPRESSION, or between 0 and 9: 1 gives best speed, 9 gives best compression, 0 gives no compression at all (the input data is simply copied a block at a time). Z_DEFAULT_COMPRESSION requests a default compromise between speed and compression (currently equivalent to level 6).  // from zlib manual.

//     // compress:
//     deflate(&stream, Z_BEST_COMPRESSION);

//     compressed_size = stream.total_out;

//     deflateEnd(&stream);
//     std::string compressed = reinterpret_cast<const char*>(compressed_data);
//     delete[] compressed_data; // releasing the memory of the buffer holding the compressed data
//     return compressed;
// }

// refractored Compression Logic
std::string Compressing_using_zlib(std::string& content) {
    if (content.empty()) return "";

    z_stream stream{};
    stream.zalloc = nullptr;
    stream.zfree = nullptr;
    stream.opaque = nullptr;
    stream.avail_in = static_cast<uInt>(content.size());
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(content.data()));

    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        throw std::runtime_error("Failed to initialize zlib deflate.");
    }

    std::vector<unsigned char> compressedData(compressBound(content.size()));
    stream.avail_out = compressedData.size();
    stream.next_out = compressedData.data();

    if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&stream);
        throw std::runtime_error("Failed to deflate data.");
    }

    compressedData.resize(stream.total_out);

    if (deflateEnd(&stream) != Z_OK) {
        throw std::runtime_error("Failed to finalize zlib deflate.");
    }

    return std::string(compressedData.begin(), compressedData.end());
}


// Author Class:
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
    return path + "/.yeet/HEAD";
}

// Updates the HEAD file to the latest commit
void Refs::update_HEAD(std::string oid){
    // std::cout<<path<<std::endl;
    std::ofstream headFile( path+"/.yeet/HEAD");
    if (headFile.is_open()) {
        headFile << oid;
        headFile.close();
    } else {
        throw std::runtime_error("Failed to open .yeet/HEAD file.\n");
    }
}

std::string Refs::Read_HEAD(){
    std::ifstream headFile(Refs::HEAD_path(),std::ios::binary);
    std::string FileContent; // Becuase I am using string, It will not pick up anything after a space. I need to use getLine() function.
    if(headFile){
        headFile>>FileContent; // All content of the file into the string
    }
    return FileContent;
}
