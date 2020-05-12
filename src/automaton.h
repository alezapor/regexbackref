//
// Created by osboxes on 4/24/20.
//

#ifndef REGEX_MATCHER_AUTOMATON_H
#define REGEX_MATCHER_AUTOMATON_H


#include <string>
#include <set>
#include <vector>

template <class T=int>
class Automaton {
public:
    /**
    * A default constructor that creates a simulator of any automaton
    */
    Automaton(T start, T end);

    /**
     * A copy constructor.
     * @param automaton an automaton to copy
     */
    Automaton(const Automaton & automaton);

    virtual /**
     * A member function that initializes the the automaton to simulate for input string
     * @param input an input string
     */
    void initialize(std::string input){}

    T getMInitialState() const;

    void setMInitialState(T mInitialState);

    void addMFinalState(T mFinalState);

    const std::set<T> &getMFinalStates() const;

    T getMCurState() const;

    void setMCurState(T mCurState);

    int getMStateCnt() const;

    void setMStateCnt(int mStateCnt);

    void removeFinalStates();


    /**
     * A member function that simulates the automaton computation
     * @return returns yes if the automaton accepted the input string
     */
    virtual bool accepts() = 0;

    void setInput(std::set<char> input);

    std::set<char> & getInput();

    void incStateCnt();

    /**
     * A member function that prints the automaton definition
     */
    virtual void print() = 0;



protected:

    /**
    * An initial state of an automaton. The initial state equals 0 by default (q0).
    */
    T m_InitialState;

    /**
     * A set of final states (F)
     */
    std::set<T> m_FinalStates;

    /**
     * A current state of an automaton.
     */
    T m_CurState;

    /**
     * A state counter.
     */
    int m_StateCnt;

    /**
     * A set of input symbols (I).
     */
    std::set<char> m_Input;

};



#endif //REGEX_MATCHER_AUTOMATON_H
