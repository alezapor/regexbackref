cmake_minimum_required(VERSION 3.15)
project(regex_matcher)

set(CMAKE_CXX_STANDARD 14)

add_executable(regex_matcher src/main.cpp src/lexer.cpp src/lexer.h src/parser.cpp src/parser.h src/ast.h src/tape.cpp src/tape.h src/ndtm.cpp src/ndtm.h src/ast.cpp)