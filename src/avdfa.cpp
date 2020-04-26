//
// Created by osboxes on 4/25/20.
//

#include <deque>
#include <iostream>
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
    R0->print();
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
    R1->print();
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

