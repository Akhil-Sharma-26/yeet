#pragma once
#include"../../YeetUtils/include/libs.hpp"
class Commit{
private:
    // Lists all the files in the current yeet repo.
    void ListFiles(std::string path,std::vector<fs::path>& FilePath); 
    // TODO: This ListFiles need to check the diffs also to disnguish betweeen new and old files.
    // TODO: Add a check that you can only list files if a .yeet dir is present/ initialized.

    
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