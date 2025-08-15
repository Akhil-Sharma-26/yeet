#include"../../YeetUtils/include/libs.hpp"
#include"../../YeetUtils/include/file_utils.hpp"
#include<regex>

namespace Branch{
    void currBranch(std::filesystem::path currPath);
    void HandleBranch();
    void createBranch(std::string BranchName, std::filesystem::path currPath);
    void deleteBranch();
    void SeeBranches(std::filesystem::path path);
}



namespace CheckOut{
    void SwitchBranch(std::filesystem::path path, std::string swtichToBranchName);
}
