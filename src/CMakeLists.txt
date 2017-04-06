set(core_SRCS Scanner/Scanner.cpp Scanner/State.cpp State.h Token.cpp Token.h Scanner/IntegerToken.cpp Scanner/IntegerToken.h Scanner/FloatToken.cpp Scanner/FloatToken.h Scanner/ErrorToken.cpp Scanner/ErrorToken.h Parser/SymbolTable.cpp Parser/SymbolTable.h Parser/Parser.cpp Parser/Parser.h  Parser/SymbolRecord.cpp Parser/SymbolRecord.h Parser/ExpressionTree.cpp Parser/ExpressionTree.h Parser/ExpressionNode.cpp Parser/ExpressionNode.h Parser/CodeGenerator.cpp Parser/CodeGenerator.h Parser/IntAndStringConversion.cpp Parser/IntAndStringConversion.h)

add_library(scanner ${core_SRCS} Scanner/Scanner.h)
add_executable(exe main.cpp)
target_link_libraries(exe scanner)
target_compile_features(exe PRIVATE cxx_range_for)