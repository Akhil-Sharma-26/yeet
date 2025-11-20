# Define the library
add_library(YeetCore STATIC
    branch.cpp
    tree.cpp
    commit.cpp
    repository.cpp
    db.cpp
    refs_auth.cpp
    history.cpp
)

# Include directories
target_include_directories(YeetCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)


# Compiler-specific options
if(MSVC)
    target_compile_options(YeetCore PRIVATE)
else()
    target_compile_options(YeetCore PRIVATE)
endif()