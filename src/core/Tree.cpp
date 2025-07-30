#include"../../include/core/Tree.hpp"

TreeEntry& TreeEntry::operator=(const TreeEntry& other){
    if(this != &other) { // to protect against self-assignment
        this->name = other.name;
        this->oid = other.oid;
        this->stat = other.stat;
    }
    return *this;
}

TreeEntry::TreeEntry(std::string name, std::string oid, std::string stat) :
        name(name),
        oid(oid), 
        stat(stat) {}

Tree::Tree(std::vector<TreeEntry>& enteries) :
        enteries(enteries) {}

std::string Tree::Type() {
    return "tree";
}

std::string Tree::Return_String(){
    std::ostringstream result;

    // Sort entries by name
    std::sort(enteries.begin(), enteries.end(), [](const TreeEntry& a, const TreeEntry& b) {
        return a.name < b.name;
    });

    for (const auto& entry : enteries) {
        std::string FileStat = (entry.stat == "Exe") ? entry.EXE_MODE : entry.REGULAR_MODE;
        result << FileStat << " " << entry.name << " " << entry.oid<< "\n";
    }
    return result.str();
}