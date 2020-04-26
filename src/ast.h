//
// Created by osboxes on 11/27/19.
//

#ifndef REGEX_MATCHER_AST_H
#define REGEX_MATCHER_AST_H

#include <memory>
#include "ndtm.h"
#include "avdfa.h"

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
     * @param tapes A set of signals that indicate whether to write on the ith tape
     * @param start An initial state for a node (q_{start})
     * @param end An accepting state for a node (q_{end})
     * @param vars A set of variables of the regex
     *
     */
    virtual void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                             int end, bool withAvd = false) = 0;

    virtual void constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, NodeAST *>> &last,
                                std::vector<int> &avd, int in, int out, bool underDefinition = false);

    virtual int getVar() const { return 0; }

    void setLastRefDef(bool lastRefDef);

protected:
    bool lastRefDef;

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
    AtomAST(int val) : m_Val(val) { setLastRefDef(false); }

    void print();

    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end, bool withAvd = false);

    void constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, NodeAST *>> &last,
                        std::vector<int> &avd, int in, int out, bool underDefinition = false);

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
    VarAST(int val) : m_Name(val) { setLastRefDef(false); }

    void print();

    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end, bool withAvd = false);

    void constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, NodeAST *>> &last,
                        std::vector<int> &avd, int in, int out, bool underDefinition = false);

    int getVar() const;
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
            : m_LHS(std::move(LHS)), m_RHS(std::move(RHS)) { setLastRefDef(false); }

    void print();

    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end, bool withAvd = false);

    void constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, NodeAST *>> &last,
                        std::vector<int> &avd, int in, int out, bool underDefinition = false);
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
            : m_LHS(std::move(LHS)), m_RHS(std::move(RHS)) { setLastRefDef(false); }

    void print();

    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end, bool withAvd = false);

    void constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, NodeAST *>> &last,
                        std::vector<int> &avd, int in, int out, bool underDefinition = false);
};

/**
 * A class for iteration
 */
class IterationAST : public NodeAST {
    /**
     * The inner expression
     */
    std::shared_ptr<NodeAST> m_Expr;
public:
    IterationAST(std::shared_ptr<NodeAST> expr)
            : m_Expr(std::move(expr)) { setLastRefDef(false); }

    void print();

    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end, bool withAvd = false);

    void constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, NodeAST *>> &last,
                        std::vector<int> &avd, int in, int out, bool underDefinition = false);
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
            : m_Expr(std::move(expr)), m_Var(var) { setLastRefDef(false); }

    void print();

    void constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end, bool withAvd = false);

    void constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, NodeAST *>> &last,
                        std::vector<int> &avd, int in, int out, bool underDefinition = false);

    int getVar() const;
};


#endif //REGEX_MATCHER_AST_H
