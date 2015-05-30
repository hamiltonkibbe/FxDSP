# Use FindGTest
INCLUDE(FindGTest)

# Look for existing GTest
FIND_PACKAGE(GTest)


IF(GTEST_FOUND)
    MESSAGE("-- Using existing GTest installation")
    
ELSE(GTEST_FOUND)
    
    # No existing GTEST installation. Build it here.
    MESSAGE("-- Will build GTest from source.")
    
    # Required stuff to build
    FIND_PACKAGE(Threads REQUIRED)
    INCLUDE(ExternalProject)
    
    # Download and install GoogleTest
    ExternalProject_Add(
    gtest
    URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
    )

    # Create a libgtest target to be used as a dependency by test programs
    ADD_LIBRARY(libgtest IMPORTED STATIC GLOBAL)
    ADD_DEPENDENCIES(libgtest gtest)

    # Set gtest properties
    ExternalProject_Get_Property(gtest source_dir binary_dir)
    SET_TARGET_PROPERTIES(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    )
    
    # Set gtest variables
    SET(GTEST_INCLUDE_DIRS "${source_dir}/include/")
    SET(GTEST_LIBRARIES libgtest "${binary_dir}/libgtest.a")

ENDIF(GTEST_FOUND)


ENABLE_TESTING()
FILE (GLOB SRCS src/*.cpp)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include/" ${GTEST_INCLUDE_DIRS})

ADD_EXECUTABLE(testFxDSP ${SRCS})
TARGET_LINK_LIBRARIES (testFxDSP FxDSP ${GTEST_LIBRARIES})

ADD_TEST (testFxDSP testFxDSP)
