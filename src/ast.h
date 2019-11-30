//
// Created by osboxes on 11/27/19.
//

#ifndef REGEX_MATCHER_AST_H
#define REGEX_MATCHER_AST_H

#include <memory>
#include "ndtm.h"

/**
 * A base class for all nodes
 */
class NodeAST {
public:
    virtual ~NodeAST() {}
    virtual void print() {}
    /**
     * A virtual function that adds a transition to the NDTM corresponding the type of the node
     * @param tm A pointer to the NDTM
     * @param tapes A set of singals that indicate whether to write on the ith tape
     */
    virtual void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes){}
    void addSimpleTransition(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes, int q, int p);

};

/**
 * A class for empty regex
 */
class EmptyAST : public NodeAST {
public:
    EmptyAST() {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes);
};

/**
 * A class for primitives (letters and numbers)
 */
class PrimitiveAST : public NodeAST {
    /**
     * A value of a primitive
     */
    int m_Val;
public:
    PrimitiveAST(int val) : m_Val(val) {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes);
};

/**
 * A class for backreferences
 */
class BackreferenceAST : public NodeAST {
    /**
     * An index of the paranthesis
     */
    int m_Num;
public:
    BackreferenceAST(int val) : m_Num(val) {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes);
};

/**
 * A class for alteration
 */
class AlterationAST : public NodeAST {
    /**
     * The left node
     */
    std::shared_ptr<NodeAST> m_LHS;
    /**
     * The right node
     */
    std::shared_ptr<NodeAST> m_RHS;
public:
    AlterationAST(std::shared_ptr<NodeAST> LHS,
                  std::shared_ptr<NodeAST> RHS)
            : m_LHS(std::move(LHS)), m_RHS(std::move(RHS)) {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes);
};

/**
 * A class for concatenation
 */
class ConcatenationAST : public NodeAST {
    /**
  * The left node
  */
    std::shared_ptr<NodeAST> m_LHS;
    /**
     * The right node
     */
    std::shared_ptr<NodeAST> m_RHS;
public:
    ConcatenationAST(std::shared_ptr<NodeAST> LHS,
                  std::shared_ptr<NodeAST> RHS)
            : m_LHS(std::move(LHS)), m_RHS(std::move(RHS)) {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes);
};

/**
 * A class for iteration
 */
class IterationAST : public NodeAST {
    std::shared_ptr<NodeAST> m_Expr;
public:
    IterationAST(std::shared_ptr<NodeAST> expr)
            : m_Expr(std::move(expr)) {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes);
};

/**
 * A class for paretheses
 */
class ParenthesesAST : public NodeAST {
    /**
     * An index of the parenthesis
     */
    int m_Num;
    /**
     * An expression inside
     */
    std::shared_ptr<NodeAST> m_Expr;
public:
    ParenthesesAST(int num, std::shared_ptr<NodeAST> expr)
            : m_Num(num), m_Expr(std::move(expr)) {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes);
};

#endif //REGEX_MATCHER_AST_H
