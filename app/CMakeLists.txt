cmake_minimum_required( VERSION 3.0 )

## Required software
find_package( Threads REQUIRED )

## Sources and headers
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

## Find all source files to be compiled into object files
file( GLOB SOURCES *.cc *.cpp )



## C++ compiler options

## Source file containing the "main" function
set( MAIN_SOURCES main.cpp Game.cpp Game.h)
## Specify a name for the generated executable file
set( MAIN_EXE_NAME prizrak )

include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/src)


## 1. Compile...
add_executable( ${MAIN_EXE_NAME} ${MAIN_SOURCES}
        ${SOURCES}
        )

## 2. Link...
target_link_libraries( ${MAIN_EXE_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
        prizrak_lib
        )

## 3. Install...
install( TARGETS ${MAIN_EXE_NAME}
        RUNTIME DESTINATION bin )
