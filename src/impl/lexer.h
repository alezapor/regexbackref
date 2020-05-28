
#ifndef REGEX_MATCHER_LEXER_H
#define REGEX_MATCHER_LEXER_H

#include <cstdio>
#include <string>
#include <sstream>

/**
 * An enumeration that represents token types of a lexer
 */
enum Token {
    tokenEOF = -1,

    // special signs
    tokenIter = -2,
    tokenUnion= -3,
    tokenVar = -4,

    // groups
    tokenAtom = -20
};

/**
 * A class that represents a lexer
 */
class Lexer {
public:
    Lexer(std::istringstream * is) {this->is = is;}

    int val;
    int getTok();
    std::istringstream * is;
};



#endif //REGEX_MATCHER_LEXER_H
