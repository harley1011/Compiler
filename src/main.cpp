#include <iostream>
#include "./Parser/Parser.h"
#include <sys/stat.h>

using namespace std;

int main(int argc, char *argv[] ) {
    string input;
    string token_output;
    string token_error;
    string syntax_output;
    string semantic_output;
    string syntax_error_output;
    string semantic_error_output;

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

        cout << "Enter token syntax_output location: ";
        getline(cin, token_output);

        cout << "Enter error syntax_output location: ";
        getline(cin, token_error);
    }
    else {
        input = argv[1];
        token_output = argv[2];
        token_error = argv[3];
    }

    Scanner scanner(token_output, token_error);
    vector<Token*> tokens = scanner.generate_tokens(input, true);

    cout << "Lexical analyzer has converted the program to tokens, see syntax_output file" << endl;

    string answer;
    cout << "Do you want to run the parser? y/n:";
    getline(cin, answer);

    if (answer == "Y" || answer == "y") {

        std::cout << "Enter derivation syntax output location:";
        getline(cin, syntax_output);

        cout << "Enter syntax error syntax_output location: ";
        getline(cin, syntax_error_output);

        std::cout << "Enter symbol table syntax_output location:";
        getline(cin, semantic_output);

        cout << "Enter semantic error syntax_output location: ";
        getline(cin, semantic_error_output);

        Parser syntaxParser(syntax_output, semantic_output, syntax_error_output, semantic_error_output);


        bool parse_result = syntaxParser.parse(tokens);

        if (!parse_result) {
            cout << "Program is syntactically invalid, errors are below:" << endl;
            for (int i = 0; i < syntaxParser.syntax_errors.size(); i++) {
                cout << syntaxParser.syntax_errors[i] << endl;
            }
        } else {
            cout << "Program is syntactically valid" << endl;
        }

        if (syntaxParser.semantic_errors_.size() == 0) {
            cout << "Program is semantically valid" << endl;
        } else {
            cout << "Program is semantically invalid, errors are below:" << endl;
            for (int i = 0; i < syntaxParser.semantic_errors_.size(); i++) {
                cout << syntaxParser.semantic_errors_[i] << endl;
            }
        }
    }


    return 0;
}
