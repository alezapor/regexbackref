//
// Created by osboxes on 11/28/19.
//

#ifndef REGEX_MATCHER_TAPE_H
#define REGEX_MATCHER_TAPE_H

#include <string>
#include <memory>
#include <vector>

/**
 * An enumeration that describes shift direction
 */
enum ShiftType {
    left,
    noShift,
    right
};


/**
 * A class that simulates a tape of automaton
 */
class Tape {
public:
    /**
     * A constructor that creates an empty tape
     * @param mLength a length of the tape
     * @param mHead an initial position of the tape head
     * @param mBlank a blank symbol
     */
    Tape(int mLength, char mBlank = 'B');

    /**
     * A constructor that creates a tape which contains a string
     * @param s the string to initialize the tape with
     * @param mHead an initial position of the tape
     */
    Tape(std::string s="");

    /**
     * A copy constructor
     * @param tape a tape to copy
     */
    Tape(const Tape & tape);

    const std::string &getMCells() const;

    void setMCells(std::string mCells);

protected:
/**
 * A sequence of cells
 */
std::string m_Cells;
};


/**
 * A class that simulates a tape of a multitape NDTM
 */
class OneHeadTape : public Tape{
public:
    /**
  * A constructor that creates an empty tape with one head
  * @param mLength a length of the tape
  * @param mHead an initial position of the tape head
  * @param mBlank a blank symbol
  */
    OneHeadTape(int mLength, int mHead = 0, char mBlank = 'B');

    /**
     * A constructor that creates a tape which contains a string
     * @param s the string to initialize the tape with
     * @param mHead an initial position of the tape
     */
    OneHeadTape(std::string s, int mHead = 0);

    /**
    * A copy constructor
    * @param tape a tape to copy
    */
    OneHeadTape(const OneHeadTape & tape);

    int getMHead() const;

    void setMHead(int mHead);

    /**
     * A member function that the symbol scanned by the head
     * @return a symbol scanned by the head
     */
    char readSymbol() const;

    /**
     * A member function that replaces the symbol scanned by the head by the other one
     * @param c a symbol to replace with
     */
    void writeSymbol(char c);

    /**
     * A member function that moves the tape one position to the left, to the right, or no position
     * @param shiftType a type of shift operation
     */
    void moveHead(ShiftType shiftType);

    bool isEmpty(char blank = 'B');

    /**
     * A member function that creates a replica of the tape
     * @return a copy of the tape
     */
    std::shared_ptr<OneHeadTape> clone();

private:
    /**
    * A tape head location
    */
    int m_Head;

};

/**
 * A class that simulates a tape of a multitape NDTM
 */
class MultiHeadTape : public Tape{
public:
    /**
  * A constructor that creates an empty tape with N heads
  * @param mLength a length of the tape
  * @param mHeadNum a number of heads
  * @param mBlank a blank symbol
  */
    MultiHeadTape(int mLength, int mHeadNum = 1, char mBlank = 'B');

    /**
     * A constructor that creates a read-only tape which contains a string
     * @param s the string to initialize the tape with
     * @param mHeadNum a number of heads
     */
    MultiHeadTape(std::string s, int mHeadNum = 1);

    /**
    * A copy constructor
    * @param tape a tape to copy
    */
    MultiHeadTape(const MultiHeadTape & tape);

    std::vector<int> getMHeads() const;

    void setMHeads(std::vector<int> mHeads);

    /**
     * A member function that returns the symbols scanned by the heads
     * @return a string scanned by the heads
     */
    std::string readSymbols() const;


    /**
     * A member function that moves the tape heads
     * @param shiftTypes a vector of types of shift operation
     */
    void moveHeads(std::vector<ShiftType> shiftTypes);

    /**
     * A member function that creates a replica of the tape
     * @return a copy of the tape
     */
    std::shared_ptr<MultiHeadTape> clone();

private:
    /**
    * A tape head location
    */
    std::vector<int> m_Heads;

};


#endif //REGEX_MATCHER_TAPE_H
