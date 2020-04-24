

#include "twoway.h"

#include <iostream>
#include "ndtm.h"

TWFA::TWFA() : m_InitialState(0), m_FinalState(1), m_StateCnt(2), m_StartSymbol('>'), m_EndSymbol('<'), m_CurState(0){}


TWFA::TWFA(const TWFA & automaton) {
    this->m_InitialState = automaton.m_InitialState;
    this->m_StartSymbol = automaton.m_StartSymbol;
    this->m_EndSymbol = automaton.m_EndSymbol;
    this->m_CurState = automaton.m_CurState;
    this->m_StateCnt = automaton.m_StateCnt;
    this->m_Input = automaton.m_Input;
    this->m_FinalState = automaton.m_FinalState;
    this->m_Transitions = automaton.m_Transitions;
    this->m_Tape = std::move((automaton.getTape())->clone());
}

void TWFA::loadTape(std::shared_ptr<MultiHeadTape> t) {
    m_CurState = m_InitialState;

    m_Tape = std::move(t);
}

std::string TWFA::readSymbols(){
    return this->m_Tape->readSymbols();
}

void TWFA::addTransition(int state, std::string readSym, int newState, std::vector<ShiftType> moves) {
    m_Transitions[std::make_pair(state, readSym)].push_back(std::make_pair(newState, moves));
}

void TWFA::execTransition(int state, std::vector<ShiftType>  moves) {
    m_CurState = state;
    m_Tape->moveHeads(moves);
}

void TWFA::setInput(std::set<char> input) {
    m_Input = input;
}

std::set<char> &TWFA::getInput() {
    return m_Input;
}

void TWFA::setFinalState(int state) {
    m_FinalState = state;
}


bool TWFA::accepts() {
    std::vector<std::string> readSymbols;
    readSymbols.push_back(this->readSymbols());
    auto trans =  m_Transitions[std::make_pair(m_CurState, this->readSymbols())];
    for (int i = 0; i < this->readSymbols().size(); i++){
        if (readSymbols[0][i] == '>' ||  readSymbols[0][i] == '<' || this->m_Input.find(readSymbols[0][i]) != this->m_Input.end()) {
            int size = readSymbols.size();
            for (int j = 0; j < size; j++) {
                std::string readString = readSymbols[j];
                readString[i] = 'E';
                readSymbols.emplace_back(readString);
                auto newTrans =  m_Transitions[std::make_pair(m_CurState, readString)];
                for (auto it = newTrans.begin(); it != newTrans.end(); it++){
                    trans.emplace_back(*it);
                }
            }
        }
        if (this->m_Input.find(readSymbols[0][i]) != this->m_Input.end()) {
            int size = readSymbols.size();
            for (int j = 0; j < size; j++) {
                std::string readString = readSymbols[j];
                readString[i] = 'A';
                readSymbols.emplace_back(readString);
                auto newTrans =  m_Transitions[std::make_pair(m_CurState, readString)];
                for (auto it = newTrans.begin(); it != newTrans.end(); it++){
                    trans.emplace_back(*it);
                }
            }
        }
    }
    for (int i = 0; i < trans.size(); i++){
        auto tm = this->clone();
        tm->execTransition(trans[i].first, trans[i].second);
        if (tm->accepts()) return true;
    }
    return m_CurState == m_FinalState;
}

std::shared_ptr<TWFA> TWFA::clone() {
    return std::make_shared<TWFA>(*this);
}

int TWFA::getMCur() {
    return m_CurState;
}

std::shared_ptr<MultiHeadTape> TWFA::getTape()  const{
    return m_Tape;
}

void TWFA::setMCur(int state) {
    m_CurState = state;
}

int TWFA::getStateCnt() {
    return m_StateCnt;
}

void TWFA::incStateCnt() {
    m_StateCnt++;
}


void TWFA::print() {
    std::cout << "\n\n2NKA = ({" << m_InitialState;

    for (int i = 1; i < m_StateCnt; i++) std::cout << "," << i;

    std::cout << "},{";
    for (auto it = m_Input.begin(); it != m_Input.end(); it++) std::cout << "," << *it;

    std::cout << "},>,<,f,0,{" << m_FinalState << "})" << std::endl;

    std::cout << "Transition function f:" << std::endl;
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++){
        std::cout << "f("  << it->first.first;
        for (int i = 0; i < m_Tape->getMHeads().size(); i++){
            std::cout << "," << it->first.second[i];
        }
        std::cout << ") = {";
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++){
            std::cout << "\n\t("  << it1->first;
            for (int i = 0; i < m_Tape->getMHeads().size(); i++){
                std::cout << ", " << it1->second[i] - 1 ;
            }
            std::cout << ")";
        }
        std::cout << "\n}" << std::endl;
    }
}

int TWFA::getFinalState() {
    return m_FinalState;
}



