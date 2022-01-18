
#include "parser.h"

Parser::Parser(const char* regex) : m_Lexer(std::make_unique<Lexer>(regex)) {

}

void Parser::MatchError(Token token) {
    printf("Match error: token %d.\n", token);
    exit(1);
}

void Parser::ExpandError(std::string nonTerminal, Token token) {
    std::cout << "Expand error: nonterminal " << nonTerminal << ", token: " << token << std::endl;
    exit(1);
}

// A -> B A'
std::unique_ptr<NodeAST> Parser::ParseA() {
    if (m_CurTok == tokenAtom ||
        m_CurTok == '(' || m_CurTok == tokenVar) {
        return ParseARest(ParseB());
    } else {
        ExpandError("A", (Token) m_CurTok);
        return nullptr;
    }
}

// A' -> + A | eps
std::unique_ptr<NodeAST> Parser::ParseARest(std::unique_ptr<NodeAST> left) {
    if (m_CurTok == tokenUnion) {
        getNextToken();
        auto right = ParseA();
        return std::make_unique<UnionAST>(std::move(left), std::move(right));
    } else if (m_CurTok == tokenEOF || m_CurTok == ')' || m_CurTok == '}') {
        return left;
    } else {
        ExpandError("A'", (Token) m_CurTok);
        return nullptr;
    }
}

// B -> C B'
std::unique_ptr<NodeAST> Parser::ParseB() {
    if (m_CurTok == tokenAtom ||
        m_CurTok == '(' || m_CurTok == tokenVar) {
        return ParseBRest(ParseC());
    } else {
        ExpandError("B", (Token) m_CurTok);
        return nullptr;
    }
}

// B' -> B | eps
std::unique_ptr<NodeAST> Parser::ParseBRest(std::unique_ptr<NodeAST> left) {
    if (m_CurTok == tokenAtom ||
        m_CurTok == '(' || m_CurTok == tokenVar) {
        auto right = ParseB();
        return std::make_unique<ConcatenationAST>(std::move(left), std::move(right));
    } else if (m_CurTok == tokenUnion || m_CurTok == ')' || m_CurTok == '}' ||
               m_CurTok == tokenEOF) {
        return left;
    } else {
        ExpandError("B'", (Token) m_CurTok);
        return nullptr;
    }
}

// C -> D C'
std::unique_ptr<NodeAST> Parser::ParseC() {
    if (m_CurTok == tokenAtom ||
        m_CurTok == '(' || m_CurTok == tokenVar) {
        return ParseCRest(ParseD());
    } else {
        ExpandError("C", (Token) m_CurTok);
        return nullptr;
    }
}

// C' -> * | eps
std::unique_ptr<NodeAST> Parser::ParseCRest(std::unique_ptr<NodeAST> left) {
    if (m_CurTok == tokenIter) {
        getNextToken();
        return std::make_unique<IterationAST>(std::move(left));
    } else if (m_CurTok == tokenUnion || m_CurTok == ')' || m_CurTok == '}' || m_CurTok == tokenVar ||
               m_CurTok == tokenEOF || m_CurTok == '(' || m_CurTok == tokenAtom) {
        return left;
    } else {
        ExpandError("C'", (Token) m_CurTok);
        return nullptr;
    }
}

// D -> atom | var F' | ( A )
std::unique_ptr<NodeAST> Parser::ParseD() {
    if (m_CurTok == '(') {
        getNextToken();
        auto expr = ParseA();
        if (m_CurTok != ')') {
            MatchError((Token) m_CurTok);
            return nullptr;
        }
        getNextToken();
        return expr;
    } else if (m_CurTok == tokenAtom) {

        if (m_Lexer->val != '0' && m_Lexer->val != '?') {
            m_Input.insert(m_Lexer->val);
        }
        auto prim = std::make_unique<AtomAST>(m_Lexer->val);
        getNextToken();
        return prim;
    } else if (m_CurTok == tokenVar) {
        int val = m_Lexer->val;
        m_Vars.insert(m_Lexer->val);
        getNextToken();
        return ParseFRest(val);
    } else {
        ExpandError("D", (Token) m_CurTok);
        return nullptr;
    }
}

// F' -> { A } | eps
std::unique_ptr<NodeAST> Parser::ParseFRest(int var) {
    if (m_CurTok == '{') {
        getNextToken();
        auto expr = ParseA();
        if (m_CurTok != '}') {
            MatchError((Token) m_CurTok);
            return nullptr;
        }
        getNextToken();
        return std::make_unique<DefinitionAST>(var, std::move(expr));
    } else if (m_CurTok == tokenUnion || m_CurTok == ')' || m_CurTok == '}' || m_CurTok == tokenVar ||
               m_CurTok == tokenEOF || m_CurTok == '(' || m_CurTok == tokenAtom || m_CurTok == tokenIter) {
        return std::make_unique<BackRefAST>(var);
    } else {
        ExpandError("C'", (Token) m_CurTok);
        return nullptr;
    }
}

int Parser::getNextToken() {
    return m_CurTok = m_Lexer->getTok();

}

std::set<char> &Parser::getInput() {
    return m_Input;
}

std::set<char> &Parser::getVars() {
    return m_Vars;
}

