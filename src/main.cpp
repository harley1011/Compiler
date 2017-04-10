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
    string code_output;

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

        cout << "Enter token output path location: ";
        getline(cin, token_output);

        cout << "Enter error output path location: ";
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

        std::cout << "Enter derivation syntax output path location:";
        getline(cin, syntax_output);

        cout << "Enter syntax error output path location: ";
        getline(cin, syntax_error_output);

        std::cout << "Enter symbol table output path location:";
        getline(cin, semantic_output);

        cout << "Enter semantic error output path location: ";
        getline(cin, semantic_error_output);

        cout << "Enter program code output path location: ";
        getline(cin, code_output);
        string yes_or_no;
        cout << "Do you want to append comments to the moon code?(y/n): ";
        getline(cin, yes_or_no);

        Parser syntaxParser(syntax_output, semantic_output, syntax_error_output, semantic_error_output);

        if (yes_or_no == "y")
            syntaxParser.code_generator_->enable_comments_ = true;
        else
            syntaxParser.code_generator_->enable_comments_ = false;


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
