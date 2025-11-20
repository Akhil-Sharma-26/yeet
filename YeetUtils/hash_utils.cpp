#include"include/hash_utils.hpp"

// Creating Hash
std::string calculateSHA256Hex(const std::string& content) { // used some copilot
    // Use picosha2 helper that returns a hex string directly
    return picosha2::hash256_hex_string(content);
}

// streaming the content into RAM in 4KB of chunks, so large files don't take up too much RAM and crash the system
std::string calculateFileSHA256Hex(const std::filesystem::path& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return "";

    std::vector<unsigned char> buffer(4096);
    picosha2::hash256_one_by_one hasher;

    while (f.read((char*)buffer.data(), buffer.size())) {
        hasher.process(buffer.begin(), buffer.end());
    }
    // Process remaining bytes
    hasher.process(buffer.begin(), buffer.begin() + f.gcount());
    
    hasher.finish();

    std::string hex_string;
    picosha2::get_hash_hex_string(hasher, hex_string);
    return hex_string;
}

std::string timeToString(time_t currtime){
    std::stringstream ss;
    ss << std::put_time(localtime(&currtime), "%Y-%m-%d %H:%M:%S"); // this is new to me.
    return ss.str();
}