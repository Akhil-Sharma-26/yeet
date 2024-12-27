cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(yeet)

add_executable(yeet main.cpp controllers.cpp)

# Add compiler options for optimization and error handling
target_compile_options(yeet PRIVATE -O2 -Wall)

target_link_libraries(yeet cryptopp z)

# Include CPack module
include(CPack)