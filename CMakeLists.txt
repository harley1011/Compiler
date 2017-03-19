cmake_minimum_required(VERSION 3.3)

project(Compiler)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)
