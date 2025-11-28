#pragma once

#include"libs.hpp"
#include<fstream>

/**
 * 
 */
struct Edit
{
    enum Type
    {
        EQL,
        DEL,
        INS
    } type;
    std::string old_line;
    std::string new_line;

    Edit(Type t, const std::string &oldLine, const std::string &newLine)
        : type(t), old_line(oldLine), new_line(newLine) {}
};


/**
 * 
 */
std::vector<std::string> splitIntoLines(const std::string& str);


/**
 * 
 */
int Shortest_Edit_Search(const std::vector<std::string>& a, const std::vector<std::string>& b, 
                         std::vector<std::vector<int>>& trace);

/**
 * 
 */
std::vector<Edit> diff(const std::vector<std::string>& a, 
                      const std::vector<std::string>& b,
                      const std::vector<std::vector<int>>& trace,
                      int d);


void storeDiff(const std::vector<Edit>& edits);

/**
 * 
 */
class Printer {
public:
    Printer(std::ostream& output = std::cout) : output(output) {}
    void print(const std::vector<Edit> &diff)
    {
        // Store in Diff file
        storeDiff(diff); 
        // for (const auto &edit : diff)
        // {
            
        //     // Print out in the terminal
        //     print_edit(edit);
        // }
    }
    

private:
    std::ostream& output;
    void print_edit(const Edit &edit)
    {
        std::string col, reset = "\033[39m";
        std::string tag;
        

        switch (edit.type)
        {
        case Edit::EQL:
            col = "";
            tag = " ";
            break;
        case Edit::DEL:
            col = "\033[31m";
            tag = "-";
            break;
        case Edit::INS:
            col = "\033[32m";
            tag = "+";
            break;
        }

        std::string old_line = edit.old_line.empty() ? "" : edit.old_line;
        std::string new_line = edit.new_line.empty() ? "" : edit.new_line;

        // output << col << tag << " " << std::setw(4) << old_line << " " << std::setw(4) << new_line << "    " << (old_line.empty() ? new_line : old_line) << reset << std::endl;

    }
};
