

#ifndef REGEX_MATCHER_TWOWAY_H
#define REGEX_MATCHER_TWOWAY_H


#include <set>
#include <vector>
#include <map>
#include <memory>
#include "tape.h"

/**
 * A comparator for a transition function represented with a map
 */
struct comp1 {
    bool operator() (const std::pair<int, std::string> &o1, const std::pair<int, std::string>& o2){
        if (o1.first == o2.first) return o1.second < o2.second;
        return o1.first < o2.first;
    }
};



/**
 * A class that simulates a two-way finite automaton
 */
class TWFA {

public:

    /**
     * A default constructor that creates a simulator of an two-way finite automaton with an initial state
     * and an empty transition function.
     */
    TWFA();

    /**
     * A copy constructor.
     * @param automaton a TWFA copy
     */
    TWFA(const TWFA & automaton);

    /**
     * A member function that initializes the input tape with an input string
     * @param t A pointer to the tape that contains an input string
     */
    void loadTape(std::shared_ptr<MultiHeadTape> t);

    /**
     * A member function that reads symbols which are located on each tape head
     * @return A string that contains read symbols
     */
    std::string readSymbols();

    /**
     * A member function that adds a transition to the transition function of the TWFA
     * @param state the present state
     * @param readSym the characters to be read from the tapes
     * @param newState the new state
     * @param moves shifts to be exectuted
     */
    void addTransition(int state, std::string readSym, int newState, std::vector<ShiftType> moves);

    /**
     * A member function that executes a transition updating the current state and the tape heads positions
     * @param state the new state
     * @param moves shifts to be exectuted
     */
    void execTransition(int state, std::vector<ShiftType> moves);


    /**
     * A member function that sets a final state of the TWFA
     * @param state a final state
     */
    void setFinalState(int state);


    int getFinalState();

    /**
     * A member function that simulates the TWFA computation
     * @return returns yes if the TWFA accepted the input string
     */
    bool accepts();

    /**
     * A member function that creates a replica of the TWFA
     * @return a copy of the TWFA
     */
    std::shared_ptr<TWFA> clone();

    void setInput(std::set<char> input);

    std::set<char> & getInput();

    int getMCur();


    /**
     * A member function that returns the tape
     * @return the input tape
     */
    std::shared_ptr<MultiHeadTape> getTape() const;

    void setMCur(int state);

    int getStateCnt();

    void incStateCnt();

    /**
     * A member function that prints the NDTM definition
     */
    void print();

private:
    /**
     * An initial state of an TWFA. The initial state equals 0 by default (q0).
     */
    int m_InitialState;

    /**
     * A final state (f ∈ F).
     */
    int m_FinalState;

    /**
     * A start symbol of an TWFA (>).
     */
    char m_StartSymbol;

    /**
    * An end symbol of an TWFA (<).
    */
    char m_EndSymbol;

    /**
     * A current state of an TWFA.
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
     * A read-only tape.
     */
    std::shared_ptr<MultiHeadTape> m_Tape;

    /**
     * A transition function Q x (I U {>, <})^k  -> 2^(Q x {L, N, R}^k).
     */
    std::map  <std::pair<int, std::string>,
      std::vector<std::pair<int, std::vector<ShiftType>>>, comp1> m_Transitions;

};

#endif //REGEX_MATCHER_TWOWAY_H
