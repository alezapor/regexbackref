//
// Created by osboxes on 4/24/20.
//

#include "automaton.h"

Automaton::Automaton() : m_InitialState(0), m_FinalState(1), m_StateCnt(2), m_CurState(0){}


Automaton::Automaton(const Automaton & automaton) {
    this->m_InitialState = automaton.m_InitialState;
    this->m_CurState = automaton.m_CurState;
    this->m_StateCnt = automaton.m_StateCnt;
    this->m_Input = automaton.m_Input;
    this->m_FinalState = automaton.m_FinalState;
}

int Automaton::getMInitialState() const {
    return m_InitialState;
}

void Automaton::setMInitialState(int mInitialState) {
    m_InitialState = mInitialState;
}

int Automaton::getMFinalState() const {
    return m_FinalState;
}

void Automaton::setMFinalState(int mFinalState) {
    m_FinalState = mFinalState;
}

int Automaton::getMCurState() const {
    return m_CurState;
}

void Automaton::setMCurState(int mCurState) {
    m_CurState = mCurState;
}

int Automaton::getMStateCnt() const {
    return m_StateCnt;
}

void Automaton::setMStateCnt(int mStateCnt) {
    m_StateCnt = mStateCnt;
}

void Automaton::setInput(std::set<char> input) {
    m_Input = input;
}

std::set<char> &Automaton::getInput() {
    return m_Input;
}

void Automaton::incStateCnt() {
    m_StateCnt++;
}

int Automaton::getMVarSize() const {
    return m_VarSize;
}

void Automaton::setMVarSize(int mVarSize) {
    m_VarSize = mVarSize;
}
