set(core_SRCS Scanner.cpp State.cpp State.h Token.cpp Token.h)

add_library(scanner ${core_SRCS} Scanner.h)
add_executable(exe main.cpp)
target_link_libraries(exe scanner)