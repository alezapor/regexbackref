//
// Created by osboxes on 4/24/20.
//

#ifndef REGEX_MATCHER_MATCHER_H
#define REGEX_MATCHER_MATCHER_H


#include <memory>
#include "parser.h"

class Matcher {
public:
    Matcher();

    Matcher(std::shared_ptr<Parser> parser, char * option);

    bool match(std::string w);

private:
    std::shared_ptr<Parser> m_Parser;
    BaseAutomaton *  m_Simulator;
    std::unique_ptr<NodeAST> m_Root;
};


#endif //REGEX_MATCHER_MATCHER_H
