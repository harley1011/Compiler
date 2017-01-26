#include <iostream>
#include <string>
#include <list>
#include <map>
#include "State.h"
#include "Token.h"
#include <fstream>
#include "Scanner.h"

using namespace std;

State initial_state("", false, false);

ifstream fs;

map<int, State> table;

list<token> generateTokens(char* path) {
    fs.open(path);

    while(true) {
        nextToken();
    }
}

string nextToken() {
    State currentState = initial_state;

    while(currentState.token_.length() == 0) {

    }
    return currentState.token_;


}

char nextChar() {

}