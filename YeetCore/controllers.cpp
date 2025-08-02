#include"include/controllers.hpp"
#include <corecrt_io.h>

#define fs std::filesystem

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

void ListFiles(std::string path, std::vector<std::filesystem::path>& FilePath) {
    // Copy implementation from controllers.cpp
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        
        // TODO: Make a file and get names from there
        // My .yeet-ignore
        const bool IGNORE = entry.path().generic_string().find(".git") != std::string::npos ||
        entry.path().generic_string().find(".yeet") != std::string::npos || 
                          entry.path().generic_string().find(".vscode") != std::string::npos || 
                          entry.path().generic_string().find(".xmake") != std::string::npos || 
                          entry.path().generic_string().find(".cmake") != std::string::npos || 
                          entry.path().generic_string().find("/build") != std::string::npos;

        if(IGNORE){
            continue; // SKip that
        }

        if(entry.is_directory()){
            ListFiles(entry.path().string(), FilePath); // Recurse into that directory
        }
        else {
            FilePath.push_back(entry);
        }
    }
}

std::string readFile(std::filesystem::path path) {
    // Copy implementation from controllers.cpp
    std::ifstream f(path, std::ios::in);
    const auto sz = std::filesystem::file_size(path);
    std::string result(sz, '\0');
    f.read(result.data(), sz);
    return result;
}


namespace CommitHelper{
    void YeetStatus(std::string path, std::vector<std::filesystem::path>& FilesWithChanges){
        // std::cout << "DEBUG: YeetStatus starting with path: " << path << std::endl;
    
        std::vector<std::filesystem::path> FilePath;
        
        // Getting list of all files
        // std::cout << "DEBUG: About to call ListFiles" << std::endl;
        ListFiles(path, FilePath);
        // std::cout << "DEBUG: ListFiles found " << FilePath.size() << " total files" << std::endl;
    
        // Making a visited map for later
        std::unordered_map<std::filesystem::path, bool> visited;
        for(auto it:FilePath){
            visited[it] = false;
        }
        
        std::string StoreData;
        std::fstream Store(path+"/.yeet/Store");
        // std::cout << "DEBUG: Opening Store file at: " << path+"/.yeet/Store" << std::endl;
        
        // Putting content of the Store file in the string StoreData
        if(Store.is_open()){
            std::string line;
            while (std::getline(Store, line)) {
                StoreData += line + "\n";
            }
            Store.close();
            // std::cout << "DEBUG: Store file contents: " << StoreData << std::endl;
        }
        else{
            std::cout<<"ERROR::STATUS::Error in opening Store File"<<std::endl;
            return;
        }
        rtrim(StoreData);
    
        if (StoreData == "Empty Store") {
            // std::cout << "DEBUG: Empty Store detected, adding all files" << std::endl;
            // Add all current files as changes for the initial commit
            FilesWithChanges = FilePath;
            // std::cout << "DEBUG: Added " << FilesWithChanges.size() << " files for initial commit" << std::endl;
            return;
        }
    
        int Totaladditions,Totaldeletions;
        Totaladditions = 0, Totaldeletions = 0;
    
        bool space = false;
        std::string PathofFile, oid;
        PathofFile = ""; oid = "";
        std::vector<std::string> FilePaths;
        std::vector<std::string> oids;
        std::istringstream storeStream(StoreData);

        std::string line;
        while (std::getline(storeStream, line)) {
            size_t tabPos = line.find('\t');
            if (tabPos != std::string::npos) {
                FilePaths.push_back(line.substr(0, tabPos));
                oids.push_back(line.substr(tabPos + 1));
            }
        }
    
        // Main Loop
        for(int i=0;i<oids.size();i++){
    
            int additions,deletions;
            additions = 0, deletions = 0;
            std::string thePathOfOid = "";
            std::string fileName = oids[i].substr(2, oids[i].size() - 2); 
            thePathOfOid = oids[i].substr(0, 2) + "/" + fileName;
    
            std::string FullPath = path + "/.yeet/objects/" + thePathOfOid;
    
            std::string InflatedContent = Inflate(FullPath);
    
            if (std::filesystem::exists(FilePaths[i])) {
                std::string NewFileContent="";
                std::ifstream NowFile(FilePaths[i]);
    
                if(NowFile.is_open()){
                    std::string line;
                    while(std::getline(NowFile,line)){
                        NewFileContent+=line+"\n";
                    }
                    NowFile.close();
                }
    
                // Calling Diffs algo here
                std::vector<std::string> NewFileinLines = splitIntoLines(NewFileContent);
                std::vector<std::string> OldFileinLines = splitIntoLines(InflatedContent);
    
                std::vector<std::vector<int>> trace;
                int ans = Shortest_Edit_Search(NewFileinLines, OldFileinLines, trace); 
    
                // std::cout<<ans<<std::endl;
                if(ans==0) {
                    // TODO: Don't add in commit
                    // std::cout<<"Files are identical."<<std::endl;
                    continue;
                }            
                
                std::vector<Edit> diff_result = diff(OldFileinLines, NewFileinLines, trace, ans);
    
                for(auto it:diff_result){
                    
                    // TODO: Add number of lines.
                    if(it.type == Edit::DEL) {
                        deletions++;
                        Totaldeletions++;
                    }
                    else if(it.type == Edit::INS) {
                        additions++;
                        Totaladditions++;
                    }
                }
    
                // Don't print exec file diffs.
    
                // TODO: Check exe using this method. ie using cpp filesystem library, it's cross platform the access function only works in linux.
                // Commit exec files also
                // if(std::filesystem::status(FilePath[i].c_str()).permissions() & std::filesystem::perms::owner_exec )
                // if(! access (FilePaths[i].c_str(), X_OK)){
                //     continue;
                // }
    
                // don't show file if nothing changed
                if(additions == 0 && deletions == 0){
                    continue;
                }
    
                FilesWithChanges.push_back(FilePaths[i]);
            }
        }
    }

}


bool isExecutableFile(const std::filesystem::path& path) {
#ifdef _WIN32
    // On Windows, check file extension
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(),
        [](unsigned char c) { return std::tolower(c); });

    // Common executable extensions on Windows
    return ext == ".exe" || ext == ".bat" || ext == ".cmd" ||
        ext == ".com" || ext == ".ps1" || ext == ".vbs";
#else
    // On Unix-like systems, use access
    return access(path.c_str(), X_OK) == 0;
#endif
}


namespace Helper{
    std::string readFile(std::string path){
        // Open the stream to 'lock' the file.
        std::ifstream f(path, std::ios::in);

        // Obtain the size of the file.
        const auto sz = fs::file_size(path);
        std::string result(sz, '\0');

        // Read the whole file into the buffer. 
        // TODO: This will fail at large files
        f.read(result.data(), sz);

        return result;
    }

    // For the branch namespace to store the commit id to the new branch file.
    void update_HEAD(std::string oid, std::string NewBranchPath){
        // std::cout<<path<<std::endl;
        std::ofstream headFile(NewBranchPath);
        if (headFile.is_open()) {
            headFile << oid;
            headFile.close();
        } else {
            throw std::runtime_error("ERROR::UPDATE::Failed to open .yeet/refs/heads/ file.\n");
        }
    }
}

void YeetStatus(std::string path){

    std::vector<std::filesystem::path> FilePath;
    ListFiles(path, FilePath);

    std::unordered_map<std::filesystem::path, bool> visited;
    for (const auto& it : FilePath) {
        visited[it] = false;
    }

    int Totaladditions = 0, Totaldeletions = 0;
    std::string StoreData;
    std::fstream Store(path + "/.yeet/Store");

    if (Store.is_open()) {
        std::string line;
        while (std::getline(Store, line)) {
            StoreData += line + "\n";
        }
        Store.close();
    } else {
        std::cout << "Error opening Store File" << std::endl;
        return;
    }
    rtrim(StoreData);
    if (StoreData == "Empty Store") {
        // Clear existing Diff file
        std::ofstream clearDiff(path + "/.yeet/Diff", std::ios::trunc);
        clearDiff.close();

        std::vector<Edit> all_edits;
        for (const auto& file : FilePath) {
            std::ifstream newFile(file);
            if (newFile.is_open()) {
                std::string content;
                std::string line;
                while (std::getline(newFile, line)) {
                    content += line + "\n";
                }
                newFile.close();
                std::vector<std::string> lines = splitIntoLines(content);
                for (const auto& l : lines) {
                    all_edits.emplace_back(Edit::INS, "", l);
                }
                Totaladditions += lines.size();
                std::cout << file << " (new file)" << std::endl;
            }
        }
        storeDiff(all_edits);
        std::cout << "Total additions: " << Totaladditions << "\n";
        std::cout << "No previous commit. All files are new.\n";
        return;
    }

    bool space = false;
    std::string PathofFile, oid;
    PathofFile = ""; oid = "";
    std::vector<std::string> FilePaths;
    std::vector<std::string> oids;
    std::istringstream storeStream(StoreData);

    std::string line;
    while (std::getline(storeStream, line)) {
        size_t tabPos = line.find('\t');
        if (tabPos != std::string::npos) {
            FilePaths.push_back(line.substr(0, tabPos));
            oids.push_back(line.substr(tabPos + 1));
        }
    }

    std::vector<Edit> all_edits;
    for (size_t i = 0;i<oids.size();i++) {

        int additions,deletions;
        additions = 0, deletions = 0;
        std::string thePathOfOid = "";
        std::string fileName = oids[i].substr(2, oids[i].size() - 2); 
        thePathOfOid = oids[i].substr(0, 2) + "/" + fileName;

        std::string FullPath = path + "/.yeet/objects/" + thePathOfOid;

        std::string InflatedContent = Inflate(FullPath);

        if (std::filesystem::exists(FilePaths[i])) {
            std::string NewFileContent="";
            std::ifstream NowFile(FilePaths[i]);

            if(NowFile.is_open()){
                std::string line;
                while(std::getline(NowFile,line)){
                    NewFileContent+=line+"\n";
                }
                NowFile.close();
            }

            // Call Diffs algo here
            std::vector<std::string> NewFileinLines = splitIntoLines(NewFileContent);
            std::vector<std::string> OldFileinLines = splitIntoLines(InflatedContent);

            std::vector<std::vector<int>> trace;
            int ans = Shortest_Edit_Search(NewFileinLines, OldFileinLines, trace); 

            // std::cout<<ans<<std::endl;
            if(ans==0) {
                // TODO: Don't add in commit
                // std::cout<<"Files are identical."<<std::endl;
                continue;
            }            
            
            std::vector<Edit> diff_result = diff(OldFileinLines, NewFileinLines, trace, ans);

            for(auto it:diff_result){
                
                // TODO: Add number of lines.
                if(it.type == Edit::DEL) {
                    deletions++;
                    Totaldeletions++;
                }
                else if(it.type == Edit::INS) {
                    additions++;
                    Totaladditions++;
                }
            }

            // Don't print exec file diffs.
            if (!isExecutableFile(FilePaths[i])) {
                continue;
            }

            // don't show file if nothing changed
            if(additions == 0 && deletions == 0){
                continue;
            }

            // The file we are checking:
            std::cout<<FilePaths[i]<<std::endl;
            all_edits.insert(all_edits.end(), diff_result.begin(), diff_result.end());
            std::cout<<"This file additions: "<<additions<<"\n";
            std::cout<<"This file deletions: "<<deletions<<std::endl;
            // Printing the diffs
            Printer printer;
            printer.print(diff_result);

            
        } else {
            deletions+=InflatedContent.size();
        }
        visited[FilePaths[i]] = true;
    }

    // Clear existing Diff file

    std::ofstream clearDiff(path + "/.yeet/Diff", std::ios::trunc);
    clearDiff.close();
    storeDiff(all_edits); // Write all collected diffs

    for (const auto& it : FilePath) {
        if (!visited[it]) {
            std::ifstream newFile(it);
            if (newFile.is_open()) {
                std::string line;
                while (std::getline(newFile, line)) {
                    Totaladditions++;
                }
                newFile.close();
            }
        }
    }

    if(Totaladditions == 0  && Totaldeletions == 0){
        std::cout<<"No Change, Can't commit"<<std::endl;
    }
    else{
        std::cout<<"Total addtions: "<<Totaladditions<<"\nTotal deletions: "<<Totaldeletions<<std::endl;
    }

    std::cout<<"DIFF:: All the changes have stored in the /.yeet/Diff file"<<std::endl;


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
        // He can enter . --> init in pwd ------> Will Work
        // He can enter ebc --> init in pwd/ebc --------> not work, you to mkdir ebc first
        // He can enter ebc/ --> init in pwd/ebc only not pwd/ebc/ -------> same as above
        // He can enter full path from root --> init at that path ---------> will not work
        std::string pwd = std::filesystem::current_path().string();
        std::string temp_pwd = pwd;
        std::string _actualPath=pwd+'/'+path+".yeet";
        if(path.back()!='/' && path.back()!='.'){ 
            _actualPath=pwd+'/'+path+"/.yeet";
            temp_pwd=pwd+'/'+path;
        }

        if(path.back()=='.') 
            _actualPath=pwd+"/.yeet"; 

        if(std::filesystem::exists(temp_pwd+"/.yeet"))
            throw std::runtime_error("ERROR::INIT::A yeet folder already exists in this directory. \n");
        
        // std::system("tree .");

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
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/HEAD file.\n");
            }

        std::ofstream masterBranch(_actualPath+"/refs/heads/master");
        if (masterBranch.is_open()) {
            masterBranch << "master";
            masterBranch.close();
        } else {
            throw std::runtime_error("ERROR::INIT::Failed to create .yeet/refs/heads/master file.\n");
        }

        // Making Description file.
        std::ofstream descFile(_actualPath+"/description");
            if(descFile.is_open()){
                descFile<<"Demo Description. This file contains the info and descriptio about the repository.\n";
                descFile.close();
            }
            else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/description file.\n");
            }
        
        // Making config file
        std::ofstream configFile(_actualPath+"/config");
            if(configFile.is_open()){
                // TODO: Find a configparser for C++. and replace the content of this configFile.
                configFile<<"Demo Config\n";
                configFile.close();
            }
            else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/config file.\n");
            }

        // Making Store File
        std::ofstream StoreFile(_actualPath+"/Store");
        if(StoreFile.is_open()){
            StoreFile<<"Empty Store";
            StoreFile.close();
        }
        else {
            throw std::runtime_error("ERROR::INIT::Failed to create .yeet/Store file.\n");
        }

        // Make Diff file.
        std::ofstream DiffFile(_actualPath+"/Diff");
        if (DiffFile.is_open()) {
            DiffFile << "No Diffs Yet";
            DiffFile.close();
        } else {
            throw std::runtime_error("ERROR::INIT::Failed to create .yeet/Diff file.\n");
        }

        // Make Current Branch file.
        std::ofstream BranchFile(_actualPath+"/Branch");
        if (BranchFile.is_open()) {
            BranchFile << "master";
            BranchFile.close();
        } else {
            throw std::runtime_error("ERROR::INIT::Failed to create .yeet/Diff file.\n");
        }

        std::cout << "YEET::Initialized yeet directory\n";
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    
}

void YeetAdd(){

}


void Commit::CommitMain(std::string path){
    try {
        // std::cout << "DEBUG: Starting CommitMain with path: " << path << std::endl;
        
        std::vector<TreeEntry> TreeEntries;
        Database DbObj(Commit::path+"/.yeet/objects");
        Refs RefObj(Commit::path);

        std::string message;
        std::cout << "\nCOMMIT::Please enter your Commit Message:\n";
        std::getline(std::cin >> std::ws, message);
        
        std::vector<std::filesystem::path> FilePath;
        // std::cout << "DEBUG: About to call YeetStatus" << std::endl;
        
        CommitHelper::YeetStatus(path, FilePath);
        // std::cout << "DEBUG: YeetStatus returned " << FilePath.size() << " files" << std::endl;
        
        if(FilePath.empty()) {
            std::cout << "ERROR::COMMIT:: Nothing to commit" << std::endl;
            return;
        }
        
        // Debug: Print all files being committed
        // std::cout << "DEBUG: Files to be committed:" << std::endl;
        for(const auto& file : FilePath) {
            std::cout << "  - " << file << std::endl;
        }

        for (const auto& entry : FilePath) {
            std::string _stat = "Non-Exe";
            if (!isExecutableFile(entry)) {
                _stat = "Exe";
            }
            // std::cout << "DEBUG: Processing file: " << entry << " (status: " << _stat << ")" << std::endl;
            
            std::string data = readFile(entry);
            // std::cout << "DEBUG: Read " << data.length() << " bytes" << std::endl;
            
            Blob newBlobObject(data);
            // std::cout << "DEBUG: Created blob with oid: " << newBlobObject.oid << std::endl;
            
            DbObj.storeContentInDB(newBlobObject, entry.generic_string());
            
            TreeEntry TreeEntryObj(entry.generic_string(), newBlobObject.oid, _stat);
            TreeEntries.push_back(TreeEntryObj);
        }

        // std::cout << "DEBUG: Store contents:" << std::endl;
        for(auto it : DbObj.Store) {
            std::cout << "  " << it.first << " -> " << it.second << std::endl;
        }
        // TODO: I changed this function output and Store datatype to map instead of unordered_map, is this causing the segmentation faults?
        // Save the store in /Store file
        writeStoreinDB(DbObj.Store);

        if (!TreeEntries.empty()) {
            Tree TreeObject(TreeEntries);
            DbObj.storeContentInDB(TreeObject);
            // std::cout << "My Tree Id is wao: " << TreeObject.oid << std::endl;

            std::string parent = RefObj.Read_HEAD(path); // The oid of previous commit
            std::string name = getenv("YEET_AUTHOR_NAME");
            std::string email = getenv("YEET_AUTHOR_EMAIL");
            // std::cout<<"Name: "<<name<<"\nmail: "<<email<<"\n"; // working
            time_t currtime = time(nullptr);
            Author NewAuthorObj(name,email,currtime);
            std::string author = NewAuthorObj.to_stringg();
            // std::cin>>message; // This doesn't takes any spaces " "
            
            Commit MainCommitObj(TreeObject.oid,author,message,parent);
            DbObj.storeContentInDB(MainCommitObj);
            RefObj.update_HEAD(MainCommitObj.oid); // Updating the HEAD file to new commit
            // std::cout<<"the parent value: "<<parent<<std::endl;
            bool is_RootCommit = false;
            if(parent=="master") is_RootCommit=true;
            std::cout<<"Commit added in the branch: "<<RefObj.currentBranch()<<std::endl;
            if(is_RootCommit) std::cout<<"\nThis is a root commit"<<std::endl;
            std::cout<<"COMMIT::Your Commit id is: "<<MainCommitObj.oid<<"\nCommit-Message: "<<MainCommitObj.CommitMessage<<"\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "\nERROR::COMMITMAIN:: An error occured while commit your latest changes. \nError by e.what(): "<< e.what();
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
 * Helper function to get the content of a file into a string
*  @param: `path` is of type fs::path. It needs the path to the file and then it reads all the content of it.
*  @return: a `string` with the content of the file;
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

void Database::storeContentInDB(Blob& object, const std::string& path){
    std::string Data = object.data;

    // TODO: make proper diff here
    // ! I am putiing only the data in the content to simplify the process of diff
    // std::string content = object.type() + " " + std::to_string(Data.size()) + "\0" + Data; // The null character is included just to use when we itterate over it.
    
    std::string content = Data;
    object.oid = calculateSHA1Hex(content);
    Store[path] = object.oid;
    
    // std::cout<<object.oid<<std::endl; // Hashes are coming out.
    write_object(object.oid,content); // Writing/ making directories of the commit object/blob
}

void Database::storeContentInDB(Tree& object){
    std::string Data = object.Return_String();
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
// std::string Tree::Return_String(){
//     std::ostringstream result;

//     // std::ostringstream result; // The ostringstream stands for output string stream just like ofstream
//     // Using the 'result' stream to concatenate strings and numbers
//     // result << "The answer to life, the universe, and everything is " << 42 << ".";
//     // Sort entries by name

//     std::sort(enteries.begin(), enteries.end(), [](const TreeEntry& a, const TreeEntry& b) {
//         return a.name < b.name;
//     });

//     for (const auto& entry : enteries) {
//         std::string FileStat = (entry.stat == "Exe") ? entry.EXE_MODE : entry.REGULAR_MODE;
//         result << FileStat << " " << entry.name << " " << entry.oid<< "\n";
//     }
//     // std::cout<<result.str()<<std::endl;
//     return result.str();
// }



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
        throw std::runtime_error("ERROR::COMPRESSION:: Failed to initialize zlib deflate.");
    }

    std::vector<unsigned char> compressedData(compressBound(content.size()));
    stream.avail_out = compressedData.size();
    stream.next_out = compressedData.data();

    if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&stream);
        throw std::runtime_error("ERROR::COMPRESSION:: Failed to deflate data.");
    }

    compressedData.resize(stream.total_out);

    if (deflateEnd(&stream) != Z_OK) {
        throw std::runtime_error("ERROR::COMPRESSION:: Failed to finalize zlib deflate.");
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
    std::string currBranch = Helper::readFile(path+"/.yeet/Branch");
    return path + "/.yeet/refs/heads/" + currBranch;
}

std::string Refs::currentBranch(){
    std::string currBranch = Helper::readFile(path+"/.yeet/Branch");
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

void writeStoreinDB(std::map<std::string, std::string> Store){
    for(auto it:Store){
        std::cout<<it.first<<" "<<it.second<<std::endl;
    }
    std::string _actualPath = fs::current_path().string();

    std::ofstream StoreFile(_actualPath+"/.yeet/Store");
    if(StoreFile.is_open()){
        for(auto it:Store){
            StoreFile << it.first << "\t" << it.second << "\n";
        }
    
        StoreFile.close();
    }

    else {
        throw std::runtime_error("ERROR::STORE:: Failed to create .yeet/Store file.\n");
    }
}

std::vector<unsigned char> readFile(const std::string& filename) {
    if(filename == "/home/akhil/dev/yeet/.yeet/objects/St/ore"){
        std::vector<unsigned char> res = {};
        return res;
    }  
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "ERROR::READINGFILE:: Cannot open file: " << filename << std::endl;
        return {};
    }
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::vector<unsigned char> decompressData(const std::vector<unsigned char>& compressedData) {
    z_stream strm = {};
    strm.total_in = strm.avail_in = compressedData.size();
    strm.next_in = (Bytef*)compressedData.data();

    std::vector<unsigned char> decompressedData(compressedData.size() * 2); // Start with double the input size

    if (inflateInit(&strm) != Z_OK) {
        std::cerr << "inflateInit failed" << std::endl;
        return {};
    }

    int ret;
    do {
        strm.avail_out = decompressedData.size() - strm.total_out;
        strm.next_out = (Bytef*)(decompressedData.data() + strm.total_out);

        ret = inflate(&strm, Z_NO_FLUSH);

        switch (ret) {
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                std::cerr << "inflate error: " << ret << std::endl;
                inflateEnd(&strm);
                return {};
            case Z_BUF_ERROR:
            case Z_OK:
                if (strm.avail_out == 0) {
                    // Output buffer is full, increase its size
                    decompressedData.resize(decompressedData.size() * 2);
                }
                break;
        }
    } while (ret != Z_STREAM_END);

    inflateEnd(&strm);
    decompressedData.resize(strm.total_out); // Resize to actual decompressed size
    return decompressedData;
}

std::string Inflate(std::string path){

    std::string inputFilename = path; 
    std::string response = "";

    std::cout<<inputFilename<<std::endl;
    if(inputFilename == "/home/akhil/dev/yeet/.yeet/objects/Store") return "";

    auto compressedData = readFile(inputFilename);
    if (compressedData.empty()) return "Error in compressed data";

    auto decompressedData = decompressData(compressedData);
    if (decompressedData.empty()) return "Error in decompresssion";

    for(auto it:decompressedData){
        response+=it;
    }

    return response;
}

// Diffs Algo

// A function to convert a string to differnt lines. like a vector of strings
std::vector<std::string> splitIntoLines(const std::string& str) {
    std::vector<std::string> lines;
    std::string temp;
    for (char c : str) {
        if (c == '\n') {
            lines.push_back(temp);
            temp.clear();
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) {
        lines.push_back(temp);
    }
    return lines;
}

int Shortest_Edit_Search(const std::vector<std::string>& a, const std::vector<std::string>& b, 
                         std::vector<std::vector<int>>& trace) {
    int n = a.size(), m = b.size();
    if (n == 0) return m;  // All insertions if `a` is empty
    if (m == 0) return n;  // All deletions if `b` is empty

    int max_distance = n + m;
    int diagonal_offset = max_distance;  // To shift diagonals into the array
    std::vector<int> diagonals(2 * max_distance + 1, -1);  // Track edit points for each diagonal
    trace.clear();

    diagonals[diagonal_offset + 1] = 0;  // Initialize diagonal k=1

    for (int d = 0; d <= max_distance; ++d) {
        for (int k = -d; k <= d; k += 2) {
            int x;
            if (k == -d || (k != d && diagonals[diagonal_offset + k - 1] < diagonals[diagonal_offset + k + 1])) {
                // Move down
                x = diagonals[diagonal_offset + k + 1];
            } else {
                // Move right
                x = diagonals[diagonal_offset + k - 1] + 1;
            }

            int y = x - k;  // Corresponding y-coordinate
            while (x < n && y < m && a[x] == b[y]) {
                ++x;
                ++y;
            }

            diagonals[diagonal_offset + k] = x;

            // If we've reached the end of both strings
            if (x >= n && y >= m) {
                trace.push_back(diagonals);  // Store the final state
                return d;
            }
        }
        trace.push_back(diagonals);  // Store the state for this edit distance
    }

    return -1;  // This should never happen
}



std::vector<Edit> diff(const std::vector<std::string>& a, 
                      const std::vector<std::string>& b,
                      const std::vector<std::vector<int>>& trace,
                      int d) {
    std::vector<Edit> result;
    if (trace.empty() || d < 0) return result;
    
    int x = a.size(), y = b.size();
    int offset = x + y;
    
    for (int i = d; i > 0; --i) {
        const std::vector<int>& v = trace[i];
        int k = x - y;
        
        bool down = (k == -i || (k != i && v[offset + k - 1] < v[offset + k + 1]));
        int k_prev = down ? k + 1 : k - 1;

        int x_prev = v[offset + k_prev];
        int y_prev = x_prev - k_prev;

        while (x > x_prev && y > y_prev) {
            if (a[x - 1] == b[y - 1]) {
                result.push_back(Edit(Edit::EQL, a[x - 1], b[y - 1]));
            } else {
                result.push_back(Edit(Edit::DEL, a[x - 1], ""));
                result.push_back(Edit(Edit::INS, "", b[y - 1]));
            }
            --x;
            --y;
        }

        while (x > x_prev) {
            result.push_back(Edit(Edit::DEL, a[x - 1], ""));
            --x;
        }

        while (y > y_prev) {
            result.push_back(Edit(Edit::INS, "", b[y - 1]));
            --y;
        }
    }

    while (x > 0) {
        result.push_back(Edit(Edit::DEL, a[x - 1], ""));
        --x;
    }

    while (y > 0) {
        result.push_back(Edit(Edit::INS, "", b[y - 1]));
        --y;
    }

    std::reverse(result.begin(), result.end());

    // for (const auto& edit : result) {
    //     std::cout << "Edit Type: " << (edit.type == Edit::EQL ? "EQL" : 
    //                                 (edit.type == Edit::INS ? "INS" : "DEL"))
    //             << ", Orig: " << edit.old_line
    //             << ", Updated: " << edit.new_line
    //             << std::endl;
    // }

    return result;
}


// Storing diffs in file
void storeDiff(const std::vector<Edit>& edits) {
    std::ofstream diff_file(".yeet/Diff", std::ios::trunc); 
    if (diff_file.is_open()) {
        for (const auto& edit : edits) {
            std::string tag = (edit.type == Edit::INS) ? "+" : "-";
            std::string old_line = edit.old_line.empty() ? "" : edit.old_line;
            std::string new_line = edit.new_line.empty() ? "" : edit.new_line;
            diff_file << tag << " " << std::setw(4) << old_line << " " << std::setw(4) << new_line << "    " << (old_line.empty() ? new_line : old_line) << std::endl;
        }
        diff_file.close();
    } else {
        std::cerr << "ERROR::DIFFS:: Unable to open file for writing diffs" << std::endl;
    }
}

namespace Branch{
    void createBranch(std::string BranchName, fs::path currPath){
        // check if the names is valid:
        if(BranchName.empty()) return;
        
        std::regex reg(R"(^\.|\/\.|\.\.|\/$|\.lock$|@\{|[\x00-\x20*:?\[\\^~\x7f])");

        if(std::regex_match(BranchName, reg)){
            // invalid name of the branch
            std::cout<<"ERROR::BRANCH:: Invalid name of the Branch"<<std::endl;
            return;
        }

        std::string actPath = currPath.string() + "/.yeet/refs/heads/" + BranchName;

        if(fs::exists(actPath)){
            std::cout<<"ERROR::BRANCH:: Branch with this name already exists"<<std::endl;
            return;
        }

        // TODO: store the current commit in the new file
        // get the oid, update the update_HEAD function of the REfs classs. make refs object.
        // then pass the branch name also to the function to get telll which branch it shoul upate. ig
        

        // updating the head file with the latest commit.
        // putting content of the master into the new branch file.

        // creating the new bracnh file:
        std::string PrevBranch = Helper::readFile(currPath.string()+"/.yeet/Branch");

        // TODO: add option to choose the base branch
        // ! Currently making the prev branch as base branch.
        
        std::string CommitID_ofPrevBranch = Helper::readFile(currPath.string()+"/.yeet/refs/heads/" + PrevBranch);
        // making the file for the new Branch and storing the prevBranch ID into it.
        
        // TODO: Test this function
        Helper::update_HEAD(CommitID_ofPrevBranch, actPath);

        std::ofstream f(actPath); // fstream is not capable of making new file, so use ofstream instead.
        if(f.is_open()){
            f<<CommitID_ofPrevBranch;
            f.close();
        }
        else {
            throw std::runtime_error("ERROR::BRANCH:: Failed to create new Branch file.\n");
        }       

    }
    void SeeBranches(std::filesystem::path path){
        std::filesystem::path BranchesDir= path.string() + "/.yeet/refs/heads";
        
        std::cout<<"BRANCH:: Your branches are: "<<std::endl<<std::endl;
        int count = 1;
        for (const auto& it : std::filesystem::directory_iterator(BranchesDir)) {
            std::cout << count <<" ";
            std::cout << it.path().filename().string() << " ";
            std::fstream ff(it.path());
            if(ff.is_open()){
                std::string line;
                while (std::getline(ff, line)) {
                    std::cout << line << std::endl;
                }
                ff.close();
            }
            count++;
        }

    }

    void currBranch(std::filesystem::path currPath){
        std::fstream ff(currPath.string() + "/.yeet/Branch");
        std::stringstream ss;
        if(ff.is_open()){
            ss << ff.rdbuf();
            ff.close();
        }
        std::cout<<"BRANCH:: Your Current Branch is: "<<ss.str()<<std::endl;
    }
}


namespace CheckOut{
    void SwitchBranch(std::filesystem::path path, std::string switchToBranchName){
        std::string actPath = path.string() + "/.yeet/refs/heads/" + switchToBranchName;

        if(fs::exists(actPath)){
            std::ifstream currentBranchFile(path.string() + "/.yeet/Branch");
            std::string currentBranch;
            if(currentBranchFile.is_open()){
                std::getline(currentBranchFile, currentBranch);
                currentBranchFile.close();
            }

            if(currentBranch == switchToBranchName){
                std::cout<<"CHECKOUT::Already in the branch "<<switchToBranchName<<std::endl;
                return;
            }

            std::ofstream branchFile(path.string() + "/.yeet/Branch", std::ios::trunc);
            if(branchFile.is_open()){
                branchFile << switchToBranchName;
                branchFile.close();
                std::cout<<"CHECKOUT::Successfully Changed the branch to: "<<switchToBranchName<<std::endl;
            } else {
                std::cerr<<"ERROR::CHECKOUT::Failed to update the branch file"<<std::endl;
            }
        }
        else{
            std::cout<<"ERROR::CHECKOUT::Branch "<<switchToBranchName<<" does not exist"<<std::endl;
        }
    }
}

