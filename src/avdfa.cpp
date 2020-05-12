//
// Created by osboxes on 4/25/20.
//

#include <deque>
#include <iostream>
#include <utility>
#include "avdfa.h"

AvdFA::AvdFA() : Automaton(){

}

AvdFA::AvdFA(const AvdFA &t) {
    m_Transitions=t.m_Transitions;
}

void AvdFA::addTransition(int state, std::string readSym, int newState) {
    m_Transitions[std::make_pair(state, readSym)].push_back(newState);
}

bool AvdFA::accepts() {
    std::map<int, std::vector<int>> simpleTransitions;
    for (auto it = m_Transitions.begin(); it!=m_Transitions.end(); it++){
        for (auto it1 = it->second.begin(); it1!=it->second.end(); it1++)
            simpleTransitions[it->first.first].push_back(*it1);
    }
    std::vector<bool> visited;
    visited.resize(m_StateCnt, false);
    std::deque<int> states;
    states.push_back(m_InitialState);
    visited[m_InitialState] = true;
    while(!states.empty()){
        int i = states.front();
        if (m_FinalStates.find(i) != m_FinalStates.end()) return true;
        states.pop_front();
        auto vec = simpleTransitions[i];
        for (auto it = vec.begin(); it != vec.end(); it++){
            if (!visited[*it]) {
                states.push_back(*it);
                visited[*it]=true;
            }
        }
    }
    return false;
}

std::shared_ptr<AvdFA> AvdFA::constructR0(int state, char var) {
    std::shared_ptr<AvdFA> R0 = std::make_shared<AvdFA>();
    R0->setMInitialState(m_InitialState);
    std::string s0 = "["; s0.push_back(var); // "[x"
    R0->setMStateCnt(m_StateCnt*2);
    R0->removeFinalStates();
    R0->addMFinalState(m_StateCnt+state);
    for (auto it = m_Transitions.begin(); it!=m_Transitions.end(); it++){
        std::vector<int> states = it->second;
        for (auto it1 = states.begin(); it1 != states.end(); it1++)  *it1 += m_StateCnt;
        if (it->first.second != s0) {
            R0->m_Transitions[std::make_pair(it->first.first, "E")] = it->second;
            R0->m_Transitions[std::make_pair(it->first.first+m_StateCnt, "E")] = states;
        }
        else R0->m_Transitions[std::make_pair(it->first.first, it->first.second)] = states;
    }
    return std::move(R0);
}

std::shared_ptr<AvdFA> AvdFA::constructR1(int state, char var) {
    std::shared_ptr<AvdFA> R1 = std::make_shared<AvdFA>();
    R1->setMInitialState(state);
    std::string s0 = "["; s0.push_back(var); // "[x"
    std::string s1 = ""; s1.push_back(var); // "x"
    R1->setMStateCnt(m_StateCnt*3);
    R1->removeFinalStates();
    for (auto it = m_FinalStates.begin(); it!=m_FinalStates.end(); it++) R1->addMFinalState(m_StateCnt+*it);
    for (auto it = m_Transitions.begin(); it!=m_Transitions.end(); it++){
        std::vector<int> states = it->second, states2 = it->second;
        for (auto it1 = states.begin(); it1 != states.end(); it1++)  *it1 += m_StateCnt;
        for (auto it1 = states2.begin(); it1 != states2.end(); it1++)  *it1 += 2*m_StateCnt;
        if (it->first.second == s0) {
            R1->m_Transitions[std::make_pair(it->first.first, it->first.second)] = states2;
        }
        else if (it->first.second == s1) {
            R1->m_Transitions[std::make_pair(it->first.first, it->first.second)] = states;
        }
        else {
            R1->m_Transitions[std::make_pair(it->first.first, "E")] = it->second;
            R1->m_Transitions[std::make_pair(it->first.first+m_StateCnt, "E")] = states;
        }
        R1->m_Transitions[std::make_pair(it->first.first+m_StateCnt, it->first.second)] = states;
    };
    return std::move(R1);
}

void AvdFA::print() {
    std::cout << "\n\nFA = ({" << m_InitialState;

    for (int i = 1; i < m_StateCnt; i++) std::cout << "," << i;

    std::cout << "},E,f," << m_InitialState <<",{" << *m_FinalStates.begin();
    for (auto it = ++m_FinalStates.begin(); it != m_FinalStates.end(); it++) std::cout << "," << *it;
    std::cout << "})" << std::endl;

    std::cout << "Transition function f:" << std::endl;
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++){
        std::cout << "f("  << it->first.first << ", " << it->first.second<<") = {";
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++){
            std::cout << "\n\t("  << *it1 << "), ";
        }
        std::cout << "\n}" << std::endl;
    }

}

const std::map<std::pair<int, std::string>, std::vector<int>, comp1> &AvdFA::getMTransitions() const {
    return m_Transitions;
}

MemoryAutomaton::MemoryAutomaton(const std::map<std::pair<int, std::string>,
        std::vector<int>, comp1> &trans, const Automaton & automaton) {
    m_CurState = automaton.getMCurState();
    m_InitialState = automaton.getMInitialState();
    m_StateCnt = automaton.getMStateCnt();
    m_FinalStates = automaton.getMFinalStates();
    m_Transitions.resize(m_StateCnt);
    for (auto it = trans.begin(); it!=trans.end();it++)
        for (auto it1 = it->second.begin();it1!=it->second.end(); it1++)
            m_Transitions[it->first.first].push_back(std::make_pair(it->first.second, *it1));
}

void MemoryAutomaton::initialize(std::string input) {
    m_Tape = input;
    m_Pos = 0;
    for (auto it = m_Memory.begin(); it != m_Memory.end(); it++){
        it->second.first=false;
        it->second.second="";
    }
    std::vector<std::string> s;
    s.resize(m_Memory.size(), "");
    m_ConfigurationsMemory.resize(m_StateCnt, std::make_pair(-1,s));
}

bool MemoryAutomaton::accepts() {
    if (m_CurState >= 0){
        if (this->checkCycle()) return false;
        auto trans = m_Transitions[m_CurState];
        for (auto it = trans.begin(); it!= trans.end(); it++){
            std::shared_ptr<MemoryAutomaton> automaton = this->getClone();
            automaton->execTransition(it->first, it->second);
            if (automaton->accepts()) return true;
        }
    }
    return m_Pos == m_Tape.size() && m_FinalStates.find(m_CurState) != m_FinalStates.end();
}

std::shared_ptr<MemoryAutomaton> MemoryAutomaton::getClone() {
    return std::make_shared<MemoryAutomaton>(*this);
}

MemoryAutomaton::MemoryAutomaton(const MemoryAutomaton &automaton) : Automaton(automaton){
    m_Tape=automaton.m_Tape;
    m_Transitions.resize(automaton.m_Transitions.size());

    for (int i = 0; i <automaton.m_Transitions.size(); i++){
        for (auto it = automaton.m_Transitions[i].begin(); it!= automaton.m_Transitions[i].end();it++) {
            m_Transitions[i].push_back(std::make_pair(it->first, it->second));
        }
    }
    m_Pos=automaton.m_Pos;
    for (auto it = automaton.m_Memory.begin(); it!= automaton.m_Memory.end();it++){
        m_Memory[it->first] = std::make_pair(it->second.first, it->second.second);
    }
    std::vector<std::string> vec;
    vec.resize(automaton.m_Memory.size(), "");
    m_ConfigurationsMemory.resize(automaton.m_ConfigurationsMemory.size());
    for (int i = 0; i < m_ConfigurationsMemory.size(); i++){
        m_ConfigurationsMemory[i].first = automaton.m_ConfigurationsMemory[i].first;
        for (int j = 0; j < automaton.m_Memory.size(); j++){
            m_ConfigurationsMemory[i].second[j] = automaton.m_ConfigurationsMemory[i].second[j];
        }
    }
}

void MemoryAutomaton::execTransition(std::string s, int state) {
    if (s=="?"){
        m_CurState=state;
    }
    else if (s=="0"){
        m_CurState=INT16_MIN;
    }
    else if(s[0]=='['){
        m_Memory[s[1]].first = true;
        m_Memory[s[1]].second = "";
        m_CurState=state;
    }
    else if (s[0]==']'){
        m_Memory[s[1]].first = false;
        m_CurState=state;
    }
    else if (isupper(s[0])){
        bool equal = true;
        if (m_Pos+m_Memory[s[0]].second.size() <= m_Tape.size() && !m_Memory[s[0]].first) {
            for (int i = 0; i < m_Memory[s[0]].second.size(); i++) {
                if (m_Memory[s[0]].second[i] != m_Tape[m_Pos + i]) {
                    equal = false;
                    break;
                }
            }
        }
        else equal = false;
        if (equal)
        {
            for (auto it = m_Memory.begin(); it != m_Memory.end(); it++){
                if (it->second.first) {
                    it->second.second = it->second.second+m_Memory[s[0]].second;
                }
            }
            m_Pos +=m_Memory[s[0]].second.size();
            m_CurState=state;
        }
        else m_CurState=INT16_MIN;
    }
    else if (islower(s[0]) && m_Pos < m_Tape.size() && s[0] == m_Tape[m_Pos]) {
        for (auto it = m_Memory.begin(); it != m_Memory.end(); it++){
            if (it->second.first) {
                it->second.second = it->second.second+s;
            }
        }
        m_Pos ++;
        m_CurState=state;
    }
    else m_CurState=INT16_MIN;
}

void MemoryAutomaton::print() {
    std::cout << "\n\nFA = ({" << m_InitialState;

    for (int i = 1; i < m_StateCnt; i++) std::cout << "," << i;

    std::cout << "},E,f," << m_InitialState <<",{" << *m_FinalStates.begin();
    for (auto it = ++m_FinalStates.begin(); it != m_FinalStates.end(); it++) std::cout << "," << *it;
    std::cout << "})" << std::endl;

    std::cout << "Transition function f:" << std::endl;
    for (int i = 0; i <m_Transitions.size(); i++){
        std::cout << i << " ->\n";
        for (auto it = m_Transitions[i].begin(); it!= m_Transitions[i].end();it++) {
            std::cout << "\t(" <<it->first << "," << it->second << ")\n";
        }
    }
    std::cout << std::endl;

}

bool MemoryAutomaton::checkCycle() {
    bool hasCycle = true;
    std::string s;
    for (int j = 0; j < m_ConfigurationsMemory[m_CurState].second.size(); j++){
        if (m_Memory[j].first) s = "O";
        else s= "C";
        if(m_ConfigurationsMemory[m_CurState].second[j] != s+m_Memory[j].second) hasCycle=false;
    }
    if (m_ConfigurationsMemory[m_CurState].first == m_Pos && hasCycle) return true;
    m_ConfigurationsMemory[m_CurState].first = m_Pos;
    for (int j = 0; j < m_ConfigurationsMemory[m_CurState].second.size(); j++){
            if (m_Memory[j].first) s = "O";
            else s= "C";
            m_ConfigurationsMemory[m_CurState].second[j] = s+m_Memory[j].second;
    }
    return false;
}
