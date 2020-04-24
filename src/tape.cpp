

#include "tape.h"

Tape::Tape(int mLength, char mBlank) {
    m_Cells.resize(mLength, mBlank);
}

OneHeadTape::OneHeadTape(int mLength, int mHead, char mBlank) : Tape(mLength, mBlank) {
    this->m_Head = mHead;
}

Tape::Tape(std::string s) : m_Cells(s){
}

OneHeadTape::OneHeadTape(std::string s, int mHead) : Tape(s){
    m_Head = mHead;
}

Tape::Tape(const Tape &tape) {
    this->m_Cells = tape.getMCells();
}

OneHeadTape::OneHeadTape(const OneHeadTape &tape) : Tape(tape) {
    this->m_Head = tape.getMHead();
}

const std::string &Tape::getMCells() const {
    return m_Cells;
}

void Tape::setMCells(std::string mCells) {
    m_Cells = mCells;
}

int OneHeadTape::getMHead() const {
    return m_Head;
}

void OneHeadTape::setMHead(int mHead) {
    m_Head = mHead;
}

char OneHeadTape::readSymbol() const {
    return m_Cells[m_Head];
}

void OneHeadTape::writeSymbol(char c) {
    m_Cells[m_Head] = c;
}

std::shared_ptr<OneHeadTape> OneHeadTape::clone() {
    return std::make_shared<OneHeadTape>(this->m_Cells, this->m_Head);
}

void OneHeadTape::moveHead(ShiftType shiftType) {
        if (shiftType == left) m_Head--;
        else if (shiftType == right) m_Head++;
}

bool OneHeadTape::isEmpty(char blank) {
    for (int i = 0; i < m_Cells.size(); i++){
        if (m_Cells[i] != blank) return false;
    }
    return true;
}

MultiHeadTape::MultiHeadTape(int mLength, int mHeadNum, char mBlank) : Tape(mLength, mBlank){
    m_Heads.resize(mHeadNum, 0);
}

MultiHeadTape::MultiHeadTape(std::string s, int mHeadNum) : Tape(s) {
    m_Heads.resize(mHeadNum, 0);
}

MultiHeadTape::MultiHeadTape(const MultiHeadTape &tape) : Tape(tape) {
    this->m_Heads = tape.getMHeads();
}

std::vector<int> MultiHeadTape::getMHeads() const {
    return m_Heads;
}

void MultiHeadTape::setMHeads(std::vector<int> mHeads) {
    m_Heads = mHeads;
}

std::string MultiHeadTape::readSymbols() const {
    std::string s0 = "";
    for (int i = 0; i < m_Heads.size(); i++){
        s0.push_back(m_Cells[m_Heads[i]]);
    }
    return s0;
}

std::shared_ptr<MultiHeadTape> MultiHeadTape::clone() {
    auto ptr = std::make_shared<MultiHeadTape>("", this->m_Heads.size());
    ptr->setMHeads(this->getMHeads());
    ptr->setMCells(this->getMCells());
    return std::move(ptr);
}

void MultiHeadTape::moveHeads(std::vector<ShiftType> shiftTypes) {
    for (int i = 0; i < m_Heads.size(); i++){
        if (shiftTypes[i] == left) m_Heads[i]--;
        if (shiftTypes[i] == right) m_Heads[i]++;
    }
}
