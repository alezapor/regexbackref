#include <iostream>
#include <fstream>
#include "parser.h"
#include "matcher.h"

int main(int argc, char * argv[]) {

    std::fstream is;
    is.open(argv[1], std::fstream::in);
    if(!is){
        printf("Cannot open file. Set working directory as in. Usage %s <regex_file>\n", argv[0]);
        exit(1);
    }

    std::shared_ptr<Parser> parser = std::make_shared<Parser>(&is);
    std::shared_ptr<NDTM> tm = std::make_shared<NDTM>();
    std::shared_ptr<Matcher> matcher = std::make_shared<Matcher>(std::move(parser), tm);
    matcher->match("acac");
    matcher->match("aacaaccbbbcbbbc");
    matcher->match("aacaaccbbcbbbc");

    is.close();
    return 0;

}