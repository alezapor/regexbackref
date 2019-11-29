//
// Created by osboxes on 11/28/19.
//

#include "tape.h"

Tape::Tape(int mLength, int mHead) : m_Length(mLength), m_Head(mHead) {
    m_Cells.resize(m_Length, '\0');
    m_Head = 0;
}

const std::string &Tape::getMCells() const {
    return m_Cells;
}

void Tape::setMCells(const std::string &mCells) {
    m_Cells = mCells;
}

int Tape::getMLength() const {
    return m_Length;
}

void Tape::setMLength(int mLength) {
    m_Length = mLength;
}

int Tape::getMHead() const {
    return m_Head;
}

void Tape::setMHead(int mHead) {
    m_Head = mHead;
}

char Tape::readSymbol() const {
    if(m_Head < 0 || m_Head >= m_Length) return '\0';
    return m_Cells[m_Head];
}

void Tape::writeSymbol(char c) {
    m_Cells[m_Head] = c;
}

Tape Tape::clone() {
    return Tape(m_Head, m_Cells);
}

Tape::Tape(int mHead, std::string s) {
    m_Head = mHead;
    m_Cells = s;
    m_Length = s.size();
}

