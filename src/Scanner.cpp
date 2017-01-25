#include <iostream>
#include <string>
#include <list>
#include "State.h"
#include "Token.h"
#include <fstream>
#include "Scanner.h"

using namespace std;

State initialState;
ifstream fs;



list<token> generateTokens(char* path) {
    fs.open(path);

    while(true) {
        nextToken();
    }
}

string nextToken() {
    State currentState;

    while(currentState.token.length() == 0) {

    }
    return currentState.token;


}

char nextChar() {

}