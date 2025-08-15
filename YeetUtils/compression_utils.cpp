#include"include/compression_utils.hpp"
 

std::vector<unsigned char> decompressData(const std::vector<unsigned char>& compressedData) {
    z_stream strm = {};
    strm.total_in = strm.avail_in = compressedData.size();
    strm.next_in = (Bytef*)compressedData.data();

    std::vector<unsigned char> decompressedData(compressedData.size() * 2); // Start with double the input size

    if (inflateInit(&strm) != Z_OK) {
        std::cerr << "inflateInit failed" << std::endl;
        return {};
    }

    int ret;
    do {
        strm.avail_out = decompressedData.size() - strm.total_out;
        strm.next_out = (Bytef*)(decompressedData.data() + strm.total_out);

        ret = inflate(&strm, Z_NO_FLUSH);

        switch (ret) {
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                std::cerr << "inflate error: " << ret << std::endl;
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

    inflateEnd(&strm);
    decompressedData.resize(strm.total_out); // Resize to actual decompressed size
    return decompressedData;
}

std::string Inflate(std::string path){

    std::string inputFilename = path; 
    std::string response = "";

    std::cout<<inputFilename<<std::endl;
    if(inputFilename == "/home/akhil/dev/yeet/.yeet/objects/Store") return "";

    auto compressedData = readFile(inputFilename);
    if (compressedData.empty()) return "Error in compressed data";

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
