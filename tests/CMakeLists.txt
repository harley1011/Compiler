
set(gtest_disable_pthreads on) #needed in MinGW
set(test_SRCS gtest.cpp)
add_executable(runUnitTests gtest.cpp ScannerTests.cpp ScannerTests.h SyntaxTests.cpp SyntaxTests.h ../src/Parser/Parser.cpp SemanticTests.cpp SemanticTests.h)


target_link_libraries(runUnitTests gtest scanner)

target_compile_features(runUnitTests PRIVATE cxx_range_for)

add_custom_command(TARGET runUnitTests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/tests/testPrograms $<TARGET_FILE_DIR:runUnitTests>)

add_custom_command(TARGET runUnitTests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/tests/syntaxTestPrograms $<TARGET_FILE_DIR:runUnitTests>)