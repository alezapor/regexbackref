//
// Created by osboxes on 4/25/20.
//

#ifndef REGEX_MATCHER_AVDFA_H
#define REGEX_MATCHER_AVDFA_H

#include <set>
#include <map>
#include <vector>
#include "tape.h"
#include "automaton.h"


struct comp1 {
    bool operator() (const std::pair<int, std::string> &o1, const std::pair<int, std::string>& o2){
        if (o1.first == o2.first) return o1.second < o2.second;
        return o1.first < o2.first;
    }
};

template <class T=int>
class MemoryAutomaton : public Automaton<T> {
public:
    MemoryAutomaton(T start, T end, int memSize, std::set<char> vars);
    MemoryAutomaton(const MemoryAutomaton<T> & automaton);
    bool accepts();
    void initialize(std::string input);
    void execTransition(std::string s, T state);
    void addTransition(T state, std::string readSym, T newState);
    std::shared_ptr<MemoryAutomaton> getClone();
    bool addState(T state);
    void print();
    bool checkCycle();
    int getMemory(T state, int x);
private:
    std::string m_Tape;
    std::set<T> m_States;
    std::set<char> m_Vars;

    int m_Pos;
    std::vector<std::pair<bool, std::string>> m_Memory;
    std::map<T, std::vector<std::pair<std::string, T>>> m_Transitions;

    /**
     * A memory of tapes states for each state -> used to avoid infinite cycle
     */
    std::map<T, std::pair<int, std::vector<std::string>>> m_ConfigurationsMemory;
};

/**
 * An object of this class simulates a helping finite automaton used for commputing avd of a regex
 */

class AvdFA : public Automaton<int> {

public:

    /**
     * A default constructor that creates a simulator of a helping finite automaton with an initial state
     * and an empty transition function.
     */
    AvdFA();

    /**
     * A copy constructor.
     * @param t an automaton to copy
     */
    AvdFA(const AvdFA & t);


    /**
     * A member function that adds a transition to the transition function of the FA
     * @param state the present state
     * @param readSym the character to be read from the tape
     * @param newState the new state
     * @param shiftType a type of shift
     */
    void addTransition(int state, std::string readSym, int newState);


    /**
     * A member function that if FA accepts any word
     * @return returns yes iff L(FA) is not an empty set
     */
    bool accepts();

    /**
     * Memeber function that constructs an automaton
     * that accepts only refwords w with definitions of a variable x
     *   |w|([x) >= 1
     * @param state a new final state
     * @param var a variable x
     */
    std::shared_ptr<AvdFA> constructR0(int state, char var);

    /**
    * Memeber function that constructs an automaton
    * that accepts only refwords wxu with no definitions of x before a reference
    *   |w|([x) = 0 and
    * @param state a new initial state
    * @param var a variable x
    */
    std::shared_ptr<AvdFA> constructR1(int state, char var);

    /**
     * A member function that prints the avdFA definition
     */
    void print();

    const std::map<std::pair<int, std::string>, std::vector<int>, comp1> &getMTransitions() const;

    MemoryAutomaton<int>* simpleMemory (std::set<char> s);

    MemoryAutomaton<MemoryState>* avdMemory (std::set<char> s, int avd);

protected:
    /**
     * A transition function
     */
    std::map  <std::pair<int, std::string>,
            std::vector<int>, comp1> m_Transitions;

};


#endif //REGEX_MATCHER_AVDFA_H
