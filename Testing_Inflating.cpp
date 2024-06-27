#include <zlib.h>
#include <iostream>
#include <vector>
#include<fstream>

// Function to inflate (decompress) input data

std::vector<unsigned char> Inflate(const std::vector<unsigned char>& compressedData) {
    if (compressedData.empty()) {
        std::cerr << "Error: Empty input data" << std::endl;
        return {};
    }

    z_stream strm = {};
    strm.total_in = strm.avail_in = compressedData.size();
    strm.next_in = (Bytef*)compressedData.data();

    std::vector<unsigned char> decompressedData(2 * compressedData.size());

    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        std::cerr << "inflateInit error: " << ret << std::endl;
        return {};
    }

    do {
        strm.avail_out = decompressedData.size() - strm.total_out;
        strm.next_out = (Bytef*)(decompressedData.data() + strm.total_out);
        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            (void)inflateEnd(&strm);
            std::cerr << "inflate error: " << ret << std::endl;
            return {};
        }
    } while (strm.avail_out == 0);

    if (inflateEnd(&strm) != Z_OK) {
        std::cerr << "inflateEnd error" << std::endl;
        return {};
    }

    decompressedData.resize(strm.total_out);
    return decompressedData;
}

int main() {
    // Example usage
    // Note: Replace `compressedData` with your actual compressed data
        std::vector<unsigned char> compressedData;
        std::ifstream file("/home/akhil/dev/yeet/.yeet/objects/ca/4786927f5c094469091b5b1696c20420fe068c", std::ios::binary);
        
        if (file) {
            file.seekg(0, std::ios::end);
            std::streampos fileSize = file.tellg();
            file.seekg(0, std::ios::beg);
            compressedData.resize(fileSize);
            file.read(reinterpret_cast<char*>(compressedData.data()), fileSize);
            file.close();
        } else {
            std::cerr << "Failed to open file" << std::endl;
            return 1;
        }

        std::vector<unsigned char> decompressedData = Inflate(compressedData);
        // Output the decompressed data as a string
        std::string output(decompressedData.begin(), decompressedData.end());
        std::cout << "Decompressed data: " << output << std::endl;
        return 0;
}