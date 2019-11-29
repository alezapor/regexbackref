//
// Created by osboxes on 11/27/19.
//

#include "lexer.h"

/// getTok - Return the next token from standard input.
int Lexer::getTok() {
    static int lastChar = ' ';
    std::string numStr;

    // Skip any whitespace.
    while (isspace(lastChar))
        lastChar = is->get();

    if (isalpha(lastChar)) { // letter: [a-zA-Z]
        val = lastChar;
        lastChar = is->get();
        return tokenLetter;
    }

    if (lastChar == '*') {
        lastChar = is->get();
        return tokenIteration;
    }

    if (lastChar == '+'){
        lastChar = is->get();
        return tokenAlternation;
    }

    // Check for end of file.  Don't eat the EOF.
    if (lastChar == EOF) {
        return tokenEOF;
    }

    // Otherwise, just return the character as its ascii value.
    int thisChar = lastChar;
    lastChar = is->get();

    if (thisChar == '\\') {  // backReference
        if (isdigit(lastChar)) {
            do {
                numVal = numVal * 10 + lastChar - '0';
                lastChar = is->get();
            } while (isdigit(lastChar));

            return tokenBackreference;
        }
    }


    return thisChar;
}


