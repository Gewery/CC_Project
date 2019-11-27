#!/usr/bin/env bash
bison -d --output ./src/SyntaxAnalysis/yacc.cpp ./src/SyntaxAnalysis/yacc.y
g++ -o parser ./src/SyntaxAnalysis//AST.cpp ./src/SemanticAnalysis/Semantic.cpp ./src/SyntaxAnalysis/yacc.cpp ./src/LexicalAnalysis/lex.cpp
cat input.txt | ./parser
