set(core_SRCS Scanner.cpp)

add_library(scanner ${core_SRCS} Scanner.h)
add_executable(exe main.cpp)
target_link_libraries(exe scanner)