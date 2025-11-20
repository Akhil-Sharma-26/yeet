cmake_minimum_required(VERSION 3.14)
project(YEET VERSION 0.1.0 LANGUAGES C CXX)

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

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

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