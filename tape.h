//
// Created by osboxes on 11/28/19.
//

#ifndef REGEX_MATCHER_TAPE_H
#define REGEX_MATCHER_TAPE_H

#include <string>

class Tape {
public:
    Tape(int mLength, int mHead);
    Tape(int mHead, std::string s);

    const std::string &getMCells() const;

    void setMCells(const std::string &mCells);

    int getMLength() const;

    void setMLength(int mLength);

    int getMHead() const;

    void setMHead(int mHead);

    char readSymbol() const;

    void writeSymbol(char c);

    void moveHead(int dir){
        if (dir == 1) m_Head++;
        else if (dir == -1) m_Head--;
    }

    bool isEmpty(){
        for (int i = 0; i < m_Cells.size(); i++){
            if (m_Cells[i] != '\0') return false;
        }
        return true;
    }

    Tape clone();
private:
    std::string m_Cells;
    int m_Length;
    int m_Head;
};


#endif //REGEX_MATCHER_TAPE_H
