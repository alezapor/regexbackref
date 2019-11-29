//
// Created by osboxes on 11/27/19.
//

#include "parser.h"

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
    if(curTok == tokenLetter ||
       curTok == '(' || curTok == tokenBackreference){
        return std::move(ParseA());
    }
    else if (curTok == tokenEOF) {
        return std::make_unique<EmptyAST>();
    }
    else {
        ExpandError("S", (Token) curTok);
        return nullptr;
    }

}

// A -> B A'
std::unique_ptr<NodeAST> Parser::ParseA(){
    if(curTok == tokenLetter ||
       curTok == '(' || curTok == tokenBackreference){
        return std::move(ParseARest(std::move(ParseB())));
    }
    else {
        ExpandError("A", (Token) curTok);
        return nullptr;
    }
}

// A' -> + A | eps
std::unique_ptr<NodeAST> Parser::ParseARest(std::unique_ptr<NodeAST> left) {
    if(curTok == tokenAlternation){
        getNextToken();
        auto right = std::move(ParseA());
        return std::make_unique<AlterationAST>(std::move(left), std::move(right));
    }
    else if (curTok == tokenEOF || curTok == ')') {
        return std::move(left);
    }
    else {
        ExpandError("A'", (Token) curTok);
        return nullptr;
    }
}

// B -> C B'
std::unique_ptr<NodeAST> Parser::ParseB(){
    if(curTok == tokenLetter ||
       curTok == '(' || curTok == tokenBackreference){
        return std::move(ParseBRest(std::move(ParseC())));
    }
    else {
        ExpandError("B", (Token) curTok);
        return nullptr;
    }
}

// B' -> B | eps
std::unique_ptr<NodeAST> Parser::ParseBRest(std::unique_ptr<NodeAST> left) {
    if(curTok == tokenLetter ||
       curTok == '(' || curTok == tokenBackreference){
        auto right = std::move(ParseB());
        return std::make_unique<ConcatenationAST>(std::move(left), std::move(right));
    }
    else if (curTok == tokenAlternation || curTok == ')' ||
        curTok == tokenEOF) {
        return std::move(left);
    }
    else {
        ExpandError("B'", (Token) curTok);
        return nullptr;
    }
}

// C -> R C'
std::unique_ptr<NodeAST> Parser::ParseC(){
    if(curTok == tokenLetter ||
       curTok == '(' || curTok == tokenBackreference){
        return std::move(ParseCRest(std::move(ParseR())));
    }
    else {
        ExpandError("C", (Token) curTok);
        return nullptr;
    }
}

// C' -> * | eps
std::unique_ptr<NodeAST> Parser::ParseCRest(std::unique_ptr<NodeAST> left) {
    if(curTok == tokenIteration){
        getNextToken();
        return std::make_unique<IterationAST>(std::move(left));
    }
    else if (curTok == tokenAlternation || curTok == ')' || curTok == tokenBackreference ||
             curTok == tokenEOF || curTok == '(' || curTok == tokenLetter) {
        return std::move(left);
    }
    else {
        ExpandError("C'", (Token) curTok);
        return nullptr;
    }
}

// R -> num | let | ( A )
std::unique_ptr<NodeAST> Parser::ParseR() {
    if(curTok == '('){
        getNextToken();
        auto expr = std::make_unique<ParenthesesAST>(++parCnt, std::move(ParseA()));
        if (curTok != ')'){
            MatchError((Token) curTok);
            return nullptr;
        }
        getNextToken();
        return std::move(expr);
    }
    else if (curTok == tokenLetter) {
        auto prim = std::make_unique<PrimitiveAST>(m_Lexer.val);
        m_Abc.insert((char)m_Lexer.val);
        getNextToken();
        return std::move(prim);
    }
    else if (curTok == tokenBackreference) {
        auto back = std::make_unique<BackreferenceAST>(m_Lexer.numVal);
        getNextToken();
        return std::move(back);
    }

    else {
        ExpandError("R", (Token) curTok);
        return nullptr;
    }
}

int Parser::getParCnt() const {
    return parCnt;
}
