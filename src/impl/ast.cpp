
#include <iostream>
#include "ast.h"

void AtomAST::print() {
    std::cout << "{ Atom: " << (char) m_Val << " }";
}


void AtomAST::constructTM(NDTM* tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                          int end) {
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
    } else {      //eps-transition for the input tape
        NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }

    for (int i = 1; i <= (int) tapes.size(); i++) { // transition for each tape from 1...|tapes|
        if (tm->getInput().find(this->m_Val) == tm->getInput().end() || !tapes[i - 1]) { // eps-transition
            NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
        }
            /*
             * write to the ith tape if it is open
             */
        else {
            for (int j = 0; j < (int) readSymbols.size(); j++) {
                readSymbols[j].push_back((char) tm->getMBlank());
                operations[j].emplace_back(std::make_pair(this->m_Val, right));
            }
        }
    }
    if (this->m_Val == '0') {
        int p = tm->getMStateCnt();
        tm->incStateCnt();
        for (int j = 0; j < (int) readSymbols.size(); j++)
            if (readSymbols[j].size() == tapes.size() + 1) tm->addTransition(start, readSymbols[j], p, operations[j]);
    } else {
        for (int j = 0; j < (int) readSymbols.size(); j++)
            if (readSymbols[j].size() == tapes.size() + 1) tm->addTransition(start, readSymbols[j], end, operations[j]);
    }
}

void AtomAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, avd, in, out, underDefinition);
    std::string s0;
    s0.push_back(m_Val);
    automaton->addTransition(in, s0, out);
}


void BackRefAST::print() {
    std::cout << "{ Backref: " << (char) m_Var << " }";
}

void BackRefAST::constructTM(NDTM* tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                         int end) {

        int p = tm->getMStateCnt();
        tm->incStateCnt();
        int q = tm->getMStateCnt();
        tm->incStateCnt();

        std::set<char> input = tm->getInput();
        input.insert('B');

        std::vector<std::string> readSymbols, readSymbols1, readSymbols2;
        std::vector<tapesOperations> operations, operations1, operations2;

        for (int i = 0; i <= (int)tapes.size(); i++) {
            if (i == memory[m_Var]) NDTM::epsilonTransitionHelper(readSymbols, operations, left, tm->getInput());
            else NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
        }
        for (int j = 0; j < (int) readSymbols.size(); j++) {
            if (readSymbols[j].size() == tapes.size() + 1) tm->addTransition(p, readSymbols[j], p, operations[j]);
        }

        for (int i = 0; i <= (int) tapes.size(); i++) {
            if (i == memory[m_Var]) {
                for (int j = 0; j < (int) readSymbols1.size(); j++) {
                    readSymbols1[j].push_back('B');
                    operations1[j].emplace_back(std::make_pair('B', left));
                }
            } else NDTM::epsilonTransitionHelper(readSymbols1, operations1, noShift, input);
        }
        for (int j = 0; j < (int) readSymbols1.size(); j++) {
            if (readSymbols1[j].size() == tapes.size() + 1)
                tm->addTransition(start, readSymbols1[j], p, operations1[j]);
        }


        for (int j = 0; j < (int) readSymbols1.size(); j++) {
            operations1[j][memory[m_Var]].second = right;
            if (readSymbols1[j].size() == tapes.size() + 1) tm->addTransition(p, readSymbols1[j], q, operations1[j]);
        }

        for (int j = 0; j < (int) readSymbols1.size(); j++) {
            operations1[j][memory[m_Var]].second = noShift;
            if ((int) readSymbols1[j].size() == (int) tapes.size() + 1) {
                tm->addTransition(q, readSymbols1[j], end, operations1[j]);
            }
        }

        for (auto it = tm->getInput().begin(); it != tm->getInput().end(); it++) { // <z, B, 1> - zero tape
            std::string s0 = "";
            tapesOperations s1;
            s0.push_back(*it);
            s1.emplace_back(std::make_pair('B', right));
            readSymbols2.push_back(s0);
            operations2.emplace_back(s1);
        }

        for (int i = 1; i <= (int) tapes.size(); i++) {
            if (i == memory[m_Var]) {
                for (int j = 0; j < (int) readSymbols2.size(); j++) {
                    readSymbols2[j].push_back(readSymbols2[j][0]);
                    operations2[j].emplace_back(std::make_pair(readSymbols2[j][0], right)); // <z, z, 1>
                }
            } else NDTM::epsilonTransitionHelper(readSymbols2, operations2, noShift, input);
        }
        for (int j = 0; j < (int) readSymbols2.size(); j++) {
            if (readSymbols2[j].size() == tapes.size() + 1) tm->addTransition(q, readSymbols2[j], q, operations2[j]);
        }
}

void BackRefAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, avd, in, out, underDefinition);
    std::string s0;
    s0.push_back(m_Var);
    automaton->addTransition(in, s0, out);
}

int VarAST::getVar() const {
    return m_Var;
}

void UnionAST::print() {
    std::cout << "{ Union(+), left:";
    m_LHS->print();
    std::cout << ", right:";
    m_RHS->print();
    std::cout << " }";
}

void UnionAST::constructTM(NDTM* tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                           int end) {
    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols;
    std::vector<tapesOperations> operations;
    for (int i = 0; i <= (int) tapes.size(); i++) {
        NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    int startLeft = tm->getMStateCnt();
    tm->incStateCnt();
    int startRight = tm->getMStateCnt();
    tm->incStateCnt();
    int endLeft = tm->getMStateCnt();
    tm->incStateCnt();
    int endRight = tm->getMStateCnt();
    tm->incStateCnt();

    for (int j = 0; j < (int) readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size() + 1) {
            tm->addTransition(start, readSymbols[j], startLeft, operations[j]);
            tm->addTransition(start, readSymbols[j], startRight, operations[j]);
            tm->addTransition(endLeft, readSymbols[j], end, operations[j]);
            tm->addTransition(endRight, readSymbols[j], end, operations[j]);
        }
    }
    m_LHS->constructTM(tm, tapes, memory, startLeft, endLeft);
    m_RHS->constructTM(tm, tapes, memory, startRight, endRight);

}

void UnionAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, avd, in, out, underDefinition);
    int inLeft = automaton->getMStateCnt();
    automaton->incStateCnt();
    int inRight = automaton->getMStateCnt();
    automaton->incStateCnt();
    int outLeft = automaton->getMStateCnt();
    automaton->incStateCnt();
    int outRight = automaton->getMStateCnt();
    automaton->incStateCnt();
    automaton->addTransition(in, "?", inLeft);
    automaton->addTransition(in, "?", inRight);
    automaton->addTransition(outLeft, "?", out);
    automaton->addTransition(outRight, "?", out);

    m_LHS->constructAvdFA(automaton, avd, inLeft, outLeft);
    m_RHS->constructAvdFA(automaton, avd, inRight, outRight);
}


void ConcatenationAST::print() {
    std::cout << "{ Concatenation(.), left:";
    m_LHS->print();
    std::cout << "\n, right:";
    m_RHS->print();
    std::cout << " }";
}

void ConcatenationAST::constructTM(NDTM* tm, std::vector<bool> &tapes, std::map<char, int> &memory,
                                   int start, int end) {
    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols;
    std::vector<tapesOperations> operations;
    for (int i = 0; i <= (int) tapes.size(); i++) {
        NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    int startLeft = tm->getMStateCnt();
    tm->incStateCnt();
    int startRight = tm->getMStateCnt();
    tm->incStateCnt();
    int mid = tm->getMStateCnt();
    tm->incStateCnt();
    int endLeft = tm->getMStateCnt();
    tm->incStateCnt();
    int endRight = tm->getMStateCnt();
    tm->incStateCnt();
    for (int j = 0; j < (int) readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size() + 1) {
            tm->addTransition(start, readSymbols[j], startLeft, operations[j]);
            tm->addTransition(endLeft, readSymbols[j], mid, operations[j]);
            tm->addTransition(mid, readSymbols[j], startRight, operations[j]);
            tm->addTransition(endRight, readSymbols[j], end, operations[j]);
        }
    }

    m_LHS->constructTM(tm, tapes, memory, startLeft, endLeft);
    m_RHS->constructTM(tm, tapes, memory, startRight, endRight);
}

void
ConcatenationAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, avd, in, out, underDefinition);
    int inLeft = automaton->getMStateCnt();
    automaton->incStateCnt();
    int inRight = automaton->getMStateCnt();
    automaton->incStateCnt();
    int outLeft = automaton->getMStateCnt();
    automaton->incStateCnt();
    int outRight = automaton->getMStateCnt();
    automaton->incStateCnt();
    int mid = automaton->getMStateCnt();
    automaton->incStateCnt();
    automaton->addTransition(in, "?", inLeft);
    automaton->addTransition(mid, "?", inRight);
    automaton->addTransition(outLeft, "?", mid);
    automaton->addTransition(outRight, "?", out);

    m_LHS->constructAvdFA(automaton, avd, inLeft, outLeft);
    m_RHS->constructAvdFA(automaton, avd, inRight, outRight);
}

void IterationAST::print() {
    std::cout << "{ Iteration(*), expr:";
    m_Expr->print();
    std::cout << " }";
}

void
IterationAST::constructTM(NDTM* tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                          int end) {
    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols;
    std::vector<tapesOperations> operations;
    for (int i = 0; i <= (int) tapes.size(); i++) {
        NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    int startInner = tm->getMStateCnt();
    tm->incStateCnt();
    int endInner = tm->getMStateCnt();
    tm->incStateCnt();

    for (int j = 0; j < (int) readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size() + 1) {
            tm->addTransition(start, readSymbols[j], startInner, operations[j]);
            tm->addTransition(start, readSymbols[j], end, operations[j]);
            tm->addTransition(endInner, readSymbols[j], end, operations[j]);
            tm->addTransition(endInner, readSymbols[j], startInner, operations[j]);
        }
    }

    m_Expr->constructTM(tm, tapes, memory, startInner, endInner);
}

void IterationAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition){
    NodeAST::constructAvdFA(automaton, avd, in, out, underDefinition);
    int inInner = automaton->getMStateCnt();
    automaton->incStateCnt();
    int outInner = automaton->getMStateCnt();
    automaton->incStateCnt();

    automaton->addTransition(in, "?", inInner);
    automaton->addTransition(in, "?", out);
    automaton->addTransition(out, "?", in);
    automaton->addTransition(outInner, "?", out);

    m_Expr->constructAvdFA(automaton, avd, inInner, outInner);
}

void DefinitionAST::print() {
    std::cout << "{ Definition of " << (char) m_Var << ", expr:";
    m_Expr->print();
    std::cout << " }";
}

void
DefinitionAST::constructTM(NDTM* tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                           int end) {

                int startInner = tm->getMStateCnt();
                tm->incStateCnt();
                int endInner = tm->getMStateCnt();
                tm->incStateCnt();
                int p = tm->getMStateCnt();
                tm->incStateCnt();

                std::set<char> input = tm->getInput();
                input.insert('B');

                std::vector<std::string> readSymbols, readSymbols1, readSymbols2;
                std::vector<tapesOperations> operations, operations1, operations2;
                for (int i = 0; i <= (int) tapes.size(); i++) {
                    NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
                }
                for (int j = 0; j < (int) readSymbols.size(); j++) {
                    if (readSymbols[j].size() == tapes.size() + 1)
                        tm->addTransition(endInner, readSymbols[j], end, operations[j]);
                }


                for (int i = 0; i <= (int) tapes.size(); i++) {
                    if (i == memory[m_Var]) {
                        for (int j = 0; j < (int) readSymbols1.size(); j++) {
                            readSymbols1[j].push_back('B');
                            operations1[j].emplace_back(std::make_pair('B', left));
                        }
                    } else NDTM::epsilonTransitionHelper(readSymbols1, operations1, noShift, input);
                }
                for (int j = 0; j < (int) readSymbols1.size(); j++) {
                    if (readSymbols1[j].size() == tapes.size() + 1)
                        tm->addTransition(start, readSymbols1[j], p, operations1[j]);
                }

                for (int j = 0; j < (int) readSymbols1.size(); j++) {
                    operations1[j][memory[m_Var]].second = right;
                    if (readSymbols1[j].size() == tapes.size() + 1)
                        tm->addTransition(p, readSymbols1[j], startInner, operations1[j]);
                }

                for (int i = 0; i <= (int) tapes.size(); i++) {
                    if (i == memory[m_Var])
                        NDTM::epsilonTransitionHelper(readSymbols2, operations2, left, tm->getInput(), 'B');
                    else NDTM::epsilonTransitionHelper(readSymbols2, operations2, noShift, input);
                }
                for (int j = 0; j < (int) readSymbols2.size(); j++) {
                    if (readSymbols2[j].size() == tapes.size() + 1)
                        tm->addTransition(p, readSymbols2[j], p, operations2[j]);
                }
                tapes[memory[m_Var] - 1] = true;
                m_Expr->constructTM(tm, tapes, memory, startInner, endInner);
                tapes[memory[m_Var] - 1] = false;


}

void DefinitionAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, avd, in, out, underDefinition);
    int inInner = automaton->getMStateCnt();
    automaton->incStateCnt();
    int outInner = automaton->getMStateCnt();
    automaton->incStateCnt();

    std::string sOpen, sClose;
    sOpen.push_back('[');
    sOpen.push_back(m_Var); // "x"
    sClose.push_back(']');
    sClose.push_back(m_Var); // "x"

    automaton->addTransition(in, sOpen, inInner);
    automaton->addTransition(outInner, sClose, out);

    m_Expr->constructAvdFA(automaton, avd, inInner, outInner, true);
}


void
NodeAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::set<int> &avd, int in, int out, bool underDefinition) {
    if(underDefinition) avd.insert(in);
}
