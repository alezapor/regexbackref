
#ifndef REGEX_MATCHER_AVDFA_H
#define REGEX_MATCHER_AVDFA_H

#include <set>
#include <map>
#include <vector>
#include "tape.h"
#include "automaton.h"


struct comp1 {
    bool operator()(const std::pair<int, std::string> &o1, const std::pair<int, std::string> &o2) {
        if (o1.first == o2.first) return o1.second < o2.second;
        return o1.first < o2.first;
    }
};

/**
 * A class that represents a memory automaton
 * @tparam T
 */
template<class T=int>
class MemoryAutomaton : public Automaton<T> {
public:
    /**
     * A constructor for creating memory automaton with an empty transition function
     * @param start an initial state
     * @param end an accepting state
     * @param memSize a number of memories
     * @param vars a set of variables
     */
    MemoryAutomaton(T start, T end, int memSize, std::set<char> vars);

    /**
     * A copy constructor
     * @param automaton to copy
     */
    MemoryAutomaton(const MemoryAutomaton<T> &automaton);

    /**
     * A function that simulates a memory automaton computation from the current configuration
     * @return true if the memory automaton accepts from the current configuration
     */
    bool accepts();

    /**
     * A function that creates an initial configuration for a word
     * @param input a word to initialize with
     */
    void initialize(std::string input);

    /**
     * A function that simulates a transition
     * @param s the transition type
     * @param state the new state
     */
    void execTransition(std::string s, T state);

    /**
     * A function that adds a transition to the transition function
     * @param state the first state
     * @param readSym the transtion type
     * @param newState the last state
     */
    void addTransition(T state, std::string readSym, T newState);

    /**
     * A function that creates a deep copy of the memory automaton
     * @return a pointer to the created copy
     */
    std::shared_ptr<MemoryAutomaton> getClone();

    bool addState(T state);

    void print();

    /**
     * A function that checks configuration repeating to prevent cycles
     * @return true if the current configuration has already been visited in this branch
     */
    bool checkCycle();

    /**
     * A function that returns memory number for the variable
     * @param state used for avdMemory algorithm (checks memory in memory list)
     * @param x a variable
     * @return memory from memory list (for avdMemory), else return position in variable list (for simpleMemory)
     */
    int getMemory(T state, int x);

private:
    /**
     * An input tape
     */
    std::string m_Tape;
    /**
     * A set of automaton states
     */
    std::set<T> m_States;
    /**
     * A set of variables
     */
    std::set<char> m_Vars;

    /**
     * A position of the head
     */
    int m_Pos;
    /**
     * A vector of automata memories
     */
    std::vector<std::pair<bool, std::string>> m_Memory;
    /**
     * A transition function
     */
    std::shared_ptr<std::map<T, std::vector<std::pair<std::string, T>>>> m_Transitions;

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
    AvdFA(const AvdFA &t);


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

    /**
     * A function that creates a memory automaton from using simpleMemory algorithm
     * @param s
     * @return
     */
    MemoryAutomaton<int> *simpleMemory(std::set<char> s);

    /**
     * A function that creates a memory automaton from using avdMemory algorithm
     * @param s a set of variables
     * @param avd active variable degree
     * @return
     */
    MemoryAutomaton<MemoryState> *avdMemory(std::set<char> s, int avd);

protected:
    /**
     * A transition function
     */
    std::map<std::pair<int, std::string>,
            std::vector<int>, comp1> m_Transitions;

};


#endif //REGEX_MATCHER_AVDFA_H
