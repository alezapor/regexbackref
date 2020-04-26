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

    if (islower(lastChar) || lastChar == '0' || lastChar == '?') { // abeceda [a-z?0]
        val = lastChar;
        lastChar = is->get();
        return tokenAtom;
    }

    if (isupper(lastChar)) { // : []
        val = lastChar;
        lastChar = is->get();
        return tokenVar;
    }
    if (lastChar == '*') {
        lastChar = is->get();
        return tokenIter;
    }

    if (lastChar == '+') {
        lastChar = is->get();
        return tokenUnion;
    }

    // Check for end of file.  Don't eat the EOF.
    if (lastChar == EOF) {
        return tokenEOF;
    }

    // Otherwise, just return the character as its ascii value.
    int thisChar = lastChar;
    lastChar = is->get();

    return thisChar;
}


