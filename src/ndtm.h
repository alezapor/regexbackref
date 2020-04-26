//
// Created by osboxes on 11/28/19.
//

#ifndef REGEX_MATCHER_NDTM_H
#define REGEX_MATCHER_NDTM_H


#include <set>
#include <vector>
#include <map>
#include <memory>
#include "tape.h"
#include "automaton.h"

/**
 * A comparator for a transition function represented with a map
 */
struct comp {
    bool operator() (const std::pair<int, std::string> &o1, const std::pair<int, std::string>& o2){
        if (o1.first == o2.first) return o1.second < o2.second;
        return o1.first < o2.first;
    }
};

/**
 * A sequence of operations (symbols to write and shifts) to execute on tapes
 */
typedef std::vector<std::pair<char, ShiftType>> tapesOperations;


/**
 * A class that simulates a nondeterministic Turing machine
 */
class NDTM : public Automaton{

public:

    /**
     * A default constructor that creates a simulator of an NDTM with an initial state
     * and an empty transition function.
     */
    NDTM();

   /**
    * A copy constructor.
    * @param tm an NDTM to copy
    */
    NDTM(const NDTM & tm);

    /**
     * A member function that initializes the first tape with an input string
     * and other tapes with blank symbols
     * @param t A pointer to the tape that contains an input string
     */
    void loadTapes(std::shared_ptr<Tape> t);

    /**
     * A member function that reads symbols which are located on tape head of each tape
     * @return A string that contains read symbols
     */
    std::string readSymbols();

    /**
     * A member function that adds a transition to the transition function of the NDTM
     * @param state the present state
     * @param readSym the characters to be read from the tapes
     * @param newState the new state
     * @param moves the characters to be written on the tapes and shifts to be exectuted
     */
    void addTransition(int state, std::string readSym, int newState, tapesOperations moves);

    /**
     * A member function that executes a transition updating the current state and the tapes
     */
    void execTransition(std::pair<int, tapesOperations>);


    /**
     * A member function that simulates the NDTM computation
     * @return returns yes if the NDTM accepted the input string
     */
    bool accepts();

    /**
     * A member function that creates a replica of the NDTM
     * @return a copy of the NDTM
     */
    std::shared_ptr<NDTM> clone();

    char getMBlank();

    /**
     * A member function that returns the ith tape
     * @param i the index of the tape
     * @return the ith tape
     */
    std::shared_ptr<Tape> getTape(int i);

    bool checkCycle();

    /**
     * A member function that prints the NDTM definition
     */
    void print();

    /**
     * A member function that creates an initial configuration of the NDTM for input word
     */
    void initialize(std::string input);

    /**
    * A function that helps generate transitions from epsilon transitions for i-th tape
    * @param readSymbols a vector of strings that contains read symbols for 0...i-1 tapes
    * @param operations a vector of operations for 0...i-1 tapes
    * @param shiftType a type of shift for ith tape
    * @param alphabet a reference to the alphabet
    * @param insertSymbol symbol to write on ith tape
    */
    static void epsilonTransitionHelper(std::vector<std::string> & readSymbols, std::vector<tapesOperations> & operations,
                                        ShiftType shiftType, const std::set<char> & alphabet, char insertSymbol = 'E');
    int getMTapeCnt() const;

    void setMTapeCnt(int mTapeCnt);

private:

    /**
     * A blank symbol of an NDTM (B).
     */
    char m_Blank;

    /**
     * A memory of tapes states for each state -> used to avoid infinite cycle
     */
    std::vector<std::pair<std::string, std::vector<int>>> m_ConfigurationsMemory;

    /**
     * A vector of tapes.
     */
    std::vector<std::shared_ptr<Tape>> m_Tapes;

    /**
     * A transition function Q \ F  x (I U {B})^k -> 2^(Q x ((I U {B}) x {L, N, R})^k).
     */
    std::map  <std::pair<int, std::string>,
                          std::vector <std::pair <int, tapesOperations>>, comp> m_Transitions;

};


#endif //REGEX_MATCHER_NDTM_H
