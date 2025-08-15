#include"include/repository.hpp"


/**
 * Initializes a .yeet directory with default configs
 *
 * @param path write the name of the directory should be entered according to pwd, see README for more on it or `.` The default is `.`
 * @attention For now, You can only init in the directory which has already been made.
 * @return Nothing for now
 * @author Akhil Sharma
 */
void YeetInit(std::string path){
    try
    {
        // std::cout<<path;
        // He can enter . --> init in pwd ------> Will Work
        // He can enter ebc --> init in pwd/ebc --------> not work, you to mkdir ebc first
        // He can enter ebc/ --> init in pwd/ebc only not pwd/ebc/ -------> same as above
        // He can enter full path from root --> init at that path ---------> will not work
        std::string pwd = std::filesystem::current_path().string();
        std::string temp_pwd = pwd;
        std::string _actualPath=pwd+'/'+path+".yeet";
        if(path.back()!='/' && path.back()!='.'){ 
            _actualPath=pwd+'/'+path+"/.yeet";
            temp_pwd=pwd+'/'+path;
        }

        if(path.back()=='.') 
            _actualPath=pwd+"/.yeet"; 

        if(std::filesystem::exists(temp_pwd+"/.yeet"))
            throw std::runtime_error("ERROR::INIT::A yeet folder already exists in this directory. \n");
        
        // std::system("tree .");

        std::filesystem::create_directory(_actualPath);
        std::filesystem::create_directory(_actualPath+"/objects");
        std::filesystem::create_directory(_actualPath+"/refs");
        std::filesystem::create_directory(_actualPath+"/refs/heads");
        std::filesystem::create_directory(_actualPath+"/refs/tags");

        // Make HEAD file.
        std::ofstream headFile(_actualPath+"/HEAD");
            if (headFile.is_open()) {
                headFile << "ref: refs/heads/main\n";
                headFile.close();
            } else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/HEAD file.\n");
            }

        std::ofstream masterBranch(_actualPath+"/refs/heads/master");
        if (masterBranch.is_open()) {
            masterBranch << "master";
            masterBranch.close();
        } else {
            throw std::runtime_error("ERROR::INIT::Failed to create .yeet/refs/heads/master file.\n");
        }

        // Making Description file.
        std::ofstream descFile(_actualPath+"/description");
            if(descFile.is_open()){
                descFile<<"Demo Description. This file contains the info and descriptio about the repository.\n";
                descFile.close();
            }
            else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/description file.\n");
            }
        
        // Making config file
        std::ofstream configFile(_actualPath+"/config");
            if(configFile.is_open()){
                // TODO: Find a configparser for C++. and replace the content of this configFile.
                configFile<<"Demo Config\n";
                configFile.close();
            }
            else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/config file.\n");
            }

        // Making Store File
        std::ofstream StoreFile(_actualPath+"/Store");
        if(StoreFile.is_open()){
            StoreFile<<"Empty Store";
            StoreFile.close();
        }
        else {
            throw std::runtime_error("ERROR::INIT::Failed to create .yeet/Store file.\n");
        }

        // Make Diff file.
        std::ofstream DiffFile(_actualPath+"/Diff");
        if (DiffFile.is_open()) {
            DiffFile << "No Diffs Yet";
            DiffFile.close();
        } else {
            throw std::runtime_error("ERROR::INIT::Failed to create .yeet/Diff file.\n");
        }

        // Make Current Branch file.
        std::ofstream BranchFile(_actualPath+"/Branch");
        if (BranchFile.is_open()) {
            BranchFile << "master";
            BranchFile.close();
        } else {
            throw std::runtime_error("ERROR::INIT::Failed to create .yeet/Diff file.\n");
        }

        std::cout << "YEET::Initialized yeet directory\n";
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    
}

void YeetAdd(){

}

void YeetStatus(std::string path){

    std::vector<std::filesystem::path> FilePath;
    ListFiles(path, FilePath);

    std::unordered_map<std::filesystem::path, bool> visited;
    for (const auto& it : FilePath) {
        visited[it] = false;
    }

    int Totaladditions = 0, Totaldeletions = 0;
    std::string StoreData;
    std::fstream Store(path + "/.yeet/Store");

    if (Store.is_open()) {
        std::string line;
        while (std::getline(Store, line)) {
            StoreData += line + "\n";
        }
        Store.close();
    } else {
        std::cout << "Error opening Store File" << std::endl;
        return;
    }
    rtrim(StoreData);
    if (StoreData == "Empty Store") {
        // Clear existing Diff file
        std::ofstream clearDiff(path + "/.yeet/Diff", std::ios::trunc);
        clearDiff.close();

        std::vector<Edit> all_edits;
        for (const auto& file : FilePath) {
            std::ifstream newFile(file);
            if (newFile.is_open()) {
                std::string content;
                std::string line;
                while (std::getline(newFile, line)) {
                    content += line + "\n";
                }
                newFile.close();
                std::vector<std::string> lines = splitIntoLines(content);
                for (const auto& l : lines) {
                    all_edits.emplace_back(Edit::INS, "", l);
                }
                Totaladditions += lines.size();
                std::cout << file << " (new file)" << std::endl;
            }
        }
        storeDiff(all_edits);
        std::cout << "Total additions: " << Totaladditions << "\n";
        std::cout << "No previous commit. All files are new.\n";
        return;
    }

    // bool space = false;
    std::string PathofFile, oid;
    PathofFile = ""; oid = "";
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

    std::vector<Edit> all_edits;
    for (size_t i = 0;i<oids.size();i++) {

        int additions,deletions;
        additions = 0, deletions = 0;
        std::string thePathOfOid = "";
        std::string fileName = oids[i].substr(2, oids[i].size() - 2); 
        thePathOfOid = oids[i].substr(0, 2) + "/" + fileName;

        std::string FullPath = path + "/.yeet/objects/" + thePathOfOid;

        std::string InflatedContent = Inflate(FullPath);

        if (std::filesystem::exists(FilePaths[i])) {
            std::string NewFileContent="";
            std::ifstream NowFile(FilePaths[i]);

            if(NowFile.is_open()){
                std::string line;
                while(std::getline(NowFile,line)){
                    NewFileContent+=line+"\n";
                }
                NowFile.close();
            }

            // Call Diffs algo here
            std::vector<std::string> NewFileinLines = splitIntoLines(NewFileContent);
            std::vector<std::string> OldFileinLines = splitIntoLines(InflatedContent);

            std::vector<std::vector<int>> trace;
            int ans = Shortest_Edit_Search(NewFileinLines, OldFileinLines, trace); 

            // std::cout<<ans<<std::endl;
            if(ans==0) {
                // TODO: Don't add in commit
                // std::cout<<"Files are identical."<<std::endl;
                continue;
            }            
            
            std::vector<Edit> diff_result = diff(OldFileinLines, NewFileinLines, trace, ans);

            for(auto it:diff_result){
                
                // TODO: Add number of lines.
                if(it.type == Edit::DEL) {
                    deletions++;
                    Totaldeletions++;
                }
                else if(it.type == Edit::INS) {
                    additions++;
                    Totaladditions++;
                }
            }

            // Don't print exec file diffs.
            if(! access (FilePaths[i].c_str(), X_OK)){
                continue;
            }

            // don't show file if nothing changed
            if(additions == 0 && deletions == 0){
                continue;
            }

            // The file we are checking:
            std::cout<<FilePaths[i]<<std::endl;
            all_edits.insert(all_edits.end(), diff_result.begin(), diff_result.end());
            std::cout<<"This file additions: "<<additions<<"\n";
            std::cout<<"This file deletions: "<<deletions<<std::endl;
            // Printing the diffs
            Printer printer;
            printer.print(diff_result);

            
        } else {
            deletions+=InflatedContent.size();
        }
        visited[FilePaths[i]] = true;
    }

    // Clear existing Diff file

    std::ofstream clearDiff(path + "/.yeet/Diff", std::ios::trunc);
    clearDiff.close();
    storeDiff(all_edits); // Write all collected diffs

    for (const auto& it : FilePath) {
        if (!visited[it]) {
            std::ifstream newFile(it);
            if (newFile.is_open()) {
                std::string line;
                while (std::getline(newFile, line)) {
                    Totaladditions++;
                }
                newFile.close();
            }
        }
    }

    if(Totaladditions == 0  && Totaldeletions == 0){
        std::cout<<"No Change, Can't commit"<<std::endl;
    }
    else{
        std::cout<<"Total addtions: "<<Totaladditions<<"\nTotal deletions: "<<Totaldeletions<<std::endl;
    }

    std::cout<<"DIFF:: All the changes have stored in the /.yeet/Diff file"<<std::endl;


}
