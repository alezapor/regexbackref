
#include <cstring>
#include "matcher.h"

Matcher::Matcher() {

}

Matcher::Matcher(std::unique_ptr<Parser> parser, const char *option) :
        m_Parser(std::move(parser)) {
    std::vector<bool> tapes;
    std::map<char, int> memory;
    std::set<int> avd;
    m_Parser->getNextToken();


    m_Root = std::move(m_Parser->ParseA());

    if (strcmp(option, "1") == 0) { // simpleMemory
        std::shared_ptr<AvdFA> avdFA = std::make_shared<AvdFA>();
        m_Root->constructAvdFA(avdFA, avd, avdFA->getMInitialState(), *avdFA->getMFinalStates().begin());


        m_Simulator = avdFA->simpleMemory(m_Parser->getVars());

    } else if (strcmp(option, "0") == 0) { //simpleTM
        tapes.resize(m_Parser->getVars().size(), false);

        for (auto it = m_Parser->getVars().begin(); it != m_Parser->getVars().end(); it++) {
            memory[*it] = (int) std::distance(m_Parser->getVars().begin(), it) + 1;
        }
        NDTM *tm = new NDTM();
        tm->setMTapeCnt((int) m_Parser->getVars().size() + 1);
        tm->setInput(m_Parser->getInput());
        m_Root->constructTM(tm, tapes, memory, tm->getMInitialState(),
                            *tm->getMFinalStates().begin());
        m_Simulator = tm;
    } else if (strcmp(option, "2") == 0) { // avdMemory
        int avdmax = 0;
        std::shared_ptr<AvdFA> avdFA = std::make_shared<AvdFA>();
        m_Root->constructAvdFA(avdFA, avd, avdFA->getMInitialState(), *avdFA->getMFinalStates().begin());

        // avd computing 
        for (auto it = avd.begin(); it != avd.end(); it++) {
            int avdState = 0;
            std::vector<char> avs;
            for (auto it1 = m_Parser->getVars().begin(); it1 != m_Parser->getVars().end(); it1++) {
                if (avdFA->constructR0(*it, *it1)->accepts() && avdFA->constructR1(*it, *it1)->accepts()) {
                    avdState++;
                    avs.push_back(*it1);
                }
            }
            if (avdState > avdmax) avdmax = avdState;
        }

        m_Simulator = avdFA->avdMemory(m_Parser->getVars(), avdmax);
        // m_Simulator->print();


    } else {
        printf("Usage ./regex_matcher <option> <regex> <word>\n <option>: use 0 for the simpleTM, 1 for simpleMemory and 2 for avdMemory algorithm\n");
        exit(1);
    }

}


bool Matcher::match(std::string w) {
    m_Simulator->initialize(w);
    return m_Simulator->accepts();
}

Matcher::~Matcher() {
    delete m_Simulator;
}

bool matches(const char* regex, const char* option, const char* text) {
    auto matcher = std::make_unique<Matcher>(std::make_unique<Parser>(regex), option);
    return matcher->match(text);
}

