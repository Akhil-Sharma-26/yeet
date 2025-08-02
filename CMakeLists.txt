cmake_minimum_required(VERSION 3.14)
project(YEET VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build settings
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set policy for timestamp extraction
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

# --- ZLIB ---
find_package(ZLIB QUIET)
if(NOT ZLIB_FOUND)
    message(STATUS "ZLIB not found, downloading and building zlib")
    FetchContent_Declare(
        zlib
        URL https://zlib.net/zlib131.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(zlib)
    if(NOT TARGET ZLIB::ZLIB)
        add_library(ZLIB::ZLIB ALIAS zlib)
    endif()
    set(ZLIB_LIBRARIES zlib)
    set(ZLIB_INCLUDE_DIRS ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
else()
    set(ZLIB_LIBRARIES ZLIB::ZLIB)
endif()

# --- Use local Crypto++ ---
set(CRYPTOPP_SRC_DIR ${CMAKE_SOURCE_DIR}/external/cryptopp)
if(NOT EXISTS ${CRYPTOPP_SRC_DIR}/sha.h)
    message(FATAL_ERROR "Crypto++ source not found at ${CRYPTOPP_SRC_DIR}. Please download from https://www.cryptopp.com/ and extract to 'external/cryptopp'")
endif()

# Build Crypto++ as a library
file(GLOB CRYPTOPP_SOURCES ${CRYPTOPP_SRC_DIR}/*.cpp)

# Remove the problematic cpu.cpp file
list(REMOVE_ITEM CRYPTOPP_SOURCES "${CRYPTOPP_SRC_DIR}/cpu.cpp")

# Add our custom stub file
list(APPEND CRYPTOPP_SOURCES "${CMAKE_SOURCE_DIR}/cpu_stub.cpp")

# Create the library target
add_library(cryptopp STATIC ${CRYPTOPP_SOURCES})
target_include_directories(cryptopp PUBLIC ${CRYPTOPP_SRC_DIR})

# Additional compile definitions
target_compile_definitions(cryptopp PUBLIC 
    CRYPTOPP_DISABLE_ASM
    CRYPTOPP_DISABLE_X86ASM
    CRYPTOPP_DISABLE_SSE2
    CRYPTOPP_DISABLE_SSSE3
    CRYPTOPP_DISABLE_SSE4
    CRYPTOPP_DISABLE_AESNI
    CRYPTOPP_DISABLE_SHANI
    CRYPTOPP_DISABLE_AVX
    CRYPTOPP_DISABLE_AVX2
    CRYPTOPP_NO_CPU_FEATURE_PROBES
    _CRT_SECURE_NO_WARNINGS
)

if(MSVC)
    target_compile_options(cryptopp PRIVATE /W0)  # Disable warnings for Crypto++
endif()

# Set variables for our components to use
set(CRYPTOPP_LIBRARY cryptopp)
set(CRYPTOPP_INCLUDE_DIR ${CRYPTOPP_SRC_DIR})



message(STATUS "Using local Crypto++ from: ${CRYPTOPP_INCLUDE_DIR}")

# Add subdirectories for each component
add_subdirectory(YeetCore)
add_subdirectory(YeetUtils)
add_subdirectory(YeetCLI)

# Option for testing - disabled by default for now
option(BUILD_TESTING "Build the testing tree." OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(YeetTests)
endif()