#include "gtest/gtest.h"
#include "../src/Scanner/Scanner.h"
#include "../src/Scanner/IntegerToken.h"
#include "../src/Scanner/FloatToken.h"
#include <vector>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    cout << RUN_ALL_TESTS();
    string c;
    //cin >> c;
    return 0 ;
}
bool check_location(string program, string search_string, int row, int column);
bool compare_files(string first_path, string second_path);
// TEST KEYWORDS






