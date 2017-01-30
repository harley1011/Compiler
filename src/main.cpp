#include <iostream>
#include "Scanner.h"
#include <sys/stat.h>

using namespace std;


int main() {
    string input;
    string output;
    string error_output;
    struct stat info;

    while (true) {
        std::cout << "Enter path location:";
        getline(cin, input);

        if (stat(input.c_str(), &info) != 0)
            printf("Cannot access %s\n", input.c_str());
        else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows
            printf("%s is a directory\n", input.c_str());
        else
            break;

    }

    cout << "Enter output location: ";
    getline(cin, output);

    cout << "Enter error output location: ";
    getline(cin, error_output);

    Scanner scanner(output, error_output);
    scanner.generate_tokens(input, true);

    cout << "Lexical analyzer has converted the program to tokens, see output file";
    return 0;
}

