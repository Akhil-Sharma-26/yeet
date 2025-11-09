# Define the library
add_library(YeetCore STATIC
    branch.cpp
    tree.cpp
    commit.cpp
    repository.cpp
    db.cpp
    refs_auth.cpp
)

# Include directories
target_include_directories(YeetCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${ZLIB_INCLUDE_DIRS}
        ${CRYPTOPP_INCLUDE_DIR}  # This will point to external/cryptopp
)

# Link dependencies
target_link_libraries(YeetCore
    PUBLIC
        ${ZLIB_LIBRARIES}
    PRIVATE
        ${CRYPTOPP_LIBRARY}      # This will be the cryptopp library we built
)

# Compiler-specific options
if(MSVC)
    target_compile_options(YeetCore PRIVATE /W4)
else()
    target_compile_options(YeetCore PRIVATE -Wall -Wextra)
endif()