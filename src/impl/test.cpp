#define BOOST_TEST_MODULE regex_final_test

#include <boost/test/included/unit_test.hpp>

#include "matcher.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <optional>
#include <utility>

const char* OPTION_TM = "0";
const char* OPTION_MA = "1";
const char* OPTION_AVD = "2";


BOOST_AUTO_TEST_CASE(simple)
{
    {
        const char* regex = "a";
        const char* text = "a";
        bool res = true;
        BOOST_TEST(matches(regex, OPTION_TM, text) == res);
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

    {
        const char* regex = "a*";
        const char* text = "aaaaabaa";
        bool res = false;
        BOOST_TEST(matches(regex, OPTION_TM, text) == res);
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

    {
        const char* regex = "X{a*b*}bX";
        const char* text = "aabbbaabbb";
        bool res = false;
        BOOST_TEST(matches(regex, OPTION_TM, text) == res);
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }
}


BOOST_AUTO_TEST_CASE(simpleReg)
{
    {
        const char* regex = "(X{a+b*}X)*";
        const char* text = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbb";
        bool res = true;
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

    {
        const char* regex = "X{a+b*}X";
        const char* text = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        bool res = false;
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

}

BOOST_AUTO_TEST_CASE(nVar)
{
    {
        const char* regex = "(A{a}+B{b}+C{ab}+D{aa}+E{ba}+F{bb}+G{a*}+H{b*})ABCDEFGH";
        const char* text = "baba";
        bool res = true;
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

    {
        const char* regex = "(A{a}+B{b}+C{ab}+D{aa}+E{ba}+F{bb}+G{a*}+H{b*})ABCDEFGH";
        const char* text = "";
        bool res = true;
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

}

BOOST_AUTO_TEST_CASE(nSigma)
{
    {
        const char* regex = "(X{c*defrga*b})X*";
        const char* text = "cccdefrgabcccdefrgabcdefrgab";
        bool res = false;
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

    {
        const char* regex = "(((a+b+c+d+e+f+g+h)*)*)*";
        const char* text = "asdafdsafgfv";
        bool res = false;
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

}


BOOST_AUTO_TEST_CASE(avd)
{
    {
        const char* regex = "A{B{C{b*}}}AC{a}";
        const char* text = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbab";
        bool res = false;
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

    {
        const char* regex = "(X{Y{b*}}+c*)Y{a*}Y";
        const char* text = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        bool res = true;
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

}

BOOST_AUTO_TEST_CASE(hard)
{
    {
        const char* regex = "(A{a}+B{b}+F{a*}+G{b*})ABFG";
        const char* text = "baaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        bool res = false;
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

    {
        const char* regex = "B{ab}(A{a}+B{b}+F{a*}+G{b*})ABFG";
        const char* text = "abbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
        bool res = false;
        BOOST_TEST(matches(regex, OPTION_MA, text) == res);
        BOOST_TEST(matches(regex, OPTION_AVD, text) == res);
    }

}