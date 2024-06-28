#include <iostream>
#include <fstream>
#include <vector>
#include <zlib.h>
#include<chrono>
std::vector<unsigned char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return {};
    }
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

bool writeFile(const std::string& filename, const std::vector<unsigned char>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return false;
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return true;
}

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
int main() {
    std::cout<<"Help"<<std::endl; 
    auto start = std::chrono::high_resolution_clock::now();
    // This saved me a lot of time today. 
    std::string inputFilename = "/home/akhil/dev/yeet/.yeet/objects/80/01cdc26aac0acc2f9f767eff0ba009e2fea3b5"; 
    std::string outputFilename = "/home/akhil/dev/yeet/test-abc";

    auto compressedData = readFile(inputFilename);
    if (compressedData.empty()) return 1;
    // std::cout<<"Help"<<std::endl;
    auto decompressedData = decompressData(compressedData);
    if (decompressedData.empty()) return 1;
    // std::cout<<"Help"<<std::endl;
    if (!writeFile(outputFilename, decompressedData)) return 1;
    // std::cout<<"Help"<<std::endl;


    auto end = std::chrono::high_resolution_clock::now(); // Get the end time
    std::chrono::duration<double, std::milli> duration = end - start; // Calculate the duration

    std::cout << "Execution time: " << duration.count() << " ms" << std::endl; // Output the duration in milliseconds
    std::cout << "Decompression completed successfully." << std::endl;
    return 0;
}