#include "include/commit.hpp"

namespace CommitHelper{
    void YeetStatus(std::string path, std::vector<fs::path>& FilesWithChanges){
        // std::cout << "DEBUG: YeetStatus starting with path: " << path << std::endl;
    
        std::vector<fs::path> FilePath;
        
        // Getting list of all files
        // std::cout << "DEBUG: About to call ListFiles" << std::endl;
        ListFiles(path, FilePath);
        // std::cout << "DEBUG: ListFiles found " << FilePath.size() << " total files" << std::endl;
    
        // Making a visited map for later
        std::unordered_map<fs::path, bool> visited;
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
    
        // bool space = false;
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
        for(size_t i=0;i<oids.size();i++){
    
            int additions,deletions;
            additions = 0, deletions = 0;
            std::string thePathOfOid = "";
            std::string fileName = oids[i].substr(2, oids[i].size() - 2); 
            thePathOfOid = oids[i].substr(0, 2) + "/" + fileName;
    
            std::string FullPath = path + "/.yeet/objects/" + thePathOfOid;
    
            std::string InflatedContent = Inflate(FullPath);
    
            if (fs::exists(FilePaths[i])) {
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
                // if(fs::status(FilePath[i].c_str()).permissions() & fs::perms::owner_exec )
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


void Commit::CommitMain(std::string path){
    try {
        // std::cout << "DEBUG: Starting CommitMain with path: " << path << std::endl;
        
        std::vector<TreeEntry> TreeEntries;
        Database DbObj(Commit::path+"/.yeet/objects");
        Refs RefObj(Commit::path);

        std::string message;
        std::cout << "\nCOMMIT::Please enter your Commit Message:\n";
        std::getline(std::cin >> std::ws, message);
        
        std::vector<fs::path> FilePath;
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
