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

# Find external packages
find_package(cryptopp QUIET)
find_package(ZLIB REQUIRED)

# Define source files explicitly while transitioning
set(CORE_SOURCES
    src/core/Tree.cpp
    src/core/Commit.cpp
)

set(LEGACY_SOURCES
    main.cpp
    controllers.cpp
)

# Create the executable with current sources
add_executable(yeet ${LEGACY_SOURCES} ${CORE_SOURCES})

# Link against required libraries
target_link_libraries(yeet PRIVATE cryptopp z)

# Add compiler options for optimization and error handling
target_compile_options(yeet PRIVATE -O2 -Wall)

# Enable testing infrastructure (to be used later)
enable_testing()

# Include CPack module for packaging
include(CPack)