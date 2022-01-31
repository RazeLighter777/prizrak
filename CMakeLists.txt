cmake_minimum_required( VERSION 3.0 )

cmake_policy(SET CMP0079 NEW)

## Load CMAKE configuration from environment variables
set( CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} )
set( CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH} )

## Specify a project name
project( prizrak
        VERSION 0.0.0
        DESCRIPTION "Everything")
set(CMAKE_CXX_STANDARD 20)


## Set the build type
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE "Release"
            CACHE STRING "Build configuration 'Release' or 'Debug'."
            FORCE )
endif()

## Install the executable to <install_directory>/bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
## Install libraries (if any) to <install_directory>/bin
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/app bin)


enable_testing()
add_subdirectory(test)

