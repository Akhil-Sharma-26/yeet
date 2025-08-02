#include <iostream>
#include "../YeetCore/include/controllers.hpp"
#include <filesystem>
#define ll long long
using namespace std;


int main(int argc, char* argv[]) {
    // Flushing the cout and cerr.
    cout<<unitbuf; 
    cerr<<unitbuf;
    cout<<"Using cpp version: "<<__cplusplus<<endl; // To know my cpp version
    string path = fs::current_path().string();
    // cout<<path<<endl;
    try{
        if(argc==1){
            // TODO Add some useful string in this case. As is for user to know the different types of commands.
            cerr<<"No argument Provied \n Use Command: yeet init . or yeet commit etc. or refer docs\n";
            return EXIT_FAILURE;
            }
        string arg=argv[1];

        if (argc==3) {
            // TODO Add if-else conditions to distinguish different argument that the user is passing.
            
            // cout<<subArg<<endl;
            // cout<<arg<<endl;
            string subArg=argv[2];
            if(arg=="init"){
                YeetInit(subArg);
            }

            else if(arg=="add"){
                YeetAdd();
            }
            // testing status

        } 
        // testing the commit


        if(argc==2){
            if(arg=="status"){
                YeetStatus(path);
            }
            else if(arg=="commit"){
                Commit myCommitObject(path);
                myCommitObject.CommitMain(path);
            }
            else if(arg=="branch"){
                std::cout<<"What you want to do?\ncreate a branch[1]\nsee all branches[2]\ndelete a branch[3]\nCurrent Branch[4]"<<std::endl;
                int in;
                cin>>in;
                if(in == 1){
                    std::cout<<"BRANCH::Tell us the name of the branch (NO SPACES)"<<std::endl;
                    std::string BranchName;
                    cin>>BranchName;
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
            else if(arg == "checkout"){
                // It can be done by changing the value in the `Branch` file.
                // `Branch` file should contain the name of the current branch we are in.
                std::cout << "\033[1m\033[3mWhich branch you want to switch into:? (TYPE FULL EXACT NAME OF THAT BRANCH)\033[0m" << std::endl;
                Branch::SeeBranches(std::filesystem::current_path());
                std::string BranchName;
                cin>>BranchName;
                CheckOut::SwitchBranch(fs::current_path(), BranchName);
            }
            else if(arg == "init"){
                std::cout<<"ERROR::INIT::Add . after the init to initialise a yeet repo in the pwd. Like \nyeet init ."<<std::endl;
            }
        }

        // else if(argc > 2) print("only one argiment is supported for now!");
        // test
        // cv
    }
    catch(const std::exception& e){
        // TODO write the number of arguments and the argv required for each command in the Documentation.
        std::cout<<"ERROR:: ";
        std::cerr << e.what() << "\nException Happened somewhere! Error:501 \n";
    }
    
    return EXIT_SUCCESS;
}