


#include "SyntaxParser.h"


SyntaxParser::SyntaxParser() {

    _first_sets["prog"] = {"class", "program"};
}

bool SyntaxParser::parse(vector<Token *> tokens) {

    _current_token_position = 0;
    _tokens = tokens;
    next_token();

    for (int i = 0; i < _errors.size(); i++) {
        cout << _errors[i] << endl;
    }
    return prog();
}


bool SyntaxParser::prog() {
    _current_rhs_derivation = "<classDeclLst> <progBody>";
    _derivations.push_back(_current_rhs_derivation);
    cout << _current_rhs_derivation << endl;
    if (!skip_errors({"CLASS"}, {}, false))
        return false;
    if (_lookahead == "CLASS") {
        if (classDeclLst() && progBody()) {
            return true;
        }

    } else if (_lookahead == "PROGRAM") {

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
        form_derivation_string("<classDeclLst>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::classDecl() {
    if (_lookahead == "CLASS") {
        form_derivation_string("<classDecl>", "class id { <classBody> } ;");
        if (match("CLASS") && match("ID") && match("OPENCURL") && classBody() && match("CLOSECURL") && match("DELI")) {
            return true;
        }
    }
    return false;
}

bool SyntaxParser::classBody() {
    if (is_lookahead_a_type()) {
        form_derivation_string("<classBody>", "<classInDecl> <classBody>");
        if (classInDecl() && classBody()) {
            return true;
        }
    } else if (_lookahead == "CLOSECURL") { // FOLLOW SET
        form_derivation_string("<classBody>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::classInDecl() {
    if (is_lookahead_a_type()) {
        form_derivation_string("<classInDecl>", "<type> id <postTypeId>");
        if (type() && match("ID") && postTypeId()) {
            return true;
        }
    }
    return false;
}

bool SyntaxParser::postTypeId() {
    if (_lookahead == "OPENBRA" || _lookahead == "DELI") {
        form_derivation_string("<postTypeId>", "<arraySize> ;");
        if (arraySize() && match("DELI")){
            return true;
        }
    } else if (_lookahead == "OPENPARA") {
        form_derivation_string("<postTypeId>", "( <fParams> ) <funcBody>");
        if (match("OPENPARA") && fParams() && match("CLOSEPARA") && funcBody()) {
            return true;
        }
    }
    return false;
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
    if (_lookahead == "OPENCURL") {
        form_derivation_string("<funcBody>", "( <funcInBodyLst> )");
        if (match("OPENCURL") && funcInBodyLst() && match("CLOSECURL")) {
            return true;
        }
    }

}


bool SyntaxParser::funcInBodyLst() {
    if (is_lookahead_a_type || is_lookahead_a_statement()) {
        form_derivation_string("<funcInBodyLst>", "<funcInBody> <funcInBodyLst>");
        if (funcInBody() && funcInBodyLst()) {
            return true;
        }
    } else if (_lookahead == "CLOSECURL")
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

bool SyntaxParser::is_lookahead_a_type() {
    return _lookahead == "ID" || _lookahead == "INT" || _lookahead == "FLOAT";
}

bool SyntaxParser::is_lookahead_a_statement() {
    return _lookahead == "FOR" || _lookahead == "GET" || _lookahead == "PUT" || _lookahead == "RETURN" || _lookahead == "IF";
}

bool SyntaxParser::skip_errors(set<string> first_set, set<string> follow_set, bool epsilon) {
    if (first_set.find(_lookahead) == first_set.end() || (epsilon && follow_set.find(_lookahead) == follow_set.end())) {

        _errors.push_back("Syntax Error at " + to_string(_current_token->row_location_) + " : " + to_string(_current_token->column_location_));
        do {
            next_token();
//            if (!epsilon && follow_set.find(_lookahead) == follow_set.end())
//                return false;
        } while (first_set.find(_lookahead) == first_set.end() || (epsilon && follow_set.find(_lookahead) == follow_set.end()));
        return true;
    }
    return true;
}

bool SyntaxParser::arraySize() {
    if ( _lookahead == "OPENBRA") {
        if (match("OPENBRA") && match("INUM") && match("CLOSEBRA")) {
            form_derivation_string("<arraySize>", "[ integer ]");
            return true;
        }
    } else if (_lookahead == "COM" || _lookahead == "DELI" || _lookahead == "CLOSEPARA") {
        form_derivation_string("<arraySize>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::type() {

    if (_lookahead == "INT" && match("INT")) {
        form_derivation_string("<type>", "int");
        return true;
    } else if (_lookahead == "FLOAT" && match("FLOAT")) {
        form_derivation_string("<type>", "float");
        return true;
    } else if (_lookahead == "ID" && match("ID")) {
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
    } else {
        _errors.push_back("Syntax Error at " + to_string(_current_token->row_location_) + " : " + to_string(_current_token->column_location_));
        _lookahead = next_token();
    }
    return false;
}

bool check_if_list_has_string(vector<string> tokens, string token) {

}

bool SyntaxParser::form_derivation_string(string non_terminal, string rhs) {
    int begin_index = _current_rhs_derivation.find(non_terminal);
    if (begin_index == string::npos)
        return false;
    _current_rhs_derivation.replace(begin_index, non_terminal.size(), rhs);
    cout << _current_rhs_derivation << endl;
    _derivations.push_back(_current_rhs_derivation);
    return true;
}

string SyntaxParser::next_token() {
    if (_current_token_position < _tokens.size()) {
        _current_token = _tokens[_current_token_position++];
        _lookahead = _current_token->token_identifier_;
    } else {
        _lookahead = "END";
    }
    return _lookahead;
}

