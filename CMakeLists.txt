cmake_minimum_required(VERSION 3.14)
project(Project-3)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a library for your emulator code
add_library(emu4380_lib emu4380.cpp emu4380.h cache.cpp cache.h)

# Create the main executable
add_executable(emu4380 main.cpp)
target_link_libraries(emu4380 emu4380_lib)

# Setup/install GoogleTest and build test executable
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Create the test executable
add_executable(runTests myTests.cpp)
target_link_libraries(runTests emu4380_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(runTests)

# For the main executable
target_sources(emu4380 PRIVATE main.cpp)

# For the test executable
target_sources(runTests PRIVATE myTests.cpp)