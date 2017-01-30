//
// Created by Harley on 1/27/2017.
//

#include "ErrorToken.h"

ErrorToken::ErrorToken(const string &token_identifier, const string &lexeme, int location, int row_location,
                       int column_location, string error_message) : Token(token_identifier, lexeme, location, row_location,
                                                    column_location) {
    error_message_ = error_message;
}

string ErrorToken::generate_error_message() {
    return "Error:" + error_message_ + ":" + to_string(row_location_) + ":" + to_string(column_location_);
}
