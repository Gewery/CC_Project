#!/usr/bin/env bash
bison -d --output yacc.cpp yacc.y
g++ yacc.cpp lex.cpp AST.h AST.cpp -o parser -Wno-c++11-extensions
chmod +x ./parser
cat input.txt | ./parser