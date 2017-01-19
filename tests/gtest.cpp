#include "gtest/gtest.h"
#include "../src/Scanner.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


TEST(test_case_name, test_name) {
    EXPECT_EQ(scan(), 'c');
}