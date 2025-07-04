#include "../../include/Commit.hpp"
#include "../../include/Tree.hpp"

void Commit::ListFiles(std::string path,std::vector<fs::path>& FilePath){
    for(auto const&it: fs::directory_iterator(path)){

        // TODO: Find a good way to do this thing
        // My .yeetIgnore
        const bool IGNORE = it.path().generic_string().find(".git") != std::string::npos || 
                            it.path().generic_string().find(".yeet") != std::string::npos || 
                            it.path().generic_string().find(".vscode") != std::string::npos ||
                            it.path().generic_string().find(".xmake") != std::string::npos || 
                            it.path().generic_string().find(".cmake") != std::string::npos || 
                            it.path().generic_string().find("/build") != std::string::npos;

        if(IGNORE) continue;

        if(it.is_directory()) 
            ListFiles(it.path(), FilePath); // Recurssing into the the sub Directory

        else   
            FilePath.push_back(it.path()); // Pushing the paths into the final Vector.
    }
}

// void Commit::CommitMain(std::string path){
//     try
//     {
//         std::vector<TreeEntry> TreeEnteries; // Will store all the TreeEnteries


//     }
//     catch(const std::exception& e)
//     {
//         std::cerr << "\nERROR::COMMIT_MAIN:: An error occured while commiting your latest changes. \nError by e.what(): "<< e.what() <<std::endl;
//     }
    
// }