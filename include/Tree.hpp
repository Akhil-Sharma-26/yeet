#include"libs.hpp"

/* An TreeEntry is a simple structure that exists to package up the information that Tree needs to
know about its contents: the filename, and the object ID. Tree will also need to know the mode
of each file, but for now all our source code is in non-executable regular files, so we will hard-
code the 100644 mode string that appears in the tree file.
*/
class TreeEntry{
    
    
    public:
    std::string name;
    std::string oid; // Object ID of this Tree Entry
    
    // TODO: Customize these codes later for yeet.
    const std::string REGULAR_MODE = "100644";
    const std::string EXE_MODE = "100755";
    
    std::string stat;

    // copy Assignment Opertor => defines how an existing TreeEntry object should behave when assigned the value of another TreeEntry object using the = operator.
    TreeEntry& operator=(const TreeEntry& other);

    TreeEntry(std::string name, std::string oid, std::string stat);

};

// This struct represents the oid of the Tree and the vector of all the TreeEnteries it has in it.
class Tree
{
   

public:
    std::string oid; // The OID of this Tree
    std::vector<TreeEntry> enteries; // The treeEnteries this Tree stores

    // constructor to initialsie this class
    Tree(std::vector<TreeEntry> & enteries);

    // The type of this DS. returns "tree";
    std::string Type(); 

    // serializes this class into a string
    std::string Return_String();
};