#include"include/repository.hpp"


/**
 * Initializes a .yeet directory with default configs
 *
 * @param path write the name of the directory should be entered according to pwd, see README for more on it or `.` The default is `.`
 * @attention For now, You can only init in the directory which has already been made.
 * @return Nothing for now
 * @author Akhil Sharma
 */
void YeetInit(std::string src){
    try
    {
        std::filesystem::path path = std::filesystem::path(src);
        path = path / ".yeet";
        if(std::filesystem::exists(path))
            throw std::runtime_error("ERROR::INIT::A yeet folder already exists in this directory. \n");

        std::filesystem::create_directory(path);
        std::filesystem::create_directory(path / "objects");
        std::filesystem::create_directory(path / "refs");
        std::filesystem::create_directory(path / "refs" / "heads");
        std::filesystem::create_directory(path / "refs" / "tags");

        // Make HEAD file.
        std::filesystem::path headPath = path / "HEAD";
        std::ofstream headFile(headPath.string());
            if (headFile.is_open()) {
                headFile << "ref: refs/heads/main\n";
                headFile.close();
            } else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/HEAD file.\n");
            }

        // making the head files
        std::ofstream masterBranch(path / "refs" / "heads" / "master");
            if (masterBranch.is_open()) {
                masterBranch << "master";
                masterBranch.close();
            } else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/refs/heads/master file.\n");
            }

        // Making Description file.
        std::ofstream descFile(path / "description");
            if(descFile.is_open()){
                descFile<<"Demo Description. This file contains the info and descriptio about the repository.\n";
                descFile.close();
            }
            else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/description file.\n");
            }
        
        // Making config file
        std::ofstream configFile(path / "config");
            if(configFile.is_open()){
                // TODO: Find a configparser for C++. and replace the content of this configFile. or make my own
                configFile<<"Demo Config\n";
                configFile.close();
            }
            else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/config file.\n");
            }

        // Making Store File
        std::ofstream StoreFile(path / "Store");
            if(StoreFile.is_open()){
                StoreFile<<"Empty Store";
                StoreFile.close();
            }
            else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/Store file.\n");
            }

        // Make Diff file.
        std::ofstream DiffFile(path / "Diff");
            if (DiffFile.is_open()) {
                DiffFile << "No Diffs Yet";
                DiffFile.close();
            } else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/Diff file.\n");
            }

        // Make Current Branch file.
        std::ofstream BranchFile(path / "Branch");
            if (BranchFile.is_open()) {
                BranchFile << "master";
                BranchFile.close();
            } else {
                throw std::runtime_error("ERROR::INIT::Failed to create .yeet/Branch file.\n");
            }

        // Handling author id and email
        #ifdef _WIN32
            #ifdef _WINNT_
            #undef _WINNT_
            #endif
            #include<windows.h>
            char buff_name[512], buff_mail[512];
            if(!GetEnvironmentVariableA("YEET_AUTHOR_NAME", buff_name, sizeof(buff_name))==0 || GetEnvironmentVariableA("YEET_AUTHOR_EMAIL", buff_mail, sizeof(buff_mail))==0 ){ // no env exists
            std::string name, email;
            std::cout<<"\n> You are making your first Yeet repository in this computer."<<std::endl;
            std::cout<<"> You have to set \"YEET_AUTHOR_NAME\" and \"YEET_AUTHOR_EMAIL\" environment variables."<<std::endl;
            std::cout<<"> YEET_AUTHOR_NAME: ";
            getline(std::cin, name);
            std::cout<<"\n> YEET_AUTHOR_EMAIL: ";
            getline(std::cin, email);
            if(SetEnvironmentVariableA("YEET_AUTHOR_NAME", name)){
                std::cout<<"\n> Author Name env set succesffully"<<std::endl;
            }
            else{
                std::cout<<"> There was an error during setting the YEET_AUTHOR_NAME env. Error: "<<GetLastError()<<std::endl;
            }

            if(SetEnvironmentVariableA("YEET_AUTHOR_EMAIL", email)){
                std::cout<<"\n> Author EMAIL env set succesffully"<<std::endl;
            }
            else{
                std::cout<<"> There was an error during setting the YEET_AUTHOR_EMAIL env. Error: "<<
                GetLastError()<<std::endl;
            }
        #endif

        #ifdef __linux__
            if(!getenv("YEET_AUTHOR_NAME") && !getenv("YEET_AUTHOR_EMAIL")){
                std::string name, email;
                std::cout<<"\n> You are making your first Yeet repository in this computer."<<std::endl;
                std::cout<<"> You have to set \"YEET_AUTHOR_NAME\" and \"YEET_AUTHOR_EMAIL\" environment variables."<<std::endl;
                std::cout<<"> YEET_AUTHOR_NAME: ";
                getline(std::cin, name);
                std::cout<<"\n> YEET_AUTHOR_EMAIL: ";
                getline(std::cin, email);

                if( setenv("YEET_AUTHOR_NAME", name.c_str(), 1) != 0 ||  setenv("YEET_AUTHOR_EMAIL",
                    email. c_str(), 1)!=0){
                    std::cout<<"\n> Error Setting up the env variables. Please do it Manually! "<<std::endl;
                }
            }
        #endif

        std::cout << "Initialized yeet directory at\n"<< path << std::endl;
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
    
}

void YeetAdd(){

}

void YeetStatus(std::string path){
    if(!std::filesystem::exists(std::filesystem::path(path) / ".yeet")){
        std::cout<<"> You are not in a yeet Repository."<<std::endl;
        return;
    }

    std::vector<std::filesystem::path> FilePath;
    ListFiles(path, FilePath);

    // checks for visited files
    std::unordered_map<std::filesystem::path, bool> visited;
    for (const auto& it : FilePath) {
        visited[it] = false;
    }

    // total additions and delections to show at the end
    int Totaladditions = 0, Totaldeletions = 0;
    std::string StoreData;

    std::filesystem::path act = std::filesystem::path(path);
    // Store contains the prev files
    std::fstream Store(act / ".yeet" / "Store");

    // getting the data from the store
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
    rtrim(StoreData); // remvoing any extra spaces;
    if (StoreData == "Empty Store") {
        // Clear existing Diff file beacuse its a new commit
        std::ofstream clearDiff(act / ".yeet" / "Diff", std::ios::trunc);
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
    // this is parsing of the store file acc to the format I've stored the data in
    // its like filePaths + "\t" + oids
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

        std::string yas = path + "/.yeet/objects/" + thePathOfOid;
        fs::path FullPath = fs::path(yas);

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
            if(access (FilePaths[i].c_str(), X_OK)){ // fixed a bug here. apparanlty, access function returns 0 if the file is executable.
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

            
        } else { // file is deleted
            // This is a BUG. I have to add the lines. not the size of the file ie not the chars
                // deletions+=InflatedContent.size();
            std::vector<std::string> lines = splitIntoLines(InflatedContent);
            int line_count = lines.size();
            deletions +=line_count;
            Totaldeletions += line_count;
            for(auto &line: lines)
            all_edits.emplace_back(Edit::DEL, line, "");
        }
        visited[FilePaths[i]] = true;
    }

    // Clear existing Diff file

    std::ofstream clearDiff(act / ".yeet" / "Diff", std::ios::trunc);
    clearDiff.close();
    // storeDiff(all_edits); // Write all collected diffs --> this is a BUG. it will be called later. after updating all_edits

    for (const auto& it : FilePath) {
        if (!visited[it]) {
	        std::cout<<it<<" (new file) "<<std::endl;
            std::ifstream newFile(it);
            if (newFile.is_open()) {
		        std::string content="";
                std::string line;
		        int new_file_additions = 0;
                while (std::getline(newFile, line)) {
		            content += line+"\n";
		            all_edits.emplace_back(Edit::INS, "", line);
                    new_file_additions++;
                }
                newFile.close();
		        Totaladditions+=new_file_additions;
		        std::cout<<"This file's additions: "<<new_file_additions<<"\n";
            }
        }
    }

    // now Storing the diffs for the next commit
    storeDiff(all_edits);

    if(Totaladditions == 0  && Totaldeletions == 0){
        std::cout<<"No Change, Can't commit"<<std::endl;
    }
    else{
        std::cout<<"Total addtions: "<<Totaladditions<<"\nTotal deletions: "<<Totaldeletions<<std::endl;
    }

    std::cout<<"DIFF:: All the changes have stored in the /.yeet/Diff file"<<std::endl;


}
