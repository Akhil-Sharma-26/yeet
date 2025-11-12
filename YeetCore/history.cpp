#include "include/history.hpp"
#include "include/refs_auth.hpp"

namespace history
{
    std::string getCommitPath(std::string repo, std::string oid){

    }
    CommitStruct parse(std::string){

    }
    void display_graph(std::vector<CommitStruct>& commits){
        
    }
    void showHistory(const std::string& repo){
        try
        {
            Refs ref(repo);
            std::string currOid = ref.Read_HEAD(repo); // curr commit

            if(currOid.empty() || currOid == "ref: refs/heads/main"){
                std::cout<<"HISTORY:: No commits yet in this repo\n> Use command \"yeet commit\" To commit the version"<<std::endl;
                return;
            }

            std::vector<CommitStruct> commits;
            int commit_count = 0;
            const int MAX_COMMITS = 1000; // to stop infinite rec

            while(!currOid.empty() && currOid!="ref: refs/heads/main" && commit_count<MAX_COMMITS){
                std::string commitPath = getCommitPath(repo, currOid);

                if(!fs::exists(commitPath)){
                    std::cerr<<"HISTORY::ERROR:: Commit object not found: "<<currOid<<std::endl;
                    break;
                }

                std::string commitContent = Inflate(commitPath);

                if(commitContent.empty()){
                    std::cerr<<"HISTORY::ERROR:: Failed to read commit: "<<currOid<<std::endl;
                    break;
                }

                // parsing the new commit
                CommitStruct info = parse(commitContent);
                info.oid = currOid;

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
