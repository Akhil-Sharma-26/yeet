#include"include/hash_utils.hpp"

// Creating Hash
std::string calculateSHA256Hex(const std::string& content) { // used some copilot
    // Use picosha2 helper that returns a hex string directly
    return picosha2::hash256_hex_string(content);
}

std::string timeToString(time_t currtime){
    std::stringstream ss;
    ss << std::put_time(localtime(&currtime), "%Y-%m-%d %H:%M:%S"); // this is new to me.
    return ss.str();
}