#include "./include/command.hpp"

int commands(int num_of_arg, char* argv[]){
    // Flushing the cout and cerr.
    std::cout<<std::unitbuf;  // enbaling the unitbuf flag for automatic flushing for output. has no effect on input
    std::cerr<<std::unitbuf;
    // std::cout<<"You are using cpp version: "<<__cplusplus<<std::endl; // To know my cpp version
    std::string pwd = fs::current_path();

    // TODO: instead of so many if-else, implement a map based approach
    try{
        if(num_of_arg==1){
            std::cerr<<"No argument Provied! \n> Use Command: yeet init [Path] to initialize a repository \n> yeet commit to commit \n> yeet branch to see the branching options \n> yeet status to see the changes etc to the files etc. or refer docs\n";
            return EXIT_FAILURE;
        }

        std::string command = argv[1];

        if (num_of_arg == 3) {
            if(command == "init"){
                std::string path = argv[2];
                if(fs::exists(path)){
                    YeetInit(path); // initialises a .init file
                }
                else{
                    std::cerr<<"The Path you provided doesn't exist!\nPlease check it and Retry."<<std::endl;
                }
            }

            else if(command == "add"){
		        // TODO: Implement this functionality. currently everthing is added automatically
                std::vector<std::string> File_names(num_of_arg - 2);
                for(int i=2;i<num_of_arg;i++){
                    File_names[i] = argv[i];
                }

                // Then will pass this vector of std::string to the Add function
                YeetAdd();
            }

        } 

        if(num_of_arg == 2){
            if(command == "status"){
		        // Prints the total number of addiion and deletion happened since the last commit
                YeetStatus(pwd);
            }
            else if(command == "commit"){
		        // Commits the current instance into history
                Commit myCommitObject(pwd); // TODO: Improve this, I think this can be improved
                myCommitObject.CommitMain(); 
            }

            else if(command == "history"){
                if(is_yeet_repo(pwd)){
                    history::showHistory(pwd);
                }
            }


            else if(command == "branch"){
                // and yes, there is: TODO: Make a map of commands and there functions. yes!
                std::cout<<"What you want to do?\ncreate a branch[1]\nsee all branches[2]\ndelete a branch[3]\nCurrent Branch[4]"<<std::endl;
                int in;
                std::cin>>in;
                if(in == 1){
                    std::cout<<"BRANCH::Tell us the name of the branch (NO SPACES)"<<std::endl;
                    std::string BranchName;
                    std::cin>>BranchName;
                    Branch::createBranch(BranchName , fs::current_path());
                }
                else if(in == 2){
                    Branch::SeeBranches(fs::current_path());
                }
                else if(in == 3){
                    // TODO: Select what branch you want to delete. // idk if it's possible or not
                    std::cout<<"BRANCH:: This feature is currently under development"<<std::endl;
                }
                else if(in == 4){
                    Branch::currBranch(fs::current_path());
                }

            }
            else if(command == "checkout"){
                // It can be done by changing the value in the `Branch` file.
                // `Branch` file should contain the name of the current branch we are in.
                std::cout << "\033[1m\033[3mWhich branch you want to switch into:? (TYPE FULL EXACT NAME OF THAT BRANCH)\033[0m" << std::endl;
		        // TODO: I think I can parse the file and let the user select what branch they wanna checkout to
                Branch::SeeBranches(std::filesystem::current_path());
                std::string BranchName;
                std::cin>>BranchName;
                CheckOut::SwitchBranch(fs::current_path(), BranchName);
            }
            else if(command == "init"){
                std::cout<<"ERROR::INIT::Add path name after the init to initialise a yeet repoLike \nyeet init [path]"<<std::endl;
            }
        }

    }
    catch(const std::exception& e){
        // TODO write the number of arguments and the argv required for each command in the Documentation.
        std::cout<<"ERROR:: ";
        std::cerr << e.what() << "\nException Happened somewhere! Error:501 \n";
    }    
    return EXIT_SUCCESS;
}