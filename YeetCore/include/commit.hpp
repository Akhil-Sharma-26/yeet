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
    std::string TreeOID; // The Tree DS I made. It's Object ID
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
    void CommitMain(); 
    


    // Read the content of the file. Give it a path
    std::string readFile(fs::path path); 
};

namespace CommitHelper{
    std::string inline relative_path(const fs::path& full_path, const std::string& root_path) {
        try {
            return fs::relative(full_path, root_path).generic_string();
        } catch(...) {
            return full_path.generic_string(); // Fallback
        }
    }

    bool inline YeetStatus(std::string path, std::vector<fs::path>& FilesWithChanges){
    
        std::vector<fs::path> DiskPaths;

        // Gets a complete list of files in the directory rn
        ListFiles(path, DiskPaths);

        // Map for fast lookup of current files on disk (Normalized Path -> Actual Path)
        std::map<std::string, fs::path> diskFileMap;
        for(const auto &it : DiskPaths){
            diskFileMap[relative_path(it, path)] = it;
        }
        
        std::string StoreData;
        fs::path storePath = fs::path(path) / ".yeet" / "Store";
        std::fstream Store(storePath); // store file contains the file paths and oids from the prev commit
    
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
            // Fallback for space-separated if tab is missing (handles copy-paste errors)
            if (tabPos == std::string::npos) tabPos = line.find(' ');

            if (tabPos != std::string::npos) {
                StorePaths.push_back(line.substr(0, tabPos));
                StoreOids.push_back(line.substr(tabPos + 1));
            }
        }
    
        // Main Loop
        for(size_t i=0; i<StoreOids.size(); i++){
            std::string relStorePath = StorePaths[i];
            
            // Handle absolute paths in store by converting them
            if(relStorePath.find(path) == 0) {
                relStorePath = relative_path(fs::path(relStorePath), path);
            }

            if(diskFileMap.count(relStorePath)){
                // File still exists on disk. Add to snapshot.
                FilesWithChanges.push_back(diskFileMap[relStorePath]);
            }
        }

        // 3. Check New Files (Iterate Disk)
        std::unordered_set<std::string> storePathSet;
        for(const auto& sp : StorePaths) {
            if(sp.find(path) == 0) storePathSet.insert(relative_path(fs::path(sp), path));
            else storePathSet.insert(sp);
        }

        for(const auto& it : DiskPaths){
            if(storePathSet.find(relative_path(it, path)) == storePathSet.end()){
                FilesWithChanges.push_back(it); // New File
            }
        }

        return !FilesWithChanges.empty();
    }

    // This function brings the oid of the Tree object from the .yeet/object
    std::string inline getTreeOidFromCommit(const std::string CommitContent){
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

    // This function brings the oid of the Tree object from the .yeet/object
    std::string inline getParentOidFromCommit(const std::string CommitContent){
        std::istringstream st(CommitContent);
        std::string line;

        while(std::getline(st, line)){
            // in the commit obj the tree oid is stored as tree <oid>
            if(line.rfind("parent", 0) == 0){
                return line.substr(8);
            }
        }

        return "";
    }

    std::string inline trimm(const std::string& str){
        const std::string ignr = " \n\r\t\f\v";
        size_t first = str.find_first_not_of(ignr);

        if(std::string::npos == first){
            return str;
        }

        size_t last = str.find_last_not_of(ignr);
        return str.substr(first, (last-first+1));
    }

}
