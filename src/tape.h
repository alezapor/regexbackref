//
// Created by osboxes on 11/28/19.
//

#ifndef REGEX_MATCHER_TAPE_H
#define REGEX_MATCHER_TAPE_H

#include <string>
#include <memory>

/**
 * An enumeration that describes shift direction
 */
enum ShiftType {
    left,
    noShift,
    right
};


/**
 * A class that simulates a tape of an NDTM
 */
class Tape {
public:
    /**
     * A constructor that creates an empty tape
     * @param mLength a length of the tape
     * @param mHead an initial position of the tape head
     * @param mBlank a blank symbol
     */
    explicit Tape(int mLength, int mHead = 0, char mBlank = 'B');

    /**
     * A constructor that creates a tape which contains a string
     * @param s the string to initialize the tape with
     * @param mHead an initial position of the tape
     */
    Tape(std::string s, int mHead = 0);

    /**
     * A copy constructor
     * @param tape a tape to copy
     */
    Tape(const Tape &tape);

    const std::string &getMCells() const;

    void setMCells(std::string mCells);

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
    std::shared_ptr<Tape> clone();

private:
    /**
     * A sequence of cells
     */
    std::string m_Cells;

    /**
     * A tape head location
     */
    int m_Head;
};


#endif //REGEX_MATCHER_TAPE_H
