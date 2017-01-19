cmake_minimum_required(VERSION 3.3)

project(Compiler)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)