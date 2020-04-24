

#include "twoway.h"

#include <iostream>
#include "ndtm.h"

TWFA::TWFA() : Automaton(), m_StartSymbol('>'), m_EndSymbol('<'){}


TWFA::TWFA(const TWFA & automaton) : Automaton(automaton){
    this->m_StartSymbol = automaton.m_StartSymbol;
    this->m_EndSymbol = automaton.m_EndSymbol;
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

std::shared_ptr<MultiHeadTape> TWFA::getTape()  const{
    return m_Tape;
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

void TWFA::initialize(std::string input) {
    std::shared_ptr<MultiHeadTape> tape = std::make_shared<MultiHeadTape>(">"+input+"<", m_VarSize*3+2);
    this->loadTape(std::move(tape));
}



