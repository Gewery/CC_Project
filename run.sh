#!/usr/bin/env bash
bison -d --output yacc.cpp yacc.y
g++ -o parser AST.cpp Semantic.cpp yacc.cpp lex.cpp
cat input.txt | ./parser
