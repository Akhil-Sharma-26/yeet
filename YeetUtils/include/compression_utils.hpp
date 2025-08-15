#pragma once

// Include the zlib from the FetchContent build
#ifdef HAVE_ZLIB_H
  #include HAVE_ZLIB_H
#else
  // Use full path if defined in compilation
  #include <zlib.h> 
#endif
#include <sha.h>
#include"libs.hpp"
#include"file_utils.hpp"


/**
 * 
 */
std::vector<unsigned char> decompressData(const std::vector<unsigned char>& compressedData);

/**
 * 
 */
std::string Inflate(std::string path);

/**
 * The below code is setup of zlib compression
    https://www.zlib.net/manual.html#Basic
    @param the content of be compressed
    @return The compressed string
*/
std::string Compressing_using_zlib(std::string& content);