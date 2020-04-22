//
// Created by osboxes on 11/29/19.
//

#include <iostream>
#include "ast.h"

void AtomAST::print() {
    std::cout << "{ Atom: " << (char) m_Val << " }";
}

void AtomAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::set<char> & vars, int start, int end) {
    std::string s0 = "";
    tapesOperations s1;
    if (tm->getInput().find(this->m_Val) != tm->getInput().end()) {
        s0.push_back((char) this->m_Val);
        s1.emplace_back(std::make_pair(tm->getMBlank(), right)); // replace with a blank symbol and shift to the left
    }
    else {
        s0.push_back((char) 'E');   // read any symbol
        s1.emplace_back(std::make_pair('E', noShift)); // insert the read symbol, no shift
    }

    for (int i = 1; i <= tapes.size(); i++) {
        if (tm->getInput().find(this->m_Val) == tm->getInput().end() || !tapes[i-1]) { // eps-transition
            s0.push_back((char) 'E');
            s1.emplace_back(std::make_pair('E', noShift));
        }
          /*
           * write to the ith tape if it is open
           */
        else {
            s0.push_back((char) tm->getMBlank());
            s1.emplace_back(std::make_pair(this->m_Val, right));
        }
    }
    if (this->m_Val == '0') {
        int p = tm->getStateCnt();
        tm->incStateCnt();
        tm->addTransition(start,s0,p,s1);
    }
    else {
        tm->addTransition(start,s0,end,s1);
    }
}

void VarAST::print() {
    std::cout << "{ Backref: " << (char) m_Name << " }";
}

void VarAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes,std::set<char> & vars, int start, int end) {
    int tapePos = std::distance(vars.begin(), vars.find(m_Name))+1;
    int p = tm->getStateCnt();
    tm->incStateCnt();
    int q = tm->getStateCnt();
    tm->incStateCnt();

    std::string s0 = "";
    tapesOperations s1;
    for (int i = 0; i <= tapes.size(); i++) {
        s0.push_back((char) 'E');
        s1.emplace_back(std::make_pair('E', noShift));
    }

    s0[tapePos] = 'B';
    s1[tapePos].first = 'B';
    s1[tapePos].second = noShift;
    tm->addTransition(q,s0,end,s1);

    s1[tapePos].second = left;
    tm->addTransition(start,s0,p,s1);
    s1[tapePos].second = right;
    tm->addTransition(p,s0,q,s1);

    s0[tapePos] = 'A';
    s1[tapePos].first = 'A';
    s1[tapePos].second = left;
    tm->addTransition(p,s0,p,s1);

    for (auto it = tm->getInput().begin(); it != tm->getInput().end(); it++){
        s0[tapePos] = *it;
        s1[tapePos].first = *it;
        s1[tapePos].second = right;
        s0[0] = *it;
        s1[0].first = 'B';
        s1[0].second = right;
        tm->addTransition(q,s0,q,s1);
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
    std::string s0 = "";
    tapesOperations s1;
    for (int i = 0; i <= tapes.size(); i++) {
            s0.push_back((char) 'E');
            s1.emplace_back(std::make_pair('E', noShift));
    }
    int startLeft = tm->getStateCnt();
    tm->incStateCnt();
    int startRight= tm->getStateCnt();
    tm->incStateCnt();
    int endLeft = tm->getStateCnt();
    tm->incStateCnt();
    int endRight= tm->getStateCnt();
    tm->incStateCnt();
    tm->addTransition(start,s0,startLeft,s1);
    tm->addTransition(start,s0,startRight,s1);
    tm->addTransition(endLeft,s0,end,s1);
    tm->addTransition(endRight,s0,end,s1);
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
    std::string s0 = "";
    tapesOperations s1;
    for (int i = 0; i <= tapes.size(); i++) {
        s0.push_back((char) 'E');
        s1.emplace_back(std::make_pair('E', noShift));
    }
    int startLeft = tm->getStateCnt();
    tm->incStateCnt();
    int startRight= tm->getStateCnt();
    tm->incStateCnt();
    int mid= tm->getStateCnt();
    tm->incStateCnt();
    int endLeft = tm->getStateCnt();
    tm->incStateCnt();
    int endRight= tm->getStateCnt();
    tm->incStateCnt();
    tm->addTransition(start,s0,startLeft,s1);
    tm->addTransition(endLeft,s0,mid,s1);
    tm->addTransition(mid,s0,startRight,s1);
    tm->addTransition(endRight,s0,end,s1);
    m_LHS->constructTM(tm, tapes, vars, startLeft, endLeft);
    m_RHS->constructTM(tm, tapes, vars, startRight, endRight);
}

void IterationAST::print() {
    std::cout << "{ Iteration(*), expr:";
    m_Expr->print();
    std::cout << " }";
}

void IterationAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::set<char> & vars,  int start, int end) {
    std::string s0 = "";
    tapesOperations s1;
    for (int i = 0; i <= tapes.size(); i++) {
        s0.push_back((char) 'E');
        s1.emplace_back(std::make_pair('E', noShift));
    }
    int startInner = tm->getStateCnt();
    tm->incStateCnt();
    int endInner= tm->getStateCnt();
    tm->incStateCnt();

    tm->addTransition(start,s0,startInner,s1);
    tm->addTransition(start,s0,end,s1);
    tm->addTransition(endInner,s0,end,s1);
    tm->addTransition(endInner,s0,startInner,s1);
    m_Expr->constructTM(tm, tapes, vars, startInner, endInner);
}

void DefinitionAST::print() {
    std::cout << "{ Definition of " << (char) m_Var << ", expr:";
    m_Expr->print();
    std::cout << " }";
}

void DefinitionAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::set<char> & vars, int start, int end) {

    int tapePos = std::distance(vars.begin(), vars.find(m_Var))+1;
    int p = tm->getStateCnt();
    tm->incStateCnt();
    int startInner = tm->getStateCnt();
    tm->incStateCnt();
    int endInner= tm->getStateCnt();
    tm->incStateCnt();

    std::string s0 = "";
    tapesOperations s1;
    for (int i = 0; i <= tapes.size(); i++) {
        s0.push_back((char) 'E');
        s1.emplace_back(std::make_pair('E', noShift));
    }
    tm->addTransition(endInner,s0,end,s1);

    s0[tapePos] = 'B';
    s1[tapePos].first = 'B';
    s1[tapePos].second = left;
    tm->addTransition(start,s0,p,s1);
    s1[tapePos].second = right;
    tm->addTransition(p,s0,startInner,s1);
    s0[tapePos] = 'A';
    s1[tapePos].second = left;
    tm->addTransition(p,s0,p,s1);
    tapes[tapePos-1] = true;
    m_Expr->constructTM(tm, tapes, vars, startInner, endInner);
    tapes[tapePos-1] = false;
}

