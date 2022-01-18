
#ifndef REGEX_MATCHER_MATCHER_H
#define REGEX_MATCHER_MATCHER_H


#include <memory>
#include "parser.h"

bool matches(const char*, const char*, const char*);

/**
 * A class that represents regex matcher
 */
class Matcher {
public:
    Matcher();

    virtual ~Matcher();

    /**
     * A constructor that creates matcher using chosen algorithm
     * @param parser a pointer to regex parser
     * @param option the chosen algorithm 
     */
    Matcher(std::unique_ptr<Parser> parser, const char *option);

    /**
     * A function that simulates regex matching
     * @param w an input word
     * @return true if regex matches an input word
     */
    bool match(std::string w);

private:
    /**
     * A poiner to regex parser
     */
    std::unique_ptr<Parser> m_Parser;
    /**
     * A pointer to generated automaton that accepts language of the regex
     */
    BaseAutomaton *m_Simulator;
    /**
     * A pointer to the root of the AST for regex
     */
    std::unique_ptr<NodeAST> m_Root;
};


#endif //REGEX_MATCHER_MATCHER_H
