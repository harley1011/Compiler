


#include "SyntaxParser.h"


SyntaxParser::SyntaxParser() {

    _first_sets["prog"] = {"class", "program"};
}

bool SyntaxParser::parse(vector<Token*> tokens) {
    _current_token_position = 0;
    _tokens = tokens;
    next_token();

    return prog();
}


bool SyntaxParser::prog() {
    _current_rhs_derivation = "<classDeclLst> <progBody>";
    if ( _lookahead == "CLASS" ) {
        if (classDeclLst() && progBody()) {
            return true;
        }

    } else if ( _lookahead == "PROGRAM" ) {

    }
    return false;
}

bool SyntaxParser::classDeclLst() {
    if (_lookahead == "CLASS") {
        form_derivation_string("<classDeclLst>", "<classDecl> <classDeclLst>");
        if (classDecl() && classDeclLst()) {
            return true;
        }
    } else if (_lookahead == "PROGRAM") { // FOLLOW SET
        return true;
    }
    return false;
}

bool SyntaxParser::classDecl() {
    if(_lookahead == "CLASS") {
        form_derivation_string("<classDecl>", "class id { <classBody> } ;");
        if (match("CLASS") && match("ID") && match("OPENCURL") && classBody() && match("CLOSECURL") && match("DELI") ) {
            return true;
        }
    }
    return false;
}

bool SyntaxParser::classBody() {
    if (_lookahead == "INT" || _lookahead == "FLOAT" || _lookahead == "ID" ) {
        form_derivation_string("<classBody>", "<classInDecl> <classBody>");
        if (classInDecl() && classBody()) {
            return true;
        }
    }
    return true;
}

bool SyntaxParser::classInDecl() {
    if (_lookahead == "INT" || _lookahead == "FLOAT" || _lookahead == "ID" ) {
        if ( type() && match("ID") && postTypeId()) {
            return true;
        }

    }
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

    if ( match("INT")) {
        form_derivation_string("<type>", "int");
        return true;
    } else if ( match("FLOAT")) {
        form_derivation_string("<type>", "float");
        return true;
    }
    else if ( match("ID")) {
        form_derivation_string("<type>", "id");
        return true;
    }
    return false;
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

bool SyntaxParser::form_derivation_string(string non_terminal, string rhs) {
    int begin_index = _current_rhs_derivation.find(non_terminal);
    if (begin_index == string::npos)
        return false;
    _current_rhs_derivation.replace(begin_index, non_terminal.size(), rhs);
    return true;
}

string SyntaxParser::next_token() {
    if (_current_token_position < _tokens.size()) {
        _lookahead = _tokens[_current_token_position++]->token_identifier_;
    } else {
        _lookahead = "END";
    }
    return _lookahead;
}

