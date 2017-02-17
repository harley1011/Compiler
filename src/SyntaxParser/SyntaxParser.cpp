


#include "SyntaxParser.h"


SyntaxParser::SyntaxParser() {

    _first_sets["prog"] = {"class", "program"};
}

bool SyntaxParser::parse(vector<Token*> tokens) {
    _current_token_position = 0;
    _tokens = tokens;

    return prog();
}


bool SyntaxParser::prog() {
    if ( _lookahead == "class" ) {


    } else if ( _lookahead == "program" ) {

    }
}

bool SyntaxParser::classDeclLst() {

}

bool SyntaxParser::classDecl() {

}
bool SyntaxParser::classBody() {

}

bool SyntaxParser::classInDecl() {

}
bool SyntaxParser::postTypeId() {

}

bool SyntaxParser::progBody() {

}
bool SyntaxParser::funcHead() {

}

bool SyntaxParser::funcDefLst() {

}
bool SyntaxParser::funcDef() {

}

bool SyntaxParser::funcBody() {

}


bool SyntaxParser::funcInBodyLst() {

}

bool SyntaxParser::funcInBody() {

}

bool SyntaxParser::varOrStat() {

}

bool SyntaxParser::statementLst() {

}

bool SyntaxParser::statement() {

}

bool SyntaxParser::statementRes() {

}
bool SyntaxParser::assignStat() {

}
bool SyntaxParser::statBlock() {

}
bool SyntaxParser::expr() {

}
bool SyntaxParser::relOrAri() {

}
bool SyntaxParser::arithExpr() {

}
bool SyntaxParser::arithExprD() {

}
bool SyntaxParser::sign() {

}
bool SyntaxParser::term() {

}
bool SyntaxParser::termD() {

}
bool SyntaxParser::factor() {

}
bool SyntaxParser::factorVarOrFunc() {

}
bool SyntaxParser::varOrFuncIdNest() {

}
bool SyntaxParser::variable() {

}
bool SyntaxParser::variableIndice() {

}

bool SyntaxParser::idnest() {

}

bool SyntaxParser::indice() {

}

bool SyntaxParser::indiceLst() {

}


bool SyntaxParser::arraySize() {

}

bool SyntaxParser::type() {

}

bool SyntaxParser::numType() {

}
bool SyntaxParser::fParams() {

}

bool SyntaxParser::aParams() {

}
bool SyntaxParser::fParamsTail() {

}

bool SyntaxParser::aParamsTail() {

}
bool SyntaxParser::assignOp() {

}

bool SyntaxParser::relOp() {

}
bool SyntaxParser::addOp() {

}

bool SyntaxParser::multOp() {

}


bool SyntaxParser::num() {

}
bool SyntaxParser::match(string token) {
    if (_lookahead == token) {
        _lookahead = next_token();
        return (true);
    }
    else
        _lookahead = next_token();
    return false;
}

bool check_if_list_has_string(vector<string> tokens, string token) {

}

string SyntaxParser::next_token() {
    _lookahead = _tokens[_current_token_position]->token_identifier_;
}

