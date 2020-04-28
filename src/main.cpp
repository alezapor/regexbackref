#include <iostream>
#include <sstream>
#include <cstring>
#include "parser.h"
#include "matcher.h"

int main(int argc, char *argv[]) {

    std::istringstream is;
    std::shared_ptr<Parser> parser;
    std::shared_ptr<NDTM> tm;
    std::shared_ptr<Matcher> matcher;

    is.str(argv[2]);

    if (argc == 4 && strcmp(argv[1], "-") == 0) {
        is.str(argv[2]);
        parser = std::make_shared<Parser>(&is);
        std::shared_ptr<NDTM> tm = std::make_shared<NDTM>();
        std::shared_ptr<Matcher> matcher = std::make_shared<Matcher>(std::move(parser), tm, true);
        std::cout << argv[3] << (matcher->match(argv[3])? " matches ": " does not match ") << argv[2] << std::endl;
    } else if (argc == 3) {
        is.str(argv[1]);
        parser = std::make_shared<Parser>(&is);
        std::shared_ptr<NDTM> tm = std::make_shared<NDTM>();
        std::shared_ptr<Matcher> matcher = std::make_shared<Matcher>(std::move(parser), tm, false);
        std::cout << argv[2] << (matcher->match(argv[2])? " matches ": " does not match ") << argv[1] << std::endl;
    } else {
        printf("Usage %s [-] <regex> <word>\n Use - for avdTM algorithm\n", argv[0]);
        exit(1);
    }


    return 0;

}