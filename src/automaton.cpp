//
// Created by osboxes on 4/24/20.
//

#include "automaton.h"

Automaton::Automaton() : m_InitialState(0), m_StateCnt(2), m_CurState(0) {
    m_FinalStates.insert(1);
}


Automaton::Automaton(const Automaton &automaton) {
    this->m_InitialState = automaton.m_InitialState;
    this->m_CurState = automaton.m_CurState;
    this->m_StateCnt = automaton.m_StateCnt;
    this->m_Input = automaton.m_Input;
    this->m_FinalStates = automaton.m_FinalStates;
}

int Automaton::getMInitialState() const {
    return m_InitialState;
}

void Automaton::setMInitialState(int mInitialState) {
    m_InitialState = mInitialState;
}

void Automaton::addMFinalState(int mFinalState) {
    m_FinalStates.insert(mFinalState);
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

const std::set<int> &Automaton::getMFinalStates() const {
    return m_FinalStates;
}

void Automaton::removeFinalStates() {
    this->m_FinalStates.erase(m_FinalStates.begin(), m_FinalStates.end());

}
