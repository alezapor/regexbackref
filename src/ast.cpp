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
        NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }

    for (int i = 1; i <= tapes.size(); i++) { // transition for each tape from 1...|tapes|
        if (tm->getInput().find(this->m_Val) == tm->getInput().end() || !tapes[i-1]) { // eps-transition
            NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
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

void AtomAST::constructTW(std::shared_ptr<TWFA> automaton, std::vector<bool> &tapes, std::set<char> & vars, int start, int end) {
    std::vector<std::string> readSymbols;
    std::vector<std::vector<ShiftType>> operations;
    std::set<char> input = automaton->getInput();
    input.insert(automaton->getMStartSymbol());
    input.insert(automaton->getMEndSymbol());
    if (automaton->getInput().find(this->m_Val) != automaton->getInput().end()) {
        std::string s0;
        std::vector<ShiftType> s1;
        s0.push_back((char) this->m_Val);
        s1.push_back(right); // replace with a blank symbol and shift to the left
        readSymbols.push_back(s0);
        operations.push_back(s1);
    }
    else {      //eps-transition for the input tape
        TWFA::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }

    for (int i = 1; i < automaton->getMVarSize()*3+2; i++) { // transition for each tape from 1...|tapes|
        if (i == 1 && automaton->getInput().find(this->m_Val) != automaton->getInput().end()) { // main counter
            TWFA::epsilonTransitionHelper(readSymbols, operations, right, input);
        }

        else if (automaton->getInput().find(this->m_Val) != automaton->getInput().end()
                            && i % 3 == 0 && tapes[i/3-1]){ //counter for ith head
            TWFA::epsilonTransitionHelper(readSymbols, operations, right, input);
        }
        else TWFA::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    if (this->m_Val == '0') {
        int p = automaton->getMStateCnt();
        automaton->incStateCnt();
        for (int j = 0; j < readSymbols.size(); j++)
            if (readSymbols[j].size() == 3*automaton->getMVarSize()+2) automaton->addTransition(start,readSymbols[j],p,operations[j]);
    }
    else {
        for (int j = 0; j < readSymbols.size(); j++)
            if (readSymbols[j].size() == 3*automaton->getMVarSize()+2) automaton->addTransition(start,readSymbols[j],end,operations[j]);
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
        if (i==tapePos) NDTM::epsilonTransitionHelper(readSymbols, operations, left, tm->getInput());
        else NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
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
        else NDTM::epsilonTransitionHelper(readSymbols1, operations1, noShift, input);
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
        else NDTM::epsilonTransitionHelper(readSymbols2, operations2, noShift, tm->getInput());
    }
    for (int j = 0; j < readSymbols2.size(); j++) {
        if (readSymbols2[j].size() == tapes.size()+1) tm->addTransition(q, readSymbols2[j], q, operations2[j]);
    }

}

void VarAST::constructTW(std::shared_ptr<TWFA> automaton, std::vector<bool> &tapes, std::set<char> &vars, int start,
                         int end) {

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
        NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
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

void UnionAST::constructTW(std::shared_ptr<TWFA> automaton, std::vector<bool> &tapes, std::set<char> &vars, int start,
                           int end) {
    std::set<char> input = automaton->getInput();
    input.insert(automaton->getMEndSymbol());
    input.insert(automaton->getMStartSymbol());

    std::vector<std::string> readSymbols;
    std::vector<std::vector<ShiftType>> operations;
    for (int i = 0; i < automaton->getMVarSize()*3+2; i++) {
        TWFA::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    int startLeft = automaton->getMStateCnt();
    automaton->incStateCnt();
    int startRight= automaton->getMStateCnt();
    automaton->incStateCnt();
    int endLeft = automaton->getMStateCnt();
    automaton->incStateCnt();
    int endRight= automaton->getMStateCnt();
    automaton->incStateCnt();

    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == 3*automaton->getMVarSize()+2) {
            automaton->addTransition(start, readSymbols[j], startLeft, operations[j]);
            automaton->addTransition(start, readSymbols[j], startRight, operations[j]);
            automaton->addTransition(endLeft, readSymbols[j], end, operations[j]);
            automaton->addTransition(endRight, readSymbols[j], end, operations[j]);
        }
    }
    m_LHS->constructTW(automaton, tapes, vars, startLeft, endLeft);
    m_RHS->constructTW(automaton, tapes, vars, startRight, endRight);

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
        NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
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

void ConcatenationAST::constructTW(std::shared_ptr<TWFA> automaton, std::vector<bool> &tapes, std::set<char> &vars,
                                   int start, int end) {
    std::set<char> input = automaton->getInput();
    input.insert(automaton->getMEndSymbol());
    input.insert(automaton->getMStartSymbol());

    std::vector<std::string> readSymbols;
    std::vector<std::vector<ShiftType>> operations;
    for (int i = 0; i < automaton->getMVarSize()*3+2; i++) {
        TWFA::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    int startLeft = automaton->getMStateCnt();
    automaton->incStateCnt();
    int startRight= automaton->getMStateCnt();
    automaton->incStateCnt();
    int mid= automaton->getMStateCnt();
    automaton->incStateCnt();
    int endLeft = automaton->getMStateCnt();
    automaton->incStateCnt();
    int endRight= automaton->getMStateCnt();
    automaton->incStateCnt();

    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == 3*automaton->getMVarSize()+2) {
            automaton->addTransition(start, readSymbols[j], startLeft, operations[j]);
            automaton->addTransition(endLeft, readSymbols[j], mid, operations[j]);
            automaton->addTransition(mid, readSymbols[j], startRight, operations[j]);
            automaton->addTransition(endRight, readSymbols[j], end, operations[j]);
        }
    }
    m_LHS->constructTW(automaton, tapes, vars, startLeft, endLeft);
    m_RHS->constructTW(automaton, tapes, vars, startRight, endRight);
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
        NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
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

void
IterationAST::constructTW(std::shared_ptr<TWFA> automaton, std::vector<bool> &tapes, std::set<char> &vars, int start,
                          int end) {
    std::set<char> input = automaton->getInput();
    input.insert(automaton->getMEndSymbol());
    input.insert(automaton->getMStartSymbol());

    std::vector<std::string> readSymbols;
    std::vector<std::vector<ShiftType>> operations;
    for (int i = 0; i < automaton->getMVarSize()*3+2; i++) {
        TWFA::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    int startInner = automaton->getMStateCnt();
    automaton->incStateCnt();
    int endInner= automaton->getMStateCnt();
    automaton->incStateCnt();


    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == 3*automaton->getMVarSize()+2) {
            automaton->addTransition(start, readSymbols[j], startInner, operations[j]);
            automaton->addTransition(endInner, readSymbols[j], end, operations[j]);
            automaton->addTransition(start, readSymbols[j], end, operations[j]);
            automaton->addTransition(endInner, readSymbols[j], startInner, operations[j]);
        }
    }
    m_Expr->constructTW(automaton, tapes, vars, startInner, endInner);
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
        NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
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
        else NDTM::epsilonTransitionHelper(readSymbols1, operations1, noShift, input);
    }
    for (int j = 0; j < readSymbols1.size(); j++) {
        if (readSymbols1[j].size() == tapes.size()+1) tm->addTransition(start, readSymbols1[j], p, operations1[j]);
    }

    for (int j = 0; j < readSymbols1.size(); j++) {
        operations1[j][tapePos].second = right;
        if (readSymbols1[j].size() == tapes.size()+1) tm->addTransition(p, readSymbols1[j], startInner, operations1[j]);
    }

    for (int i = 0; i <= tapes.size(); i++) {
        if (i==tapePos) NDTM::epsilonTransitionHelper(readSymbols2, operations2, left, tm->getInput(), 'B');
        else NDTM::epsilonTransitionHelper(readSymbols2, operations2, noShift, input);
    }
    for (int j = 0; j < readSymbols2.size(); j++) {
        if (readSymbols2[j].size() == tapes.size()+1) tm->addTransition(p, readSymbols2[j], p, operations2[j]);
    }

    tapes[tapePos-1] = true;
    m_Expr->constructTM(tm, tapes, vars, startInner, endInner);
    tapes[tapePos-1] = false;
}

void
DefinitionAST::constructTW(std::shared_ptr<TWFA> automaton, std::vector<bool> &tapes, std::set<char> &vars, int start,
                           int end) {

}



