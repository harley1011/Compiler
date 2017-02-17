


#include "SyntaxParser.h"



bool SyntaxParser::parse(vector<Token*> tokens) {
    _current_token_position = 0;
    _tokens = tokens;

    return prog();
}

string SyntaxParser::next_token() {
    _lookahead = _tokens[_current_token_position]->token_identifier_;
}

