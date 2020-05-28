
#ifndef REGEX_MATCHER_PARSER_H
#define REGEX_MATCHER_PARSER_H


#include <memory>
#include <algorithm>
#include <sstream>
#include <iostream>
#include "lexer.h"
#include "ast.h"

/**
 * A class that simulates a parser of the regular expression with backreferences
 */
class Parser {
    /**
     * A token to be read
     */
    int m_CurTok;

    /**
     * A pointer to the lexer
     */
    std::unique_ptr<Lexer> m_Lexer;

    /**
     * A set of input symbols from regular expression
     */
    std::set<char> m_Input;

    /**
    * A set of variables
    */
    std::set<char> m_Vars;

public:
    /**
     * A constructor
     * @param is a file stream that contains a regular expression to be parsed
     */
    Parser(std::istringstream *is);

    std::set<char> &getInput();

    std::set<char> &getVars();

    std::unique_ptr<NodeAST> ParseA();

    std::unique_ptr<NodeAST> ParseARest(std::unique_ptr<NodeAST>);

    std::unique_ptr<NodeAST> ParseB();

    std::unique_ptr<NodeAST> ParseBRest(std::unique_ptr<NodeAST>);

    std::unique_ptr<NodeAST> ParseC();

    std::unique_ptr<NodeAST> ParseCRest(std::unique_ptr<NodeAST>);

    std::unique_ptr<NodeAST> ParseD();

    std::unique_ptr<NodeAST> ParseFRest(int val);

    int getNextToken();

    void MatchError(Token token);

    void ExpandError(std::string nonTerminal, Token token);


};


#endif //REGEX_MATCHER_PARSER_H
