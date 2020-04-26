//
// Created by osboxes on 11/28/19.
//

#include "tape.h"

Tape::Tape(int mLength, int mHead, char mBlank) : m_Head(mHead) {
    m_Cells.resize(mLength, mBlank);
}

Tape::Tape(std::string s, int mHead) : m_Head(mHead), m_Cells(s) {
}

Tape::Tape(const Tape &tape) {
    this->m_Cells = tape.getMCells();
    this->m_Head = tape.getMHead();
}

const std::string &Tape::getMCells() const {
    return m_Cells;
}

void Tape::setMCells(std::string mCells) {
    m_Cells = mCells;
}

int Tape::getMHead() const {
    return m_Head;
}

void Tape::setMHead(int mHead) {
    m_Head = mHead;
}

char Tape::readSymbol() const {
    return m_Cells[m_Head];
}

void Tape::writeSymbol(char c) {
    m_Cells[m_Head] = c;
}

std::shared_ptr<Tape> Tape::clone() {
    return std::make_shared<Tape>(this->m_Cells, this->m_Head);
}

void Tape::moveHead(ShiftType shiftType) {
    if (shiftType == left) m_Head--;
    else if (shiftType == right) m_Head++;
}

bool Tape::isEmpty(char blank) {
    for (int i = 0; i < m_Cells.size(); i++) {
        if (m_Cells[i] != blank) return false;
    }
    return true;
}

