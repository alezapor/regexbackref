//
// Created by osboxes on 11/28/19.
//

#ifndef REGEX_MATCHER_NDTM_H
#define REGEX_MATCHER_NDTM_H


#include <set>
#include <vector>
#include <map>
#include "tape.h"

struct comp {
    bool operator() (const std::pair<int, std::string> &o1, const std::pair<int, std::string>& o2){
        if (o1.first == o2.first) return o1.second < o2.second;
        return o1.first < o2.first;
    }
};


class NDTM {

public:
    NDTM(int mStart, int tapesCnt);

    NDTM(int mStart, const std::set<int> &mEnd, int mCur, std::vector<Tape> mTapes,
         const std::map<std::pair<int, std::string>, std::vector<std::pair<int, std::vector<std::pair<char, int>>>>, comp> &mTransitions, int cnt);

    void loadTapes(Tape t, int tapesCnt);
    std::string readSymbols();
    void addTransition(int state, std::string readSym, int newState, std::vector<std::pair<char, int>> moves);
    void execTransition(std::pair<int, std::vector<std::pair<char, int>>>);
    void addFinalState(int state){
        m_End.insert(state);
    }
    bool accepts();
    NDTM clone();

    int getMCur();
    Tape & getTape(int i);
    void setMCur(int state);
    int getStateCnt();
    void incStateCnt() {m_Cnt++;}
    void print();
    std::set<char> & getABC(){ return m_Abc;}
    void setABC (std::set<char> & abc) {m_Abc = abc;}
private:
    int m_Start;
    std::set<int> m_End;
    std::set<char> m_Abc;
    int m_Cur;
    int m_Cnt;
    std::vector<Tape> m_Tapes;
    std::map  <std::pair<int, std::string>,
                          std::vector <std::pair <int, std::vector<std::pair<char, int>>>>, comp> m_Transitions;

};


#endif //REGEX_MATCHER_NDTM_H
