//
// Created by osboxes on 11/27/19.
//

#ifndef REGEX_MATCHER_PARSER_H
#define REGEX_MATCHER_PARSER_H


#include <memory>
#include <algorithm>
#include <iostream>
#include "lexer.h"
#include "ast.h"
#include <set>

class Parser {
    int curTok;
    Lexer m_Lexer;
    int parCnt;
    std::set<char> m_Abc;
public:
    int getParCnt() const;
    std::set<char>& getAbc(){return m_Abc;}

public:

    Parser(std::fstream * is) : m_Lexer(Lexer(is)), parCnt(0) { }

    std::unique_ptr<NodeAST> ParseS();

    std::unique_ptr<NodeAST> ParseA();

    std::unique_ptr<NodeAST> ParseARest(std::unique_ptr<NodeAST>);

    std::unique_ptr<NodeAST> ParseB();

    std::unique_ptr<NodeAST> ParseBRest(std::unique_ptr<NodeAST>);

    std::unique_ptr<NodeAST> ParseC();

    std::unique_ptr<NodeAST> ParseCRest(std::unique_ptr<NodeAST>);

    std::unique_ptr<NodeAST> ParseR();

    int getNextToken() {
        return curTok = m_Lexer.getTok();
    }

    void MatchError(Token token);

    void ExpandError(std::string nonTerminal, Token token);


};



#endif //REGEX_MATCHER_PARSER_H
