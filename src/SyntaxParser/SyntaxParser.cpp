


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
    if (_lookahead == "PROGRAM") {
        form_derivation_string("<progBody>", "program <funcBody> ; <funcDefLst>");
        if (match("PROGRAM") && funcBody() && match("DELI") && funcDefLst())
            return true;
    }
    return false;
}

bool SyntaxParser::funcHead() {
    if (is_lookahead_a_type()) {
        form_derivation_string("<funcHead>", "<type> id ( <fParams> )");
        if (type() && match("ID") && match("OPENPARA") && fParams() && match("CLOSEPARA"))
            return true;
    }
    return false;
}

bool SyntaxParser::funcDefLst() {
    if (is_lookahead_a_type()) {
        form_derivation_string("<funcDefLst>", "<funcDef> <funcDefLst>");
        if (funcDef() && funcDefLst())
            return true;
    } else if (_lookahead == "END") {
        form_derivation_string("<funcDefLst>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::funcDef() {
    if (is_lookahead_a_type()) {
        form_derivation_string("<funcDef>", "<funcHead> <funcBody> ;");
        if (funcHead() && funcBody() && match("DELI"))
            return true;
    }
    return false;
}

bool SyntaxParser::funcBody() {
    if (_lookahead == "OPENCURL") {
        form_derivation_string("<funcBody>", "{ <funcInBodyLst> }");
        if (match("OPENCURL") && funcInBodyLst() && match("CLOSECURL")) {
            return true;
        }
    }
    return false;
}


bool SyntaxParser::funcInBodyLst() {
    if (is_lookahead_a_type() || is_lookahead_a_statement()) {
        form_derivation_string("<funcInBodyLst>", "<funcInBody> <funcInBodyLst>");
        if (funcInBody() && funcInBodyLst()) {
            return true;
        }
    } else if (_lookahead == "CLOSECURL") { // Follow set
        form_derivation_string("<funcInBodyLst>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::funcInBody() {
    if (_lookahead == "ID") {
        form_derivation_string("<funcInBody>", "ID <varOrStat>");
        if (match("ID") && varOrStat()) {
            return true;
        }
    } else if (is_lookahead_a_statement()) {
        form_derivation_string("<funcInBody>", "<statementRes>");
        if (statementRes()) {
            return true;
        }
    } else if (_lookahead == "INT" || _lookahead == "FLOAT") {
        form_derivation_string("<funcInBody>", "<numType> id <arraySize> ;");
        if (numType() && match("ID") && arraySize() && match("DELI")) {
            return true;
        }
    }
    return false;
}

bool SyntaxParser::varOrStat() {
    if (_lookahead == "ID") {
        form_derivation_string("<varOrStat>", "id <arraySize>");
        if (match("ID") && arraySize()) {
            return true;
        }
    } else if (_lookahead == "OPENBRA" || _lookahead == "EQUAL") {
        form_derivation_string("<varOrStat>", "<indiceLst> <idnest> <assignOp>");
        if (indiceLst() && idnest() && assignOp()) {
            return true;
        }
    }
    return false;
}

bool SyntaxParser::statementLst() {
    if (_lookahead == "ID" || is_lookahead_a_statement()) {
        form_derivation_string("<statementLst>", "<statement> <statementLst>");
        if (statement() && statementLst())
            return true;
    } else if (_lookahead == "CLOSECURL") {
        form_derivation_string("<statementLst>", "");
        return true;
    }
}

bool SyntaxParser::statement() {
    if (_lookahead == "ID") {
        form_derivation_string("<statement>", "<assignStat> ;");
        if (assignStat() && match("DELI"))
            return true;
    } else if (is_lookahead_a_statement()) {
            form_derivation_string("<statement>", "<statementRes>");
            if (statementRes())
                return true;
    }
    return false;
}

bool SyntaxParser::statementRes() {
    if (_lookahead == "IF") {
        form_derivation_string("<statementRes>", "if ( <expr> ) then <statBlock> else <statBlock> ;");
        if (match("IF") && match("OPENPARA") && expr() && match("CLOSEPARA") && match("THEN") && statBlock() && match("ELSE") && statBlock() && match("DELI")) {
            return true;
        }
    } else if (_lookahead == "FOR") {
        form_derivation_string("<statementRes>", "for ( <type> id <assignOp> <expr> ; <relExpr> ; <assignStat> ) <statBlock> ;");
        if (match("FOR") && match("OPENPARA") && type() && match("ID") && assignOp() && expr() && match("DELI") && relExpr() && match("DELI") && assignStat() && match("CLOSEPARA") && statBlock() && match("DELI")) {
            return true;
        }
    } else if (_lookahead == "GET") {
        form_derivation_string("<statementRes>", "get ( <variable> ) ;");
        if (match("GET") && match("OPENPARA") && variable() && match("CLOSEPARA") && match("DELI")) {
            return true;
        }
    } else if (_lookahead == "PUT") {
        form_derivation_string("<statementRes>", "put ( <expr> ) ;");
        if (match("PUT") && match("OPENPARA") && expr() && match("CLOSEPARA") && match("DELI")) {
            return true;
        }
    } else if (_lookahead == "RETURN") {
        form_derivation_string("<statementRes>", "return ( <expr> ) ;");
        if (match("RETURN") && match("OPENPARA") && expr() && match("CLOSEPARA") && match("DELI")) {
            return true;
        }
    }
    return false;
}

bool SyntaxParser::assignStat() {
    if (_lookahead == "ID") {
        form_derivation_string("<assignStat>", "<variable> <assignOp> <expr>");
        if (variable() && assignOp() && expr())
            return true;
    }
    return false;
}

bool SyntaxParser::statBlock() {
    if (_lookahead == "OPENCURL") {
        form_derivation_string("<statBlock>", "{ <statementLst> }");
        if (match("OPENCURL") && statementLst() && match("CLOSECURL"))
            return true;
    } else if (_lookahead == "ID" || is_lookahead_a_statement()) {
        form_derivation_string("<statBlock>", "<statement>");
        if (statement())
            return true;
    } else if (_lookahead == "ELSE" || _lookahead == "DELI") { // Follow set
        form_derivation_string("<statBlock>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::expr() {
    if (is_lookahead_a_value() || _lookahead == "OPENPARA" || _lookahead == "NOT" || _lookahead == "PLUS" || _lookahead == "SUB") {
        form_derivation_string("<expr>", "<term> <relOrAri>");
        if (term() && relOrAri()) {
            return true;
        }
    }
    return false;
}

bool SyntaxParser::relOrAri() {
    if (_lookahead == "EQUIV" || _lookahead == "NOTEQ" || _lookahead == "LT" || _lookahead == "GT" || _lookahead == "LTEQ" || _lookahead == "GTEQ") {
        form_derivation_string("<relOrAri>", "<relOp> <arithExpr>");
        if (relOp() && arithExpr())
            return true;
    } else if (_lookahead == "PLUS" || _lookahead == "SUB" || _lookahead == "OR") {
        form_derivation_string("<relOrAri>", "<arithExprD");
        if (arithExprD())
            return true;
    }
}


bool SyntaxParser::relExpr() {
    if (is_lookahead_a_value() || _lookahead == "OPENPARA" || _lookahead == "NOT" || _lookahead == "PLUS" || _lookahead == "SUB") {
        form_derivation_string("<arithExpr>", "<arithExpr> <relOp> <arithExpr>");
        if (arithExpr() && relOp() && arithExpr())
            return true;
    }
    return false;

}

bool SyntaxParser::arithExpr() {
    if (is_lookahead_a_value() || _lookahead == "OPENPARA" || _lookahead == "NOT" || _lookahead == "PLUS" || _lookahead == "SUB") {
        form_derivation_string("<arithExpr>", "<term> <arithExprD>");
        if (term() && arithExprD())
            return true;
    }
    return false;
}

bool SyntaxParser::arithExprD() {
    if (_lookahead == "PLUS" || _lookahead == "SUB" || _lookahead == "OR") {
        form_derivation_string("<arithExprD>", "<addOp> <term> <arithExprD>");
        if (addOp() && term() && arithExprD())
            return true;
    } else if (check_if_lookahead_is_in_set( {"EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "CLOSEBRA", "CLOSEPARA", "DELI", "C OM"})) { // follow set
        form_derivation_string("<arithExprD>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::sign() {
    if (_lookahead == "PLUS") {
        form_derivation_string("<sign>", "+");
        if (match("PLUS"))
            return true;
    } else if (_lookahead == "SUB") {
        form_derivation_string("<sign>", "-");
        if (match("SUB"))
            return true;
    }
    return false;
}

bool SyntaxParser::term() {
    if (is_lookahead_a_value() || _lookahead == "OPENPARA" || _lookahead == "NOT" || _lookahead == "PLUS" || _lookahead == "SUB") {
        form_derivation_string("<term>", "<factor> <termD>");
        if (factor() && termD()) {
            return true;
        }
    }
    return false;
}

bool SyntaxParser::termD() {
    if (_lookahead == "MULTI" || _lookahead == "DASH" || _lookahead == "AND") {
        form_derivation_string("<termD>", "<multOp> <factor> <termD>");
        if (multOp() && factor() && termD())
            return true;
    } else if (check_if_lookahead_is_in_set({"PLUS", "SUB", "OR", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "CLOSEBRA", "CLOSEPARA", "DELI", "COM" ,"DOT"})) { // follow set
        return true;
    }
    return false;
}

bool SyntaxParser::factor() {
    if (_lookahead == "ID") {
        form_derivation_string("<factor>", "ID <factorVarOrFunc>");
        if (match("ID") && factorVarOrFunc())
            return true;
    } else if (_lookahead == "INUM" || _lookahead == "FNUM") {
        form_derivation_string("<factor>", "<num>");
        if (num())
            return true;
    } else if (_lookahead == "OPENPARA") {
        form_derivation_string("<factor>", "( <arithExpr> )");
        if (match("OPENPARA") && arithExpr() && match("CLOSEPARA"))
            return true;
    } else if (_lookahead == "NOT") {
        form_derivation_string("<factor>", "not <factor>");
        if (match("NOT") && factor())
            return true;
    } else if (_lookahead == "PLUS" || _lookahead == "SUB") {
        form_derivation_string("<factor>", "<sign> <factor>");
        if (sign() && factor())
            return true;
    }
    return false;
}

bool SyntaxParser::factorVarOrFunc() {
    if (_lookahead == "OPENBRA") {
        form_derivation_string("<factorVarOrFunc>", "<indice> <factorVarOrFunc>");
        if (indice() && factorVarOrFunc())
            return true;
    } else if (_lookahead == "DOT") {
        form_derivation_string("<factorVarOrFunc>", ". id <varOrFuncIdNest>");
        if (match("DOT") && match("ID") && varOrFuncIdNest())
            return true;
    } else if (check_if_lookahead_is_in_set({"MULTI", "DASH", "AND", "PLUS", "SUB", "OR", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "CLOSEBRA", "CLOSEPARA", "DELI", "COM" ,"DOT"})) { // follow set
        form_derivation_string("<factorVarOrFunc>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::varOrFuncIdNest() {
    if (_lookahead == "OPENPARA") {
        form_derivation_string("<varOrFuncIdNest>", "( <aParams> )");
        if (match("OPENPARA") && aParams() && match("CLOSEPARA"))
            return true;
    } else if (_lookahead == "DOT" || _lookahead == "OPENBRA") {
        form_derivation_string("<varOrFuncIdNest>", "<factorVarOrFunc>");
        if (factorVarOrFunc())
            return true;
    } else if (check_if_lookahead_is_in_set({"MULTI", "DASH", "AND", "PLUS", "SUB", "OR", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "CLOSEBRA", "CLOSEPARA", "DELI", "COM" ,"DOT"})) { // follow set
        form_derivation_string("<varOrFuncIdNest>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::variable() {
    if (_lookahead == "ID") {
        form_derivation_string("<variable>", "id <variableIndice>");
        if (match("ID") && variableIndice())
            return true;
    }
    return false;
}

bool SyntaxParser::variableIndice() {
    if (_lookahead == "OPENBRA" || _lookahead == "DOT") {
        form_derivation_string("<variableIndice>", "<indiceLst> <idnest>");
        if (indiceLst() && idnest())
            return true;
    } else if (_lookahead == "EQUAL" || _lookahead == "CLOSEPARA") {
        form_derivation_string("<variableIndice>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::check_if_lookahead_is_in_set(set<string> values) {
    return values.find(_lookahead) == values.end();
}

bool SyntaxParser::idnest() {
    if (_lookahead == "DOT") {
        form_derivation_string("<idnest>", ". id <indiceLst> <idnest>");
        if (match("DOT") && match("ID") && indiceLst() && idnest()) {
            return true;
        }
    } else if (_lookahead == "EQUAL") {
        form_derivation_string("<idnest>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::indice() {
    if (_lookahead == "OPENBRA") {
        form_derivation_string("<indice>", "[ <arithExpr> ]");
        if (match("OPENBRA") && arithExpr() && match("CLOSEBRA")) {
            return true;
        }
    }
    return false;
}

bool SyntaxParser::indiceLst() {
    if (_lookahead == "OPENBRA") {
        form_derivation_string("<indiceLst>", "<indice> <indiceLst>");
        if (indice() && indiceLst()) {
            return true;
        }
    } else if (_lookahead == "DOT" || _lookahead == "EQUAL" || _lookahead == "CLOSEPAR" ) { // Follow set
        form_derivation_string("<indiceLst>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::is_lookahead_a_type() {
    return _lookahead == "ID" || _lookahead == "INT" || _lookahead == "FLOAT";
}

bool SyntaxParser::is_lookahead_a_value() {
    return _lookahead == "INUM" || _lookahead == "FNUM" || _lookahead == "ID";
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
    if (_lookahead == "INT") {
        form_derivation_string("<numType>", "int");
        if (match("INT"))
            return true;
    } else if (_lookahead == "FLOAT") {
        form_derivation_string("<numType>", "float");
        if (match("FLOAT"))
            return true;
    }
}

bool SyntaxParser::fParams() {
    if (is_lookahead_a_type()) {
        form_derivation_string("<fParams>", "<type> id <arraySize> <fParamsTail>");
        if (type() && match("ID") && arraySize() && fParamsTail())
            return true;
    } else if (_lookahead == "CLOSEPARA") {
        form_derivation_string("<fParams>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::aParams() {
    if (is_lookahead_a_value() || _lookahead == "OPENPARA" || _lookahead == "NOT" || _lookahead == "PLUS" || _lookahead == "SUB") {
        form_derivation_string("<aParams>", "<expr> <aParamsTail>");
        if (expr() && aParamsTail())
            return true;
    } else if (_lookahead == "CLOSEPARA") {
        form_derivation_string("<aParams>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::fParamsTail() {
    if (_lookahead == "COM") {
        form_derivation_string("<fParamsTail>", ", <type> id <arraySize> <fParamsTail>");
        if (match("COM") && type() && match("ID") && arraySize() && fParamsTail() )
            return true;
    } else if (_lookahead == "CLOSEPARA") { // Follow set
        form_derivation_string("<fParamsTail>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::aParamsTail() {
    if (_lookahead == "COM") {
        form_derivation_string("<aParamsTail>", ", <expr> <aParamsTail>");
        if (match("COM") && expr() && aParamsTail() )
            return true;
    } else if (_lookahead == "CLOSEPARA") { // Follow set
        form_derivation_string("<aParamsTail>", "");
        return true;
    }
    return false;
}

bool SyntaxParser::assignOp() {
    if (_lookahead == "EQUAL" && match("EQUAL")) {
        form_derivation_string("<assignOP>", "=");
        return true;
    }
    return false;
}

bool SyntaxParser::relOp() {
    if (_lookahead == "EQUIV") {
        form_derivation_string("<relOP>", "==");
        if (match("EQUIV"))
            return true;
    } else if (_lookahead == "NOTEQ") {
        form_derivation_string("<relOP>", "<>");
        if (match("NOTEQ"))
            return true;
    } else if (_lookahead == "LT") {
        form_derivation_string("<relOP>", "<");
        if (match("LT"))
            return true;
    } else if (_lookahead == "GT") {
        form_derivation_string("<relOP>", ">");
        if (match("GT"))
            return true;
    } else if (_lookahead == "LTEQ") {
        form_derivation_string("<relOP>", "<=");
        if (match("LTEQ"))
            return true;
    } else if (_lookahead == "GTEQ") {
        form_derivation_string("<relOP>", ">=");
        if (match("GTEQ"))
            return true;
    }
    return false;
}

bool SyntaxParser::addOp() {
    if (_lookahead == "PLUS") {
        form_derivation_string("<addOp>", "+");
        if (match("PLUS"))
            return true;
    } else if (_lookahead == "SUB") {
        form_derivation_string("<addOp>", "-");
        if (match("SUB"))
            return true;
    } else if (_lookahead == "OR") {
        form_derivation_string("<addOp>", "or");
        if (match("OR"))
            return true;
    }
    return false;
}

bool SyntaxParser::multOp() {
    if (_lookahead == "MULTI") {
        form_derivation_string("<multOp>", "*");
        if (match("MULTI"))
            return true;
    } else if (_lookahead == "DASH") {
        form_derivation_string("<multOp>", "/");
        if (match("DASH"))
            return true;
    } else if (_lookahead == "AND") {
        form_derivation_string("<multOp>", "and");
        if (match("AND"))
            return true;
    }
    return false;
}


bool SyntaxParser::num() {
    if (_lookahead == "INUM") {
        form_derivation_string("<num>", "integer");
        if (match("INUM"))
            return true;
    } else if (_lookahead == "FNUM") {
        form_derivation_string("<num>", "float");
        if (match("FNUM"))
            return true;
    }
    return false;
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
    if (rhs.size() == 0)
        _current_rhs_derivation.erase(begin_index - 1, non_terminal.size() + 1);
    else
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

