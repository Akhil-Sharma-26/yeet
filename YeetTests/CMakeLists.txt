# Find GTest
find_package(GTest REQUIRED)

# Define test executable
add_executable(yeet_tests
    core_tests.cpp
    utils_tests.cpp
)

# Include directories
target_include_directories(yeet_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include      # Test headers
        ${CMAKE_SOURCE_DIR}/YeetCore/include     # Core public headers
        ${CMAKE_SOURCE_DIR}/YeetUtils/include    # Utils public headers
        ${GTEST_INCLUDE_DIRS}                    # GTest headers
)

# Link dependencies
target_link_libraries(yeet_tests
    PRIVATE
        YeetCore
        YeetUtils
        GTest::GTest
        GTest::Main
)

# Add tests to CTest
add_test(NAME CoreTests COMMAND yeet_tests)