#!/usr/bin/env bash
bison -d --output yacc.cpp yacc.y
g++ yacc.cpp lex.cpp -o parser -Wno-c++11-extensions
#rm yacc.cpp yacc.hpp
./parser input.txt
