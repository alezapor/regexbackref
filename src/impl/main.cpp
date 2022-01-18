#include <iostream>
#include <sstream>
#include <cstring>
#include "matcher.h"

#include "Config.h"


int main(int argc, char *argv[]) {

    std::cout << "Backreference Regex Matcher version " << RegexMatcher_VERSION_MAJOR << "." << RegexMatcher_VERSION_MINOR << "\n\n";

    if (argc == 4) {
        std::cout << argv[3] << (matches(argv[2], argv[1], argv[3])? " matches ": " does not match ") << argv[2] << std::endl;
        //std::cout << (matcher->match(argv[3]) ? "yes" : "no") << std::endl;
    } else {
        std::cout << "Usage " << argv[0] << " <option> <regex> <word>\n <option>: use 0 for the simpleTM, 1 for simpleMemory and 2 for avdMemory algorithm" << std::endl;
        exit(1);
    }

    return 0;

}
