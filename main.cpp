#include <iostream>
#include "controllers.hpp"
#include<filesystem>
using namespace std;

int main(int argc, char* argv[]) {
    // Flushing the cout and cerr.
    cout<<unitbuf; 
    cerr<<unitbuf;
    cout<<__cplusplus<<endl; // To know my cpp version
    try{
        string arg=argv[1];
        string subArg=argv[2];


        if (argc == 3) {
            // TODO Add if-else conditions to distinguish different argument that the user is passing.
            
            // cout<<subArg<<endl;
            // cout<<arg<<endl;
            if(arg=="init"){
                YeetInit(subArg);
            }

            else if(arg=="add"){
                YeetAdd();
            } 
        } 

        if(argc==2){
            if(arg=="status"){
                YeetStatus();
            }
        }

        // else if(argc > 2) print("only one argiment is supported for now!");
        else if(argc==1){
            // TODO Add some useful string in this case. As is for user to know the different types of commands.
            cerr<<"No argument Provied \n";
            return EXIT_FAILURE;
            }
    }
    catch(const std::exception& e){
        // TODO write the number of arguments and the argv required for each command in the Documentation.
        std::cerr << e.what() << "\nPlease give the enough arguments with the commands. \n";
    }
    
    return EXIT_SUCCESS;
}