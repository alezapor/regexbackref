//
// Created by osboxes on 11/27/19.
//

#ifndef REGEX_MATCHER_LEXER_H
#define REGEX_MATCHER_LEXER_H

#include <cstdio>
#include <string>
#include <fstream>

/**
 * An enumeration that represents token types of a lexer
 */
enum Token {
    tokenEOF = -1,

    // special signs
    tokenIteration = -2,
    tokenAlternation = -3,
    tokenBackreference = -4,
    //tokenEmpty = -5,

    // groups
    tokenCharacter = -20
};

/**
 * A class that represents a lexer
 */
class Lexer {
public:
    Lexer(std::fstream * is) {this->is = is;}

    int val;                     // Filled in if tokenLetter/tokenNumber
    int numVal;                   // Filled in if tokenBackreference
    int getTok();
    std::fstream * is;
};



#endif //REGEX_MATCHER_LEXER_H
