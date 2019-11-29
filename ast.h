//
// Created by osboxes on 11/27/19.
//

#ifndef REGEX_MATCHER_AST_H
#define REGEX_MATCHER_AST_H

#include <memory>
#include "ndtm.h"

// NodeAST - Base class for all nodes.
class NodeAST {
public:
    virtual ~NodeAST() {}
    virtual void print() {}
    virtual void constructTM(NDTM & tm, std::vector<bool> & tapes, int final = 0){}

};

class EmptyAST : public NodeAST {
public:
    EmptyAST() {}
    void print() {
        std::cout << "{ Empty }";
    }
    void constructTM(NDTM & tm, std::vector<bool> & tapes, int final = 0) {
        tm.addFinalState(tm.getMCur());
    }
};

class PrimitiveAST : public NodeAST {
    int m_Val;
public:
    PrimitiveAST(int val) : m_Val(val) {}
    void print() {
        std::cout << "{ Primitive: " << (char)m_Val << " }";
    }
    void constructTM(NDTM & tm, std::vector<bool> & tapes, int final = 0) {
        int q = tm.getMCur(), p;
        if (final == 0) {
            p = tm.getStateCnt();
            tm.incStateCnt();
        }
        else p = final;
        std::string s0 = "";
        s0.push_back((char)m_Val);
        std::vector<std::pair<char, int>> s1;
        s1.emplace_back(std::make_pair('\0', 1));
        for (int i = 0; i < tapes.size(); i++) {
            s0.push_back('\0');
            if (tapes[i]) s1.emplace_back(std::make_pair((char)m_Val, 1));
            else s1.emplace_back(std::make_pair('\0', 0));
        }
        tm.addTransition(q,s0,p,s1);
        tm.setMCur(p);
    }
};


class BackreferenceAST : public NodeAST {
    int m_Num;
public:
    BackreferenceAST(int val) : m_Num(val) {}
    void print() {
        std::cout << "{ Backref: " << m_Num << " }";
    }
    void constructTM(NDTM & tm, std::vector<bool> & tapes, int final = 0) {
        int qCur = tm.getMCur();
        int q1 = tm.getStateCnt();
        tm.incStateCnt();
        int q2;
        if (final != 0){
            q2 = tm.getStateCnt();
            tm.incStateCnt();
        }
        else q2 = final;
        //1
        for (char i = 'a'; i != 'z'; i++){
            std::string s0 = "" + i;
            std::vector<std::pair<char, int>> s1;
            s1.emplace_back(std::make_pair(i, 0));
            for (int i = 0; i < tapes.size(); i++) {
                s0.push_back('\0');
                if (i == m_Num - 1) {
                    s1.emplace_back(std::make_pair('\0', 0));
                }
                else s1.emplace_back(std::make_pair('\0', 0));
            }
           // TODO
        }

    }
};


class AlterationAST : public NodeAST {
    std::unique_ptr<NodeAST> m_LHS, m_RHS;
public:
    AlterationAST(std::unique_ptr<NodeAST> LHS,
                  std::unique_ptr<NodeAST> RHS)
            : m_LHS(std::move(LHS)), m_RHS(std::move(RHS)) {}
    void print() {
        std::cout << "{ Alteration(+), left:";
        m_LHS->print();
        std::cout << ", right:";
        m_RHS->print();
        std::cout << " }";
    }
    void constructTM(NDTM & tm, std::vector<bool> & tapes, int final = 0) {
        int start = tm.getMCur();
        m_LHS->constructTM(tm, tapes);
        tm.setMCur(start);
        m_RHS->constructTM(tm, tapes, final);
        int endRight = tm.getMCur();
    }
};

class ConcatenationAST : public NodeAST {
    std::unique_ptr<NodeAST> m_LHS, m_RHS;
public:
    ConcatenationAST(std::unique_ptr<NodeAST> LHS,
                  std::unique_ptr<NodeAST> RHS)
            : m_LHS(std::move(LHS)), m_RHS(std::move(RHS)) {}
    void print() {
        std::cout << "{ Concatenation(.), left:";
        m_LHS->print();
        std::cout << ", right:";
        m_RHS->print();
        std::cout << " }";
    }
    void constructTM(NDTM & tm, std::vector<bool> & tapes, int final = 0) {
        m_LHS->constructTM(tm, tapes);
        m_RHS->constructTM(tm, tapes, final);
    }
};

class IterationAST : public NodeAST {
    std::unique_ptr<NodeAST> m_Expr;
public:
    IterationAST(std::unique_ptr<NodeAST> expr)
            : m_Expr(std::move(expr)) {}
    void print() {
        std::cout << "{ Iteration(*), expr:";
        m_Expr->print();
        std::cout << " }";
    }
    void constructTM(NDTM & tm, std::vector<bool> & tapes, int final = 0) {
        int q = tm.getMCur();
        m_Expr->constructTM(tm, tapes, q);
        if (final != 0) {
            // prevod q -> final (nic nezapisovat, nic necist)
            for (char i = 'a'; i != 'z'; i++){
                std::string s0 = "" + i;
                std::vector<std::pair<char, int>> s1;
                s1.emplace_back(std::make_pair(i, 0));
                for (int i = 0; i < tapes.size(); i++) {
                    s0.push_back('\0');
                    s1.emplace_back(std::make_pair('\0', 0));
                }
                tm.addTransition(q,s0,final,s1);
            }
            std::string s0 = "\0";
            std::vector<std::pair<char, int>> s1;
            s1.emplace_back(std::make_pair('\0', 0));
            for (int i = 0; i < tapes.size(); i++) {
                s0.push_back('\0');
                s1.emplace_back(std::make_pair('\0', 0));
            }
            tm.addTransition(q,s0,final,s1);
            tm.setMCur(final);
        }
    }
};

class ParenthesesAST : public NodeAST {
    int m_Num;
    std::unique_ptr<NodeAST> m_Expr;
public:
    ParenthesesAST(int num, std::unique_ptr<NodeAST> expr)
            : m_Num(num), m_Expr(std::move(expr)) {}
    void print() {
        std::cout << "{ Parentheses: (" << m_Num << "), expr:";
        m_Expr->print();
        std::cout << " }";
    }
    void constructTM(NDTM & tm, std::vector<bool> & tapes, int final = 0) {
        tapes[m_Num-1] = true;
        m_Expr->constructTM(tm, tapes, final);
        tapes[m_Num-1] = false;
    }
};

#endif //REGEX_MATCHER_AST_H
