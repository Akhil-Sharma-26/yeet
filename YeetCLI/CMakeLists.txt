# Define the executable
add_executable(yeet
    main.cpp
    commands.cpp
)

# Include directories
target_include_directories(yeet
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include      # Private CLI headers
        ${CMAKE_SOURCE_DIR}/YeetCore/include     # Core public headers
        ${CMAKE_SOURCE_DIR}/YeetUtils/include    # Utils public headers
)

# Link dependencies
target_link_libraries(yeet
    PRIVATE
        YeetCore
        YeetUtils
)

# Compiler options
target_compile_options(yeet PRIVATE -Wall -Wextra)