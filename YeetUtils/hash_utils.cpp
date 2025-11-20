#include"include/hash_utils.hpp"

std::string toHex(const std::vector<unsigned char>& bytes){
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for( unsigned char byte: bytes){
        ss << ss::setw(2) << (int)byte;
    }

    return ss.str();
}

// Creating Hash
std::string calculateSHA256Hex(const std::string& content) { // used some copilot
    std::vector<unsigned char> hash(picosha2::k_digest_size);

    picosha2::hash256(content.begin(), content.end(), hash.begin());

    return toHex(hash);
}

std::string timeToString(time_t currtime){
    std::stringstream ss;
    ss << std::put_time(localtime(&currtime), "%Y-%m-%d %H:%M:%S"); // this is new to me.
    return ss.str();
}