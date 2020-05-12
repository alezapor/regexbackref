//
// Created by osboxes on 4/24/20.
//

#include "matcher.h"

Matcher::Matcher() {

}

Matcher::Matcher(std::shared_ptr<Parser> parser, bool withAvd) :
        m_Parser(std::move(parser)) {
    std::vector<bool> tapes;
    std::map<char, int> memory;
    m_Parser->getNextToken();


    m_Root = std::move(m_Parser->ParseA());
    //m_Root->print();
    if (withAvd) {
        int avdmax = 0;

        std::vector<int> avd;
        std::vector<std::pair<int, VarAST *>> last;
        std::shared_ptr<AvdFA> avdFA = std::make_shared<AvdFA>();
        m_Root->constructAvdFA(avdFA, last, avd, avdFA->getMInitialState(), *avdFA->getMFinalStates().begin(), false);

        //avdFA->print();
       /* for (auto it = avd.begin(); it != avd.end(); it++) {
            int avdState = 0;
            std::vector<char> avs;
            for (auto it1 = m_Parser->getVars().begin(); it1 != m_Parser->getVars().end(); it1++) {
                if (avdFA->constructR0(*it, *it1)->accepts() && avdFA->constructR1(*it, *it1)->accepts()) {
                    avdState++;
                    avs.push_back(*it1);
                }
            }
            if (avdState > avdmax) avdmax = avdState;
        }


        for (auto it = last.begin(); it != last.end(); it++) {
            std::set<int> avs;
            for (auto it1 = m_Parser->getVars().begin(); it1 != m_Parser->getVars().end(); it1++) {
                if (avdFA->constructR0(it->first, *it1)->accepts() && avdFA->constructR1(it->first, *it1)->accepts()) {
                    avs.insert(*it1);
                }
            }
            it->second->setAcviteVars(avs);

            if (!avdFA->constructR1(it->first, it->second->getVar())->accepts()) {
                it->second->setLastRefDef(true);

            }
            if (!avdFA->constructR0(it->first, it->second->getVar())->accepts()) {
                it->second->setNoDefBefore(true);

            }
        }*/

        m_Simulator = new MemoryAutomaton(avdFA->getMTransitions(), *avdFA.get());

        /*memoryAut->print();
        memoryAut->initialize("abab");
        if (memoryAut->accepts()) std::cout << "+" <<std::endl;


        tapes.resize(avdmax, false);
        for (auto it1 = m_Parser->getVars().begin(); it1 != m_Parser->getVars().end(); it1++)
                memory[*it1]=0;
        m_Simulator->setMTapeCnt(avdmax + 1);
        m_Simulator->setInput(m_Parser->getInput());

        m_Root->constructTM(m_Simulator, tapes, memory, m_Simulator->getMInitialState(),
                            *m_Simulator->getMFinalStates().begin(), true);*/
        //m_Simulator->print();
    } else {
        tapes.resize(m_Parser->getVars().size(), false);

        for (auto it = m_Parser->getVars().begin(); it != m_Parser->getVars().end(); it++) {
            memory[*it] = std::distance(m_Parser->getVars().begin(), it) + 1;
        }
        NDTM* tm = new NDTM();
        tm->setMTapeCnt(m_Parser->getVars().size() + 1);
        tm->setInput(m_Parser->getInput());
        m_Root->constructTM(tm, tapes, memory, tm->getMInitialState(),
                            *tm->getMFinalStates().begin());
        m_Simulator = tm;
        //m_Simulator->print();
    }

}


bool Matcher::match(std::string w) {
    m_Simulator->initialize(w);
    return m_Simulator->accepts();
}
