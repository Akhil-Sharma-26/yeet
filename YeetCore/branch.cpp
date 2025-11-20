#include"include/branch.hpp"

namespace Branch{
    void createBranch(std::string BranchName, fs::path currPath){
        // check if the names is valid:
        if(BranchName.empty()) return;
        
        std::regex reg(R"(^\.|\/\.|\.\.|\/$|\.lock$|@\{|[\x00-\x20*:?\[\\^~\x7f])");

        if(std::regex_match(BranchName, reg)){
            // invalid name of the branch
            std::cout<<"ERROR::BRANCH:: Invalid name of the Branch"<<std::endl;
            return;
        }

        std::string actPath = currPath.string() + "/.yeet/refs/heads/" + BranchName;

        if(fs::exists(actPath)){
            std::cout<<"ERROR::BRANCH:: Branch with this name already exists"<<std::endl;
            return;
        }

        // TODO: store the current commit in the new file
        // get the oid, update the update_HEAD function of the REfs classs. make refs object.
        // then pass the branch name also to the function to get telll which branch it shoul upate. ig
        

        // updating the head file with the latest commit.
        // putting content of the master into the new branch file.

        // creating the new bracnh file:
        std::string PrevBranch = Helper::readFile(fs::path(currPath.string()+"/.yeet/Branch").string());

        // TODO: add option to choose the base branch
        // ! Currently making the prev branch as base branch.
        
        std::string CommitID_ofPrevBranch = Helper::readFile(fs::path(currPath.string()+"/.yeet/refs/heads/" + PrevBranch).string());
        // making the file for the new Branch and storing the prevBranch ID into it.
        
        // TODO: Test this function
        Helper::update_HEAD(CommitID_ofPrevBranch, actPath);

        std::ofstream f(actPath); // fstream is not capable of making new file, so use ofstream instead.
        if(f.is_open()){
            f<<CommitID_ofPrevBranch;
            f.close();
        }
        else {
            throw std::runtime_error("ERROR::BRANCH:: Failed to create new Branch file.\n");
        }       

    }
    void SeeBranches(std::filesystem::path path){
        std::filesystem::path BranchesDir= path.string() + "/.yeet/refs/heads";
        
        std::cout<<"BRANCH:: Your branches are: "<<std::endl<<std::endl;
        int count = 1;
        for (const auto& it : std::filesystem::directory_iterator(BranchesDir)) {
            std::cout << count <<" ";
            std::cout << it.path().filename().string() << " ";
            std::fstream ff(it.path());
            if(ff.is_open()){
                std::string line;
                while (std::getline(ff, line)) {
                    std::cout << line << std::endl;
                }
                ff.close();
            }
            count++;
        }

    }

    void currBranch(std::filesystem::path currPath){
        std::fstream ff(currPath.string() + "/.yeet/Branch");
        std::stringstream ss;
        if(ff.is_open()){
            ss << ff.rdbuf();
            ff.close();
        }
        std::cout<<"BRANCH:: Your Current Branch is: "<<ss.str()<<std::endl;
    }
}


namespace CheckOut{
    void SwitchBranch(std::filesystem::path path, std::string switchToBranchName){
        std::string actPath = path.string() + "/.yeet/refs/heads/" + switchToBranchName;

        if(fs::exists(actPath)){
            std::ifstream currentBranchFile(path.string() + "/.yeet/Branch");
            std::string currentBranch;
            if(currentBranchFile.is_open()){
                std::getline(currentBranchFile, currentBranch);
                currentBranchFile.close();
            }

            if(currentBranch == switchToBranchName){
                std::cout<<"CHECKOUT::Already in the branch "<<switchToBranchName<<std::endl;
                return;
            }

            std::ofstream branchFile(path.string() + "/.yeet/Branch", std::ios::trunc);
            if(branchFile.is_open()){
                branchFile << switchToBranchName;
                branchFile.close();
                std::cout<<"CHECKOUT::Successfully Changed the branch to: "<<switchToBranchName<<std::endl;
            } else {
                std::cerr<<"ERROR::CHECKOUT::Failed to update the branch file"<<std::endl;
            }
        }
        else{
            std::cout<<"ERROR::CHECKOUT::Branch "<<switchToBranchName<<" does not exist"<<std::endl;
        }
    }
}

