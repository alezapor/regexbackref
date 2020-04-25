

#include "twoway.h"

#include <iostream>
#include "ndtm.h"
#include "ast.h"

TWFA::TWFA() : Automaton(), m_StartSymbol('>'), m_EndSymbol('<'){


}


TWFA::TWFA(const TWFA & automaton) : Automaton(automaton){
    this->m_StartSymbol = automaton.m_StartSymbol;
    this->m_EndSymbol = automaton.m_EndSymbol;
    this->m_Transitions = automaton.m_Transitions;
    this->m_Tape = std::move((automaton.getTape())->clone());
    this->m_ConfigurationsMemory.resize(m_StateCnt);
    for (int i = 0; i < m_StateCnt; i++){
        this->m_ConfigurationsMemory[i] = automaton.m_ConfigurationsMemory[i];
    }
}

void TWFA::loadTape(std::shared_ptr<MultiHeadTape> t) {
    m_CurState = m_InitialState;
    m_Tape = std::move(t);
    m_ConfigurationsMemory.resize(m_StateCnt);
    for (auto it = m_ConfigurationsMemory.begin(); it != m_ConfigurationsMemory.end(); it++){
        it->erase(it->begin(), it->end());
    }
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
    if (!m_ConfigurationsMemory[m_CurState].empty() && this->checkCycle()){
        return false;
    }
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
        for (int i = 0; i < m_VarSize*3+2; i++){
            std::cout << "," << it->first.second[i];
        }
        std::cout << ") = {";
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++){
            std::cout << "\n\t("  << it1->first;
            for (int i = 0; i < m_VarSize*3+2; i++){
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
    std::vector<ShiftType> trans;
    trans.push_back(right);
    for (int i = 1; i < m_VarSize*3+2; i++) {
        if (i%3==2) trans.push_back(right);
        else trans.push_back(noShift);
    }
    execTransition(m_CurState, trans);
}


void TWFA::epsilonTransitionHelper(std::vector<std::string> &readSymbols, std::vector<std::vector<ShiftType>> &operations,
                                        ShiftType shiftType, const std::set<char> &alphabet) {
    if (!alphabet.empty()){
        if (readSymbols.empty()){
            std::string s = "";
            std::vector<ShiftType> s1;
            readSymbols.push_back(s);
            operations.push_back(s1);
        }
        auto it = alphabet.begin();
        int size = readSymbols.size();
        for (int i = 0; i < alphabet.size()*size; i++){
            if (i >= size){
                std::string s = readSymbols[i%size];
                std::vector<ShiftType> t = operations[i%size];
                s[s.size()-1] = *it;
                t[t.size()-1]  = shiftType;
                readSymbols.push_back(s);
                operations.push_back(t);
            }
            else {
                readSymbols[i].push_back(*it);
                operations[i].push_back(shiftType);
            }
            if (i % size == size - 1) it++;
        }
    }
}

bool TWFA::checkCycle() {
    std::vector<int> pos;
    bool hasCycle = true;
    for (int i = 0; i < m_StateCnt; i++){
        pos.push_back(getTape()->getMHeads()[i]);
        if (m_ConfigurationsMemory[m_CurState][i] != getTape()->getMHeads()[i]) hasCycle = false;
    }
    if (m_ConfigurationsMemory[m_CurState] == pos && hasCycle) return true;
    m_ConfigurationsMemory[m_CurState] = pos;
    return false;
}

char TWFA::getMStartSymbol() const {
    return m_StartSymbol;
}

void TWFA::setMStartSymbol(char mStartSymbol) {
    m_StartSymbol = mStartSymbol;
}

char TWFA::getMEndSymbol() const {
    return m_EndSymbol;
}

void TWFA::setMEndSymbol(char mEndSymbol) {
    m_EndSymbol = mEndSymbol;
}

void TWFA::addAcceptingTransitions() {

    int p = this->m_StateCnt;
    this->incStateCnt();
    std::vector<std::string> s;
    std::string s1= "";
    s1.push_back(m_EndSymbol);
    s.push_back(s1);
    std::vector<std::vector<ShiftType>> operations;
    std::vector<ShiftType> shift;
    shift.push_back(noShift);
    operations.push_back(shift);
    std::set<char> input = m_Input;
    input.insert(m_EndSymbol);
    input.insert(m_StartSymbol);
    for (int i = 1; i < 3*m_VarSize+2; i++){
        epsilonTransitionHelper(s, operations, noShift, input);
    }
    for (int j = 0; j < s.size(); j++)
        if (s[j].size() == 3*m_VarSize+2) this->addTransition(p, s[j],this->getMFinalState(),operations[j]);
}



