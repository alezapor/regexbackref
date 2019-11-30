//
// Created by osboxes on 11/29/19.
//

#include <iostream>
#include "ast.h"


void EmptyAST::print() {
    std::cout << "{ Empty }";
}

void EmptyAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes) {
    tm->addFinalState(tm->getMCur());
}

void PrimitiveAST::print() {
    std::cout << "{ Primitive: " << (char) m_Val << " }";
}

void PrimitiveAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes) {
    int q = tm->getMCur(), p = tm->getStateCnt();  // Transition q(cur) -> p
    tm->incStateCnt();

    std::string s0 = "";
    s0.push_back((char)m_Val); // read symbol m_Val
    tapesOperations s1;
    s1.emplace_back(std::make_pair(tm->getMBlank(), right)); // replace with a blank symbol and shift to the left
    for (int i = 0; i < tapes.size(); i++) {
        s0.push_back(tm->getMBlank());  // nothing to read from other tape
        /*
         * write to the ith tape if this node is inside of the ith parethesis
         */
        if (tapes[i]) s1.emplace_back(std::make_pair((char)m_Val, right));
        else s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
    }
    tm->addTransition(q,s0,p,s1);
    tm->setMCur(p);
}

void BackreferenceAST::print() {
    std::cout << "{ Backref: " << m_Num << " }";
}

void BackreferenceAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes) {
    int qCur = tm->getMCur();
    int q1 = tm->getStateCnt();
    tm->incStateCnt();
    int q2 = tm->getStateCnt();
    tm->incStateCnt();
    int p = tm->getStateCnt();
    tm->incStateCnt();

    // 1 + 2: add transitions to find the beginning of the tape m_Num
    for (auto it = tm->getInput().begin(); it != tm->getInput().end(); it++){
        for (auto it1 = tm->getInput().begin(); it1 != tm->getInput().end(); it1++){
            std::string s0 = "";
            s0.push_back(*it);
            tapesOperations s1;
            s1.emplace_back(std::make_pair(*it, noShift));
            for (int i = 0; i < tapes.size(); i++) {
                if (i == m_Num - 1) {
                    s0.push_back(*it1);
                    s1.emplace_back(std::make_pair(*it1,  left));
                }
                else {
                    s0.push_back(tm->getMBlank());
                    s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
                }
            }
            tm->addTransition(qCur,s0,q1,s1);
            tm->addTransition(q1,s0,q1,s1);
        }
    }
    for (auto it1 = tm->getInput().begin(); it1 != tm->getInput().end(); it1++){
        std::string s0 = "";
        s0.push_back(tm->getMBlank());
        tapesOperations s1;
        s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
        for (int i = 0; i < tapes.size(); i++) {
            s0.push_back(tm->getMBlank());
            if (i == m_Num - 1) {
                s0.push_back(*it1);
                s1.emplace_back(std::make_pair(*it1,  left));
            }
            else {
                s0.push_back(tm->getMBlank());
                s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
            }
        }
        tm->addTransition(qCur,s0,q1,s1);
        tm->addTransition(q1,s0,q1,s1);
    }
    //3: add transitions to move head of the m_Num tape to the first index

    for (auto it = tm->getInput().begin(); it != tm->getInput().end(); it++){
        std::string s0 = "";
        s0.push_back(*it);
        tapesOperations s1;
        s1.emplace_back(std::make_pair(*it, noShift));
        for (int i = 0; i < tapes.size(); i++) {
            s0.push_back(tm->getMBlank());
            if (i == m_Num - 1) {
                s1.emplace_back(std::make_pair(tm->getMBlank(), right));
            }
            else s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
        }
        tm->addTransition(q1,s0,q2,s1);
    }


        std::string s0 = "";
        s0.push_back(tm->getMBlank());
        tapesOperations s1;
        s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
        for (int i = 0; i < tapes.size(); i++) {
            s0.push_back(tm->getMBlank());
            if (i == m_Num - 1) {
                s1.emplace_back(std::make_pair(tm->getMBlank(), right));
            } else s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
        }
        tm->addTransition(q1, s0, q2, s1);

    //4: Compare symbols on the 0th tape and the m_Num tape
    for (auto it = tm->getInput().begin(); it != tm->getInput().end(); it++){
        std::string s0 = "";
        s0.push_back(*it);
        tapesOperations s1;
        s1.emplace_back(std::make_pair(tm->getMBlank(), right));
        for (int i = 0; i < tapes.size(); i++) {
            if (i == m_Num - 1) {
                s1.emplace_back(std::make_pair(*it, right));
                s0.push_back(*it);
            }
            else {
                s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
                s0.push_back(tm->getMBlank());
            }
        }
        tm->addTransition(q2,s0,q2,s1);
    }

    //5: Check if all symbols were compared
    for (auto it = tm->getInput().begin(); it != tm->getInput().end(); it++){
        std::string s0 = "";
        s0.push_back(*it);
        tapesOperations s1;
        s1.emplace_back(std::make_pair(*it, noShift));
        for (int i = 0; i < tapes.size(); i++) {
            s0.push_back(tm->getMBlank());
            if (i == m_Num - 1) {
                s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
            }
            else s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
        }
        tm->addTransition(q2,s0,p,s1);
    }

    s0 = "";
    s0.push_back(tm->getMBlank());
    tapesOperations s2;
    s2.emplace_back(std::make_pair(tm->getMBlank(), noShift));
    for (int i = 0; i < tapes.size(); i++) {
        s0.push_back(tm->getMBlank());
        if (i == m_Num - 1) {
            s2.emplace_back(std::make_pair(tm->getMBlank(), noShift));
        }
        else s2.emplace_back(std::make_pair(tm->getMBlank(), noShift));
    }
    tm->addTransition(q2,s0,p,s2);

    tm->setMCur(p);
}

void AlterationAST::print() {
    std::cout << "{ Alteration(+), left:";
    m_LHS->print();
    std::cout << ", right:";
    m_RHS->print();
    std::cout << " }";
}

void AlterationAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes) {
    int start = tm->getMCur();
    m_LHS->constructTM(tm, tapes);
    int q1 = tm->getMCur();
    tm->setMCur(start);
    m_RHS->constructTM(tm, tapes);
    int q2 = tm->getMCur();
    int p = tm->getStateCnt();
    tm->incStateCnt();
    addSimpleTransition(tm, tapes, q1, p);
    addSimpleTransition(tm, tapes, q2, p);
}

void ConcatenationAST::print() {
    std::cout << "{ Concatenation(.), left:";
    m_LHS->print();
    std::cout << ", right:";
    m_RHS->print();
    std::cout << " }";
}

void ConcatenationAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes) {
    m_LHS->constructTM(tm, tapes);
    m_RHS->constructTM(tm, tapes);
}

void IterationAST::print() {
    std::cout << "{ Iteration(*), expr:";
    m_Expr->print();
    std::cout << " }";
}

void IterationAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes) {
    int q = tm->getMCur();
    m_Expr->constructTM(tm, tapes);
    addSimpleTransition(tm, tapes, tm->getMCur(), q);
}

void ParenthesesAST::print() {
    std::cout << "{ Parentheses: (" << m_Num << "), expr:";
    m_Expr->print();
    std::cout << " }";
}

void ParenthesesAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes) {
    //cleanTape(n_Num); TODO
    tapes[m_Num - 1] = true;
    m_Expr->constructTM(tm, tapes);
    tapes[m_Num - 1] = false;
}

void NodeAST::addSimpleTransition(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, int q, int p) {
    for (auto it = tm->getInput().begin(); it != tm->getInput().end(); it++){
        std::string s0 = "";
        s0.push_back(*it);
        tapesOperations s1;
        s1.emplace_back(std::make_pair(*it, noShift));
        for (int i = 0; i < tapes.size(); i++) {
            s0.push_back(tm->getMBlank());
            s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
        }
        tm->addTransition(q,s0,p,s1);
    }
    std::string s0 = "";
    s0.push_back(tm->getMBlank());
    tapesOperations s1;
    s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
    for (int i = 0; i < tapes.size(); i++) {
        s0.push_back(tm->getMBlank());
        s1.emplace_back(std::make_pair(tm->getMBlank(), noShift));
    }
    tm->addTransition(q,s0,p,s1);

    tm->setMCur(p);
}
