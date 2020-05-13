//
// Created by osboxes on 5/13/20.
//

#include "memorystate.h"

MemoryState::MemoryState(int num, std::string list) : m_Num(num), m_MemoryList(list) {
}

std::ostream &operator<<(std::ostream &os, const MemoryState &state) {
    os << "(" << state.m_Num << ", <";
    for (auto it = state.m_MemoryList.begin(); it != state.m_MemoryList.end(); it++)
        os << ((*it == '0') ? '-' : *it) << ", ";
    os << ">)";
    return os;
}

bool MemoryState::operator==(const MemoryState &rhs) const {
    if (m_Num != rhs.m_Num) return false;
    return m_MemoryList==rhs.m_MemoryList;
}

bool MemoryState::operator!=(const MemoryState &rhs) const {
    return !(rhs == *this);
}

bool MemoryState::operator<(const MemoryState &rhs) const {
    if (m_Num < rhs.m_Num)
        return true;
    else if (m_Num > rhs.m_Num)
        return false;
    else return m_MemoryList < rhs.m_MemoryList;
}

bool MemoryState::operator>(const MemoryState &rhs) const {
    return rhs < *this;
}

bool MemoryState::operator<=(const MemoryState &rhs) const {
    return !(rhs < *this);
}

bool MemoryState::operator>=(const MemoryState &rhs) const {
    return !(*this < rhs);
}

int MemoryState::getMemory(char x) {
    for (int i = 0; i < m_MemoryList.size(); i++){
        if (m_MemoryList[i] == x) return i;
    }
    return -1;
}

MemoryState &MemoryState::operator=(int state) {
    m_Num = state;
    return *this;
}

bool MemoryState::operator>=(int state) const {
    return m_Num >= state;
}

MemoryState::MemoryState() : m_Num(0) {

}

MemoryState &MemoryState::operator=(const MemoryState &rhs) {
    this->m_Num = rhs.m_Num;
    this->m_MemoryList = rhs.getMMemoryList();
    return *this;
}

int MemoryState::getMNum() const {
    return m_Num;
}

void MemoryState::setMNum(int mNum) {
    m_Num = mNum;
}

std::string MemoryState::getMMemoryList() const {
    return m_MemoryList;
}

void MemoryState::setMMemoryList(std::string mMemoryList) {
    m_MemoryList = mMemoryList;
}
