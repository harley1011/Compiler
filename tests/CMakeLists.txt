
set(gtest_disable_pthreads on) #needed in MinGW
set(test_SRCS gtest.cpp)
add_executable(runUnitTests gtest.cpp)
target_link_libraries(runUnitTests gtest scanner)

