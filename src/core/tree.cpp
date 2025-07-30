#include"../../include/Tree.hpp"

TreeEntry& TreeEntry::operator=(const TreeEntry& other){
    if(this != &other) { // to protect agains self-assignment
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