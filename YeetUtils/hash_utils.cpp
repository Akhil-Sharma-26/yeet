#include"include/hash_utils.hpp"

// Creating Hash
std::string calculateSHA256Hex(const std::string& content) { // used some copilot
    CryptoPP::SHA256 SHA256;
    std::string hash;
    // Create a filter that calculates the SHA256 hash and then encodes it as hexadecimal
    CryptoPP::StringSource(content, true, new CryptoPP::HashFilter(SHA256, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash),false)));

    return hash;
}

std::string timeToString(time_t currtime){
    std::stringstream ss;
    ss << std::put_time(localtime(&currtime), "%Y-%m-%d %H:%M:%S"); // this is new to me.
    return ss.str();
}