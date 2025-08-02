# Define the library
add_library(YeetCore STATIC
    tree.cpp
    commit.cpp
    repository.cpp
    controllers.cpp
)

# Include directories
target_include_directories(YeetCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include  # Public headers
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}          # Private implementation headers
)

# Link dependencies
target_link_libraries(YeetCore
    PUBLIC
        ZLIB::ZLIB
)

# Compiler options
target_compile_options(YeetCore PRIVATE -Wall -Wextra)