//
// Created by osboxes on 11/27/19.
//

#include "parser.h"

Parser::Parser(std::fstream *is) : m_Lexer(std::make_unique<Lexer>(is)), m_ParCnt(0) {

}

void Parser::MatchError(Token token) {
    printf("Match error: token %d.\n", token);
    exit(1);
}

void Parser::ExpandError(std::string nonTerminal, Token token) {
    std::cout << "Expand error: nonterminal " << nonTerminal << ", token: " << token << std::endl;
    exit(1);
}

// S -> A | eps
std::unique_ptr<NodeAST> Parser::ParseS(){
    if(m_CurTok == tokenCharacter ||
       m_CurTok == '(' || m_CurTok == tokenBackreference){
        return std::move(ParseA());
    }
    else if (m_CurTok == tokenEOF) {
        return std::make_unique<EmptyAST>();
    }
    else {
        ExpandError("S", (Token) m_CurTok);
        return nullptr;
    }

}

// A -> B A'
std::unique_ptr<NodeAST> Parser::ParseA(){
    if(m_CurTok == tokenCharacter||
       m_CurTok == '(' || m_CurTok == tokenBackreference){
        return std::move(ParseARest(std::move(ParseB())));
    }
    else {
        ExpandError("A", (Token) m_CurTok);
        return nullptr;
    }
}

// A' -> + A | eps
std::unique_ptr<NodeAST> Parser::ParseARest(std::unique_ptr<NodeAST> left) {
    if(m_CurTok == tokenAlternation){
        getNextToken();
        auto right = std::move(ParseA());
        return std::make_unique<AlterationAST>(std::move(left), std::move(right));
    }
    else if (m_CurTok == tokenEOF || m_CurTok == ')') {
        return std::move(left);
    }
    else {
        ExpandError("A'", (Token) m_CurTok);
        return nullptr;
    }
}

// B -> C B'
std::unique_ptr<NodeAST> Parser::ParseB(){
    if(m_CurTok == tokenCharacter ||
       m_CurTok == '(' || m_CurTok == tokenBackreference){
        return std::move(ParseBRest(std::move(ParseC())));
    }
    else {
        ExpandError("B", (Token) m_CurTok);
        return nullptr;
    }
}

// B' -> B | eps
std::unique_ptr<NodeAST> Parser::ParseBRest(std::unique_ptr<NodeAST> left) {
    if(m_CurTok == tokenCharacter ||
       m_CurTok == '(' || m_CurTok == tokenBackreference){
        auto right = std::move(ParseB());
        return std::make_unique<ConcatenationAST>(std::move(left), std::move(right));
    }
    else if (m_CurTok == tokenAlternation || m_CurTok == ')' ||
        m_CurTok == tokenEOF) {
        return std::move(left);
    }
    else {
        ExpandError("B'", (Token) m_CurTok);
        return nullptr;
    }
}

// C -> R C'
std::unique_ptr<NodeAST> Parser::ParseC(){
    if(m_CurTok == tokenCharacter ||
       m_CurTok == '(' || m_CurTok == tokenBackreference){
        return std::move(ParseCRest(std::move(ParseR())));
    }
    else {
        ExpandError("C", (Token) m_CurTok);
        return nullptr;
    }
}

// C' -> * | eps
std::unique_ptr<NodeAST> Parser::ParseCRest(std::unique_ptr<NodeAST> left) {
    if(m_CurTok == tokenIteration){
        getNextToken();
        return std::make_unique<IterationAST>(std::move(left));
    }
    else if (m_CurTok == tokenAlternation || m_CurTok == ')' || m_CurTok == tokenBackreference ||
             m_CurTok == tokenEOF || m_CurTok == '(' || m_CurTok == tokenCharacter) {
        return std::move(left);
    }
    else {
        ExpandError("C'", (Token) m_CurTok);
        return nullptr;
    }
}

// R -> num | let | ( A )
std::unique_ptr<NodeAST> Parser::ParseR() {
    if(m_CurTok == '('){
        getNextToken();
        auto expr = std::make_unique<ParenthesesAST>(++m_ParCnt, std::move(ParseA()));
        if (m_CurTok != ')'){
            MatchError((Token) m_CurTok);
            return nullptr;
        }
        getNextToken();
        return std::move(expr);
    }
    else if (m_CurTok == tokenCharacter) {
        m_Input.insert(m_Lexer->val);
        auto prim = std::make_unique<PrimitiveAST>(m_Lexer->val);
        getNextToken();
        return std::move(prim);
    }
    else if (m_CurTok == tokenBackreference) {
        auto back = std::make_unique<BackreferenceAST>(m_Lexer->numVal);
        getNextToken();
        return std::move(back);
    }

    else {
        ExpandError("R", (Token) m_CurTok);
        return nullptr;
    }
}

int Parser::getParCnt() const {
    return m_ParCnt;
}

int Parser::getNextToken() {
    return m_CurTok = m_Lexer->getTok();

}

std::set<char> &Parser::getInput() {
    return m_Input;
}

