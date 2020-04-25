//
// Created by osboxes on 4/24/20.
//

#ifndef REGEX_MATCHER_MATCHER_H
#define REGEX_MATCHER_MATCHER_H


#include <memory>
#include "parser.h"
#include "twoway.h"

class Matcher {
public:
    Matcher();
    Matcher(std::shared_ptr<Parser> parser, std::shared_ptr<NDTM> automaton);
    Matcher(std::shared_ptr<Parser> parser, std::shared_ptr<TWFA> automaton);
    bool match(std::string w);

private:
    std::shared_ptr<Parser> m_Parser;
    std::shared_ptr<Automaton> m_Simulator;
};


#endif //REGEX_MATCHER_MATCHER_H
