#include"include/compression_utils.hpp"
 
// Decompresses data using the standard Zlib method
std::vector<unsigned char> decompressData(const std::vector<unsigned char>& compressedData) {
    // setting up zlib stream
    z_stream strm = {};
    strm.total_in = strm.avail_in = compressedData.size(); // total input bytes read so far
    strm.next_in = (Bytef*)compressedData.data(); // next input byte

    // fiveing random size of 2x for now
    std::vector<unsigned char> decompressedData(compressedData.size() * 2); // Start with double the input size

    // inititting the init
    if (inflateInit(&strm) != Z_OK) {
        std::cerr << "inflateInit failed" << std::endl;
        return {};
    }

    int ret;
    do {
        strm.avail_out = decompressedData.size() - strm.total_out; // available output
        strm.next_out = (Bytef*)(decompressedData.data() + strm.total_out); // next output byte will go here

        ret = inflate(&strm, Z_NO_FLUSH); // the zlib inflate function

        switch (ret) {
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                std::cerr << "inflate memory error: " << ret << std::endl;
                inflateEnd(&strm);
                return {};
            case Z_BUF_ERROR:
            case Z_OK:
                if (strm.avail_out == 0) {
                    // Output buffer is full, increase its size
                    decompressedData.resize(decompressedData.size() * 2);
                }
                break;
        }
    } while (ret != Z_STREAM_END);

    inflateEnd(&strm); // end the inflate and stream
    decompressedData.resize(strm.total_out); // Resize to actual decompressed size
    return decompressedData;
}

// inflating the compressed content
std::string Inflate(std::string path){

    std::string inputFilename = path; 
    std::string response = "";

    std::cout<<inputFilename<<std::endl;
    // this files should not be inflated. it's already inflated
    // TODO: need to add more such files here
    if(inputFilename == "/home/akhil/dev/yeet/.yeet/objects/Store") return "";

    // getting the compresses data
    auto compressedData = readFile(inputFilename);
    if (compressedData.empty()) return "Error in compressed data";

    // decompress the data
    auto decompressedData = decompressData(compressedData);
    if (decompressedData.empty()) return "Error in decompresssion";

    for(auto it:decompressedData){
        response+=it;
    }

    return response;
}

// refractored Compression Logic
std::string Compressing_using_zlib(std::string& content) {
    if (content.empty()) return "";

    z_stream stream{};
    stream.zalloc = nullptr;
    stream.zfree = nullptr;
    stream.opaque = nullptr;
    stream.avail_in = static_cast<uInt>(content.size());
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(content.data()));

    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        throw std::runtime_error("ERROR::COMPRESSION:: Failed to initialize zlib deflate.");
    }

    std::vector<unsigned char> compressedData(compressBound(content.size()));
    stream.avail_out = compressedData.size();
    stream.next_out = compressedData.data();

    if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&stream);
        throw std::runtime_error("ERROR::COMPRESSION:: Failed to deflate data.");
    }

    compressedData.resize(stream.total_out);

    if (deflateEnd(&stream) != Z_OK) {
        throw std::runtime_error("ERROR::COMPRESSION:: Failed to finalize zlib deflate.");
    }

    return std::string(compressedData.begin(), compressedData.end());
}
