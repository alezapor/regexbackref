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

    auto item = parser->ParseS();
    item->print();

    std::vector<bool> tapes;
    tapes.resize(parser->getParCnt(), false);

    std::shared_ptr<NDTM> tm = std::make_shared<NDTM>();
    tm->setInput(parser->getInput());

    item->constructTM(tm, tapes);
    tm->addFinalState(tm->getMCur());

    std::string word = "abbbb";
    std::unique_ptr<Tape> tape= std::make_unique<Tape>(word, 0);
    tm->loadTapes(std::move(tape), parser->getParCnt());
    tm->print();
    std::cout << word << " " << (tm->accepts() ? "ACCEPTED" : "NOT ACCEPTED") << std::endl;

    is.close();
    return 0;

}