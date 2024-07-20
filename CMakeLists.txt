cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(yeet)
add_executable(yeet main.cpp controllers.cpp)
target_link_libraries(yeet cryptopp z)

# Include CPack module
include(CPack)

# # Set package version (ensure these are set according to your project's versioning)
# set(CPACK_PACKAGE_VERSION "1.0.0")
# set(CPACK_PACKAGE_VERSION_MAJOR "1")
# set(CPACK_PACKAGE_VERSION_MINOR "0")
# set(CPACK_PACKAGE_VERSION_PATCH "0")

# # Set general CPack variables
# set(CPACK_PACKAGE_NAME "yeet")
# set(CPACK_PACKAGE_RELEASE 1)
# set(CPACK_PACKAGE_CONTACT "Your Name <your.email@example.com>")
# set(CPACK_PACKAGE_VENDOR "Your Company")
# set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/yeet")

# # DEB specific settings
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.1.3), libz1")
# set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
# set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
# set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

# # Set the generator to DEB
# set(CPACK_GENERATOR "DEB")

# # Include CPack
# include(CPack)