#include"include/compression_utils.hpp"


void print_mz_error(int err) {
    std::cerr << "Miniz error: " << err << std::endl;
}

// Decompresses data using the standard Zlib method
std::vector<unsigned char> decompressData(const std::vector<unsigned char>& compressedData) {
    // setting up zlib stream
    mz_stream strm = {};
    strm.total_in = strm.avail_in = (mz_uint32)compressedData.size(); // total input bytes read so far
    strm.next_in = (unsigned char*)compressedData.data(); // next input byte

    // fiveing random size of 2x for now
    std::vector<unsigned char> decompressedData(compressedData.size() * 2); // Start with double the input size

    // inititting the init
    if (mz_inflateInit(&strm) != MZ_OK) {
        std::cerr << "inflateInit failed" << std::endl;
        return {};
    }

    int ret;
    do {
        strm.avail_out = (mz_uint32)decompressedData.size() - strm.total_out; // available output
        strm.next_out = (unsigned char*)(decompressedData.data() + strm.total_out); // next output byte will go here

        ret = mz_inflate(&strm, MZ_NO_FLUSH); // the zlib inflate function

        switch (ret) {
            case MZ_NEED_DICT:
            case MZ_DATA_ERROR:
            case MZ_MEM_ERROR:
                std::cerr << "inflate memory error: " << ret << std::endl;
                mz_inflateEnd(&strm);
                return {};
            case MZ_BUF_ERROR:
            case MZ_OK:
                if (strm.avail_out == 0) {
                    // Output buffer is full, increase its size
                    decompressedData.resize(decompressedData.size() * 2);
                }
                break;
        }
    } while (ret != MZ_STREAM_END);

    mz_inflateEnd(&strm); // end the inflate and stream
    decompressedData.resize(strm.total_out); // Resize to actual decompressed size
    return decompressedData;
}

// inflating the compressed content
std::string Inflate(std::string path){

    std::string inputFilename = path; 
    std::string response = "";

    // std::cout<<inputFilename<<std::endl;
    // this files should not be inflated. it's already inflated
    // TODO: need to add more such files here
    if(inputFilename == "/home/akhil/dev/yeet/.yeet/objects/Store") return "";

    std::string cross_file =fs::path(inputFilename).filename();
    if( cross_file == "Store") return "";

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

    mz_ulong dest_len = mz_compressBound(dest_len);
    std::vector<unsigned char> compressedData(dest_len);

    int status = mz_compress(compressedData.data(), 
        &dest_len, (const unsigned char*)content.data(),
        content.size()
    );

    if(status != MZ_OK){
        throw std::runtime_error("ERROR::COMPRESSION:: Failed to compress data");
    }

    compressedData.resize(dest_len);

    return std::string(compressedData.begin(), compressedData.end());
}
