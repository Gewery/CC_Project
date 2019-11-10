
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DECLARATION_SEPARATOR = 258,
     VAR = 259,
     IS = 260,
     END = 261,
     IN = 262,
     REVERSE = 263,
     WHILE = 264,
     FOR = 265,
     FROM = 266,
     LOOP = 267,
     IF = 268,
     THEN = 269,
     ELSE = 270,
     REAL = 271,
     BOOLEAN = 272,
     INTEGER = 273,
     TYPE = 274,
     RECORD = 275,
     ROUTINE = 276,
     ARRAY = 277,
     TRUE = 278,
     FALSE = 279,
     PLUS = 280,
     MINUS = 281,
     MULT = 282,
     DIV = 283,
     REMAINDER = 284,
     NOT = 285,
     AND = 286,
     OR = 287,
     XOR = 288,
     EQ = 289,
     NOT_EQ = 290,
     GREATER = 291,
     GREATER_EQ = 292,
     LESS = 293,
     LESS_EQ = 294,
     ASSIGN = 295,
     DOT = 296,
     RANGER = 297,
     COLON = 298,
     COMMA = 299,
     PARENTHESES_L = 300,
     PARENTHESES_R = 301,
     BRACKETS_L = 302,
     BRACKETS_R = 303,
     IDENTIFIER = 304,
     INTEGER_LITERAL = 305,
     REAL_LITERAL = 306
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


