#include "include/commit.hpp"

namespace CommitHelper{
    std::string relative_path(const fs::path& full_path, const std::string& root_path) {
        try {
            return fs::relative(full_path, root_path).generic_string();
        } catch(...) {
            return full_path.generic_string(); // Fallback
        }
    }

    bool YeetStatus(std::string path, std::vector<fs::path>& FilesWithChanges){
    
        std::vector<fs::path> DiskPaths;

        // Gets a complete list of files in the directory rn
        ListFiles(path, DiskPaths);

        // Map for fast lookup of current files on disk (Normalized Path -> Actual Path)
        std::map<std::string, fs::path> diskFileMap;
        for(const auto &it : DiskPaths){
            diskFileMap[relative_path(it, path)] = it;
        }
        
        std::string StoreData;
        fs::path storePath = fs::path(path) / ".yeet" / "Store";
        std::fstream Store(storePath); // store file contains the file paths and oids from the prev commit
    
        // Putting content of the Store file in the string StoreData
        if(Store.is_open()){
            std::string line;
            while (std::getline(Store, line)) {
                StoreData += line + "\n";
            }
            Store.close();
        }
        
        // If no store, everything is new
        if (StoreData.empty() || StoreData == "Empty Store") {
            FilesWithChanges = DiskPaths;
            return !DiskPaths.empty();
        }
        rtrim(StoreData);

        // bool space = false;
        std::vector<std::string> StorePaths;
        std::vector<std::string> StoreOids;
        std::istringstream storeStream(StoreData);
        std::string line;

        while (std::getline(storeStream, line)) {
            size_t tabPos = line.find('\t');
            // Fallback for space-separated if tab is missing (handles copy-paste errors)
            if (tabPos == std::string::npos) tabPos = line.find(' ');

            if (tabPos != std::string::npos) {
                StorePaths.push_back(line.substr(0, tabPos));
                StoreOids.push_back(line.substr(tabPos + 1));
            }
        }
    
        // Main Loop
        for(size_t i=0; i<StoreOids.size(); i++){
            std::string relStorePath = StorePaths[i];
            
            // Handle absolute paths in store by converting them
            if(relStorePath.find(path) == 0) {
                relStorePath = relative_path(fs::path(relStorePath), path);
            }

            if(diskFileMap.count(relStorePath)){
                // File still exists on disk. Add to snapshot.
                FilesWithChanges.push_back(diskFileMap[relStorePath]);
            }
        }

        // 3. Check New Files (Iterate Disk)
        std::unordered_set<std::string> storePathSet;
        for(const auto& sp : StorePaths) {
            if(sp.find(path) == 0) storePathSet.insert(relative_path(fs::path(sp), path));
            else storePathSet.insert(sp);
        }

        for(const auto& it : DiskPaths){
            if(storePathSet.find(relative_path(it, path)) == storePathSet.end()){
                FilesWithChanges.push_back(it); // New File
            }
        }

        return !FilesWithChanges.empty();
    }

    // This function brings the oid of the Tree object from the .yeet/object
    std::string getTreeOidFromCommit(const std::string CommitContent){
        std::istringstream st(CommitContent);
        std::string line;

        while(std::getline(st, line)){
            // in the commit obj the tree oid is stored as tree <oid>
            if(line.rfind("tree", 0) == 0){
                return line.substr(5);
            }
        }

        return "";
    }

    std::string trimm(const std::string& str){
        const std::string ignr = " \n\r\t\f\v";
        size_t first = str.find_first_not_of(ignr);

        if(std::string::npos == first){
            return str;
        }

        size_t last = str.find_last_not_of(ignr);
        return str.substr(first, (last-first+1));
    }

}

void Commit::CommitMain(){
    try {
        std::vector<TreeEntry> TreeEntries;
        Database DbObj(fs::path(Commit::path+"/.yeet/objects").string());
        
        Refs RefObj(Commit::path);
        
        std::vector<fs::path> FilePath;
        
        if(!CommitHelper::YeetStatus(path, FilePath)){
            std::cout << "ERROR::COMMIT:: No files found to track." << std::endl;
            return;
        }
        
        for (const auto& entry : FilePath) {
            std::string _stat = isExecutableFile(entry) ? "Exe" : "Non-Exe";
            
            std::string data = readFile(entry.string());
            Blob newBlobObject(data);
            
            // Convert to relative path (e.g., "src/main.cpp")
            std::string relPath = CommitHelper::relative_path(entry, path);

            // Store RELATIVE path in the DB Store and Tree
            DbObj.storeContentInDB(newBlobObject, relPath);
            TreeEntry TreeEntryObj(relPath, newBlobObject.oid, _stat);
            TreeEntries.push_back(TreeEntryObj);
        }

        // Save the store in /Store file
        writeStoreinDB(DbObj.Store);

        if (!TreeEntries.empty()) {
            Tree TreeObject(TreeEntries);
            DbObj.storeContentInDB(TreeObject);

            std::string parent = RefObj.Read_HEAD(path); // The oid of previous commit

            if(!parent.empty() && parent != "master"){
                fs::path parentCommitPath = fs::path(path) / ".yeet" / "objects" / parent.substr(0, 2) / parent.substr(2);
                
                std::string parentCommitContent = Inflate(parentCommitPath.string());
                std::string parentTreeOid = CommitHelper::getTreeOidFromCommit(parentCommitContent);
                std::string currentTreeOid = TreeObject.oid;
                std::string prevTreeOid = CommitHelper::trimm(parentTreeOid);

                if (TreeObject.oid == CommitHelper::trimm(parentTreeOid)) {
                    std::cout << "Nothing to commit, working tree is clean." << std::endl;
                    return; 
                }
            }

            std::string name, email;
            std::string authpath = (fs::path(path) / ".yeet" / "Auth").string();

            // Parsing auth file
            {
                std::ifstream auth(authpath);
                if(auth.is_open()){
                    auto read_line = [&](std::string &out)->bool{
                        std::string tmp;
                        if(!std::getline(auth, tmp)) return false;
                        if(!tmp.empty() && tmp.back() == '\r') tmp.pop_back(); // handle CRLF
                        out = CommitHelper::trimm(tmp);
                        return true;
                    };

                    if(!read_line(name)){
                        throw std::runtime_error("Auth file missing name (first line): " + authpath + "\n");
                    }
                    if(!read_line(email)){
                        throw std::runtime_error("Auth file missing email (second line): " + authpath + "\n");
                    }
                }

                
                else {
                    // Fallback if Auth file missing
                    std::cout<<"Please reinitalize your repo or Edit /.yeet/Auth File, because I can't parse your name and email"<<std::endl;
                    name = "Unknown"; email = "unknown@yeet.com";
                }
            }

            time_t currtime = time(nullptr);
            Author NewAuthorObj(name,email,currtime);
            std::string author = NewAuthorObj.to_stringg();

            std::string message;
            std::cout << "\nCOMMIT::Please enter your Commit Message:\n";
            std::getline(std::cin, message);
            std::cout<<std::endl;
            
            Commit MainCommitObj(TreeObject.oid,author,message,parent);
            DbObj.storeContentInDB(MainCommitObj);
            RefObj.update_HEAD(MainCommitObj.oid); // Updating the HEAD file to new commit
            bool is_RootCommit = false;
            if(parent=="master") is_RootCommit=true;
            std::cout<<"Commit added in the branch: "<<RefObj.currentBranch()<<std::endl;
            if(is_RootCommit) std::cout<<"\nThis is a root commit"<<std::endl;
            std::cout<<"COMMIT::Your Commit id is: "<<MainCommitObj.oid<<"\nCommit-Message: "<<MainCommitObj.CommitMessage<<"\n";
        }
        else{
            std::cerr<<"ERROR::COMMIT:: Tree Was Empty!"<<std::endl;
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
    this->Writtenlines = "\nparent: " + parent +  "\ntree: "+TreeOID+"\nauthor: "+AuthorData+"\nCommitedBy: "+AuthorData+"\n\nCommitMessage: "+CommitMessage;
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
