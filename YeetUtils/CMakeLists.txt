# Define the library
add_library(YeetUtils STATIC
    file_utils.cpp
    hash_utils.cpp
    compression_utils.cpp
    diff_utils.cpp
    include/vendor/miniz.c
)

# Include directories
target_include_directories(YeetUtils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include  # Public headers
)


# Compiler options
if(MSVC)
    target_compile_options(YeetCore PRIVATE)
else()
    target_compile_options(YeetCore PRIVATE)
endif()