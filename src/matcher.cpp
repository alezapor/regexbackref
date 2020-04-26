//
// Created by osboxes on 4/24/20.
//

#include "matcher.h"

Matcher::Matcher() {

}

Matcher::Matcher(std::shared_ptr<Parser> parser, std::shared_ptr<NDTM> automaton, bool withAvd) :
        m_Parser(std::move(parser)), m_Simulator(std::move(automaton)) {
    std::vector<bool> tapes;
    std::map<char, int> memory;
    m_Parser->getNextToken();


    m_Root = std::move(m_Parser->ParseA());
    m_Root->print();
    if (withAvd) {
        int avdmax = 0;
        /*
         * avd
         */
        std::vector<int> avd;
        std::vector<std::pair<int, NodeAST *>> last;
        std::shared_ptr<AvdFA> avdFA = std::make_shared<AvdFA>();
        m_Root->constructAvdFA(avdFA, last, avd, avdFA->getMInitialState(), *avdFA->getMFinalStates().begin(), false);

        avdFA->print();

        for (auto it = avd.begin(); it != avd.end(); it++) {
            int avdState = 0;
            for (auto it1 = m_Parser->getVars().begin(); it1 != m_Parser->getVars().end(); it1++) {
                std::cout << avdFA->constructR0(*it, *it1)->accepts() << std::endl;
                std::cout << avdFA->constructR1(*it, *it1)->accepts() << std::endl;
                if (avdFA->constructR0(*it, *it1)->accepts() && avdFA->constructR1(*it, *it1)->accepts()) {
                    avdState++;
                }
            }
            if (avdState > avdmax) avdmax = avdState;
        }

        /*
         * last references + no def before ref
         */
        for (auto it = last.begin(); it != last.end(); it++) {
            if (!avdFA->constructR1(it->first, it->second->getVar())->accepts()) {
                it->second->setLastRefDef(true);
                std::cout << "\n LAST:";
                it->second->print();
            }
            if (!avdFA->constructR0(it->first, it->second->getVar())->accepts()) {
                it->second->setNoDefBefore(true);
                std::cout << "\nNODEF:";
                it->second->print();
            }
        }

        std::cout << "\nAvd for the regex is " << avdmax << std::endl;

        tapes.resize(avdmax, false);

        for (auto it = m_Parser->getVars().begin(); it != m_Parser->getVars().end(); it++) {
            memory[*it] = 0;
        }
        m_Simulator->setMTapeCnt(avdmax + 1);
        m_Simulator->setInput(m_Parser->getInput());

        m_Root->constructTM(m_Simulator, tapes, memory, m_Simulator->getMInitialState(),
                            *m_Simulator->getMFinalStates().begin(), true);
        m_Simulator->print();
    } else {
        tapes.resize(m_Parser->getVars().size(), false);

        for (auto it = m_Parser->getVars().begin(); it != m_Parser->getVars().end(); it++) {
            memory[*it] = std::distance(m_Parser->getVars().begin(), it) + 1;
        }

        m_Simulator->setMTapeCnt(m_Parser->getVars().size() + 1);
        m_Simulator->setInput(m_Parser->getInput());

        m_Root->constructTM(m_Simulator, tapes, memory, m_Simulator->getMInitialState(),
                            *m_Simulator->getMFinalStates().begin());
        m_Simulator->print();
    }

}


bool Matcher::match(std::string w) {
    m_Simulator->initialize(w);
    if (m_Simulator->accepts()) {
        std::cout << "'" << w << "' is ACCEPTED" << std::endl;
        return true;
    }
    std::cout << "'" << w << "' is NOT ACCEPTED" << std::endl;
    return false;
}
