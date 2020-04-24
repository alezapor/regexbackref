#include <iostream>
#include <fstream>
#include "parser.h"

int main(int argc, char * argv[]) {

    std::fstream is;
    is.open(argv[1], std::fstream::in);
    if(!is){
        printf("Cannot open file. Set working directory as in. Usage %s <regex_file>\n", argv[0]);
        exit(1);
    }

    std::unique_ptr<Parser> parser = std::make_unique<Parser>(&is);
    parser->getNextToken();

    auto item = parser->ParseA();
    item->print();

    std::vector<bool> tapes;
    tapes.resize(parser->getVars().size(), false);

    std::shared_ptr<NDTM> tm = std::make_shared<NDTM>();
    tm->setInput(parser->getInput());

    item->constructTM(tm, tapes, parser->getVars(), 0, tm->getFinalState());
    tm->print();
    std::string word = "abaa";
    std::shared_ptr<OneHeadTape> tape = std::make_shared<OneHeadTape>("B"+word+"B", 1);
    tm->loadTapes(std::move(tape), parser->getVars().size());

    std::cout << word << " is " << (tm->accepts() ? "ACCEPTED" : "NOT ACCEPTED") << std::endl;

    word = "ababbb";
    tape = std::make_shared<OneHeadTape>("B"+word+"B", 1);
    tm->loadTapes(std::move(tape), parser->getVars().size());

    std::cout << word << " is " << (tm->accepts() ? "ACCEPTED" : "NOT ACCEPTED") << std::endl;

    is.close();
    return 0;

}