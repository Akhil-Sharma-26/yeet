#include "include/revert.hpp"
// show the history

// ask for which commit you want to go to

// Tell that this will remove all the commits after that commit

// go back one by one commit removing all the hashes of that commit till we reach that commit id

// now remove all files from the folder

// construct the dirctory from ground up using the file path stored during the creating of each file's blob

// and yeah that's it

void revert(std::string commit_id){
    // check current commit
    Refs ref(fs::current_path());
    std::string curr_commit_oid = ref.Read_HEAD();

    if(commit_id == curr_commit_oid) {
        std::cerr<<"REVERT::You are already at the same commit!"<<std::endl;
    }

    // Recurrsivly till commit_id == curr_id{
    while(commit_id != curr_commit_oid){
        std::string commit_content = Inflate(fs::path(ref.path) / ".yeet" / "objects" / curr_commit_oid.substr(0, 2) / curr_commit_oid.substr(2));
        
        // get the parent commit
        std::string parent_oid = CommitHelper::getParentOidFromCommit(commit_content);
        curr_commit_oid = parent_oid;
        std::cout<<commit_content<<std::endl;

        std::string currTree_oid = CommitHelper::getTreeOidFromCommit(commit_content);
        std::string tree_content = Inflate(fs::path(ref.path) / ".yeet" / "objects" / currTree_oid.substr(0, 2) / currTree_oid.substr(2));

        std::cout<<tree_content<<std::endl;
    }
        // open the tree oid file
            // inflate it
        // {
            // idr if path is in blob or in tree.
            // in either. delete them
        // }
    // }

    // delete the whole repo except .yeet
    // read the contetn of the commit file. 
        // inflate it
    // get the parent commit
    // open the tree oid file
        // inflate it
    // {
        // idr if path is in blob or in tree.
        // in either. make file
        // inflate content and paste in the files!
    // }
    // you have successfully reverted back to the $commit_id commit!
}