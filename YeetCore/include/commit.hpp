#pragma once
#include"../../YeetUtils/include/libs.hpp"
#include"../../YeetUtils/include/file_utils.hpp"
#include"../../YeetUtils/include/compression_utils.hpp"
#include"../../YeetUtils/include/diff_utils.hpp"
#include"tree.hpp"
#include"db.hpp"
#include"refs_auth.hpp"
#include<unordered_set>

/**
 * 
 */
class Commit{
public:

    std::string path; // The Actual File path of the files that will be stored in the commit
    std::string TreeOID; // The Tree DS we make. It's Object ID
    std::string AuthorData; // The author data about the committer
    std::string CommitMessage; // Message for the current Commit
    std::string Writtenlines; // Total number of new lines in this commit
    std::string oid; // The Object ID of this commit
    std::string parent; // The parent of the current commit
    std::vector<std::string> IGNORE = {".","..",".git",".yeet"}; // Ignore these folders

    // Methods

    // Commit constructor with only "FIle path"
    Commit(std::string path); 

    // Commit constructore with TreeOID, authorData, Message and parent
    Commit(std::string TreeOid, std::string AuthorData, std::string CommitMessage, std::string parent); 

    // Main Logic/Workflow of the commit command;
    void CommitMain(std::string path); 
    


    // Read the content of the file. Give it a path
    std::string readFile(fs::path path); 
};