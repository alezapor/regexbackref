//
// Created by osboxes on 11/29/19.
//

#include <iostream>
#include "ast.h"

void AtomAST::print() {
    std::cout << "{ Atom: " << (char) m_Val << " }";
}

void AtomAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::set<char> & vars, int start, int end) {
    std::vector<std::string> readSymbols;
    std::vector<tapesOperations> operations;
    std::set<char> input = tm->getInput();
    input.insert('B');
    if (tm->getInput().find(this->m_Val) != tm->getInput().end()) {
        std::string s0;
        tapesOperations s1;
        s0.push_back((char) this->m_Val);
        s1.emplace_back(std::make_pair(tm->getMBlank(), right)); // replace with a blank symbol and shift to the left
        readSymbols.push_back(s0);
        operations.push_back(s1);
    }
    else {      //eps-transition for the input tape
        epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }

    for (int i = 1; i <= tapes.size(); i++) { // transition for each tape from 1...|tapes|
        if (tm->getInput().find(this->m_Val) == tm->getInput().end() || !tapes[i-1]) { // eps-transition
            epsilonTransitionHelper(readSymbols, operations, noShift, input);
        }
          /*
           * write to the ith tape if it is open
           */
        else {
            for (int j = 0; j < readSymbols.size(); j++){
                readSymbols[j].push_back((char) tm->getMBlank());
                operations[j].emplace_back(std::make_pair(this->m_Val, right));
            }
        }
    }
    if (this->m_Val == '0') {
        int p = tm->getMStateCnt();
        tm->incStateCnt();
        for (int j = 0; j < readSymbols.size(); j++)
            if (readSymbols[j].size() == tapes.size()+1) tm->addTransition(start,readSymbols[j],p,operations[j]);
    }
    else {
        for (int j = 0; j < readSymbols.size(); j++)
            if (readSymbols[j].size() == tapes.size()+1) tm->addTransition(start,readSymbols[j],end,operations[j]);
    }
}

void VarAST::print() {
    std::cout << "{ Backref: " << (char) m_Name << " }";
}

void VarAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes,std::set<char> & vars, int start, int end) {
    int tapePos = std::distance(vars.begin(), vars.find(m_Name))+1;
    int p = tm->getMStateCnt();
    tm->incStateCnt();
    int q = tm->getMStateCnt();
    tm->incStateCnt();

    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols, readSymbols1, readSymbols2;
    std::vector<tapesOperations> operations, operations1, operations2;

    for (int i = 0; i <= tapes.size(); i++) {
        if (i==tapePos) epsilonTransitionHelper(readSymbols, operations, left, tm->getInput());
        else epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size()+1) tm->addTransition(p, readSymbols[j], p, operations[j]);
    }

    for (int i = 0; i <= tapes.size(); i++) {
        if (i==tapePos){
            for (int j = 0; j < readSymbols1.size(); j++){
                readSymbols1[j].push_back('B');
                operations1[j].emplace_back(std::make_pair('B', left));
            }
        }
        else epsilonTransitionHelper(readSymbols1, operations1, noShift, input);
    }
    for (int j = 0; j < readSymbols1.size(); j++) {
        if (readSymbols1[j].size() == tapes.size()+1) tm->addTransition(start, readSymbols1[j], p, operations1[j]);
    }

    for (int j = 0; j < readSymbols1.size(); j++) {
        operations1[j][tapePos].second = right;
        if (readSymbols1[j].size() == tapes.size()+1) tm->addTransition(p, readSymbols1[j], q, operations1[j]);
    }

    for (int j = 0; j < readSymbols1.size(); j++) {
        operations1[j][tapePos].second = noShift;
        if (readSymbols1[j].size() == tapes.size()+1) tm->addTransition(q, readSymbols1[j], end, operations1[j]);
    }

    for (auto it = tm->getInput().begin(); it != tm->getInput().end(); it++){ // <z, B, 1> - zero tape
           std::string s0 = "";
           tapesOperations s1;
           s0.push_back(*it);
           s1.emplace_back(std::make_pair('B', right));
           readSymbols2.push_back(s0);
           operations2.emplace_back(s1);
    }

    for (int i = 1; i <= tapes.size(); i++) {
        if (i==tapePos){
                for (int j = 0; j < readSymbols2.size(); j++){
                    readSymbols2[j].push_back(readSymbols2[j][0]);
                    operations2[j].emplace_back(std::make_pair(readSymbols2[j][0], right)); // <z, z, 1>
                }
        }
        else epsilonTransitionHelper(readSymbols2, operations2, noShift, tm->getInput());
    }
    for (int j = 0; j < readSymbols2.size(); j++) {
        if (readSymbols2[j].size() == tapes.size()+1) tm->addTransition(q, readSymbols2[j], q, operations2[j]);
    }

}

void UnionAST::print() {
    std::cout << "{ Union(+), left:";
    m_LHS->print();
    std::cout << ", right:";
    m_RHS->print();
    std::cout << " }";
}

void UnionAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::set<char> & vars, int start, int end) {
    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols;
    std::vector<tapesOperations> operations;
    for (int i = 0; i <= tapes.size(); i++) {
        epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    int startLeft = tm->getMStateCnt();
    tm->incStateCnt();
    int startRight= tm->getMStateCnt();
    tm->incStateCnt();
    int endLeft = tm->getMStateCnt();
    tm->incStateCnt();
    int endRight= tm->getMStateCnt();
    tm->incStateCnt();

    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size()+1) {
            tm->addTransition(start, readSymbols[j], startLeft, operations[j]);
            tm->addTransition(start, readSymbols[j], startRight, operations[j]);
            tm->addTransition(endLeft, readSymbols[j], end, operations[j]);
            tm->addTransition(endRight, readSymbols[j], end, operations[j]);
        }
    }
    m_LHS->constructTM(tm, tapes, vars, startLeft, endLeft);
    m_RHS->constructTM(tm, tapes, vars, startRight, endRight);

}

void ConcatenationAST::print() {
    std::cout << "{ Concatenation(.), left:";
    m_LHS->print();
    std::cout << "\n, right:";
    m_RHS->print();
    std::cout << " }";
}

void ConcatenationAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::set<char> & vars, int start, int end) {
    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols;
    std::vector<tapesOperations> operations;
    for (int i = 0; i <= tapes.size(); i++) {
        epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    int startLeft = tm->getMStateCnt();
    tm->incStateCnt();
    int startRight= tm->getMStateCnt();
    tm->incStateCnt();
    int mid= tm->getMStateCnt();
    tm->incStateCnt();
    int endLeft = tm->getMStateCnt();
    tm->incStateCnt();
    int endRight= tm->getMStateCnt();
    tm->incStateCnt();
    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size()+1) {
            tm->addTransition(start, readSymbols[j], startLeft, operations[j]);
            tm->addTransition(endLeft, readSymbols[j], mid, operations[j]);
            tm->addTransition(mid, readSymbols[j], startRight, operations[j]);
            tm->addTransition(endRight, readSymbols[j], end, operations[j]);
        }
    }

    m_LHS->constructTM(tm, tapes, vars, startLeft, endLeft);
    m_RHS->constructTM(tm, tapes, vars, startRight, endRight);
}

void IterationAST::print() {
    std::cout << "{ Iteration(*), expr:";
    m_Expr->print();
    std::cout << " }";
}

void IterationAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::set<char> & vars,  int start, int end) {
    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols;
    std::vector<tapesOperations> operations;
    for (int i = 0; i <= tapes.size(); i++) {
        epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    int startInner = tm->getMStateCnt();
    tm->incStateCnt();
    int endInner= tm->getMStateCnt();
    tm->incStateCnt();

    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size()+1) {
            tm->addTransition(start, readSymbols[j], startInner, operations[j]);
            tm->addTransition(start, readSymbols[j], end, operations[j]);
            tm->addTransition(endInner, readSymbols[j], end, operations[j]);
            tm->addTransition(endInner, readSymbols[j], startInner, operations[j]);
        }
    }

    m_Expr->constructTM(tm, tapes, vars, startInner, endInner);
}

void DefinitionAST::print() {
    std::cout << "{ Definition of " << (char) m_Var << ", expr:";
    m_Expr->print();
    std::cout << " }";
}

void DefinitionAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::set<char> & vars, int start, int end) {

    int tapePos = std::distance(vars.begin(), vars.find(m_Var))+1;
    int p = tm->getMStateCnt();
    tm->incStateCnt();
    int startInner = tm->getMStateCnt();
    tm->incStateCnt();
    int endInner= tm->getMStateCnt();
    tm->incStateCnt();

    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols, readSymbols1, readSymbols2;
    std::vector<tapesOperations> operations, operations1, operations2;
    for (int i = 0; i <= tapes.size(); i++) {
        epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size()+1) tm->addTransition(endInner, readSymbols[j], end, operations[j]);
    }


    for (int i = 0; i <= tapes.size(); i++) {
        if (i==tapePos){
            for (int j = 0; j < readSymbols1.size(); j++){
                readSymbols1[j].push_back('B');
                operations1[j].emplace_back(std::make_pair('B', left));
            }
        }
        else epsilonTransitionHelper(readSymbols1, operations1, noShift, input);
    }
    for (int j = 0; j < readSymbols1.size(); j++) {
        if (readSymbols1[j].size() == tapes.size()+1) tm->addTransition(start, readSymbols1[j], p, operations1[j]);
    }

    for (int j = 0; j < readSymbols1.size(); j++) {
        operations1[j][tapePos].second = right;
        if (readSymbols1[j].size() == tapes.size()+1) tm->addTransition(p, readSymbols1[j], startInner, operations1[j]);
    }

    for (int i = 0; i <= tapes.size(); i++) {
        if (i==tapePos) epsilonTransitionHelper(readSymbols2, operations2, left, tm->getInput(), 'B');
        else epsilonTransitionHelper(readSymbols2, operations2, noShift, input);
    }
    for (int j = 0; j < readSymbols2.size(); j++) {
        if (readSymbols2[j].size() == tapes.size()+1) tm->addTransition(p, readSymbols2[j], p, operations2[j]);
    }

    tapes[tapePos-1] = true;
    m_Expr->constructTM(tm, tapes, vars, startInner, endInner);
    tapes[tapePos-1] = false;
}

void NodeAST::epsilonTransitionHelper(std::vector<std::string> &readSymbols, std::vector<tapesOperations> &operations,
                                      ShiftType shiftType, const std::set<char> &alphabet, char insertSymbol) {
    if (!alphabet.empty()){
        if (readSymbols.empty()){
            std::string s = "";
            tapesOperations s1;
            readSymbols.push_back(s);
            operations.push_back(s1);
        }
        auto it = alphabet.begin();
        int size = readSymbols.size();
        for (int i = 0; i < alphabet.size()*size; i++){
            if (i >= size){
                std::string s = readSymbols[i%size];
                tapesOperations t = operations[i%size];
                s[s.size()-1] = *it;
                t[t.size()-1]  = std::make_pair((insertSymbol == 'E')?*it:insertSymbol, shiftType);
                readSymbols.push_back(s);
                operations.push_back(t);
            }
            else {
                readSymbols[i].push_back(*it);
                operations[i].push_back(std::make_pair((insertSymbol == 'E')?*it:insertSymbol, shiftType));
            }
            if (i % size == size - 1) it++;
        }
    }
}
