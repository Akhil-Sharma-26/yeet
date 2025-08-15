# Define test executable
add_executable(yeet_tests
    core_tests.cpp
    utils_tests.cpp
)

# Include directories
target_include_directories(yeet_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/YeetCore/include
        ${CMAKE_SOURCE_DIR}/YeetUtils/include
)

# Link dependencies - use the targets directly
target_link_libraries(yeet_tests
    PRIVATE
        YeetCore
        YeetUtils
        gtest
        gtest_main
)

# Add tests to CTest
add_test(NAME CoreTests COMMAND yeet_tests)