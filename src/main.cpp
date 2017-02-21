#include <iostream>
#include "./SyntaxParser/SyntaxParser.h"
#include <sys/stat.h>

using namespace std;

int main(int argc, char *argv[] ) {
    string input;
    string output;
    string error_output;

    if (argc < 3) {
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

        cout << "Enter token output location: ";
        getline(cin, output);

        cout << "Enter error output location: ";
        getline(cin, error_output);
    }
    else {
        input = argv[1];
        output = argv[2];
        error_output = argv[3];
    }

    Scanner scanner(output, error_output);
    vector<Token*> tokens = scanner.generate_tokens(input, true);

    cout << "Lexical analyzer has converted the program to tokens, see output file" << endl;

    string answer;
    cout << "Do you want to run the syntax parser? y/n:";
    getline(cin, answer);

    if (answer == "Y" || answer == "y") {

        std::cout << "Enter derivation output location:";
        getline(cin, output);

        cout << "Enter error output location: ";
        getline(cin, error_output);

        SyntaxParser syntaxParser(output, error_output);


        bool parse_result = syntaxParser.parse(tokens);

        if (!parse_result) {
            cout << "Program is syntactically invalid, errors are below:" << endl;
            for (int i = 0; i < syntaxParser.errors_.size(); i++) {
                cout << syntaxParser.errors_[i] << endl;
            }
        } else {
            cout << "Program is syntactically valid" << endl;
        }
    }


    return 0;
}
