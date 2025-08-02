# Define the library
add_library(YeetUtils STATIC
    file_utils.cpp
    hash_utils.cpp
    compression_utils.cpp
)

# Include directories
target_include_directories(YeetUtils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include  # Public headers
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}          # Private implementation headers
)

# Link dependencies
target_link_libraries(YeetUtils
    PUBLIC
        cryptopp
)

# Compiler options
target_compile_options(YeetUtils PRIVATE -Wall -Wextra)