//
// Created by osboxes on 4/24/20.
//

#ifndef REGEX_MATCHER_AUTOMATON_H
#define REGEX_MATCHER_AUTOMATON_H


#include <string>
#include <set>

class Automaton {
public:
    /**
    * A default constructor that creates a simulator of any automaton
    */
    Automaton();

    /**
     * A copy constructor.
     * @param automaton an automaton to copy
     */
    Automaton(const Automaton & automaton);

    /**
     * A member function that initializes the the automaton to simulate for input string
     * @param input an input string
     */
    virtual void initialize(std::string input) = 0;

    int getMInitialState() const;

    void setMInitialState(int mInitialState);

    int getMFinalState() const;

    void setMFinalState(int mFinalState);

    int getMCurState() const;

    void setMCurState(int mCurState);

    int getMStateCnt() const;

    void setMStateCnt(int mStateCnt);


    /**
     * A member function that simulates the automaton computation
     * @return returns yes if the automaton accepted the input string
     */
    virtual bool accepts() = 0;

    void setInput(std::set<char> input);

    std::set<char> & getInput();

    void incStateCnt();

    int getMVarSize() const;

    void setMVarSize(int mVarSize);

    /**
     * A member function that prints the automaton definition
     */
    virtual void print() = 0;



protected:

    /**
    * An initial state of an automaton. The initial state equals 0 by default (q0).
    */
    int m_InitialState;

    /**
     * A final state (f ∈ F). We use only automata with one final state.
     */
    int m_FinalState;

    /**
     * A current state of an automaton.
     */
    int m_CurState;

    /**
     * A state counter.
     */
    int m_StateCnt;

    /**
     * A set of input symbols (I).
     */
    std::set<char> m_Input;

    /**
    * Size of a viable set
    */
    int m_VarSize;


};



#endif //REGEX_MATCHER_AUTOMATON_H
