//
// Created by osboxes on 11/28/19.
//

#include <iostream>
#include "ndtm.h"

NDTM::NDTM() : Automaton(), m_Blank('B') {}


NDTM::NDTM(const NDTM &tm) : Automaton(tm) {
    this->m_Blank = tm.m_Blank;
    this->m_Transitions = tm.m_Transitions;
    for (auto tape = tm.m_Tapes.begin(); tape != tm.m_Tapes.end(); tape++) {
        this->m_Tapes.emplace_back((*tape)->clone());
    }
    this->m_ConfigurationsMemory.resize(m_StateCnt);
    for (int i = 0; i < m_StateCnt; i++) {
        this->m_ConfigurationsMemory[i] = tm.m_ConfigurationsMemory[i];
    }
}

void NDTM::loadTapes(std::shared_ptr<Tape> t) {
    m_CurState = m_InitialState;
    std::vector<int> pos;
    m_ConfigurationsMemory.resize(m_StateCnt, std::make_pair("", pos));
    for (auto it = m_ConfigurationsMemory.begin(); it != m_ConfigurationsMemory.end(); it++) {
        it->first = "";
    }
    int size = m_Tapes.size();
    m_Tapes.erase(m_Tapes.begin(), m_Tapes.end());
    m_Tapes.emplace_back(t);

    for (int i = 1; i < size; i++) {
        m_Tapes.push_back(std::make_shared<Tape>(t->getMCells().size(), 1));
    }
}

std::string NDTM::readSymbols() {
    std::string s;
    for (int i = 0; i < m_Tapes.size(); i++) {
        s.push_back(m_Tapes[i]->readSymbol());
    }
    return s;
}

void NDTM::addTransition(int state, std::string readSym, int newState, tapesOperations moves) {
    m_Transitions[std::make_pair(state, readSym)].push_back(std::make_pair(newState, moves));
}

void NDTM::execTransition(std::pair<int, tapesOperations> trans) {
    m_CurState = trans.first;


    for (int i = 0; i < trans.second.size(); i++) {
        if (trans.second[i].first != 'E' && trans.second[i].first != 'A')
            m_Tapes[i]->writeSymbol(trans.second[i].first);
        m_Tapes[i]->moveHead(trans.second[i].second);
    }
}

bool NDTM::accepts() {
    auto trans = m_Transitions[std::make_pair(m_CurState, this->readSymbols())];
    if (this->checkCycle()) return false;
    for (int i = 0; i < trans.size(); i++) {
        auto tm = this->clone();
        tm->execTransition(trans[i]);
        if (tm->accepts()) return true;
    }
    return m_Tapes[0]->isEmpty() && m_FinalStates.find(m_CurState) != m_FinalStates.end();
}

std::shared_ptr<NDTM> NDTM::clone() {
    return std::make_shared<NDTM>(*this);
}

std::shared_ptr<Tape> NDTM::getTape(int i) {
    return m_Tapes[i];
}

char NDTM::getMBlank() {
    return m_Blank;
}

void NDTM::print() {
    std::cout << "\n\nTM = ({" << m_InitialState;

    for (int i = 1; i < m_StateCnt; i++) std::cout << "," << i;

    std::cout << "},{B";
    for (auto it = m_Input.begin(); it != m_Input.end(); it++) std::cout << "," << *it;

    std::cout << "},B,{";
    if (!m_Input.empty()) {
        std::cout << *m_Input.begin();
        for (auto it = ++m_Input.begin(); it != m_Input.end(); it++) std::cout << "," << *it;
    }

    std::cout << "},f,0,{" << *m_FinalStates.begin();
    for (auto it = ++m_FinalStates.begin(); it != m_FinalStates.end(); it++) std::cout << "," << *it;
    std::cout << "})" << std::endl;

    std::cout << "Transition function f:" << std::endl;
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++) {
        std::cout << "f(" << it->first.first;
        for (int i = 0; i < m_Transitions.begin()->first.second.size(); i++) {
            if (it->first.second[i] == m_Blank) std::cout << ",B";
            else std::cout << "," << it->first.second[i];
        }
        std::cout << ") = {";
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            std::cout << "\n\t(" << it1->first;
            for (int i = 0; i < m_Transitions.begin()->first.second.size(); i++) {
                if (it1->second[i].first == m_Blank) std::cout << ",<B," << it1->second[i].second - 1 << ">";
                else std::cout << ",<" << it1->second[i].first << "," << it1->second[i].second - 1 << ">";
            }
            std::cout << ")";
        }
        std::cout << "\n}" << std::endl;
    }
}

void NDTM::initialize(std::string input) {
    std::shared_ptr<Tape> tape = std::make_shared<Tape>("B" + input + "B", 1);
    this->loadTapes(std::move(tape));
}

bool NDTM::checkCycle() {
    std::string s = "";
    std::vector<int> pos;
    for (int i = 0; i < m_Tapes.size(); i++) {
        s += getTape(i)->getMCells();
        pos.push_back(getTape(i)->getMHead());
    }

    if (!m_ConfigurationsMemory[m_CurState].first.empty()) {
        bool hasCycle = true;
        for (int i = 0; i < m_Tapes.size(); i++) {
            if (m_ConfigurationsMemory[m_CurState].second[i] != getTape(i)->getMHead()) hasCycle = false;
        }
        if (m_ConfigurationsMemory[m_CurState].first == s && hasCycle) return true;
    }
    m_ConfigurationsMemory[m_CurState].first = s;
    m_ConfigurationsMemory[m_CurState].second = pos;
    return false;
}

void NDTM::epsilonTransitionHelper(std::vector<std::string> &readSymbols, std::vector<tapesOperations> &operations,
                                   ShiftType shiftType, const std::set<char> &alphabet, char insertSymbol) {
    if (!alphabet.empty()) {
        if (readSymbols.empty()) {
            std::string s = "";
            tapesOperations s1;
            readSymbols.push_back(s);
            operations.push_back(s1);
        }
        auto it = alphabet.begin();
        int size = readSymbols.size();
        for (int i = 0; i < alphabet.size() * size; i++) {
            if (i >= size) {
                std::string s = readSymbols[i % size];
                tapesOperations t = operations[i % size];
                s[s.size() - 1] = *it;
                t[t.size() - 1] = std::make_pair((insertSymbol == 'E') ? *it : insertSymbol, shiftType);
                readSymbols.push_back(s);
                operations.push_back(t);
            } else {
                readSymbols[i].push_back(*it);
                operations[i].push_back(std::make_pair((insertSymbol == 'E') ? *it : insertSymbol, shiftType));
            }
            if (i % size == size - 1) it++;
        }
    }
}


void NDTM::setMTapeCnt(int mTapeCnt) {
    m_Tapes.resize(mTapeCnt, std::make_shared<Tape>("BB"));
}



