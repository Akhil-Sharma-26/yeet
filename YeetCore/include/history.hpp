#pragma once

#include "../../YeetUtils/include/libs.hpp"
#include "../../YeetUtils/include/compression_utils.hpp"


namespace history{
    // commit structure
    struct CommitStruct{
        std::string oid; // every commit's oid
        std::string parent; // the parent of curr commit
        std::string message; // commit message
        std::string author; // author of the commit
        std::string time; // time of commit
        std::string branch; // TODO: merging not implemented yet, so check this member when implemt the merge
    };

    std::string getCommitPath(std::string repo, std::string oid);
    
    // parse to get the commitstruct from the commit comppresed file
    CommitStruct parse(std::string);
    void display_graph(std::vector<CommitStruct>& commits);
    // show the commit history from HEAD
    void showHistory(const std::string& repo);

}