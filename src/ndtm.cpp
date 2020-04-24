//
// Created by osboxes on 11/28/19.
//

#include <iostream>
#include "ndtm.h"

NDTM::NDTM() : m_InitialState(0), m_FinalState(1), m_StateCnt(2), m_Blank('B'), m_CurState(0){}


NDTM::NDTM(const NDTM & tm) {
    this->m_InitialState = tm.m_InitialState;
    this->m_Blank = this->m_Blank;
    this->m_CurState = tm.m_CurState;
    this->m_StateCnt = tm.m_StateCnt;
    this->m_Input = tm.m_Input;
    this->m_FinalState = tm.m_FinalState;
    this->m_Transitions = tm.m_Transitions;
    for (auto tape = tm.m_Tapes.begin(); tape != tm.m_Tapes.end(); tape++){
        this->m_Tapes.emplace_back((*tape)->clone());
    }
}

void NDTM::loadTapes(std::shared_ptr<OneHeadTape> t, int cnt) {
    m_CurState = m_InitialState;
    m_Tapes.erase(m_Tapes.begin(), m_Tapes.end());
    m_Tapes.emplace_back(t);

    for (int i = 0 ; i < cnt; i++){
        m_Tapes.push_back(std::make_shared<OneHeadTape>(t->getMCells().size(), 1));
    }
}

std::string NDTM::readSymbols(){
    std::string s;
    for (int i = 0; i < m_Tapes.size(); i++){
        s.push_back(m_Tapes[i]->readSymbol());
    }
    return s;
}

void NDTM::addTransition(int state, std::string readSym, int newState, tapesOperations moves) {
    m_Transitions[std::make_pair(state, readSym)].push_back(std::make_pair(newState, moves));
}

void NDTM::execTransition(std::pair<int, tapesOperations> trans) {
    m_CurState = trans.first;
    for (int i = 0; i < trans.second.size(); i++){
        if (trans.second[i].first != 'E' && trans.second[i].first != 'A') m_Tapes[i]->writeSymbol(trans.second[i].first);
        m_Tapes[i]->moveHead(trans.second[i].second);
    }
}

void NDTM::setInput(std::set<char> input) {
    m_Input = input;
}

std::set<char> &NDTM::getInput() {
    return m_Input;
}

void NDTM::setFinalState(int state) {
    m_FinalState = state;
}


bool NDTM::accepts() {
    std::vector<std::string> readSymbols;
    readSymbols.push_back(this->readSymbols());
    auto trans =  m_Transitions[std::make_pair(m_CurState, this->readSymbols())];
    for (int i = 0; i < trans.size(); i++){
        auto tm = this->clone();
        tm->execTransition(trans[i]);
        if (tm->accepts()) return true;
    }
    return m_Tapes[0]->isEmpty() && m_CurState == m_FinalState;
}

std::shared_ptr<NDTM> NDTM::clone() {
    return std::make_shared<NDTM>(*this);
}

int NDTM::getMCur() {
    return m_CurState;
}

std::shared_ptr<OneHeadTape> NDTM::getTape(int i)  {
    return m_Tapes[i];
}

void NDTM::setMCur(int state) {
    m_CurState = state;
}

int NDTM::getStateCnt() {
    return m_StateCnt;
}

void NDTM::incStateCnt() {
    m_StateCnt++;
}

char NDTM::getMBlank() {
    return m_Blank;
}

void NDTM::print() {
    std::cout << "\n\nTM = ({" << m_InitialState;

    for (int i = 1; i < m_StateCnt; i++) std::cout << "," << i;

    std::cout << "},{B";
    for (auto it = m_Input.begin(); it != m_Input.end(); it++) std::cout << "," << *it;

    std::cout << "},B,{" << *m_Input.begin();
    for (auto it = m_Input.begin(); it != m_Input.end(); it++) std::cout << "," << *it;

    std::cout << "},f,0,{" << m_FinalState << "})" << std::endl;

    std::cout << "Transition function f:" << std::endl;
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++){
        std::cout << "f("  << it->first.first;
        for (int i = 0; i < m_Transitions.begin()->first.second.size(); i++){
            if (it->first.second[i] == m_Blank) std::cout << ",B";
            else std::cout << "," << it->first.second[i];
        }
        std::cout << ") = {";
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++){
            std::cout << "\n\t("  << it1->first;
            for (int i = 0; i < m_Transitions.begin()->first.second.size(); i++){
                if (it1->second[i].first == m_Blank) std::cout << ",<B," << it1->second[i].second - 1 << ">";
                else std::cout << ",<" << it1->second[i].first << "," << it1->second[i].second - 1 << ">" ;
            }
            std::cout << ")";
        }
        std::cout << "\n}" << std::endl;
    }
}

int NDTM::getFinalState() {
    return m_FinalState;
}



