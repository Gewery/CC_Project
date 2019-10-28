#!/usr/bin/env bash
bison -d --output yacc.cpp yacc.y
g++ yacc.cpp lex.cpp -o parser
rm yacc.cpp yacc.hpp
./parser input.txt
