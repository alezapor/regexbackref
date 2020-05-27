#include <iostream>
#include <sstream>
#include <cstring>
#include "parser.h"
#include "matcher.h"

int main(int argc, char *argv[]) {

    std::istringstream is;
    std::shared_ptr<Parser> parser;


    is.str(argv[2]);

    if (argc == 4){
        is.str(argv[2]);
        parser = std::make_shared<Parser>(&is);
        std::shared_ptr<Matcher> matcher = std::make_shared<Matcher>(std::move(parser), argv[1]);
        //std::cout << argv[3] << (matcher->match(argv[3])? " matches ": " does not match ") << argv[2] << std::endl;
        std::cout << (matcher->match(argv[3])? "yes": "no") << std::endl;
    } else {
        printf("Usage %s <option> <regex> <word>\n <option>: use 0 for the simpleTM, 1 for simpleMemory and 2 for avdMemory algorithm\n", argv[0]);
        exit(1);
    }

    return 0;

}