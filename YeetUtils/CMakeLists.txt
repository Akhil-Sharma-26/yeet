# Define the library
add_library(YeetUtils STATIC
    file_utils.cpp
    hash_utils.cpp
    compression_utils.cpp
    diff_utils.cpp
)

# Include directories
target_include_directories(YeetUtils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include  # Public headers
    PRIVATE
        ${ZLIB_INCLUDE_DIRS}                 # Put zlib first
        ${CMAKE_CURRENT_SOURCE_DIR}          # Private implementation headers
        ${CRYPTOPP_INCLUDE_DIR}              # Crypto++ after zlib
)

# Add a definition to help locate the correct zlib.h
target_compile_definitions(YeetUtils PRIVATE
    HAVE_ZLIB_H="${zlib_SOURCE_DIR}/zlib.h"
)

# Link dependencies
target_link_libraries(YeetUtils
    PUBLIC
        ${ZLIB_LIBRARIES}                    # Link against zlib first
        cryptopp
)

# Compiler options
if(MSVC)
    target_compile_options(YeetCore PRIVATE /W4)
else()
    target_compile_options(YeetCore PRIVATE -Wall -Wextra)
endif()