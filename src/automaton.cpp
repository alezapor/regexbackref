//
// Created by osboxes on 4/24/20.
//

#include "automaton.h"

template <class T>
Automaton<T>::Automaton(T start, T end) : m_InitialState(start), m_StateCnt(2), m_CurState(start) {
    m_FinalStates.insert(end);
}


template <class T>
Automaton<T>::Automaton(const Automaton<T> &automaton) {
    this->m_InitialState = automaton.m_InitialState;
    this->m_CurState = automaton.m_CurState;
    this->m_StateCnt = automaton.m_StateCnt;
    this->m_Input = automaton.m_Input;
    this->m_FinalStates = automaton.m_FinalStates;
}

template <class T>
T Automaton<T>::getMInitialState() const {
    return m_InitialState;
}

template <class T>
void Automaton<T>::setMInitialState(T mInitialState) {
    m_InitialState = mInitialState;
}

template <class T>
void Automaton<T>::addMFinalState(T mFinalState) {
    m_FinalStates.insert(mFinalState);
}

template <class T>
T Automaton<T>::Automaton::getMCurState() const {
    return m_CurState;
}


template <class T>
void Automaton<T>::setMCurState(T mCurState) {
    m_CurState = mCurState;
}

template <class T>
int Automaton<T>::getMStateCnt() const {
    return m_StateCnt;
}

template <class T>
void Automaton<T>::setMStateCnt(int mStateCnt) {
    m_StateCnt = mStateCnt;
}

template <class T>
void Automaton<T>::setInput(std::set<char> input) {
    m_Input = input;
}

template <class T>
std::set<char> &Automaton<T>::getInput() {
    return m_Input;
}

template <class T>
void Automaton<T>::incStateCnt() {
    m_StateCnt++;
}

template <class T>
const std::set<T> &Automaton<T>::getMFinalStates() const {
    return m_FinalStates;
}

template <class T>
void Automaton<T>::removeFinalStates() {
    this->m_FinalStates.erase(m_FinalStates.begin(), m_FinalStates.end());

}
template class Automaton<int>;
template class Automaton<MemoryState>;
