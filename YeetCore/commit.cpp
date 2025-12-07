#include "include/commit.hpp"

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

            std::string parent = RefObj.Read_HEAD(); // The oid of previous commit

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
