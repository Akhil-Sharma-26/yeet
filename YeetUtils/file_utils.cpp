#include"include/file_utils.hpp"


void ListFiles(std::string path, std::vector<std::filesystem::path>& FilePath) {
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        
        // TODO: Make a file and get names from there
        // My .yeet-ignore
        const bool IGNORE = entry.path().generic_string().find(".git") != std::string::npos ||
        entry.path().generic_string().find(".yeet") != std::string::npos || 
                          entry.path().generic_string().find(".vscode") != std::string::npos || 
                          entry.path().generic_string().find(".xmake") != std::string::npos || 
                          entry.path().generic_string().find(".cmake") != std::string::npos || 
                          entry.path().generic_string().find("/build") != std::string::npos;

        if(IGNORE){
            continue; // SKip that
        }

        if(entry.is_directory()){
            ListFiles(entry.path().string(), FilePath); // Recurse into that directory
        }
        else {
            FilePath.push_back(entry);
        }
    }
}


std::string readFile(std::filesystem::path path) {
    // Copy implementation from controllers.cpp
    std::ifstream f(path, std::ios::in);
    const auto sz = std::filesystem::file_size(path);
    std::string result(sz, '\0');
    f.read(result.data(), sz);
    return result;
}


std::vector<unsigned char> readFile(const std::string& filename) {
    if(filename == "/home/akhil/dev/yeet/.yeet/objects/St/ore"){
        std::vector<unsigned char> res = {};
        return res;
    }  
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "ERROR::READINGFILE:: Cannot open file: " << filename << std::endl;
        return {};
    }
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string Directory_name_Helper(std::string Objpath){
    std::string ans="";
    ans+=Objpath[Objpath.size()-41];
    ans+=Objpath[Objpath.size()-40];
    return ans;
}

std::string File_name_Helper(std::string Objpath){
    std::string ans="";
    for(int i=Objpath.size()-1;i>=0;i--){
        if(Objpath[i]=='/'){
            break;
        }
        ans+=Objpath[i];
    }
    std::string act_ans="";
    for(int i=ans.size()-1;i>=0;i--){
        act_ans+=ans[i];
    }
    return act_ans;
}

// TODO: No need of this namespace. Make a large namespace for all file utils
namespace Helper{
    std::string readFile(std::string path){
        // Open the stream to 'lock' the file.
        std::ifstream f(path, std::ios::in);

        // Obtain the size of the file.
        const auto sz = fs::file_size(path);
        std::string result(sz, '\0');

        // Read the whole file into the buffer. 
        // TODO: This will fail at large files
        f.read(result.data(), sz);

        return result;
    }

    // For the branch namespace to store the commit id to the new branch file.
    void update_HEAD(std::string oid, std::string NewBranchPath){
        // std::cout<<path<<std::endl;
        std::ofstream headFile(NewBranchPath);
        if (headFile.is_open()) {
            headFile << oid;
            headFile.close();
        } else {
            throw std::runtime_error("ERROR::UPDATE::Failed to open .yeet/refs/heads/ file.\n");
        }
    }
}

bool isExecutableFile(const std::filesystem::path& path) {
#ifdef _WIN32
    // On Windows, check file extension
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(),
        [](unsigned char c) { return std::tolower(c); });

    // Common executable extensions on Windows
    return ext == ".exe" || ext == ".bat" || ext == ".cmd" ||
        ext == ".com" || ext == ".ps1" || ext == ".vbs";
#else
    // On Unix-like systems, use access
    return access(path.c_str(), X_OK) == 0;
#endif
}

/**
 * called in commit.hpp
 */
void writeStoreinDB(std::map<std::string, std::string> Store){
    for(auto it:Store){
        std::cout<<it.first<<" "<<it.second<<std::endl;
    }
    std::string _actualPath = fs::current_path().string();

    std::ofstream StoreFile(_actualPath+"/.yeet/Store");
    if(StoreFile.is_open()){
        for(auto it:Store){
            StoreFile << it.first << "\t" << it.second << "\n";
        }
    
        StoreFile.close();
    }

    else {
        throw std::runtime_error("ERROR::STORE:: Failed to create .yeet/Store file.\n");
    }
}