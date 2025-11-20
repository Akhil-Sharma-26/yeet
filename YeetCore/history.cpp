#include "include/history.hpp"
#include "include/refs_auth.hpp"

namespace history
{
    std::string getCommitPath(std::string repo, std::string oid){
        if(oid.empty() || oid == "master"){
            return "";
        }

        std::string dir = oid.substr(0,2);
        std::string file = oid.substr(2);
        return fs::path(repo + "/.yeet/objects/" + dir + "/" + file).string();
    }

    CommitStruct parse(std::string content){
        CommitStruct info;
        std::istringstream strm(content);
        std::string line;

        bool flag = false; // to skip the header lines(commit, size etc)

        while(std::getline(strm, line)){
            if(line.rfind("parent:",0)==0){
                // std::cout<<"found parent"<<std::endl;
                info.parent = line.substr(8); // after parent:
                info.parent.erase(0, info.parent.find_first_not_of(" \t\n\r")); // trim the whitespace
                info.parent.erase(info.parent.find_last_not_of(" \t\n\r") + 1);
            }

            // not parsing the tree

            // parsing the author
            else if(line.rfind("author:", 0) == 0){
                info.author = line.substr(8); // after author:
                info.author.erase(0, info.author.find_first_not_of(" \t\n\r"));
            }

            // the commmit message
            else if(line.rfind("CommitMessage:", 0) == 0){
                info.message = line.substr(15);
                info.message.erase(0, info.message.find_first_not_of(" \t\n\r"));
                flag = true;
            }

            // if the commit message is multiline, keep reading it
            else if(flag && !line.empty()){
                info.message += "\n" + line;
            }
        }

        return info;
    }
    void display_graph(std::vector<CommitStruct>& commits){
        std::cout << "\n";
        std::cout << "\033[1m\033[36m"; // Bold cyan
        std::cout << "======================================" << std::endl;
        std::cout << "          COMMIT HISTORY" << std::endl;
        std::cout << "======================================\033[0m" << std::endl;
        std::cout << "\n";


        for (size_t i = 0; i < commits.size(); ++i) {
            const auto& commit = commits[i];
            
            // Display commit node with color
            std::cout << "\033[1m\033[33m●\033[0m "; // Yellow bullet
            std::cout << "\033[1mCommit:\033[0m \033[32m" << commit.oid.substr(0, 8) << "...\033[0m"; // Green short hash
            
            // Show branch if present (HEAD commit)
            if (!commit.branch.empty()) {
                std::cout << " \033[1m\033[36m(" << commit.branch << ")\033[0m"; // Cyan branch
            }
            std::cout << std::endl;

            // Author
            if (!commit.author.empty()) {
                std::cout << "  \033[90mAuthor:\033[0m " << commit.author << std::endl; // Gray label
            }

            // Message
            if (!commit.message.empty()) {
                std::cout << "  \033[90mMessage:\033[0m " << commit.message << std::endl;
            }

            std::cout << std::endl;


            // Draw arrow to parent (except for root commit)
            if (i < commits.size() - 1) {
                std::cout << "  \033[90m│\033[0m" << std::endl; // Gray vertical line
                std::cout << "  \033[90m↓\033[0m" << std::endl; // Gray arrow
                std::cout << std::endl;
            } else {
                // Root commit indicator
                if (commit.parent == "master" || commit.parent.empty()) {
                    std::cout << "\033[1m\033[35m✦ Root Commit\033[0m" << std::endl; // Magenta
                }
            }
        }

        std::cout << "\n\033[90mTotal commits: " << commits.size() << "\033[0m\n" << std::endl;
    }

    void showHistory(const std::string& repo){
        try
        {
            Refs ref(repo);
            std::string currOid = ref.Read_HEAD(repo); // curr commit

            if(currOid.empty() || currOid == "master"){
                std::cout<<"HISTORY:: No commits yet in this repo\n> Use command \"yeet commit\" To commit the version"<<std::endl;
                return;
            }

            std::vector<CommitStruct> commits;
            int commit_count = 0;
            const int MAX_COMMITS = 1000; // to stop infinite rec

            while(!currOid.empty() && currOid!="master" && commit_count<MAX_COMMITS){
                std::string commitPath = getCommitPath(repo, currOid);

                if(!fs::exists(commitPath)){
                    std::cerr<<"HISTORY::ERROR:: Commit object not found: "<<currOid<<std::endl;
                    break;
                }

                std::string commitContent = Inflate(commitPath);

                // std::cout<<commitContent<<std::endl;

                if(commitContent.empty()){
                    std::cerr<<"HISTORY::ERROR:: Failed to read commit: "<<currOid<<std::endl;
                    break;
                }

                // parsing the new commit
                CommitStruct info = parse(commitContent);

                // std::cout<<"the parent:" << info.parent<<std::endl;
                info.oid = currOid;

                // ig showing the base branch will be good! 
                if(commit_count == 0){
                    info.branch = ref.currentBranch();
                }

                commits.push_back(info);

                currOid = info.parent;
                commit_count++;
            }

            if(commit_count >= MAX_COMMITS){
                std::cerr<<"HISTORY::WARNING:: Reached maximum commit traversal limit"<<std::endl;
            }

            if(commits.empty()){
                std::cout<<"HISTORY:: NO commits found. Please do some commits!"<<std::endl; return;
            }
            display_graph(commits);


        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }
} // namespace history
