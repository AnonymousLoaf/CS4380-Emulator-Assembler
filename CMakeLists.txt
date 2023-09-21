cmake_minimum_required(VERSION 3.14)
project(Project-0)
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(
emu4380
main.cpp emu4380.cpp emu4380.h
)
# Setup/install GoogleTest and build test executable
include(FetchContent)
FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_executable(
runTests
myTests.cpp emu4380.cpp emu4380.h
)
target_link_libraries(
runTests
GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(runTests)