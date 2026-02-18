#include "include/revert.hpp"
#include <filesystem>
#include <sstream>
// show the history

// ask for which commit you want to go to

// Tell that this will remove all the commits after that commit

// go back one by one commit removing all the hashes of that commit till we reach that commit id

// now remove all files from the folder

// construct the dirctory from ground up using the file path stored during the creating of each file's blob

// and yeah that's it
// Commit  
// Data Size: 258
// parent: 78fcb5bc3db469d5779c8db4944afad63248243f89a7b9426f74cfe98692643d
// tree: d6b3721d92fb8bb8bf0623845b9f157d5e88956a34e927c9cd201c56edd17777
// author: dfghj <dfghjk> 2025-12-07 14:27:41
// CommitedBy: dfghj <dfghjk> 2025-12-07 14:27:41
//
// CommitMessage: Oh yeah
// tree 4939100644 CMakeLists.txt 2a3a540b3196ec8d1c3f64bcebe9a1da425f42b83097ecbea8ced5c2b827fe5c
// 100644 CMakeSettings.json 430770e597a7520b1b01904a86910b9df9c146413c189098334a76eeb9cb2327
// 100644 CODE_OF_CONDUCT.md 192a5711a24d984bdec7403424b2693a6f07b175e97f54ebc905cac8c60f13dd
// 100755 DiffAlgo/Diff d9049cf48b70ac9473b4e0d22735a3c2bf914de16822d568f42c1d67cc568137
// 100644 DiffAlgo/DiffsAlgo.cpp 4a625d773e1ad5f94937b05af992831e18e4530a974cf1a38e8818bba90efb25
// 100644 LICENCE.md e7e2efbb4575317076f44c8843579a017cb0718dbc4da4f72aff51ec8552ca80
// 100644 README.md 8f9160b7a65ef15433b255f40b6fdc3ad51df61f5ecf16f5d8a6803c4126695f
// 100644 SetupENVvars.sh 940dfc294ae1ade21af5c4ef40c407959b809051f75dc68c5fcd62c1878364c8
// 100644 YeetCLI/CMakeLists.txt 3c9b66c1bb3815295f260e7f4a6b3b2f0518d366143915771decde17f6d3f2f0
// 100644 YeetCLI/commands.cpp bdfe79ed6b909cf6fd3f0dd5737d01afa029108a27d69ad6d66eb35934519c12
// 100644 YeetCLI/include/command.hpp 49e46664a589d2780a68a5f0afda82001eb7f87dc0adb3c290319dc52674dc07
// 100644 YeetCLI/main.cpp b37a44f733a79d2ee438e65ed80bb88bd4b1c5c1a1b7fbfb909c2509203cc899
// 100644 YeetCore/CMakeLists.txt dfa17ce78e5b64876fd56392cc79040a482a9b225b6fc722198fc84216ae05d0
// 100644 YeetCore/branch.cpp 32fa724e75672bb82f8ac7a3e4dd60372fd7668022eca548322e1e97959da770
// 100644 YeetCore/commit.cpp d221909d5e64af3d589fb39b22c6ebf3cc84557e05c91b31c22214f131596f0a
// 100644 YeetCore/db.cpp e086c5fe127e7358c009058c1bb9c82d4f689068cdd076d01bf2d9f4f28b762e
// 100644 YeetCore/history.cpp a6de65dc150fe9d6521be77aea8b37cbe5987247aebc09597de29ace80212c9a
// 100644 YeetCore/include/branch.hpp 4fb8a5b94361ae832116a8c9f6a2bed0df1b564350f6678d51ba86f9116eb43a
// 100644 YeetCore/include/commit.hpp 097c85743cc625b178e5eaef82287246a267e659f65db488eed004efb057c80e
// 100644 YeetCore/include/controllers.hpp fd1a60605b49afd6e2a30f5737bcc2286faf6b6605c9096e3cf9316a61ab42e1
// 100644 YeetCore/include/db.hpp 3e180d751b2a9e6ecf9b9ca6c45bd79b2d8c38d51e9c6c0434b5b196ff523b26
// 100644 YeetCore/include/history.hpp c0a784ae5d1f9b258ef61ac62f13e5e74e1062265b46dbaba2c079c3ff311e7c
// 100644 YeetCore/include/refs_auth.hpp 2554df52bb5a9daad1cef3442f2f8025fad1df23971461eff0bf002128f15ce4
// 100644 YeetCore/include/repository.hpp bfde3f84eace2d4c3bc4168756275c988def5ad606f1ab9a355d1d2b857776bd
// 100644 YeetCore/include/revert.hpp 46331f00b89bed63c48e42f0c5b2de1b6b193024b6adf66e45a13db9c9cb390f
// 100644 YeetCore/include/tree.hpp c9aadefb528463ffabaea429011140948725cd7c6e114df159f99a3e908847f9
// 100644 YeetCore/refs_auth.cpp 735a6c9cc0e266ff4a709c6497974f8517ae2c9f8686982b40e6903994289bb2
// 100644 YeetCore/repository.cpp e557a6e0937603ed030418b6b6fdf93c9142c4928c39016b18921adf1545a073
// 100644 YeetCore/revert.cpp 08c846d1aed9bff10677b0302a9fc58c43b03b27c63ec6e92e6bab4b04c65097
// 100644 YeetCore/tree.cpp 31c24d06fd39022f349b05ea74d18827d5828f8ae65f8364f0ceda0761feaaa1
// 100644 YeetTests/CMakeLists.txt 83940e3d6e7908b236e0d3520bbabbc034b2f3bae92cf1764ae8c87853184661
// 100644 YeetTests/core_tests.cpp e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
// 100644 YeetTests/utils_tests.cpp e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
// 100644 YeetUtils/CMakeLists.txt 8ad55f5370747f1c24b9cd1191b9922478fee4163a243442ecd4cb4376e03661
// 100644 YeetUtils/compression_utils.cpp 89147cc4344f0a3cdc1dda2f5f2730f42ad0fdf44e08801f630072d5144e17f3
// 100644 YeetUtils/diff_utils.cpp b6086022f53a215f67210d6499ebd40cf965b4b9ff2f0446646612a9d9067c79
// 100644 YeetUtils/file_utils.cpp 98580f779202384f9110307c6181b50070df6d992750e83f3ca5ebad38beb927
// 100644 YeetUtils/hash_utils.cpp 5da035405927c2cf676878e887b3acdd29489bce06715d609bdf207a2a9e098f
// 100644 YeetUtils/include/compression_utils.hpp 77b9b19f91f995d0cc6aa36cadb3dbaa9b96542cdd11a95b8a9c48662513d29e
// 100644 YeetUtils/include/diff_utils.hpp 2b7829550e7189130f538773a573cd2ba3f1984c3050aa24ae172b5a7bcdfdbf
// 100644 YeetUtils/include/file_utils.hpp 25fa33e1af802336dec7ec327bd7bc1542c34b18890dea66878a80ada46ea0f0
// 100644 YeetUtils/include/hash_utils.hpp ea5cb6f41bb05d2aecf5b40049299cb423e6e4bebb18e656183d632a60158583
// 100644 YeetUtils/include/libs.hpp d80aa238e7eae4635fccc7a36a6a9a199602b7c78b81acd1738b4f795d1591e3
// 100644 YeetUtils/include/vendor/miniz.c 7487d4c8cd761b951a99d182672bb3badfa72a5ad5760b70b392fafa95223657
// 100644 YeetUtils/include/vendor/miniz.h 13a6940a8f33b3a76d4b64fb6d482b246954bae3bf5dc713f7f054d2096d0aba
// 100644 YeetUtils/include/vendor/picosha2.h 258ef6f21f5a2cfaf020cf6b0379a957ce39adc54906c61cfd9e30e663c0f698
// 100644 assets/Branching_FLow.png 1dbfcb10dc94247e85d01117313abfc2b5836bee9b64c1f6dd540f7b064c5775
// 100644 assets/Class_Diagram.png df4349c494e9e3d94faea8a2a6d885bb512ea3c4475ad753534b7314aaa0ceb6
// 100644 assets/Data_FLow.png 0eeb401cc7ef4ef8243e4e231cc2d6113687c25798afa7da3e75b956ab50d29b
// 100644 assets/Sequence_Diagram.png f7f40de965b9a2ecbaca6801c754c7314d09c50ec7c374b2cfc59dc36d847746
// 100644 assets/YeetFlowDiagram.drawio 5cbf188bc31eda90c00429c635fa12c01d2b9e5a1a6fff9db7368a3c72093f12


void deleteDirectory(const fs::path& dirPath) {
    if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
        for (const auto& entry: fs::directory_iterator(dirPath)) {
            if(entry.path().filename().string() == ".yeet") continue;
            // Remove each entry (file or subdirectory) recursively
            try {
                fs::remove_all(entry.path());
                std::cout << "Deleted: " << entry.path() << std::endl;
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Error deleting " << entry.path() << ": " << e.what() << std::endl;
            }
        }
    } else {
        std::cerr << "Directory not found or is not a directory: " << dirPath << std::endl;
    }
}


void revert(std::string commit_id){
    // check current commit
    Refs ref(fs::current_path());
    std::string curr_commit_oid = ref.Read_HEAD();
    fs::path curr_path = fs::current_path();
    std::cout<<"Current Path: "<<curr_path<<std::endl;

    if(commit_id == curr_commit_oid) {
        std::cerr<<"REVERT::You are already at the same commit!"<<std::endl;
    }

    // // Recurrsivly till commit_id == curr_id{
    // while(commit_id != curr_commit_oid){
    //     std::string commit_content = Inflate(fs::path(ref.path) / ".yeet" / "objects" / curr_commit_oid.substr(0, 2) / curr_commit_oid.substr(2));
    //
    //     // get the parent commit
    //     std::string parent_oid = CommitHelper::getParentOidFromCommit(commit_content);
    //     curr_commit_oid = parent_oid;
    //
    //     std::string currTree_oid = CommitHelper::getTreeOidFromCommit(commit_content);
    //     std::string tree_content = Inflate(fs::path(ref.path) / ".yeet" / "objects" / currTree_oid.substr(0, 2) / currTree_oid.substr(2));
    //
    //     std::cout<<tree_content<<std::endl;
    //
    //     std::vector<fs::path> paths_to_delete;
    //     std::istringstream ss(tree_content);
    //     std::string line;
    //
    //     while(std::getline(ss, line)){
    //         if(line.substr(0,4) == "tree") continue;
    //         std::string oid = line.substr(line.size()-64);
    //         std::cout<<oid<<std::endl;
    //     }
    //
    //     // for(const auto &it:paths_to_delete){
    //     //     fs::remove(it);
    //     // }
    //     std::cout<<"So wei've reached the commit_id which the user entered in the query"<<std::endl;
    // }
    //

    // delete the whole Repo:
    deleteDirectory(curr_path);

    std::cout<<"Deleted Everything"<<std::endl;

    // now its time to restore the content for the ufcking commit


    
        // {
            // path is in tree.
            // in either. delete the file
        // }
    // }

    // delete the whole repo except .yeet
    // read the contetn of the commit file. 
        // inflate it
    // get the parent commit
    // open the tree oid file
        // inflate it
    // {
        // idr if path is in blob or in tree.
        // in either. make file
        // inflate content and paste in the files!
    // }
    // you have successfully reverted back to the $commit_id commit!
}
