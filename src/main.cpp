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
    tm->setMVarSize(parser->getVars().size());
    tm->setInput(parser->getInput());

    item->constructTM(tm, tapes, parser->getVars(), tm->getMInitialState(), tm->getMFinalState());
    tm->print();

    std::string word = "acac"; //yes
    tm->initialize(word);
    std::cout << word << " is " << (tm->accepts() ? "ACCEPTED" : "NOT ACCEPTED") << std::endl;

    word = "aacaaccbbbcbbbc"; //yes
    tm->initialize(word);
    std::cout << word << " is " << (tm->accepts() ? "ACCEPTED" : "NOT ACCEPTED") << std::endl;

    word = "aacaaccbbcbbbc"; //no
    tm->initialize(word);
    std::cout << word << " is " << (tm->accepts() ? "ACCEPTED" : "NOT ACCEPTED") << std::endl;

    is.close();
    return 0;

}