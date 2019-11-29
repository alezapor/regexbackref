cmake_minimum_required(VERSION 3.12)
project(regex_matcher)

set(CMAKE_CXX_STANDARD 14)

add_executable(regex_matcher main.cpp lexer.cpp lexer.h parser.cpp parser.h ast.h tape.cpp tape.h ndtm.cpp ndtm.h)