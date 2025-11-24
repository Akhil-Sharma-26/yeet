#include "include/commit.hpp"

namespace CommitHelper{
    std::string normalize_path(const fs::path& p) {
        return fs::absolute(p).lexically_normal().generic_string();
    }

    bool YeetStatus(std::string path, std::vector<fs::path>& FilesWithChanges){
        // std::cout << "DEBUG: YeetStatus starting with path: " << path << std::endl;
    
        std::vector<fs::path> DiskPaths;
        
        // Getting list of all files
        // std::cout << "DEBUG: About to call ListFiles" << std::endl;

        // Gets a complete list of files in the directory rn
        ListFiles(path, DiskPaths);

        // std::cout << "DEBUG: ListFiles found " << DiskPath.size() << " total files" << std::endl;

        // Map for fast lookup of current files on disk (Normalized Path -> Actual Path)
        std::map<std::string, fs::path> diskFileMap;
        for(const auto &it : DiskPaths){
            diskFileMap[normalize_path(it)] = it;
        }
        
        std::string StoreData;
        fs::path storePath = fs::path(path) / ".yeet" / "Store";
        std::fstream Store(storePath); // store file contains the file paths and oids from the prev commit
        // std::cout << "DEBUG: Opening Store file at: " << path+"/.yeet/Store" << std::endl;
        
        // another way to get the data:
        // storeData.assign(std::istreambuf_iterator<char>(storeFile), std::istreambuf_iterator<char>());
        // storeFile.close();

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
            if (tabPos != std::string::npos) {
                StorePaths.push_back(line.substr(0, tabPos));
                StoreOids.push_back(line.substr(tabPos + 1));
            }
        }
    
        // Main Loop
        for(size_t i=0; i<StoreOids.size(); i++){
            // Normalize store path to ensure it matches disk path format
            std::string normalizedOld = normalize_path(fs::path(StorePaths[i]));

            if(diskFileMap.count(normalizedOld)){
                // File still exists on disk. Add it to snapshot.
                // We use the path from DISK to ensure formatting consistency
                FilesWithChanges.push_back(diskFileMap[normalizedOld]);
            }
            // Else: File deleted. Don't add to snapshot.
        }

        // Check New Files (From Disk)
        std::unordered_set<std::string> storePathSet;
        for(const auto& sp : StorePaths) {
            storePathSet.insert(normalize_path(fs::path(sp)));
        }

        for(const auto& it : DiskPaths){
            if(storePathSet.find(normalize_path(it)) == storePathSet.end()){
                FilesWithChanges.push_back(it); // New file
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
        const std::string IGNORE = " \n\r\t\f\v";
        size_t first = str.find_first_not_of(IGNORE);

        if(std::string::npos == first){
            return str;
        }

        size_t last = str.find_last_not_of(IGNORE);
        return str.substr(first, (last-first+1));
    }

}

void Commit::CommitMain(){
    try {
        // std::cout << "DEBUG: Starting CommitMain with path: " << path << std::endl;

        std::vector<TreeEntry> TreeEntries;
        Database DbObj(fs::path(Commit::path+"/.yeet/objects"));
        
        Refs RefObj(Commit::path);
        
        std::vector<fs::path> FilePath;
        // std::cout << "DEBUG: About to call YeetStatus" << std::endl;
        if(!CommitHelper::YeetStatus(path, FilePath)){
            std::cout << "ERROR::COMMIT:: No files to track." << std::endl;
            return;
        }

        
        // Debug: Print all files being committed
        // std::cout << "DEBUG: Files to be committed:" << std::endl;
        // for(const auto& file : FilePath) {
        //     std::cout << "  - " << file << std::endl;
        // }

        // std::cout<<"above File path loop"<<std::endl;
        for (const auto& entry : FilePath) {
            std::string _stat = "Non-Exe";
            // std::cout<<"inside File path loop"<<std::endl;
            if (isExecutableFile(entry)) { 
                _stat = "Exe";
            }
            // std::cout << "DEBUG: Processing file: " << entry << " (status: " << _stat << ")" << std::endl;
            
            std::string data = readFile(entry.string());
            // std::cout << "DEBUG: Read " << data.length() << " bytes" << std::endl;
            
            Blob newBlobObject(data);
            // std::cout << "DEBUG: Created blob with oid: " << newBlobObject.oid << std::endl;
            
            DbObj.storeContentInDB(newBlobObject, entry.generic_string());
            
            TreeEntry TreeEntryObj(entry.generic_string(), newBlobObject.oid, _stat);
            TreeEntries.push_back(TreeEntryObj);
        }

        // std::cout << "DEBUG: DB Store contents:" << std::endl;

        // ---------------- commiting it for now! -----------------
        // for(auto it : DbObj.Store) {
        //     std::cout << "  " << it.first << " -> " << it.second << std::endl;
        // } ---------------------------------------------

        // TODO: I changed this function output and Store datatype to map instead of unordered_map, is this causing the segmentation faults?
        // Save the store in /Store file
        writeStoreinDB(DbObj.Store);

        // std::cout<<"wrote Store in DB"<<std::endl;
        if (!TreeEntries.empty()) {
            Tree TreeObject(TreeEntries);
            DbObj.storeContentInDB(TreeObject);
            // std::cout << "My Tree Id is wao: " << TreeObject.oid << std::endl;

            std::string parent = RefObj.Read_HEAD(path); // The oid of previous commit

            if(!parent.empty() && parent != "master"){
                fs::path parentCommitPath = fs::path(path) / ".yeet" / "objects" / parent.substr(0, 2) / parent.substr(2);
                
                std::string parentCommitContent = Inflate(parentCommitPath.string());
                std::string parentTreeOid = CommitHelper::getTreeOidFromCommit(parentCommitContent);
                std::string currentTreeOid = TreeObject.oid;
                std::string prevTreeOid = CommitHelper::trimm(parentTreeOid);

                // DEBUG PRINTS (Remove after fixing)
                std::cout << "DEBUG: Current Tree: " << currentTreeOid << std::endl;
                std::cout << "DEBUG: Parent Tree:  " << prevTreeOid << std::endl;

                if (currentTreeOid == prevTreeOid) {
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
                    // Fallback if Auth file missing (or handle error)
                    name = "Unknown"; email = "unknown@yeet.com";
                }
            }

            // std::cout<<"Name: "<<name<<"\nmail: "<<email<<"\n"; // working
            time_t currtime = time(nullptr);
            Author NewAuthorObj(name,email,currtime);
            std::string author = NewAuthorObj.to_stringg();
            // std::cin>>message; // This doesn't takes any spaces " "

            std::string message;
            std::cout << "\nCOMMIT::Please enter your Commit Message:\n";
            std::getline(std::cin, message);
            std::cout<<std::endl;
            
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
