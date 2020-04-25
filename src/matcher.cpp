//
// Created by osboxes on 4/24/20.
//

#include "matcher.h"

Matcher::Matcher() {

}

Matcher::Matcher(std::shared_ptr<Parser> parser, std::shared_ptr<NDTM> automaton) : m_Parser(std::move(parser)), m_Simulator (std::move(automaton)){
    m_Parser->getNextToken();

    auto item = m_Parser->ParseA();
    item->print();

    std::vector<bool> tapes;
    tapes.resize(m_Parser->getVars().size(), false);

    m_Simulator->setMVarSize(m_Parser->getVars().size());
    m_Simulator->setInput(m_Parser->getInput());

    item->constructTM(std::dynamic_pointer_cast<NDTM>(m_Simulator), tapes, m_Parser->getVars(), m_Simulator->getMInitialState(), m_Simulator->getMFinalState());
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

Matcher::Matcher(std::shared_ptr<Parser> parser, std::shared_ptr<TWFA> automaton) : m_Parser(std::move(parser)), m_Simulator (std::move(automaton)) {
    m_Parser->getNextToken();

    auto item = m_Parser->ParseA();
    item->print();

    std::vector<bool> tapes;
    tapes.resize(m_Parser->getVars().size(), false);

    m_Simulator->setMVarSize(m_Parser->getVars().size());
    m_Simulator->setInput(m_Parser->getInput());

    std::dynamic_pointer_cast<TWFA>(m_Simulator)->addAcceptingTransitions();
    item->constructTW(std::dynamic_pointer_cast<TWFA>(m_Simulator), tapes, m_Parser->getVars(), m_Simulator->getMInitialState(), 2);
    m_Simulator->print();
}
