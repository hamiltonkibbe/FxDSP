# We need thread support
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
gtest
URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
# Disable install step
INSTALL_COMMAND ""
)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set gtest properties
ExternalProject_Get_Property(gtest source_dir binary_dir)
set_target_properties(libgtest PROPERTIES
"IMPORTED_LOCATION" "${binary_dir}/libgtest.a"
"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
include_directories("${source_dir}/include")


ENABLE_TESTING()
FILE (GLOB SRCS src/*.cpp)
INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/include/")
ADD_EXECUTABLE (testFxDSP ${SRCS})
TARGET_LINK_LIBRARIES (testFxDSP FxDSP libgtest)
ADD_TEST (testFxDSP testFxDSP)