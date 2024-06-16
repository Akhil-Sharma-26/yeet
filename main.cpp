#include <iostream>
#include "controllers.hpp"
using namespace std;

void print(const string& s) {
    std::cout<<s<<std::endl;
}

int main(int argc, char* argv[]) {

    // Flushing the cout and cerr.
    cout<<unitbuf; 
    cerr<<unitbuf;
    cout<<__cplusplus; // To know my cpp version

    if (argc == 2) {
        // TODO 
        // Add if-else conditions to distinguish different argument that the user is passing.
        string arg=argv[1];

        if(arg=="init"){
            YeetInit();
        }

        else if(arg=="add"){
            YeetAdd();
        }

        else if(arg=="status"){
            YeetStatus();
        }
    } 








    else if(argc > 2) print("only one argiment is supported for now!");
    else if(argc==1){
        // TODO Add some useful string in this case. As is for user to know the different types of commands.
        cerr<<"No argument Provied \n";
        return EXIT_FAILURE;
    }
    return 0;
}