#include <iostream>
#include <string>
#include <map>

using namespace std;

#ifndef UNTITLED_STATE_H
#define UNTITLED_STATE_H


class State {
public:
    string token;
    bool backup;
    bool isFinalState;
    map<char, int> nextStates;

};


#endif //UNTITLED_STATE_H
