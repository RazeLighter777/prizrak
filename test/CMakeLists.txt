include(FetchContent)
FetchContent_Declare(gtest
        QUIET
        URL https://github.com/google/googletest/archive/release-1.11.0.tar.gz
        )
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(GTEST_CATCH_EXCEPTIONS 0)
set(GTEST_BREAK_ON_FAILURE 1)
set(CMAKE_CXX_FLAGS "-Wno-error")

FetchContent_MakeAvailable(gtest)

#Adds each file in the test/ directory to the unit testing executable unit_tests
add_executable(
        unit_tests
        sum_test.cpp
         raw_tests.cpp Utils.cpp)

#Adds the prizrak headers so we can use them to build the tests.
include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/src)

#Adds the prizrak and the gtest LIBRARIES that we link against for the tests.
SET(LIBTCOD_LIBRARY libtcod.so)
if (WIN32)
    SET(LIBTCOD_LIBRARY ${PROJECT_SOURCE_DIR}/external/libtcod.a)
endif (WIN32)

target_link_libraries(unit_tests
        PRIVATE
        prizrak_lib
        gtest_main
        ${LIBTCOD_LIBRARY}
        )
set(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# Finds the unit tests or some shit.
include(GoogleTest)
gtest_discover_tests(unit_tests
        PROPERTIES
        LABELS "unit"
        DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
        240
        )
