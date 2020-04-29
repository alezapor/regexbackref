
#include <iostream>
#include "ast.h"

void AtomAST::print() {
    std::cout << "{ Atom: " << (char) m_Val << " }";
}


void AtomAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                          int end, bool withAvd) {
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

    for (int i = 1; i <= tapes.size(); i++) { // transition for each tape from 1...|tapes|
        if (tm->getInput().find(this->m_Val) == tm->getInput().end() || !tapes[i - 1]) { // eps-transition
            NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
        }
            /*
             * write to the ith tape if it is open
             */
        else {
            for (int j = 0; j < readSymbols.size(); j++) {
                readSymbols[j].push_back((char) tm->getMBlank());
                operations[j].emplace_back(std::make_pair(this->m_Val, right));
            }
        }
    }
    if (this->m_Val == '0') {
        int p = tm->getMStateCnt();
        tm->incStateCnt();
        for (int j = 0; j < readSymbols.size(); j++)
            if (readSymbols[j].size() == tapes.size() + 1) tm->addTransition(start, readSymbols[j], p, operations[j]);
    } else {
        for (int j = 0; j < readSymbols.size(); j++)
            if (readSymbols[j].size() == tapes.size() + 1) tm->addTransition(start, readSymbols[j], end, operations[j]);
    }
}

void AtomAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, VarAST *>> &last,
                             std::vector<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, last, avd, in, out, underDefinition);
    std::string s0;
    s0.push_back(m_Val);
    automaton->addTransition(in, s0, out);
}


void BackRefAST::print() {
    std::cout << "{ Backref: " << (char) m_Var << " }";
}

void BackRefAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                         int end, bool withAvd) {
    if (withAvd && noDefBefore) {
        std::shared_ptr<AtomAST> undefined = std::make_shared<AtomAST>('?');
        undefined->constructTM(tm, tapes, memory, start, end, withAvd);
    } else {
        if (withAvd && memory[m_Var] == 0){
            std::vector<bool> free;
            free.resize(tapes.size(), true);
            for (auto it = memory.begin(); it != memory.end(); it++) {
                if (it->second != 0) free[it->second - 1] = false;
            }
            for (int i = 0; i < tapes.size(); i++) {
                if (free[i]) {
                    memory[m_Var] = i + 1;
                    break;
                }
            }
        }
        int p = tm->getMStateCnt();
        tm->incStateCnt();
        int q = tm->getMStateCnt();
        tm->incStateCnt();

        std::set<char> input = tm->getInput();
        input.insert('B');

        std::vector<std::string> readSymbols, readSymbols1, readSymbols2;
        std::vector<tapesOperations> operations, operations1, operations2;

        for (int i = 0; i <= tapes.size(); i++) {
            if (i == memory[m_Var]) NDTM::epsilonTransitionHelper(readSymbols, operations, left, tm->getInput());
            else NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
        }
        for (int j = 0; j < readSymbols.size(); j++) {
            if (readSymbols[j].size() == tapes.size() + 1) tm->addTransition(p, readSymbols[j], p, operations[j]);
        }

        for (int i = 0; i <= tapes.size(); i++) {
            if (i == memory[m_Var]) {
                for (int j = 0; j < readSymbols1.size(); j++) {
                    readSymbols1[j].push_back('B');
                    operations1[j].emplace_back(std::make_pair('B', left));
                }
            } else NDTM::epsilonTransitionHelper(readSymbols1, operations1, noShift, input);
        }
        for (int j = 0; j < readSymbols1.size(); j++) {
            if (readSymbols1[j].size() == tapes.size() + 1)
                tm->addTransition(start, readSymbols1[j], p, operations1[j]);
        }


        for (int j = 0; j < readSymbols1.size(); j++) {
            operations1[j][memory[m_Var]].second = right;
            if (readSymbols1[j].size() == tapes.size() + 1) tm->addTransition(p, readSymbols1[j], q, operations1[j]);
        }

        for (int j = 0; j < readSymbols1.size(); j++) {
            operations1[j][memory[m_Var]].second = noShift;
            if (readSymbols1[j].size() == tapes.size() + 1) {
                if (withAvd && lastRefDef) {
                    tm->addTransition(q, readSymbols1[j], tm->getMStateCnt(), operations1[j]);
                } else tm->addTransition(q, readSymbols1[j], end, operations1[j]);
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

        for (int i = 1; i <= tapes.size(); i++) {
            if (i == memory[m_Var]) {
                for (int j = 0; j < readSymbols2.size(); j++) {
                    readSymbols2[j].push_back(readSymbols2[j][0]);
                    operations2[j].emplace_back(std::make_pair(readSymbols2[j][0], right)); // <z, z, 1>
                }
            } else NDTM::epsilonTransitionHelper(readSymbols2, operations2, noShift, input);
        }
        for (int j = 0; j < readSymbols2.size(); j++) {
            if (readSymbols2[j].size() == tapes.size() + 1) tm->addTransition(q, readSymbols2[j], q, operations2[j]);
        }
    }

    if (withAvd && lastRefDef) {
        int r = tm->getMStateCnt();
        tm->incStateCnt();
        int s = tm->getMStateCnt();
        tm->incStateCnt();

        std::set<char> input = tm->getInput();
        input.insert('B');


        std::vector<std::string> readSymbols1, readSymbols2;
        std::vector<tapesOperations> operations1, operations2;

        for (int i = 0; i <= tapes.size(); i++) {
            if (i == memory[m_Var]) {
                for (int j = 0; j < readSymbols1.size(); j++) {
                    readSymbols1[j].push_back('B');
                    operations1[j].emplace_back(std::make_pair('B', left));
                }
            } else NDTM::epsilonTransitionHelper(readSymbols1, operations1, noShift, input);
        }
        for (int j = 0; j < readSymbols1.size(); j++) {
            if (readSymbols1[j].size() == tapes.size() + 1)
                tm->addTransition(r, readSymbols1[j], s, operations1[j]);
        }

        for (int j = 0; j < readSymbols1.size(); j++) {
            operations1[j][memory[m_Var]].second = right;
            if (readSymbols1[j].size() == tapes.size() + 1)
                tm->addTransition(s, readSymbols1[j], end, operations1[j]);
        }

        for (int i = 0; i <= tapes.size(); i++) {
            if (i == memory[m_Var])
                NDTM::epsilonTransitionHelper(readSymbols2, operations2, left, tm->getInput(), 'B');
            else NDTM::epsilonTransitionHelper(readSymbols2, operations2, noShift, input);
        }
        for (int j = 0; j < readSymbols2.size(); j++) {
            if (readSymbols2[j].size() == tapes.size() + 1)
                tm->addTransition(s, readSymbols2[j], s, operations2[j]);
        }
    }
}

void BackRefAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, VarAST *>> &last,
                            std::vector<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, last, avd, in, out, underDefinition);
    std::string s0;
    s0.push_back(m_Var);
    automaton->addTransition(in, s0, out);
    last.emplace_back(std::make_pair(out, this));
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

void UnionAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                           int end, bool withAvd) {
    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols;
    std::vector<tapesOperations> operations;
    for (int i = 0; i <= tapes.size(); i++) {
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

    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size() + 1) {
            tm->addTransition(start, readSymbols[j], startLeft, operations[j]);
            tm->addTransition(start, readSymbols[j], startRight, operations[j]);
            tm->addTransition(endLeft, readSymbols[j], end, operations[j]);
            tm->addTransition(endRight, readSymbols[j], end, operations[j]);
        }
    }
    m_LHS->constructTM(tm, tapes, memory, startLeft, endLeft, withAvd);
    m_RHS->constructTM(tm, tapes, memory, startRight, endRight, withAvd);

}

void UnionAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, VarAST *>> &last,
                              std::vector<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, last, avd, in, out, underDefinition);
    int inLeft = automaton->getMStateCnt();
    automaton->incStateCnt();
    int inRight = automaton->getMStateCnt();
    automaton->incStateCnt();
    int outLeft = automaton->getMStateCnt();
    automaton->incStateCnt();
    int outRight = automaton->getMStateCnt();
    automaton->incStateCnt();
    automaton->addTransition(in, "E", inLeft);
    automaton->addTransition(in, "E", inRight);
    automaton->addTransition(outLeft, "E", out);
    automaton->addTransition(outRight, "E", out);

    m_LHS->constructAvdFA(automaton, last, avd, inLeft, outLeft);
    m_RHS->constructAvdFA(automaton, last, avd, inRight, outRight);
}


void ConcatenationAST::print() {
    std::cout << "{ Concatenation(.), left:";
    m_LHS->print();
    std::cout << "\n, right:";
    m_RHS->print();
    std::cout << " }";
}

void ConcatenationAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory,
                                   int start,
                                   int end, bool withAvd) {
    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols;
    std::vector<tapesOperations> operations;
    for (int i = 0; i <= tapes.size(); i++) {
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
    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size() + 1) {
            tm->addTransition(start, readSymbols[j], startLeft, operations[j]);
            tm->addTransition(endLeft, readSymbols[j], mid, operations[j]);
            tm->addTransition(mid, readSymbols[j], startRight, operations[j]);
            tm->addTransition(endRight, readSymbols[j], end, operations[j]);
        }
    }

    m_LHS->constructTM(tm, tapes, memory, startLeft, endLeft, withAvd);
    m_RHS->constructTM(tm, tapes, memory, startRight, endRight, withAvd);
}

void
ConcatenationAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, VarAST *>> &last,
                                 std::vector<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, last, avd, in, out, underDefinition);
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
    automaton->addTransition(in, "E", inLeft);
    automaton->addTransition(mid, "E", inRight);
    automaton->addTransition(outLeft, "E", mid);
    automaton->addTransition(outRight, "E", out);

    m_LHS->constructAvdFA(automaton, last, avd, inLeft, outLeft);
    m_RHS->constructAvdFA(automaton, last, avd, inRight, outRight);
}

void IterationAST::print() {
    std::cout << "{ Iteration(*), expr:";
    m_Expr->print();
    std::cout << " }";
}

void
IterationAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                          int end, bool withAvd) {
    std::set<char> input = tm->getInput();
    input.insert('B');

    std::vector<std::string> readSymbols;
    std::vector<tapesOperations> operations;
    for (int i = 0; i <= tapes.size(); i++) {
        NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
    }
    int startInner = tm->getMStateCnt();
    tm->incStateCnt();
    int endInner = tm->getMStateCnt();
    tm->incStateCnt();

    for (int j = 0; j < readSymbols.size(); j++) {
        if (readSymbols[j].size() == tapes.size() + 1) {
            tm->addTransition(start, readSymbols[j], startInner, operations[j]);
            tm->addTransition(start, readSymbols[j], end, operations[j]);
            tm->addTransition(endInner, readSymbols[j], end, operations[j]);
            tm->addTransition(endInner, readSymbols[j], startInner, operations[j]);
        }
    }

    m_Expr->constructTM(tm, tapes, memory, startInner, endInner, withAvd);
}

void IterationAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, VarAST *>> &last,
                                  std::vector<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, last, avd, in, out, underDefinition);
    int inInner = automaton->getMStateCnt();
    automaton->incStateCnt();
    int outInner = automaton->getMStateCnt();
    automaton->incStateCnt();

    automaton->addTransition(in, "E", inInner);
    automaton->addTransition(in, "E", out);
    automaton->addTransition(out, "E", in);
    automaton->addTransition(outInner, "E", out);

    m_Expr->constructAvdFA(automaton, last, avd, inInner, outInner);
}

void DefinitionAST::print() {
    std::cout << "{ Definition of " << (char) m_Var << ", expr:";
    m_Expr->print();
    std::cout << " }";
}

void
DefinitionAST::constructTM(std::shared_ptr<NDTM> tm, std::vector<bool> &tapes, std::map<char, int> &memory, int start,
                           int end, bool withAvd) {

    if (withAvd && this->lastRefDef) {
        m_Expr->constructTM(tm, tapes, memory, start, end, withAvd);
    } else {
        if (withAvd && memory[m_Var] == 0) {
            std::vector<bool> free;
            free.resize(tapes.size(), true);
            for (auto it = memory.begin(); it != memory.end(); it++) {
                if (it->second != 0) free[it->second - 1] = false;
            }
            for (int i = 0; i < tapes.size(); i++) {
                if (free[i]) {
                    memory[m_Var] = i + 1;
                    break;
                }
            }
        }
        if (!withAvd) {
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
            for (int i = 0; i <= tapes.size(); i++) {
                NDTM::epsilonTransitionHelper(readSymbols, operations, noShift, input);
            }
            for (int j = 0; j < readSymbols.size(); j++) {
                if (readSymbols[j].size() == tapes.size() + 1)
                    tm->addTransition(endInner, readSymbols[j], end, operations[j]);
            }


            for (int i = 0; i <= tapes.size(); i++) {
                if (i == memory[m_Var]) {
                    for (int j = 0; j < readSymbols1.size(); j++) {
                        readSymbols1[j].push_back('B');
                        operations1[j].emplace_back(std::make_pair('B', left));
                    }
                } else NDTM::epsilonTransitionHelper(readSymbols1, operations1, noShift, input);
            }
            for (int j = 0; j < readSymbols1.size(); j++) {
                if (readSymbols1[j].size() == tapes.size() + 1)
                    tm->addTransition(start, readSymbols1[j], p, operations1[j]);
            }

            for (int j = 0; j < readSymbols1.size(); j++) {
                operations1[j][memory[m_Var]].second = right;
                if (readSymbols1[j].size() == tapes.size() + 1)
                    tm->addTransition(p, readSymbols1[j], startInner, operations1[j]);
            }

            for (int i = 0; i <= tapes.size(); i++) {
                if (i == memory[m_Var])
                    NDTM::epsilonTransitionHelper(readSymbols2, operations2, left, tm->getInput(), 'B');
                else NDTM::epsilonTransitionHelper(readSymbols2, operations2, noShift, input);
            }
            for (int j = 0; j < readSymbols2.size(); j++) {
                if (readSymbols2[j].size() == tapes.size() + 1)
                    tm->addTransition(p, readSymbols2[j], p, operations2[j]);
            }
            tapes[memory[m_Var] - 1] = true;
            m_Expr->constructTM(tm, tapes, memory, startInner, endInner, withAvd);
            tapes[memory[m_Var] - 1] = false;
        } else {
            tapes[memory[m_Var] - 1] = true;
            m_Expr->constructTM(tm, tapes, memory, start, end, withAvd);
            tapes[memory[m_Var] - 1] = false;
        }
    }

}

void DefinitionAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, VarAST *>> &last,
                                   std::vector<int> &avd, int in, int out, bool underDefinition) {
    NodeAST::constructAvdFA(automaton, last, avd, in, out, underDefinition);

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


    last.emplace_back(std::make_pair(out, this));
    m_Expr->constructAvdFA(automaton, last, avd, inInner, outInner, true);
}



void NodeAST::constructAvdFA(std::shared_ptr<AvdFA> automaton, std::vector<std::pair<int, VarAST *>> &last,
                             std::vector<int> &avd, int in, int out, bool underDefinition) {
    if (underDefinition) avd.push_back(in);
}

void VarAST::setLastRefDef(bool lastRefDef) {
    this->lastRefDef = lastRefDef;
}


void VarAST::setNoDefBefore(bool noDefBefore) {
    this->noDefBefore = noDefBefore;
}
