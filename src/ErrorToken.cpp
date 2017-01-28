//
// Created by Harley on 1/27/2017.
//

#include "ErrorToken.h"
#include <string>
#include <sstream>

ErrorToken::ErrorToken(const string &token_identifier, const string &lexeme, int location, int row_location,
                       int column_location, string error_message) : Token(token_identifier, lexeme, location, row_location,
                                                    column_location) {
    error_message_ = error_message;
}

string ErrorToken::generate_error_message() {
    string row_location_string = static_cast<ostringstream*>( &(ostringstream() << row_location_) )->str();
    string column_location_string = static_cast<ostringstream*>( &(ostringstream() << column_location_) )->str();
    return "Error:" + error_message_ + ":" + row_location_string + ":" + column_location_string;
}
