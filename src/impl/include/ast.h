#ifndef REGEX_MATCHER_AST_H
#define REGEX_MATCHER_AST_H

#include <memory>
#include "ndtm.h"
#include "avdfa.h"

class VarAST;

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
     * @param start An initial state for the node (q_{start})
     * @param end An accepting state for the node (q_{end})
     * @param vars A set of variables of the regex
     *
     */
    virtual void constructTM(NDTM *tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                             int end) = 0;

    /**
      * A virtual function that adds a transition to the automatom that accepts ref-language of a regex corresponding the type of the node
      * @param automaton A pointer to the automaton
      * @param avd a set of initial states for nodes which have a parent node of type Definition 
      * @param in An initial state for the node 
      * @param out An accepting state for the node 
      * @param underDefinition is true if the node has a parent node of type Definition
      *
      */
    virtual void
    constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition = false);

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

    void constructTM(NDTM *tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end);

    void
    constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition = false);

};

/**
 * A class for variable
 */
class VarAST : public NodeAST {
protected:
    /**
     * A variable name
     */
    int m_Var;


public:
    VarAST(int val) : m_Var(val) {}

    int getVar() const;

};


/**
 * A class for backreferences
 */
class BackRefAST : public VarAST {

public:
    BackRefAST(int val) : VarAST(val) {}

    void print();

    void constructTM(NDTM *tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end);

    void
    constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition = false);

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

    void constructTM(NDTM *tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end);

    void
    constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition = false);
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

    void constructTM(NDTM *tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end);

    void
    constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition = false);
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
            : m_Expr(std::move(expr)) {}

    void print();

    void constructTM(NDTM *tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end);

    void
    constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition = false);
};


/**
 * A class for definition
 */
class DefinitionAST : public VarAST {
    std::shared_ptr<NodeAST> m_Expr;


public:
    DefinitionAST(int var, std::shared_ptr<NodeAST> expr)
            : VarAST(var), m_Expr(std::move(expr)) {}

    void print();

    void constructTM(NDTM *tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                     int end);

    void
    constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition = false);

};


#endif //REGEX_MATCHER_AST_H
