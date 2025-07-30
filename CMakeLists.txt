cmake_minimum_required(VERSION 3.14)
project(YEET VERSION 0.1.0 
        DESCRIPTION "YEET Version Control System" 
        LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories - both old and new
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}     # Old root includes
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # New includes
)

# Find all current source files TODO: to be gradually replaced
file(GLOB_RECURSE ALL_CURRENT_SOURCES "*.cpp" "!build/*")

# Set up libraries and executables with current sources
# TODO: These will be refined in subsequent steps
add_executable(yeet ${ALL_CURRENT_SOURCES})

# Enabling testing infrastructure TODO:(to be used later)
enable_testing()