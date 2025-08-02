cmake_minimum_required(VERSION 3.14)
project(YEET VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build settings
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find ZLIB package (standard package with CMake support)
find_package(ZLIB REQUIRED)

# Find cryptopp using find_library instead of find_package
find_library(CRYPTOPP_LIBRARY
    NAMES cryptopp
    PATHS /usr/lib /usr/lib64 /usr/local/lib
    DOC "The Crypto++ library"
)

if(NOT CRYPTOPP_LIBRARY)
    message(FATAL_ERROR "Crypto++ library not found. Please install cryptopp-devel package.")
endif()

# Find cryptopp headers
find_path(CRYPTOPP_INCLUDE_DIR
    NAMES cryptopp/sha.h
    PATHS /usr/include /usr/local/include
    DOC "The Crypto++ include directory"
)

if(NOT CRYPTOPP_INCLUDE_DIR)
    message(FATAL_ERROR "Crypto++ headers not found. Please install cryptopp-devel package.")
endif()

message(STATUS "Found Crypto++: ${CRYPTOPP_LIBRARY}")
message(STATUS "Crypto++ include dir: ${CRYPTOPP_INCLUDE_DIR}")

# Add component subdirectories
add_subdirectory(YeetCore)
add_subdirectory(YeetUtils)
add_subdirectory(YeetCLI)

# Option for building tests
option(BUILD_TESTING "Build the testing tree." OFF)

# Enable testing only if the option is ON and GTest is found
if(BUILD_TESTING)
    # Try to find GTest
    find_package(GTest QUIET)
    
    if(GTest_FOUND)
        enable_testing()
        add_subdirectory(YeetTests)
    else()
        message(STATUS "GTest not found. Tests will not be built.")
        message(STATUS "To build tests, install gtest-devel and set BUILD_TESTING=ON")
    endif()
endif()