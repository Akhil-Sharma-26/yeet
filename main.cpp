#include <iostream>
using namespace std;

void print(const string& s) {
    std::cout<<s<<std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        // TODO 
        // 1. Add if-else conditions to distinguish different argument that the user is passing.
        
    } 








    else if(argc > 2) print("only one argiment is supported for now!");
    else if(argc==1){
        // TODO Add some useful string in this case. As is for user to know the different types of commands.
        print("Please add some argument");
    }
    return 0;
}