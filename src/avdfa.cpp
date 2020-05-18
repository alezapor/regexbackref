//
// Created by osboxes on 4/25/20.
//

#include <deque>
#include <iostream>
#include <utility>
#include <algorithm>
#include "avdfa.h"

AvdFA::AvdFA() : Automaton(0, 1) {

}

AvdFA::AvdFA(const AvdFA &t) : Automaton<int>(t) {
    m_Transitions = t.m_Transitions;
}

void AvdFA::addTransition(int state, std::string readSym, int newState) {
    m_Transitions[std::make_pair(state, readSym)].push_back(newState);
}

bool AvdFA::accepts() {
    std::map<int, std::vector<int>> simpleTransitions;
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++) {
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++)
            simpleTransitions[it->first.first].push_back(*it1);
    }
    std::vector<bool> visited;
    visited.resize(m_StateCnt, false);
    std::deque<int> states;
    states.push_back(m_InitialState);
    visited[m_InitialState] = true;
    while (!states.empty()) {
        int i = states.front();
        if (m_FinalStates.find(i) != m_FinalStates.end()) return true;
        states.pop_front();
        auto vec = simpleTransitions[i];
        for (auto it = vec.begin(); it != vec.end(); it++) {
            if (!visited[*it]) {
                states.push_back(*it);
                visited[*it] = true;
            }
        }
    }
    return false;
}

std::shared_ptr<AvdFA> AvdFA::constructR0(int state, char var) {
    std::shared_ptr<AvdFA> R0 = std::make_shared<AvdFA>();
    R0->setMInitialState(m_InitialState);
    std::string s0 = "[";
    s0.push_back(var); // "[x"
    R0->setMStateCnt(m_StateCnt * 2);
    R0->removeFinalStates();
    R0->addMFinalState(m_StateCnt + state);
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++) {
        std::vector<int> states = it->second;
        for (auto it1 = states.begin(); it1 != states.end(); it1++) *it1 += m_StateCnt;
        if (it->first.second != s0) {
            R0->m_Transitions[std::make_pair(it->first.first, "?")] = it->second;
            R0->m_Transitions[std::make_pair(it->first.first + m_StateCnt, "?")] = states;
        } else {
            R0->m_Transitions[std::make_pair(it->first.first, it->first.second)] = states;
            R0->m_Transitions[std::make_pair(it->first.first + m_StateCnt, it->first.second)] = states;
        }
    }
    return std::move(R0);
}

std::shared_ptr<AvdFA> AvdFA::constructR1(int state, char var) {
    std::shared_ptr<AvdFA> R1 = std::make_shared<AvdFA>();
    R1->setMInitialState(state);
    std::string s0 = "[";
    s0.push_back(var); // "[x"
    std::string s1 = "";
    s1.push_back(var); // "x"
    R1->setMStateCnt(m_StateCnt * 3);
    R1->removeFinalStates();
    for (auto it = m_FinalStates.begin(); it != m_FinalStates.end(); it++) R1->addMFinalState(m_StateCnt + *it);
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++) {
        std::vector<int> states = it->second, states2 = it->second;
        for (auto it1 = states.begin(); it1 != states.end(); it1++) *it1 += m_StateCnt;
        for (auto it1 = states2.begin(); it1 != states2.end(); it1++) *it1 += 2 * m_StateCnt;
        if (it->first.second == s0) {
            R1->m_Transitions[std::make_pair(it->first.first, it->first.second)] = states2;
        } else if (it->first.second == s1) {
            R1->m_Transitions[std::make_pair(it->first.first, it->first.second)] = states;
        } else {
            R1->m_Transitions[std::make_pair(it->first.first, "?")] = it->second;
            R1->m_Transitions[std::make_pair(it->first.first + m_StateCnt, "?")] = states;
        }
        R1->m_Transitions[std::make_pair(it->first.first + m_StateCnt, it->first.second)] = states;
    };
    return std::move(R1);
}

void AvdFA::print() {
    std::cout << "\n\nFA = ({" << m_InitialState;

    for (int i = 1; i < m_StateCnt; i++) std::cout << "," << i;

    std::cout << "},E,f," << m_InitialState << ",{" << *m_FinalStates.begin();
    for (auto it = ++m_FinalStates.begin(); it != m_FinalStates.end(); it++) std::cout << "," << *it;
    std::cout << "})" << std::endl;

    std::cout << "Transition function f:" << std::endl;
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++) {
        std::cout << "f(" << it->first.first << ", " << it->first.second << ") = {";
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            std::cout << "\n\t(" << *it1 << "), ";
        }
        std::cout << "\n}" << std::endl;
    }

}

const std::map<std::pair<int, std::string>, std::vector<int>, comp1> &AvdFA::getMTransitions() const {
    return m_Transitions;
}

MemoryAutomaton<int> *AvdFA::simpleMemory(std::set<char> m_Vars) {
    MemoryAutomaton<int> *mem = new MemoryAutomaton<int>(0, 1, m_Vars.size(), m_Vars);
    mem->setMInitialState(m_InitialState);
    for (auto it = m_FinalStates.begin(); it != m_FinalStates.end(); it++) {
        mem->addMFinalState(*it);
    }
    mem->setMStateCnt(m_StateCnt);
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++) {
        mem->addState(it->first.first);
    for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++) {
        mem->addTransition(it->first.first, it->first.second, *it1);
        mem->addState(*it1);
    }
}
    return mem;
}

MemoryAutomaton<MemoryState> *AvdFA::avdMemory(std::set<char> vars, int avd) {

    std::string memoryList;
    memoryList.resize(avd, '0');
    MemoryState start(0, memoryList);
    MemoryState end(1, memoryList);

    MemoryAutomaton<MemoryState> *mem = new MemoryAutomaton<MemoryState>(start, end, avd, vars);

    std::map<int, std::vector<std::pair<std::string, int>>> simpleTransitions;
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++) {
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++)
            simpleTransitions[it->first.first].push_back(std::make_pair(it->first.second, *it1));
    }
    std::map<MemoryState, bool> visited;
    std::deque<MemoryState> states;
    states.push_back(start);
    visited[start] = true;

    while (!states.empty()) {
        MemoryState i = states.front();
        if (m_FinalStates.find(i.getMNum()) != m_FinalStates.end()) mem->addMFinalState(i);
        states.pop_front();

        mem->addState(i);
        auto vec = simpleTransitions[i.getMNum()];
        for (auto it = vec.begin(); it != vec.end(); it++) {
            std::string memList = i.getMMemoryList();
            for (auto it1 = vars.begin(); it1!=vars.end(); it1++){
                if (!constructR1(it->second, *it1)->accepts()){
                    std::replace(memList.begin(), memList.end(), *it1, '0');
                }
            }
            if (it->first[0] == '[') {
                if (!constructR1(it->second, it->first[1])->accepts() || memList.find(it->first[1])!=memList.npos){
                    MemoryState n(it->second, memList);
                    mem->addTransition(i, it->first, n);
                    if (!visited[n]){
                        states.push_back(n);
                        visited[n]=true;
                    }
                }
                else {
                    for (int j = 0; j < memList.size(); j++){
                        if (memList[j]=='0'){
                            std::string memList1 = memList;
                            memList1[j] = it->first[1];
                            MemoryState n(it->second, memList1);
                            mem->addTransition(i, it->first, n);
                            if (!visited[n]){
                                states.push_back(n);
                                visited[n]=true;
                            }
                        }
                    }
                }
            }
            else {
                MemoryState n(it->second, memList);
                mem->addTransition(i, it->first, n);
                if (!visited[n]){
                    states.push_back(n);
                    visited[n]=true;
                }
            }
        }
    }

    return mem;
}


template<class T>
void MemoryAutomaton<T>::initialize(std::string input) {
    m_Tape = input;
    m_Pos = 0;
    for (auto it = m_Memory.begin(); it != m_Memory.end(); it++) {
        it->first = false;
        it->second = "";
    }
    std::vector<std::string> s;
    s.resize(m_Memory.size(), "");
    for (auto it = m_States.begin(); it != m_States.end(); it++)
        m_ConfigurationsMemory[*it] = std::make_pair(-1, s);
}

template<class T>
bool MemoryAutomaton<T>::accepts() {
    if (Automaton<T>::m_CurState >= 0) {
        if (this->checkCycle()) return false;
        auto trans = m_Transitions[Automaton<T>::m_CurState];
        for (auto it = trans.begin(); it != trans.end(); it++) {
            std::shared_ptr<MemoryAutomaton<T>> automaton = this->getClone();
            automaton->execTransition(it->first, it->second);
            if (automaton->accepts()) return true;
        }
    }
    return m_Pos == m_Tape.size() &&
           Automaton<T>::m_FinalStates.find(Automaton<T>::m_CurState) != Automaton<T>::m_FinalStates.end();
}

template<class T>
std::shared_ptr<MemoryAutomaton<T>> MemoryAutomaton<T>::getClone() {
    return std::make_shared<MemoryAutomaton<T>>(*this);
}

template<class T>
MemoryAutomaton<T>::MemoryAutomaton(const MemoryAutomaton<T> &automaton) : Automaton<T>(automaton) {
    m_Tape = automaton.m_Tape;
/*
    for (auto it0 = automaton.m_Transitions.begin(); it0 != automaton.m_Transitions.end(); it0++){
        for (auto it = it0->second.begin(); it!= it0->second.end();it++) {
            m_Transitions[it0->first].push_back(std::make_pair(it->first, it->second));
        }
    }*/
    m_Transitions = automaton.m_Transitions;
    m_Pos = automaton.m_Pos;
    m_Vars = automaton.m_Vars;
    m_Memory.resize(automaton.m_Memory.size());
    for (int i = 0; i < automaton.m_Memory.size(); i++) {
        m_Memory[i] = std::make_pair(automaton.m_Memory[i].first, automaton.m_Memory[i].second);
    }

    for (auto it0 = automaton.m_ConfigurationsMemory.begin(); it0 != automaton.m_ConfigurationsMemory.end(); it0++) {
        m_ConfigurationsMemory[it0->first].first = it0->second.first;
        m_ConfigurationsMemory[it0->first].second.resize(m_Memory.size(), "");
        for (int j = 0; j < automaton.m_Memory.size(); j++) {
            m_ConfigurationsMemory[it0->first].second[j] = it0->second.second[j];
        }
    }
}

template<class T>
void MemoryAutomaton<T>::execTransition(std::string s, T state) {
    if (s == "?") {
        Automaton<T>::m_CurState = state;
    } else if (s == "0") {
        Automaton<T>::m_CurState = INT16_MIN;
    } else if (s[0] == '[') {
        if (getMemory(state, s[1]) != -1) {
            m_Memory[getMemory(state, s[1])].first = true;
            m_Memory[getMemory(state, s[1])].second = "";
        }
        Automaton<T>::m_CurState = state;
    } else if (s[0] == ']') {
        if(getMemory(state, s[1]) != -1)
            m_Memory[getMemory(state, s[1])].first = false;
        Automaton<T>::m_CurState = state;
    } else if (isupper(s[0])) {
        if(getMemory(Automaton<T>::m_CurState, s[0]) != -1) {

            bool equal = true;
            if (m_Pos + m_Memory[getMemory(Automaton<T>::m_CurState, s[0])].second.size() <= m_Tape.size() &&
                !m_Memory[getMemory(Automaton<T>::m_CurState, s[0])].first) {
                for (int i = 0; i < m_Memory[getMemory(Automaton<T>::m_CurState, s[0])].second.size(); i++) {
                    if (m_Memory[getMemory(Automaton<T>::m_CurState, s[0])].second[i] != m_Tape[m_Pos + i]) {
                        equal = false;
                        break;
                    }
                }
            } else equal = false;
            if (equal) {
                for (int i = 0; i < m_Memory.size(); i++) {
                    if (m_Memory[i].first) {
                        m_Memory[i].second =
                                m_Memory[i].second + m_Memory[getMemory(Automaton<T>::m_CurState, s[0])].second;
                    }
                }
                m_Pos += m_Memory[getMemory(Automaton<T>::m_CurState, s[0])].second.size();
                Automaton<T>::m_CurState = state;
            } else Automaton<T>::m_CurState = INT16_MIN;
        }
        else Automaton<T>::m_CurState = state;
    } else if (islower(s[0]) && m_Pos < m_Tape.size() && s[0] == m_Tape[m_Pos]) {
        for (int i = 0; i < m_Memory.size(); i++) {
            if (m_Memory[i].first) {
                m_Memory[i].second = m_Memory[i].second + s;
            }
        }
        m_Pos++;
        Automaton<T>::m_CurState = state;
    } else Automaton<T>::m_CurState = INT16_MIN;
}

template<class T>
void MemoryAutomaton<T>::print() {
    std::cout << "\n\nMA = ({" << Automaton<T>::m_InitialState;

    for (auto it = ++m_States.begin(); it != m_States.end(); it++) std::cout << "," << *it;

    std::cout << "},E,f," << Automaton<T>::m_InitialState << ",{" << *Automaton<T>::m_FinalStates.begin();
    for (auto it = ++Automaton<T>::m_FinalStates.begin(); it != Automaton<T>::m_FinalStates.end(); it++)
        std::cout << "," << *it;
    std::cout << "})" << std::endl;

    std::cout << "Transition function f:" << std::endl;
    for (auto it = m_Transitions.begin(); it != m_Transitions.end(); it++) {
        std::cout << it->first << " ->\n";
        for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            std::cout << "\t(" << it1->first << "," << it1->second << ")\n";
        }
    }
    std::cout << std::endl;

}

template<class T>
bool MemoryAutomaton<T>::checkCycle() {
    bool hasCycle = true;
    std::string s;
    for (int i = 0; i < m_Memory.size(); i++) {
        if (m_Memory[i].first) s = "O";
        else s = "C";
        if (m_ConfigurationsMemory[Automaton<T>::m_CurState].second[i] != s + m_Memory[i].second) hasCycle = false;
    }
    if (m_ConfigurationsMemory[Automaton<T>::m_CurState].first == m_Pos && hasCycle) return true;
    m_ConfigurationsMemory[Automaton<T>::m_CurState].first = m_Pos;

    for (int i = 0; i < m_Memory.size(); i++) {
        if (m_Memory[i].first) s = "O";
        else s = "C";
        m_ConfigurationsMemory[Automaton<T>::m_CurState].second[i] = s + m_Memory[i].second;
    }
    return false;
}

template<class T>
MemoryAutomaton<T>::MemoryAutomaton(T start, T end, int memSize, std::set<char> vars) : Automaton<T>(start, end) {
    m_Memory.resize(memSize, std::make_pair(false, ""));
    m_Vars = vars;
    m_States.insert(start);
    m_States.insert(end);
}

template<class T>
void MemoryAutomaton<T>::addTransition(T state, std::string readSym, T newState) {
    m_Transitions[state].push_back(std::make_pair(readSym, newState));
}

template<class T>
int MemoryAutomaton<T>::getMemory(T state, int x) {
    return 0;
}

template<class T>
bool MemoryAutomaton<T>::addState(T state) {
    return m_States.insert(state).second;
}

template<>
int MemoryAutomaton<int>::getMemory(int state, int x) {
    return std::distance(m_Vars.begin(), m_Vars.find(x));
}

template<>
int MemoryAutomaton<MemoryState>::getMemory(MemoryState state, int x) {
    return state.getMemory(x);
}
