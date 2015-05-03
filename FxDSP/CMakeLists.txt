CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (FxDSP)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

ADD_SUBDIRECTORY(test)
ENABLE_TESTING()

INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/include/")
FILE (GLOB FXDSP_HEADERS "${PROJECT_SOURCE_DIR}/include/*.h*")
FILE (GLOB FXDSP_SRCS "${PROJECT_SOURCE_DIR}/src/*.c")

SET (CMAKE_C_FLAGS "-std=c99")
ADD_DEFINITIONS(-D_XOPEN_SOURCE)

IF ((USE_FFTW_FFT) OR ($ENV{USE_FFTW_FFT}))
MESSAGE ("-- Using FFTW3 for FFT")
ADD_DEFINITIONS(-DUSE_FFTW_FFT)

FIND_PATH (FFTW_INCLUDE_DIR fftw3.h /usr/include /usr/local/include)
FIND_LIBRARY(FFTW3_LIB NAMES libfftw3.a fftw3)
FIND_LIBRARY(FFTW3F_LIB NAMES libfftw3f.a fftw3f)
MESSAGE ("-- Found Accelerate framework at ${FFTW3_LIB}")
ENDIF ((USE_FFTW_FFT) OR ($ENV{USE_FFTW_FFT}))


IF ((USE_OOURA_FFT) OR ($ENV{USE_OOURA_FFT}))
MESSAGE ("-- Using Ooura fallback for FFT")
ADD_DEFINITIONS(-DUSE_OOURA_FFT)
ENDIF ((USE_OOURA_FFT) OR ($ENV{USE_OOURA_FFT}))


# Find and use Accelerate Framework on OSX
IF (APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    
    # Find Accelerate framework
    FIND_PATH (ACCELERATE_INCLUDE_DIR Accelerate/Accelerate.h /usr/include /usr/local/include
               /System/Library/Frameworks/vecLib.framework/Versions/A/Headers/ )
    FIND_LIBRARY (ACCELERATE_FRAMEWORK Accelerate )
    #    MARK_AS_ADVANCED (ACCELERATE_FRAMEWORK ACCELERATE_INCLUDE_DIR)
    MESSAGE ("-- Found Accelerate framework at ${ACCELERATE_FRAMEWORK}") 
    # Add Accelerate to project
    INCLUDE_DIRECTORIES (${ACCELERATE_INCLUDE_DIR})
    SET (ACCELERATE_LIB "${ACCELERATE_FRAMEWORK}")
    SET (CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -framework Accelerate" )
    
ENDIF (APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -W -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -W -O0 -Wall -fprofile-arcs -ftest-coverage")
SET(CMAKE_SHARED_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")



# Build Libraries
ADD_LIBRARY (FxDSP SHARED ${FXDSP_SRCS} ${FXDSP_HEADERS})
ADD_LIBRARY (FxDSPStatic STATIC  ${FXDSP_SRCS} ${FXDSP_HEADERS})

SET_TARGET_PROPERTIES(FxDSP PROPERTIES LINKER_LANGUAGE C)
SET_TARGET_PROPERTIES(FxDSPStatic PROPERTIES LINKER_LANGUAGE C)

TARGET_LINK_LIBRARIES (FxDSP ${ACCELERATE_LIB} ${FFTW3_LIB} ${FFTW3F_LIB} ${BLAS_LIBRARIES})
TARGET_LINK_LIBRARIES (FxDSPStatic ${ACCELERATE_LIB} ${FFTW3_LIB} ${FFTW3F_LIB} ${BLAS_LIBRARIES})



# Install libraries
INSTALL (TARGETS FxDSP FxDSPStatic DESTINATION lib)
INSTALL (DIRECTORY "${PROJECT_SOURCE_DIR}/include/" 
         DESTINATION "include/${PROJECT_NAME}/" 
         FILES_MATCHING PATTERN "*.h")


ADD_CUSTOM_TARGET (test-coverage
COMMAND make test
COMMAND [ -d coverage ] && rm -rf coverage/ || echo "No folder"
COMMAND mkdir coverage
COMMAND cp CMakeFiles/FxDSP.dir/src/*.gcno coverage/
COMMAND cp CMakeFiles/FxDSP.dir/src/*.o coverage/
COMMAND mv CMakeFiles/FxDSP.dir/src/*.gcda coverage
DEPENDS testFxDSP FxDSP FxDSPStatic
)

ADD_CUSTOM_TARGET(coverage-report
COMMAND make test-coverage
COMMAND lcov --capture --directory coverage --output-file coverage/coverage.info
COMMAND genhtml coverage/coverage.info --output-directory coverage/html
DEPENDS test-coverage
)
