//
// Created by osboxes on 4/24/20.
//

#include "matcher.h"

Matcher::Matcher() {

}

Matcher::Matcher(std::shared_ptr<Parser> parser, std::shared_ptr<NDTM> automaton) : m_Parser(std::move(parser)),
                                                                                m_Simulator (std::move(automaton)){
    m_Parser->getNextToken();

    m_Root = std::move(m_Parser->ParseA());
    m_Root->print();

    std::vector<bool> tapes;
    tapes.resize(m_Parser->getVars().size(), false);

    m_Simulator->setMTapeCnt(m_Parser->getVars().size()+1);
    m_Simulator->setInput(m_Parser->getInput());

    m_Root->constructTM(m_Simulator, tapes, m_Parser->getVars(), m_Simulator->getMInitialState(), *m_Simulator->getMFinalStates().begin());
    m_Simulator->print();

}


bool Matcher::match(std::string w) {
    m_Simulator->initialize(w);
    if (m_Simulator->accepts()) {
        std::cout << "'" << w << "' is ACCEPTED" << std::endl;
        return true;
    }
    std::cout << "'" <<  w << "' is NOT ACCEPTED" << std::endl;
    return false;
}
