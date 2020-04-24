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
    virtual void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes, std::set<char> & vars, int start, int end){}
    /**
     * A function that helps generate transitions from epsilon transitions for i-th tape
     * @param readSymbols a vector of strings that contains read symbols for 0...i-1 tapes
     * @param operations a vector of operations for 0...i-1 tapes
     * @param shiftType a type of shift for ith tape
     * @param alphabet a reference to the alphabet
     * @param insertSymbol symbol to write on ith tape
     */
    void epsilonTransitionHelper(std::vector<std::string> & readSymbols, std::vector<tapesOperations> & operations,
            ShiftType shiftType, const std::set<char> & alphabet, char insertSymbol = 'E');
};

/**
 * A class for primitives (letters and numbers)
 */
class AtomAST : public NodeAST {
    /**
     * A value of a primitive
     */
    int m_Val;
public:
    AtomAST(int val) : m_Val(val) {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes, std::set<char> & vars, int start, int end);
};

/**
 * A class for backreferences
 */
class VarAST : public NodeAST {
    /**
     * A variable name
     */
    int m_Name;
public:
    VarAST(int val) : m_Name(val) {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes, std::set<char> & vars, int start, int end);
};

/**
 * A class for union
 */
class UnionAST : public NodeAST {
    /**
     * The left node
     */
    std::shared_ptr<NodeAST> m_LHS;
    /**
     * The right node
     */
    std::shared_ptr<NodeAST> m_RHS;
public:
    UnionAST(std::shared_ptr<NodeAST> LHS,
                  std::shared_ptr<NodeAST> RHS)
            : m_LHS(std::move(LHS)), m_RHS(std::move(RHS)) {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes, std::set<char> & vars, int start, int end);
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
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes, std::set<char> & vars, int start, int end);
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
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes, std::set<char> & vars, int start, int end);
};


/**
 * A class for definition
 */
class DefinitionAST : public NodeAST {
    std::shared_ptr<NodeAST> m_Expr;
    /**
    * A variable name
    */
    int m_Var;
public:
    DefinitionAST(int var, std::shared_ptr<NodeAST> expr)
            : m_Expr(std::move(expr)), m_Var(var) {}
    void print();
    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> & tapes, std::set<char> & vars, int start, int end);
};



#endif //REGEX_MATCHER_AST_H
