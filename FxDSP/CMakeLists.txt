CMAKE_MINIMUM_REQUIRED (VERSION 3.1)
PROJECT (FxDSP)


INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/include/")
FILE (GLOB FXDSP_HEADERS "${PROJECT_SOURCE_DIR}/include/*.h*")
FILE (GLOB FXDSP_SRCS "${PROJECT_SOURCE_DIR}/src/*.c")

SET (CMAKE_C_STANDARD 99)

# Find and use Accelerate Framework on OSX
IF (APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    
    # Find Accelerate framework
    FIND_PATH (ACCELERATE_INCLUDE_DIR Accelerate/Accelerate.h /usr/include /usr/local/include
               /System/Library/Frameworks/vecLib.framework/Versions/A/Headers/ )
    FIND_LIBRARY (ACCELERATE_FRAMEWORK Accelerate )
    MARK_AS_ADVANCED (ACCELERATE_FRAMEWORK ACCELERATE_INCLUDE_DIR)
    
    # Add Accelerate to project
    INCLUDE_DIRECTORIES (${ACCELERATE_INCLUDE_DIR})
    SET (EXTRA_LIBS ${ACCELERATE_FRAMEWORK})
    SET (CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -framework Accelerate" )
    
ENDIF (APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


# Build Libraries
ADD_LIBRARY (FxDSP SHARED ${FXDSP_SRCS} ${FXDSP_HEADERS})
ADD_LIBRARY (FxDSPStatic STATIC  ${FXDSP_SRCS} ${FXDSP_HEADERS})

SET_TARGET_PROPERTIES(FxDSP PROPERTIES LINKER_LANGUAGE C)
SET_TARGET_PROPERTIES(FxDSPStatic PROPERTIES LINKER_LANGUAGE C)

TARGET_LINK_LIBRARIES (FxDSP ${EXTRA_LIBS})
TARGET_LINK_LIBRARIES (FxDSPStatic ${EXTRA_LIBS})

# Install libraries
INSTALL (TARGETS FxDSP FxDSPStatic DESTINATION lib)
INSTALL (DIRECTORY "${PROJECT_SOURCE_DIR}/include/" 
         DESTINATION "include/${PROJECT_NAME}/" 
         FILES_MATCHING PATTERN "*.h")

