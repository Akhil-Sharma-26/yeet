#include"include/db.hpp"

// Blob Class
Blob::Blob(std::string newdata){
    this->data = newdata;
    // std::cout<<"The blob oid is: "<<this->oid<<std::endl;
}

std::string Blob::type(){
    return "blob";
}

// Database Class
Database::Database(std::filesystem::path path){
    this->path = path;
}

void Database::storeContentInDB(Blob &object, const std::string &path){
    std::string Data = object.data;

    // TODO: make proper diff here
    // ! I am putiing only the data in the content to simplify the process of diff
    // std::string content = object.type() + " " + std::to_string(Data.size()) + "\0" + Data; // The null character is included just to use when we itterate over it.
    
    std::string content = Data;
    object.oid = calculateSHA256Hex(fs::path(path));
    Store[path] = object.oid;

    // std::cout<<object.oid<<std::endl; // Hashes are coming out.
    write_object(object.oid, content); // Writing/ making directories of the commit object/blob
}

void Database::storeContentInDB(Tree &object){
    std::string Data = object.Return_String();
    std::string content = object.Type() + " " + std::to_string(Data.size()) + "\0" + Data; // The null character is included just to use when we itterate over it.
    // std::cout<<"the content: "<<content<<std::endl;
    object.oid = calculateSHA256Hex(content);
    // std::cout<<"The hash of the tree object is: "<<object.oid<<std::endl; // Hashes are coming out.
    write_object(object.oid, content); // Writing/ making directories of the commit object/blob
}

void Database::storeContentInDB(Commit &object){
    std::string Data = object.Writtenlines;
    std::string content = "Commit  \nData Size: " + std::to_string(Data.size()) + "\0 \n" + Data; // The null character is included just to use when we itterate over it.
    // TODO: I have added Data size as extra here for readablitliy, but it may create problem later.
    // std::cout<<"the content: "<<content<<std::endl;
    object.oid = calculateSHA256Hex(content);
    // std::cout<<"The hash of the Commit object is: "<<object.oid<<std::endl; // Hashes are coming out.
    write_object(object.oid, content); // Writing/ making directories of the commit object/blob
}

// make it cross-platform
void Database::write_object(std::string oid, std::string content){
    try
    {
        std::string dirName = oid.substr(0, 2);
        std::string fileName = oid.substr(2); 

        fs::path objectDir = this->path / dirName;
        fs::path objectFile = objectDir / fileName;

        if (fs::exists(objectFile)) return;

        fs::create_directory(objectDir);
        /** Actual File Path to the object created. */
        /** res contains the return value of the `touch` command. */
        // int res = std::system(("touch " + this->path.generic_string() + "/" + Dir_name + "/" + File_name_Helper(obj_path)).c_str());
        // if (res != 0)
        //     std::runtime_error("touch is not working \n");
        // std::cout<<"Hello, I am the File: "<<this->path.generic_string()+"/"+Dir_name+"/"+File_name_Helper(obj_path).c_str()<<std::endl;
        // Compressing the content
        std::string compressed_data = Compressing_using_zlib(content);
        std::ofstream f(fs::path(objectDir / objectFile).string(), std::ios::out | std::ios::binary);
        if (f.is_open())
        {
            f.write(compressed_data.c_str(), compressed_data.size());
            f.close();
        }
        else
        {
            std::runtime_error("Unable to write in Commit Blob Files!!");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        throw;
    }
}