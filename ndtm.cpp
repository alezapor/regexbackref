//
// Created by osboxes on 11/28/19.
//

#include <iostream>
#include "ndtm.h"

NDTM::NDTM(int mStart, int cnt) : m_Start(mStart) {
    m_Cnt = 1;
    m_Cur = m_Start;
    //m_Tapes.resize(cnt+1, Tape(1, 1));
}

void NDTM::loadTapes(Tape t, int cnt) {
    m_Cur = m_Start;
    m_Tapes.push_back(t);
    for (int i = 0 ; i < cnt; i++){
        m_Tapes.push_back(Tape(t.getMLength(), 0));
    }
}

std::string NDTM::readSymbols(){
    std::string s;
    for (int i = 0; i < m_Tapes.size(); i++){
        s.push_back(m_Tapes[i].readSymbol());
    }
    return s;
}

void NDTM::addTransition(int state, std::string readSym, int newState, std::vector<std::pair<char, int>> moves) {
    m_Transitions[std::make_pair(state, readSym)].push_back(std::make_pair(newState, moves));
}

void NDTM::execTransition(std::pair<int, std::vector<std::pair<char, int>>> trans) {
    m_Cur = trans.first;
    for (int i = 0; i < trans.second.size(); i++){
        m_Tapes[i].writeSymbol(trans.second[i].first);
        m_Tapes[i].moveHead(trans.second[i].second);
    }
}

bool NDTM::accepts() {
    //this->loadTapes(Tape(0, s));
    auto trans =  m_Transitions[std::make_pair(m_Cur, this->readSymbols())];
    for (int i = 0; i < trans.size(); i++){
        NDTM tm= this->clone();
        tm.execTransition(trans[i]);
        if (tm.accepts()) return true;
    }
    return m_Tapes[0].isEmpty() && m_End.find(m_Cur) != m_End.end(); //prijima pokud vstupni paska je prazdna a nachazime se v konc stavu
}

NDTM NDTM::clone() {
    return NDTM(m_Start, m_End, m_Cur, m_Tapes, m_Transitions, m_Cnt);
}

NDTM::NDTM(int mStart, const std::set<int> &mEnd, int mCur, std::vector<Tape> mTapes,
           const std::map<std::pair<int, std::string>, std::vector<std::pair<int, std::vector<std::pair<char, int>>>>, comp> &mTransitions, int cnt)
        : m_Start(mStart), m_End(mEnd), m_Cur(mCur), m_Tapes(mTapes), m_Transitions(mTransitions) { m_Cnt = cnt; }

int NDTM::getMCur() {
    return m_Cur;
}

Tape & NDTM::getTape(int i)  {
    return m_Tapes[i];
}

void NDTM::setMCur(int state) {
    m_Cur = state;
}

int NDTM::getStateCnt() {
    return m_Cnt;
}

void NDTM::print() {
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++){
        std::cout << "\nTransition: state: "  << it->first.first << ", readString:( ";
        for (int i = 0; i < m_Tapes.size(); i++){
            if (it->first.second[i] == '\0') std::cout << "B,";
            else std::cout << it->first.second[i] << ",";
        }
        std::cout << "), moves:" << std::endl;
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++){
            std::cout << "\tto state: "  << it1->first << ", writeString:( ";
            for (int i = 0; i < m_Tapes.size(); i++){
                if (it1->second[i].first == '\0') std::cout << "<B," << it1->second[i].second << ">,";
                else std::cout << "<" << it1->second[i].first << "," << it1->second[i].second << ">," ;
            }
            std::cout << ")" << std::endl;
        }
    }
}


