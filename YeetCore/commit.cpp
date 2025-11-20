#include "include/commit.hpp"

namespace CommitHelper{
    bool YeetStatus(std::string path, std::vector<fs::path>& FilesWithChanges){
        // std::cout << "DEBUG: YeetStatus starting with path: " << path << std::endl;
    
        std::vector<fs::path> FilePath;
        
        // Getting list of all files
        // std::cout << "DEBUG: About to call ListFiles" << std::endl;

        // Gets a complete list of files in the directory rn
        ListFiles(path, FilePath);

        // std::cout << "DEBUG: ListFiles found " << FilePath.size() << " total files" << std::endl;

        // Using a Hash Set to see if a File existed in the prev commit or not
        std::unordered_set<fs::path> allCurrFiles;
        for(const auto &it:FilePath){
            allCurrFiles.insert(it);
        }
        
        std::string StoreData;
        std::fstream Store(path+"/.yeet/Store"); // store file contains the file paths and oids from the prev commit
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
            // std::cout << "DEBUG: Store file contents: " << StoreData << std::endl;
        }
        else{
            std::cout<<"ERROR::STATUS::Error in opening Store File"<<std::endl;
            return false;
        }
        rtrim(StoreData);
    
        if (StoreData == "Empty Store") {
            // std::cout << "DEBUG: Empty Store detected, adding all files" << std::endl;
            // Add all current files as changes for the initial commit
            FilesWithChanges = FilePath;
            // std::cout << "DEBUG: Added " << FilesWithChanges.size() << " files for initial commit" << std::endl;
            // returning true if there are files to commit
            return !FilePath.empty();
        }

        // bool space = false;
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

        // flag to see if there are any changes or not
        bool has_changes = false;

    
        // Main Loop
        for(size_t i=0;i<oids.size();i++){

            const auto& oldPath = FilePath[i];
            const std::string oldOid = oids[i];

            if(allCurrFiles.count(oldPath)){
                // checking mods
                std::string currContent = readFile(oldPath);

                Blob currBlob(currContent); // its constructor will calculate the hashes etc.

                // if the hash from the old store file doen't match the new file hash hten there as some changes
                if(oldOid != currBlob.oid){
                    has_changes = true;
                }

                // even ifthere is change in the file or not, as long as it exists, we have to include it in the list for the new commit's snapshot
                FilesWithChanges.push_back(oldPath);
            }
            else{
                // The file from the last commit doesn't exist anymore here. but DELETION is also a changes, threfore I must mark it as a change
                has_changes = true;
            }
        }

        // to find new files, comparing the list of files from last commit witht the curr on the disk
        // using set for just its fast lookup
        std::unordered_set<fs::path> oldFileSet(FilePath.begin(), FilePath.end());

        for(const auto& it:allCurrFiles){
            // if a file on disk can't be found in the set of old files then its NEW
            if(oldFileSet.find(it)==oldFileSet.end()){
                has_changes= true;
                FilesWithChanges.push_back(it); // the new file
            }
        }


        if(!has_changes){
            FilesWithChanges.clear(); // nothing to commit;
        }

        return has_changes;

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


// TODO: Rewrite this to ASK for the COMMIT MESSAGE at the last. before that do the pre-processing
void Commit::CommitMain(){
    try {
        // std::cout << "DEBUG: Starting CommitMain with path: " << path << std::endl;

        std::vector<TreeEntry> TreeEntries;
        Database DbObj(fs::path(Commit::path+"/.yeet/objects"));
        
        Refs RefObj(Commit::path);

        std::string message;
        std::cout << "\nCOMMIT::Please enter your Commit Message:\n";
        std::getline(std::cin, message);
        std::cout<<std::endl;
        
        std::vector<fs::path> FilePath;
        // std::cout << "DEBUG: About to call YeetStatus" << std::endl;
        
        if(!CommitHelper::YeetStatus(path, FilePath)){
            std::cout << "ERROR::COMMIT:: Nothing to commit" << std::endl;
            return;
        }
        
        // Debug: Print all files being committed
        // std::cout << "DEBUG: Files to be committed:" << std::endl;
        // for(const auto& file : FilePath) {
        //     std::cout << "  - " << file << std::endl;
        // }

        for (const auto& entry : FilePath) {
            std::string _stat = "Non-Exe";
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

        if (!TreeEntries.empty()) {
            Tree TreeObject(TreeEntries);
            DbObj.storeContentInDB(TreeObject);
            // std::cout << "My Tree Id is wao: " << TreeObject.oid << std::endl;

            std::string parent = RefObj.Read_HEAD(path); // The oid of previous commit

            if(!parent.empty() && parent != "master"){
                std::string pp = path + "/.yeet/objects/" + parent.substr(0, 2) + "/" + parent.substr(2);

                fs::path parentCommitPath = fs::path(pp);
                // Inflate (decompress) the parent commit object's content.
                std::string parentCommitContent = Inflate(parentCommitPath.string());
                
                // Use our new helper to get the parent's tree OID.
                std::string parentTreeOid = CommitHelper::getTreeOidFromCommit(parentCommitContent);

                // std::cout<<TreeObject.oid<<" "<<parentTreeOid<<std::endl;
                // THE FINAL CHECK: If the new tree's hash is the same as the parent's, abort!
                if (TreeObject.oid == CommitHelper::trimm(parentTreeOid)) {
                    std::cout << "Nothing to commit, working tree is clean." << std::endl;
                    return; // ABORT the commit.
                }

            }

            std::string name, email;
            #ifdef _WIN32
                #include<windows.h>
                char buff_name[512], buff_mail[512];
                DWORD res1, res2;
                if(!res1 = GetEnvironmentVariableA("YEET_AUTHOR_NAME", buff_name, sizeof(buff_name))==0 || res2 = GetEnvironmentVariableA("YEET_AUTHOR_EMAIL", buff_mail, sizeof(buff_mail))==0 ){ // no env exists
                    std::cout<<"\n> Please setup your YEET_AUTHOR_NAME and YEET_AUTHOR_EMAIL env variables"<<std::endl;
                    exit(1);
                }

                if ((res1 > 0 && res1 < sizeof(buff_name)) &&  (res2 > 0 && res2 < sizeof(buff_mail))) {
                    std::cout << "YEET_AUTHOR_NAME: " << buff_name << std::endl;
                    std::cout << "YEET_AUTHOR_EMAIL: " << buff_mail << std::endl;

                    std::string temp(buff_name, sizeof(buff_name));
                    std::string temp1(buff_mail, sizeof(buff_mail));

                    name = temp;
                    email = temp1;
                } else if (result == 0) {
                    std::cerr << "YEET_AUTHOR_NAME and YEET_AUTHOR_EMAIL not found." << std::endl;
                } else {
                    std::cerr << "Error retrieving YEET_AUTHOR_NAME and YEET_AUTHOR_EMAIL or buffer too small." << std::endl;
                }
            #endif

            #ifdef __linux__
                if(!getenv("YEET_AUTHOR_NAME") && !getenv("YEET_AUTHOR_EMAIL")){
                    std::cout<<"\n> Please setup your YEET_AUTHOR_NAME and YEET_AUTHOR_EMAIL env variables"<<std::endl;
                    std::cout<<"\n> Run the script \"SetupENVvars.sh\" to set those variables or do manually!!"<<std::endl;
                    exit(1);
                }
                name = getenv("YEET_AUTHOR_NAME");
                email = getenv("YEET_AUTHOR_EMAIL");
            #endif

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
