#include <iostream>
#include <fstream>
#include "parser.h"

int main(int argc, char * argv[]) {

    std::fstream is;
    is.open(argv[1], std::fstream::in);
    if(!is){
        printf("Cannot open file.\n");
        exit(1);
    }

    Parser parser(&is);
    parser.getNextToken();
    auto item = parser.ParseS();
    item->print();
    std::vector<bool> tapes;
    tapes.resize(parser.getParCnt(), false);

    NDTM tm = NDTM(0, parser.getParCnt());
    tm.setABC(parser.getAbc());
    item->constructTM(tm, tapes);
    tm.addFinalState(tm.getMCur());

    tm.loadTapes(Tape(0, "abbcc"), parser.getParCnt());
    tm.print();
    std::cout << (tm.accepts() ? "ACCEPTED" : "NOT ACCEPTED") << std::endl;

    is.close();
    return 0;

}