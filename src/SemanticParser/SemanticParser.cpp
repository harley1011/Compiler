


#include "SemanticParser.h"



SemanticParser::SemanticParser() {
    current_rhs_derivation_ = "<classDeclLst> <progBody>";
    derivations_.push_back(current_rhs_derivation_);
    enable_derivation_output_ = false;

}

SemanticParser::SemanticParser(string derivation_output_path, string error_output_path) {
    current_rhs_derivation_ = "<classDeclLst> <progBody>";
    derivations_.push_back(current_rhs_derivation_);
    enable_derivation_output_ = false;
    derivation_output_path_ = derivation_output_path;
    error_output_path_ = error_output_path;
    output_to_file_ = true;
}

SemanticParser::SemanticParser(bool enable_derivation_output) {
    current_rhs_derivation_ = "<classDeclLst> <progBody>";
    derivations_.push_back(current_rhs_derivation_);
    enable_derivation_output_ = false;
    enable_derivation_output_ = enable_derivation_output;
}

SemanticParser::SemanticParser(vector<Token *> tokens) {
    SemanticParser();
    tokens_ = tokens;
    current_token_position_ = 0;
    next_token();
}


bool SemanticParser::parse(vector<Token *> tokens) {
    if (enable_derivation_output_) {
        cout << current_rhs_derivation_ << endl;
    }
    current_token_position_ = 0;
    tokens_ = tokens;
    next_token();

    if (output_to_file_) {
        error_output_file_.open(error_output_path_);
        derivation_output_file_.open(derivation_output_path_);
    }

    bool result = prog();

    if (output_to_file_) {
        error_output_file_.close();
        derivation_output_file_.close();
    }
    if (errors_.size() > 1) {
        return false;
    }
    return result;
}


bool SemanticParser::prog() {
    global_symbol_table_.second_pass_ = false;
    if (!skip_errors({"CLASS"}, {}, false))
        return false;
    if (lookahead_ == "CLASS") {
        if (classDeclLst() && progBody()) {
            return true;
        }

    }
    return false;
}

bool SemanticParser::classDeclLst() {
    if (!skip_errors({"CLASS"}, {"PROGRAM"}, true))
        return false;
    if (lookahead_ == "CLASS") {
        form_derivation_string("<classDeclLst>", "<classDecl> <classDeclLst>");
        if (classDecl() && classDeclLst()) {
            return true;
        }
    } else if (lookahead_ == "PROGRAM") { // FOLLOW SET
        form_derivation_string("<classDeclLst>", "");
        return true;
    }
    return false;
}

bool SemanticParser::classDecl() {
    if (!skip_errors({"CLASS"}, {"PROGRAM"}, false))
        return false;
    if (lookahead_ == "CLASS") {
        form_derivation_string("<classDecl>", "class id { <classBody> } ;");

        if (match("CLASS") && match("ID") && global_symbol_table_.create_class_entry_and_table("class", "", get_last_token().lexeme_) && match("OPENCURL") && classBody() && match("CLOSECURL") && match("DELI")) {
            return true;
        }
    }
    return false;
}

bool SemanticParser::classBody() {
    if (!skip_errors({"INT", "ID", "FLOAT"}, {"CLOSECURL"}, true))
        return false;
    if (is_lookahead_a_type()) {
        form_derivation_string("<classBody>", "<classInDecl> <classBody>");
        if (classInDecl() && classBody()) {
            return true;
        }
    } else if (lookahead_ == "CLOSECURL") { // FOLLOW SET
        form_derivation_string("<classBody>", "");
        return true;
    }
    return false;
}

bool SemanticParser::classInDecl() {
    if (!skip_errors({"INT", "ID", "FLOAT"}, {"INT", "ID", "FLOAT", "CLOSECURL"}, false))
        return false;
    if (is_lookahead_a_type()) {
        form_derivation_string("<classInDecl>", "<type> id <postTypeId>");
        SymbolRecord* record = new SymbolRecord();
        if (type(record) && match("ID", {"OPENBRA", "OPENPARA", "DELI", "CLOSECURL"}) && record->set_name(get_last_token().lexeme_) && postTypeId(record)) {
            return true;
        }
    }
    return false;
}

bool SemanticParser::postTypeId(SymbolRecord* record) {
    if (!skip_errors({"OPENBRA", "OPENPARA", "DELI"}, {"INT", "ID", "FLOAT", "CLOSECURL"}, false))
        return false;
    if (lookahead_ == "OPENBRA") {
        form_derivation_string("<postTypeId>", "<arraySize> ;");
        if (arraySize(record) && match("DELI") && global_symbol_table_.create_variable_entry(record)) {
            return true;
        }
    } else if (lookahead_ == "OPENPARA") {
        record->append_to_type(" : ");
        form_derivation_string("<postTypeId>", "( <fParams> ) <funcBody> ;");
        if (match("OPENPARA") && fParams(record) && match("CLOSEPARA") && funcBody(record) && match("DELI") && global_symbol_table_.create_function_class_entry(record)) {
            return true;
        }
    } else if (lookahead_ == "DELI") {
        form_derivation_string("<postTypeId>", ";");
        if (match("DELI") && global_symbol_table_.create_variable_entry(record))
            return true;
    }

        // panic mode, missing semi colon
    form_derivation_string("<postTypeId>", "<errorMissingSemiColon>");
    return true;

    //return false;
}

bool SemanticParser::progBody() {
    if (!skip_errors({"PROGRAM"}, {"END"}, false))
        return false;
    if (lookahead_ == "PROGRAM") {
        form_derivation_string("<progBody>", "program <funcBody> ; <funcDefLst>");
        if (match("PROGRAM") && global_symbol_table_.create_program_entry_and_table() && funcBody(global_symbol_table_.current_symbol_record_) && match("DELI") && funcDefLst())
            return true;
    }
    return false;
}

bool SemanticParser::funcHead(SymbolRecord* record) {
    if (!skip_errors({"INT", "ID", "FLOAT"}, {"OPENCURL"}, false))
        return false;
    if (is_lookahead_a_type()) {
        form_derivation_string("<funcHead>", "<type> id ( <fParams> )");
        if (type(record) && match("ID") && match("OPENPARA") && fParams(record) && match("CLOSEPARA"))
            return true;
    }
    return false;
}

bool SemanticParser::funcDefLst() {
    if (!skip_errors({"INT", "ID", "FLOAT"}, {"END"}, true))
        return false;
    if (is_lookahead_a_type()) {
        form_derivation_string("<funcDefLst>", "<funcDef> <funcDefLst>");
        if (global_symbol_table_.create_function_entry_and_table() && funcDef(global_symbol_table_.current_symbol_record_) && funcDefLst())
            return true;
    } else if (lookahead_ == "END") {
        form_derivation_string("<funcDefLst>", "");
        return true;
    }
    return false;
}

bool SemanticParser::funcDef(SymbolRecord* record) {
    if (!skip_errors({"INT", "ID", "FLOAT"}, {"INT", "ID", "FLOAT", "END"}, false))
        return false;
    if (is_lookahead_a_type()) {
        form_derivation_string("<funcDef>", "<funcHead> <funcBody> ;");
        if (funcHead(record) && funcBody(record) && match("DELI"))
            return true;
    }
    return false;
}

bool SemanticParser::funcBody(SymbolRecord* record) {
    if (!skip_errors({"OPENCURL"}, {"DELI"}, false))
        return false;
    if (lookahead_ == "OPENCURL") {
        form_derivation_string("<funcBody>", "{ <funcInBodyLst> }");
        if (match("OPENCURL") && funcInBodyLst(record) && match("CLOSECURL")) {
            return true;
        }
    }
    return false;
}


bool SemanticParser::funcInBodyLst(SymbolRecord* record) {
    if (!skip_errors({"INT", "ID", "FLOAT", "IF", "FOR", "GET", "PUT", "RETURN"}, {"CLOSECURL"}, true))
        return false;
    if (is_lookahead_a_type() || is_lookahead_a_statement()) {
        form_derivation_string("<funcInBodyLst>", "<funcInBody> <funcInBodyLst>");
        if (funcInBody(record) && funcInBodyLst(record)) {
            return true;
        }
    } else if (lookahead_ == "CLOSECURL") { // Follow set
        form_derivation_string("<funcInBodyLst>", "");
        return true;
    }
    return false;
}

bool SemanticParser::funcInBody(SymbolRecord* record) {
    if (!skip_errors({"INT", "ID", "FLOAT", "IF", "FOR", "GET", "PUT", "RETURN"},
                     {"INT", "ID", "FLOAT", "IF", "FOR", "GET", "PUT", "RETURN", "CLOSECURL"}, false))
        return false;
    if (lookahead_ == "ID") {
        form_derivation_string("<funcInBody>", "id <varOrStat>");
        if (match("ID") && record->set_type(get_last_token().lexeme_) && varOrStat(record)) {
            return true;
        }
    } else if (is_lookahead_a_statement()) {
        form_derivation_string("<funcInBody>", "<statementRes>");
        if (statementRes(record)) {
            return true;
        }
    } else if (lookahead_ == "INT" || lookahead_ == "FLOAT") {
        form_derivation_string("<funcInBody>", "<numType> id <arraySize> ;");
        if (numType(record) && match("ID", {"OPENBRA", "DELI"}) && record->set_name(get_last_token().lexeme_)) {
            arraySize(record);
                if(match("DELI", {"INT", "ID", "FLOAT", "IF", "FOR", "GET", "PUT", "RETURN"}))
                    return true;
        }
    }
    return false;
}

bool SemanticParser::varOrStat(SymbolRecord* record) {
    if (!skip_errors({"ID", "OPENBRA", "EQUAL", "DOT"},
                     {"INT", "ID", "FLOAT", "IF", "FOR", "GET", "PUT", "RETURN", "CLOSECURL"}, false))
        return false;
    if (lookahead_ == "ID") {
        form_derivation_string("<varOrStat>", "id <arraySize> ;");
        if (match("ID") && record->set_name(get_last_token().lexeme_) && arraySize(record) && match("DELI", {"INT", "ID", "FLOAT", "IF", "FOR", "GET", "PUT", "RETURN", "CLOSECURL"})) {
            return true;
        }
    } else if (lookahead_ == "OPENBRA" || lookahead_ == "EQUAL" || lookahead_ == "DOT") {
        form_derivation_string("<varOrStat>", "<indiceLst> <idnest> <assignOp> <expr> ;");
        if (indiceLst() && idnest() && assignOp() && expr() && match("DELI", {"INT", "ID", "FLOAT", "IF", "FOR", "GET", "PUT", "RETURN", "CLOSECURL"}, ";", "<missingSemiColon>")) {
            return true;
        }
    }
    return false;
}

bool SemanticParser::statementLst() {
    if (!skip_errors({"INT", "ID", "FLOAT", "IF", "FOR", "GET", "PUT", "RETURN"}, {"CLOSECURL"}, true))
        return false;
    if (lookahead_ == "ID" || is_lookahead_a_statement()) {
        form_derivation_string("<statementLst>", "<statement> <statementLst>");
        if (statement() && statementLst())
            return true;
    } else if (lookahead_ == "CLOSECURL") {
        form_derivation_string("<statementLst>", "");
        return true;
    }
}

bool SemanticParser::statement() {
    if (!skip_errors({"ID", "IF", "FOR", "GET", "PUT", "RETURN"},
                     {"ID", "IF", "FOR", "GET", "PUT", "RETURN", "ELSE", "DELI", "CLOSECURL"}, false))
        return false;
    if (lookahead_ == "ID") {
        form_derivation_string("<statement>", "<assignStat> ;");
        if (assignStat() && match("DELI"))
            return true;
    } else if (is_lookahead_a_statement()) {
        form_derivation_string("<statement>", "<statementRes>");
        SymbolRecord* record = new SymbolRecord();
        if (statementRes(record))
            return true;
    }
    return false;
}

bool SemanticParser::statementRes(SymbolRecord* record) {
    if (!skip_errors({"IF", "FOR", "GET", "PUT", "RETURN"},
                     {"ID", "INT", "FLOAT", "IF", "FOR", "GET", "PUT", "RETURN", "ELSE", "DELI", "CLOSECURL",}, false))
        return false;
    if (lookahead_ == "IF") {
        form_derivation_string("<statementRes>", "if ( <expr> ) then <statThenBlock>");
        if (match("IF") && match("OPENPARA") && expr() && match("CLOSEPARA") && match("THEN") && statThenBlock()) {
            return true;
        }
    } else if (lookahead_ == "FOR") {
        form_derivation_string("<statementRes>",
                               "for ( <type> id <assignOp> <expr> ; <relExpr> ; <assignStat> ) <statBlock>");
        if (match("FOR") && match("OPENPARA") && type(record) && match("ID") && record->set_name(get_last_token().lexeme_) && assignOp() && expr() && match("DELI") &&
            relExpr() && match("DELI") && assignStat() && match("CLOSEPARA") && statBlock()) {
            return true;
        }
    } else if (lookahead_ == "GET") {
        form_derivation_string("<statementRes>", "get ( <variable> ) ;");
        if (match("GET") && match("OPENPARA") && variable() && match("CLOSEPARA") && match("DELI")) {
            return true;
        }
    } else if (lookahead_ == "PUT") {
        form_derivation_string("<statementRes>", "put ( <expr> ) ;");
        if (match("PUT") && match("OPENPARA") && expr() && match("CLOSEPARA") && match("DELI")) {
            return true;
        }
    } else if (lookahead_ == "RETURN") {
        form_derivation_string("<statementRes>", "return ( <expr> ) ;");
        if (match("RETURN") && match("OPENPARA") && expr() && match("CLOSEPARA") && match("DELI")) {
            return true;
        }
    }
    return false;
}

bool SemanticParser::statIfElseBlock() {
    if (!skip_errors({"ELSE", "DELI"}, {"DELI", "ELSE", "ID", "IF", "FOR", "GET", "PUT", "RETURN"}, false))
        return false;
    if (lookahead_ == "ELSE") {
        form_derivation_string("<statIfElseBlock>", "else <statBlock>");
        if (match("ELSE") && statBlock())
            return true;
    } else if (lookahead_ == "DELI") {
        form_derivation_string("<statIfElseBlock>", "");
        if (match("DELI"))
            return true;
    }
    return false;
}

bool SemanticParser::statElseBlock() {
    if (!skip_errors({"ELSE", "DELI"}, {"DELI", "ELSE", "ID", "IF", "FOR", "GET", "PUT", "RETURN"}, false))
        return false;
    if (lookahead_ == "ELSE") {
        form_derivation_string("<statElseBlock>", "else <statBlock>");
        if (match("ELSE") && statBlock())
            return true;
    } else if (lookahead_ == "DELI" || lookahead_ == "ELSE" || lookahead_ == "ID" || lookahead_ == "IF" ||
               lookahead_ == "FOR" || lookahead_ == "GET" || lookahead_ == "PUT" || lookahead_ == "RETURN") {
        form_derivation_string("<statElseBlock>", "");
        if (match("DELI"))
            return true;
    }
    return true;
}

bool SemanticParser::assignStat() {
    if (!skip_errors({"ID"}, {"CLOSEPARA", "DELI"}, false))
        return false;
    if (lookahead_ == "ID") {
        form_derivation_string("<assignStat>", "<variable> <assignOp> <expr>");
        if (variable() && assignOp() && expr())
            return true;
    }
    return false;
}

bool SemanticParser::statBlock() {
    if (!skip_errors({"OPENCURL", "ID", "IF", "FOR", "GET", "PUT", "RETURN"}, {"ELSE", "DELI"}, true))
        return false;
    if (lookahead_ == "OPENCURL") {
        form_derivation_string("<statBlock>", "{ <statementLst> } ;");
        if (match("OPENCURL") && statementLst() && match("CLOSECURL") && match("DELI"))
            return true;
    } else if (lookahead_ == "ID" || is_lookahead_a_statement()) {
        form_derivation_string("<statBlock>", "<statement>");
        if (statement())
            return true;
    } else if (lookahead_ == "ELSE" || lookahead_ == "DELI") { // Follow set
        form_derivation_string("<statBlock>", "");
        return true;
    }
    return false;
}

bool SemanticParser::statThenBlock() {
    if (!skip_errors({"OPENCURL", "ID", "IF", "FOR", "GET", "PUT", "RETURN"}, {"ELSE", "DELI"}, true))
        return false;
    if (lookahead_ == "OPENCURL") {
        form_derivation_string("<statThenBlock>", "{ <statementLst> } <statIfElseBlock>");
        if (match("OPENCURL") && statementLst() && match("CLOSECURL") && statIfElseBlock())
            return true;
    } else if (lookahead_ == "OPENCURL" || lookahead_ == "ID" || is_lookahead_a_statement()) {
        form_derivation_string("<statThenBlock>", "<statement> <statElseBlock>");
        if (statement() && statElseBlock())
            return true;
    }
    return false;
}

bool SemanticParser::expr() {
    if (!skip_errors({"ID", "INUM", "FNUM", "OPENPARA", "NOT", "ADD", "SUB"}, {"COM", "CLOSEPARA", "DELI"}, false))
        return false;
    if (is_lookahead_a_value() || lookahead_ == "OPENPARA" || lookahead_ == "NOT" || lookahead_ == "ADD" ||
        lookahead_ == "SUB") {
        form_derivation_string("<expr>", "<arithExpr> <relOrAri>");
        if (arithExpr() && relOrAri()) {
            return true;
        }
    }
    return false;
}

bool SemanticParser::relOrAri() {
    if (!skip_errors({"EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "ADD", "SUB", "OR"}, {"COM", "CLOSEPARA", "DELI"},
                     true))
        return false;
    if (lookahead_ == "EQUIV" || lookahead_ == "NOTEQ" || lookahead_ == "LT" || lookahead_ == "GT" ||
        lookahead_ == "LTEQ" || lookahead_ == "GTEQ") {
        form_derivation_string("<relOrAri>", "<relOp> <arithExpr>");
        if (relOp() && arithExpr())
            return true;
    } else if (lookahead_ == "ADD" || lookahead_ == "SUB" || lookahead_ == "OR") {
        form_derivation_string("<relOrAri>", "<arithExprD>");
        if (arithExprD())
            return true;
    } else if (lookahead_ == "COM" || lookahead_ == "CLOSEPARA" || lookahead_ == "DELI") {
        form_derivation_string("<relOrAri>", "");
        return true;
    }

    return false;
}


bool SemanticParser::relExpr() {
    if (!skip_errors({"OPENPARA", "ID", "INUM", "FNUM", "NOT", "ADD", "SUB"}, {"DELI"}, false))
        return false;
    if (is_lookahead_a_value() || lookahead_ == "OPENPARA" || lookahead_ == "NOT" || lookahead_ == "ADD" ||
        lookahead_ == "SUB") {
        form_derivation_string("<relExpr>", "<arithExpr> <relOp> <arithExpr>");
        if (arithExpr() && relOp() && arithExpr())
            return true;
    }
    return false;

}

bool SemanticParser::arithExpr() {
    if (!skip_errors({"OPENPARA", "ID", "INUM", "FNUM", "NOT", "ADD", "SUB"},
                     {"CLOSEBRA", "CLOSEPARA", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "DELI", "COM"}, false))
        return false;
    if (is_lookahead_a_value() || lookahead_ == "OPENPARA" || lookahead_ == "NOT" || lookahead_ == "ADD" ||
        lookahead_ == "SUB") {
        form_derivation_string("<arithExpr>", "<term> <arithExprD>");
        if (term() && arithExprD())
            return true;
    }
    return false;
}

bool SemanticParser::arithExprD() {
    if (!skip_errors({"ADD", "SUB", "OR"},
                     {"CLOSEBRA", "CLOSEPARA", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "DELI", "COM"}, true))
        return false;
    if (lookahead_ == "ADD" || lookahead_ == "SUB" || lookahead_ == "OR") {
        form_derivation_string("<arithExprD>", "<addOp> <term> <arithExprD>");
        if (addOp() && term() && arithExprD())
            return true;
    } else if (check_if_lookahead_is_in_set(
            {"EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "CLOSEBRA", "CLOSEPARA", "DELI", "COM"})) { // follow set
        form_derivation_string("<arithExprD>", "");
        return true;
    }
    return false;
}

bool SemanticParser::sign() {
    if (!skip_errors({"ADD", "SUB"},
                     {"CLOSEBRA", "CLOSEPARA", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "DELI", "COM"}, false))
        return false;
    if (lookahead_ == "ADD") {
        form_derivation_string("<sign>", "+");
        if (match("ADD"))
            return true;
    } else if (lookahead_ == "SUB") {
        form_derivation_string("<sign>", "-");
        if (match("SUB"))
            return true;
    }
    return false;
}

bool SemanticParser::term() {
    if (!skip_errors({"OPENPARA", "ID", "INUM", "FNUM", "NOT", "ADD", "SUB"},
                     {"ADD", "SUB", "OR", "CLOSEBRA", "CLOSEPARA", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "DELI",
                      "COM"}, false))
        return false;
    if (is_lookahead_a_value() || lookahead_ == "OPENPARA" || lookahead_ == "NOT" || lookahead_ == "ADD" ||
        lookahead_ == "SUB") {
        form_derivation_string("<term>", "<factor> <termD>");
        if (factor() && termD()) {
            return true;
        }
    }
    return false;
}

bool SemanticParser::termD() {
    if (!skip_errors({"MULT", "DASH", "AND"},
                     {"ADD", "SUB", "OR", "CLOSEBRA", "CLOSEPARA", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "DELI",
                      "COM"}, true))
        return false;
    if (lookahead_ == "MULT" || lookahead_ == "DASH" || lookahead_ == "AND") {
        form_derivation_string("<termD>", "<multOp> <factor> <termD>");
        if (multOp() && factor() && termD())
            return true;
    } else if (check_if_lookahead_is_in_set(
            {"ADD", "SUB", "OR", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "CLOSEBRA", "CLOSEPARA", "DELI", "COM",
             "DOT"})) { // follow set
        form_derivation_string("<termD>", "");
        return true;
    }
    return false;
}

bool SemanticParser::factor() {
    if (!skip_errors({"OPENPARA", "ID", "INUM", "FNUM", "NOT", "ADD", "SUB"},
                     {"MULT", "DASH", "AND", "ADD", "SUB", "OR", "CLOSEBRA", "CLOSEPARA", "EQUIV", "NOTEQ", "LT", "GT",
                      "LTEQ", "GTEQ", "DELI", "COM"}, false))
        return false;
    if (lookahead_ == "ID") {
        form_derivation_string("<factor>", "id <factorVarOrFunc>");
        if (match("ID") && factorVarOrFunc())
            return true;
    } else if (lookahead_ == "INUM" || lookahead_ == "FNUM") {
        form_derivation_string("<factor>", "<num>");
        if (num())
            return true;
    } else if (lookahead_ == "OPENPARA") {
        form_derivation_string("<factor>", "( <arithExpr> )");
        if (match("OPENPARA") && arithExpr() && match("CLOSEPARA"))
            return true;
    } else if (lookahead_ == "NOT") {
        form_derivation_string("<factor>", "not <factor>");
        if (match("NOT") && factor())
            return true;
    } else if (lookahead_ == "ADD" || lookahead_ == "SUB") {
        form_derivation_string("<factor>", "<sign> <factor>");
        if (sign() && factor())
            return true;
    }
    return false;
}

bool SemanticParser::factorVarOrFunc() {
    if (!skip_errors({"OPENPARA", "OPENBRA", "DOT"},
                     {"MULT", "DASH", "AND", "ADD", "SUB", "OR", "CLOSEBRA", "CLOSEPARA", "EQUIV", "NOTEQ", "LT", "GT",
                      "LTEQ", "GTEQ", "DELI", "COM"}, true))
        return false;
    if (lookahead_ == "OPENBRA") {
        form_derivation_string("<factorVarOrFunc>", "<factorVarArray>");
        if (factorVarArray())
            return true;
    } else if (lookahead_ == "DOT" || lookahead_ == "OPENPARA") {
        form_derivation_string("<factorVarOrFunc>", "<varOrFuncIdNest>");
        if (varOrFuncIdNest())
            return true;
    } else if (check_if_lookahead_is_in_set(
            {"MULT", "DASH", "AND", "ADD", "SUB", "OR", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "CLOSEBRA",
             "CLOSEPARA", "DELI", "COM", "DOT"})) { // follow set
        form_derivation_string("<factorVarOrFunc>", "");
        return true;
    }
    return false;
}

bool SemanticParser::factorVarArray() {
    if (!skip_errors({"OPENBRA"},
                     {"MULT", "DASH", "AND", "ADD", "SUB", "OR", "CLOSEBRA", "CLOSEPARA", "EQUIV", "NOTEQ", "LT", "GT",
                      "LTEQ", "GTEQ", "DELI", "COM"}, false))
        return false;
    if (lookahead_ == "OPENBRA") {
        form_derivation_string("<factorVarArray>", "<indice> <indiceLst> <factorVarArrayNestId>");
        if (indice() && indiceLst() && factorVarArrayNestId())
            return true;
    }
}

bool SemanticParser::factorVarArrayNestId() {
    if (!skip_errors({"DOT"},
                     {"MULT", "DASH", "AND", "ADD", "SUB", "OR", "CLOSEBRA", "CLOSEPARA", "EQUIV", "NOTEQ", "LT", "GT",
                      "LTEQ", "GTEQ", "DELI", "COM"}, true))
        return false;
    if (lookahead_ == "DOT") {
        form_derivation_string("<factorVarArrayNestId>", ". id <factorVarOrFunc>");
        if (match("DOT") && match("ID") && factorVarOrFunc())
            return true;
    } else if (check_if_lookahead_is_in_set(
            {"MULT", "DASH", "AND", "ADD", "SUB", "OR", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "CLOSEBRA",
             "CLOSEPARA", "DELI", "COM", "DOT"})) { // follow set
        form_derivation_string("<factorVarArrayNestId>", "");
        return true;
    }
    return false;
}

bool SemanticParser::varOrFuncIdNest() {
    if (!skip_errors({"OPENPARA", "DOT"},
                     {"MULT", "DASH", "AND", "ADD", "SUB", "OR", "CLOSEBRA", "CLOSEPARA", "EQUIV", "NOTEQ", "LT", "GT",
                      "LTEQ", "GTEQ", "DELI", "COM"}, true))
        return false;
    if (lookahead_ == "OPENPARA") {
        form_derivation_string("<varOrFuncIdNest>", "( <aParams> )");
        if (match("OPENPARA") && aParams() && match("CLOSEPARA"))
            return true;
    } else if (lookahead_ == "DOT") {
        form_derivation_string("<varOrFuncIdNest>", ". id <factorVarOrFunc>");
        if (match("DOT") && match("ID") && factorVarOrFunc())
            return true;
    }
    return false;
}

bool SemanticParser::variable() {
    if (!skip_errors({"ID"}, {"EQUAL", "CLOSEPARA"}, false))
        return false;
    if (lookahead_ == "ID") {
        form_derivation_string("<variable>", "id <variableIndice>");
        if (match("ID") && variableIndice())
            return true;
    }
    return false;
}

bool SemanticParser::variableIndice() {
    if (!skip_errors({"OPENBRA", "DOT"}, {"EQUAL", "CLOSEPARA"}, true))
        return false;
    if (lookahead_ == "OPENBRA" || lookahead_ == "DOT") {
        form_derivation_string("<variableIndice>", "<indiceLst> <idnest>");
        if (indiceLst() && idnest())
            return true;
    } else if (lookahead_ == "EQUAL" || lookahead_ == "CLOSEPARA") {
        form_derivation_string("<variableIndice>", "");
        return true;
    }
    return false;
}

bool SemanticParser::check_if_lookahead_is_in_set(set<string> values) {
    return values.find(lookahead_) != values.end();
}

bool SemanticParser::idnest() {
    if (!skip_errors({"DOT"}, {"EQUAL", "CLOSEPARA"}, true))
        return false;
    if (lookahead_ == "DOT") {
        form_derivation_string("<idnest>", ". id <indiceLst> <idnest>");
        if (match("DOT") && match("ID") && indiceLst() && idnest()) {
            return true;
        }
    } else if (lookahead_ == "EQUAL" || lookahead_ == "CLOSEPARA") {
        form_derivation_string("<idnest>", "");
        return true;
    }
    return false;
}

bool SemanticParser::indice() {
    if (!skip_errors({"OPENBRA"},
                     {"OPENBRA", "DOT", "EQUAL", "MULT", "DASH", "AND", "ADD", "SUB", "OR", "CLOSEBRA", "CLOSEPARA",
                      "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "DELI", "COM"}, false))
        return false;
    if (lookahead_ == "OPENBRA") {
        form_derivation_string("<indice>", "[ <arithExpr> ]");
        if (match("OPENBRA") && arithExpr() && match("CLOSEBRA")) {
            return true;
        }
    }
    return false;
}

bool SemanticParser::indiceLst() {
    if (!skip_errors({"OPENBRA"},
                     {"OPENBRA", "DOT", "EQUAL", "MULT", "DASH", "AND", "ADD", "SUB", "OR", "CLOSEBRA", "CLOSEPARA",
                      "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ", "DELI", "COM"}, true))
        return false;
    if (lookahead_ == "OPENBRA") {
        form_derivation_string("<indiceLst>", "<indice> <indiceLst>");
        if (indice() && indiceLst()) {
            return true;
        }
    } else if (lookahead_ == "DOT" || lookahead_ == "EQUAL" || lookahead_ == "CLOSEPARA") { // Follow set
        form_derivation_string("<indiceLst>", "");
        return true;
    } else if (check_if_lookahead_is_in_set(
            {"DOT", "EQUAL", "MULT", "DASH", "AND", "ADD", "SUB", "OR", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ",
             "CLOSEBRA", "CLOSEPARA", "DELI", "COM", "DOT"})) { // follow set
        form_derivation_string("<indiceLst>", "");
        return true;
    }
    return false;
}

bool SemanticParser::is_lookahead_a_type() {
    return lookahead_ == "ID" || lookahead_ == "INT" || lookahead_ == "FLOAT";
}

bool SemanticParser::is_lookahead_a_value() {
    return lookahead_ == "INUM" || lookahead_ == "FNUM" || lookahead_ == "ID";
}

bool SemanticParser::is_lookahead_a_statement() {
    return lookahead_ == "FOR" || lookahead_ == "GET" || lookahead_ == "PUT" || lookahead_ == "RETURN" ||
           lookahead_ == "IF";
}


void SemanticParser::report_error(string expected_token, string actual_token) {
    string error_message = "Syntax Error at " + to_string(current_token_->row_location_) + " : " +
                           to_string(current_token_->column_location_);
    if (expected_token.size() > 1 && actual_token.size() > 1) {
        error_message = "Syntax Error: expected token " + expected_token;
        if (previous_token_.size() > 1)
            error_message += " was supposed to proceed last token" + previous_token_;

        error_message +=
                " but " + actual_token + " was received instead:" + to_string(current_token_->row_location_) + ":" +
                to_string(current_token_->column_location_);
    } else {
        error_message = "Syntax Error: token " + expected_token +
                        " received not in first or follow set of current production:" +
                        to_string(current_token_->row_location_) + ":" + to_string(current_token_->column_location_);;
    }
    errors_.push_back(error_message);
    if (output_to_file_) {
        error_output_file_ << error_message + "\n";
    }
}

bool SemanticParser::arraySize(SymbolRecord* record) {
    if (!skip_errors({"OPENBRA"}, {"COM", "DELI", "CLOSEPARA"}, true))
        return false;
    if (lookahead_ == "OPENBRA") {
        form_derivation_string("<arraySize>", "[ integer ] <arraySize>");
        if (match("OPENBRA") && match("INUM") && record->append_to_type("[" + get_last_token().lexeme_) && match("CLOSEBRA") && record->append_to_type("]") && arraySize(record)) {
            return true;
        }
    } else if (lookahead_ == "COM" || lookahead_ == "DELI" || lookahead_ == "CLOSEPARA") {// Follow set
        form_derivation_string("<arraySize>", "");
        return true;
    }
    form_derivation_string("<arraySize>", "");
    return false;
}

bool SemanticParser::type(SymbolRecord* record) {
    if (!skip_errors({"INT", "FLOAT", "ID"}, {"ID"}, false))
        return false;
    if (lookahead_ == "INT" && match("INT")) {
        form_derivation_string("<type>", "int");
        record->type_ = "int";
        return true;
    } else if (lookahead_ == "FLOAT" && match("FLOAT")) {
        form_derivation_string("<type>", "float");
        record->type_ = "float";
        return true;
    } else if (lookahead_ == "ID" && match("ID")) {
        form_derivation_string("<type>", "id");
        record->type_ = "id";
        return true;
    }
    return false;
}

bool SemanticParser::numType(SymbolRecord* record) {
    if (!skip_errors({"INT", "FLOAT"}, {"ID"}, false))
        return false;
    if (lookahead_ == "INT") {
        form_derivation_string("<numType>", "int");
        if (match("INT")) {
            record->type_ = "int";
            return true;
        }
    } else if (lookahead_ == "FLOAT") {
        form_derivation_string("<numType>", "float");
        if (match("FLOAT")) {
            record->type_ = "float";
            return true;
        }
    }
}

bool SemanticParser::fParams(SymbolRecord* record) {
    if (!skip_errors({"INT", "FLOAT", "ID"}, {"CLOSEPARA"}, true))
        return false;
    if (is_lookahead_a_type()) {
        form_derivation_string("<fParams>", "<type> id <arraySize> <fParamsTail>");
        SymbolRecord* fParam_record = new SymbolRecord();
        if (type(fParam_record) && match("ID") && fParam_record->set_name(get_last_token().lexeme_) && arraySize(fParam_record) && record->symbol_table_->create_parameter_entry(fParam_record) && fParamsTail(record))
            record->generate_function_type();
            return true;
    } else if (lookahead_ == "CLOSEPARA") {
        form_derivation_string("<fParams>", "");
        return true;
    }
    return false;
}

bool SemanticParser::aParams() {
    if (!skip_errors({"ID", "INUM", "FNUM", "OPENPARA", "NOT", "ADD", "SUB"}, {"CLOSEPARA"}, true))
        return false;
    if (is_lookahead_a_value() || lookahead_ == "OPENPARA" || lookahead_ == "NOT" || lookahead_ == "ADD" ||
        lookahead_ == "SUB") {
        form_derivation_string("<aParams>", "<expr> <aParamsTail>");
        if (expr() && aParamsTail())
            return true;
    } else if (lookahead_ == "CLOSEPARA") {
        form_derivation_string("<aParams>", "");
        return true;
    }
    return false;
}

bool SemanticParser::fParamsTail(SymbolRecord* record) {
    if (!skip_errors({"COM"}, {"CLOSEPARA"}, true))
        return false;
    if (lookahead_ == "COM") {
        form_derivation_string("<fParamsTail>", ", <type> id <arraySize> <fParamsTail>");
        SymbolRecord* fParam_record;
        if (match("COM") && type(record) && match("ID") && fParam_record->set_name(get_last_token().lexeme_) && arraySize(record) && record->symbol_table_->create_parameter_entry(record) && fParamsTail(record))
            return true;
    } else if (lookahead_ == "CLOSEPARA") { // Follow set
        form_derivation_string("<fParamsTail>", "");
        return true;
    }
    return false;
}

bool SemanticParser::aParamsTail() {
    if (!skip_errors({"COM"}, {"CLOSEPARA"}, true))
        return false;
    if (lookahead_ == "COM") {
        form_derivation_string("<aParamsTail>", ", <expr> <aParamsTail>");
        if (match("COM") && expr() && aParamsTail())
            return true;
    } else if (lookahead_ == "CLOSEPARA") { // Follow set
        form_derivation_string("<aParamsTail>", "");
        return true;
    }
    return false;
}

bool SemanticParser::assignOp() {
    if (!skip_errors({"EQUAL"},
                     {"ID", "OPENPARA", "NOT", "INUM", "FNUM", "ADD", "SUB", "IF", "FOR", "GET", "PUT", "RETURN", "INT",
                      "CLOSECURL"}, false))
        return false;
    if (lookahead_ == "EQUAL" && match("EQUAL")) {
        form_derivation_string("<assignOp>", "=");
        return true;
    }
    return false;
}

bool SemanticParser::relOp() {
    if (!skip_errors({"EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ"},
                     {"ID", "OPENPARA", "NOT", "INUM", "FNUM", "ADD", "SUB"}, false))
        return false;
    if (lookahead_ == "EQUIV") {
        form_derivation_string("<relOp>", "==");
        if (match("EQUIV"))
            return true;
    } else if (lookahead_ == "NOTEQ") {
        form_derivation_string("<relOp>", "<>");
        if (match("NOTEQ"))
            return true;
    } else if (lookahead_ == "LT") {
        form_derivation_string("<relOp>", "<");
        if (match("LT"))
            return true;
    } else if (lookahead_ == "GT") {
        form_derivation_string("<relOp>", ">");
        if (match("GT"))
            return true;
    } else if (lookahead_ == "LTEQ") {
        form_derivation_string("<relOp>", "<=");
        if (match("LTEQ"))
            return true;
    } else if (lookahead_ == "GTEQ") {
        form_derivation_string("<relOp>", ">=");
        if (match("GTEQ"))
            return true;
    }
    return false;
}

bool SemanticParser::addOp() {
    if (!skip_errors({"ADD", "SUB", "OR"}, {"ID", "OPENPARA", "NOT", "INUM", "FNUM", "ADD", "SUB"}, false))
        return false;
    if (lookahead_ == "ADD") {
        form_derivation_string("<addOp>", "+");
        if (match("ADD"))
            return true;
    } else if (lookahead_ == "SUB") {
        form_derivation_string("<addOp>", "-");
        if (match("SUB"))
            return true;
    } else if (lookahead_ == "OR") {
        form_derivation_string("<addOp>", "or");
        if (match("OR"))
            return true;
    }
    return false;
}

bool SemanticParser::multOp() {
    if (!skip_errors({"MULT", "DASH", "AND"}, {"ID", "OPENPARA", "NOT", "INUM", "FNUM", "ADD", "SUB"}, false))
        return false;
    if (lookahead_ == "MULT") {
        form_derivation_string("<multOp>", "*");
        if (match("MULT"))
            return true;
    } else if (lookahead_ == "DASH") {
        form_derivation_string("<multOp>", "/");
        if (match("DASH"))
            return true;
    } else if (lookahead_ == "AND") {
        form_derivation_string("<multOp>", "and");
        if (match("AND"))
            return true;
    }
    return false;
}


bool SemanticParser::num() {
    if (!skip_errors({"INUM", "FNUM"},
                     {"MULT", "DASH", "AND", "ADD", "SUB", "OR", "EQUIV", "NOTEQ", "LT", "GT", "LTEQ", "GTEQ",
                      "CLOSEBRA", "CLOSEPARA", "DELI"}, false))
        return false;
    if (lookahead_ == "INUM") {
        form_derivation_string("<num>", "integer");
        if (match("INUM"))
            return true;
    } else if (lookahead_ == "FNUM") {
        form_derivation_string("<num>", "float");
        if (match("FNUM"))
            return true;
    }
    return false;
}

bool SemanticParser::skip_errors(set<string> first_set, set<string> follow_set, bool epsilon) {
    if (first_set.find(lookahead_) != first_set.end() || (epsilon && follow_set.find(lookahead_) != follow_set.end())) {
        return true;
    }
    report_error(lookahead_, "");
    while (first_set.find(lookahead_) == first_set.end() &&
           follow_set.find(lookahead_) == follow_set.end()) {
        next_token();
        if (lookahead_ == "END")
            return false;
//        if (!epsilon && follow_set.find(lookahead_) != follow_set.end())
//            return false;
    }
    return true;
}

bool SemanticParser::match(string token, set<string> expected_post_tokens, string message_to_replace,  string replace_message) {
    if (lookahead_ == token) {
        lookahead_ = next_token();
        return true;
    }
    do {
        report_error(token, lookahead_);
        if (expected_post_tokens.find(lookahead_) != expected_post_tokens.end()) {
            // assume token is missing and the current read one is for the next match
            form_derivation_string(message_to_replace, replace_message);
            return true;
        }
        lookahead_ = next_token();
    } while (lookahead_ != token);
    form_derivation_string(message_to_replace, replace_message);
    lookahead_ = next_token();
    return true;
}

bool SemanticParser::match(string token, set<string> expected_post_tokens) {
    if (lookahead_ == token) {
        lookahead_ = next_token();
        return true;
    }
    do {
        report_error(token, lookahead_);
        if (expected_post_tokens.find(lookahead_) != expected_post_tokens.end()) {
            // assume token is missing and the current read one is for the next match
            return true;
        }
        lookahead_ = next_token();
    } while (lookahead_ != token);
    lookahead_ = next_token();
    return true;


}

bool SemanticParser::match(string token) {
    if (lookahead_ == token) {
        lookahead_ = next_token();
        return (true);
    } else {
        report_error(token, lookahead_);
        lookahead_ = next_token();
        return false;
    }
}

bool SemanticParser::form_derivation_string(string non_terminal, string rhs) {
    int begin_index = current_rhs_derivation_.find(non_terminal);
    if (begin_index == string::npos)
        return false;
    if (rhs.size() == 0) {
        if (begin_index > 0)
            current_rhs_derivation_.erase(begin_index - 1, non_terminal.size() + 1);
        else
            current_rhs_derivation_.erase(begin_index, non_terminal.size());
    } else
        current_rhs_derivation_.replace(begin_index, non_terminal.size(), rhs);
    if (enable_derivation_output_)
        cout << current_rhs_derivation_ << endl;
    if (output_to_file_)
        derivation_output_file_ << current_rhs_derivation_ << "\n";
    derivations_.push_back(current_rhs_derivation_);
    return true;
}

string SemanticParser::next_token() {
    do {
        if (current_token_position_ < tokens_.size()) {
            current_token_ = tokens_[current_token_position_++];
            lookahead_ = current_token_->token_identifier_;
        } else {
            lookahead_ = "END";
        }
    } while (lookahead_ == "CMT");
    return lookahead_;
}


Token SemanticParser::get_last_token() {
    Token* token = tokens_[current_token_position_ - 2];
    return *(token);
}