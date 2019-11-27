#include <stdio.h>
#include <algorithm>
#include <ctype.h>
#include <vector>
#include <string>
#include <regex>
#include <iostream>

#include "lexems.h"
#include "yacc.hpp"


using namespace std;

void yyerror(const char *);

bool flag = 0;
int column = 0;
static int seeneof = 0;

FILE *yyin;
vector<string> yyextra_strings;
vector<float> yyextra_floats;
vector<int> yyextra_ints;

vector<pair<int, regex> > regex_tokens;
string source;

void read_file() {
	if (flag)
		return;
	flag = 1;
    string st;
    while (getline(cin, st))
        source += st + "\n";

    regex_tokens.push_back(make_pair(
            TokenTypes::DECLARATION_SEPARATORS,
            regex("^\\s*(;)([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(
            TokenTypes::KEYWORDS,
            regex("^\\s*(var|is|end|in|reverse|while|for|from|loop|if|then|else|real|boolean|integer|type|record|routine)(?![a-zA-Z0-9_])([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(
            TokenTypes::BOOLEAN_LITERALS,
            regex("^\\s*(false|true)(?![a-zA-Z0-9_])([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(
            TokenTypes::OPERATORS,
            regex("^\\s*((?:not|and|x?or)(?![a-zA-Z0-9_])|(?:[:><\\/])=?|(?:[\\*\\+\\-=%]))([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(
            TokenTypes::PUNCTUATORS,
            regex("^\\s*(\\.{1,2}|[\\()\\[\\]:,])([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(
            TokenTypes::IDENTIFIERS,
            regex("^\\s*([a-zA-Z_][a-zA-Z0-9_]*)([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(
            TokenTypes::FLOATING_LITERALS,
            regex("^\\s*([0-9]+\\.(?:[0-9])*|\\.[0-9]+)(?![a-zA-Z0-9_\\.])([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(
            TokenTypes::INTEGER_LITERALS,
            regex("^\\s*([0-9]+)(?![a-zA-Z0-9_\\.])([\\s\\S]*)")));
}

pair <string, pair<string, int>> find_next_token(string input_string) {
    smatch match;
    for (const auto &regex_token: regex_tokens) {
        if (regex_match(input_string, match, regex_token.second)) {
            string lexem = match[1].str() == "\n" ? "\\n" : match[1].str();  // print newline as '\n'
            cout << lexem << " | " << TokenTypeNames[regex_token.first] << endl;
            return make_pair(match[2].str(), make_pair(match[1].str(), regex_token.first));
        }
    }
    return make_pair(string(), make_pair(string(), TokenTypes::UNDEFINED));
}

extern int yylex(void) {
    read_file();
    if (!yyin)
        yyin = stdin;
    if (seeneof)
        return 0;

    auto res = find_next_token(source);
    source = res.first;
    string lexem = res.second.first;
    int token_type = res.second.second;

    //TOKENS
    if (lexem.size() == 0) {
    	seeneof = 1;
    	return 0;
    }
    else if (token_type == TokenTypes::DECLARATION_SEPARATORS) {
        return DECLARATION_SEPARATOR;
    }
    else if (token_type == TokenTypes::KEYWORDS) {
        if      (lexem == Lexems::Keywords::VAR)     return VAR;
        else if (lexem == Lexems::Keywords::IS)      return IS;
        else if (lexem == Lexems::Keywords::END)     return END;
        else if (lexem == Lexems::Keywords::IN)      return IN;
        else if (lexem == Lexems::Keywords::REVERSE) return REVERSE;
        else if (lexem == Lexems::Keywords::WHILE)   return WHILE;
        else if (lexem == Lexems::Keywords::FOR)     return FOR;
        else if (lexem == Lexems::Keywords::FROM)    return FROM;
        else if (lexem == Lexems::Keywords::LOOP)    return LOOP;
        else if (lexem == Lexems::Keywords::IF)      return IF;
        else if (lexem == Lexems::Keywords::THEN)    return THEN;
        else if (lexem == Lexems::Keywords::ELSE)    return ELSE;
        else if (lexem == Lexems::Keywords::REAL)    return REAL;
        else if (lexem == Lexems::Keywords::BOOLEAN) return BOOLEAN;
        else if (lexem == Lexems::Keywords::INTEGER) return INTEGER;
        else if (lexem == Lexems::Keywords::TYPE)    return TYPE;
        else if (lexem == Lexems::Keywords::RECORD)  return RECORD;
        else if (lexem == Lexems::Keywords::ARRAY)   return ARRAY;
        else if (lexem == Lexems::Keywords::ROUTINE) return ROUTINE;
    }
    else if (token_type == TokenTypes::BOOLEAN_LITERALS) {
        if      (lexem == Lexems::BooleanLiterals::TRUE)  return TRUE;
        else if (lexem == Lexems::BooleanLiterals::FALSE) return FALSE;
    }
    else if (token_type == TokenTypes::OPERATORS) {
        if      (lexem == Lexems::Operators::NOT)        return NOT;
        else if (lexem == Lexems::Operators::AND)        return AND;
        else if (lexem == Lexems::Operators::OR)         return OR;
        else if (lexem == Lexems::Operators::XOR)        return XOR;
        else if (lexem == Lexems::Operators::MULT)       return MULT;
        else if (lexem == Lexems::Operators::PLUS)       return PLUS;
        else if (lexem == Lexems::Operators::MINUS)      return MINUS;
        else if (lexem == Lexems::Operators::EQ)         return EQ;
        else if (lexem == Lexems::Operators::REMAINDER)  return REMAINDER;
        else if (lexem == Lexems::Operators::COLON)      return COLON;
        else if (lexem == Lexems::Operators::ASSIGN)     return ASSIGN;
        else if (lexem == Lexems::Operators::GREATER)    return GREATER;
        else if (lexem == Lexems::Operators::GREATER_EQ) return GREATER_EQ;
        else if (lexem == Lexems::Operators::LESS)       return LESS;
        else if (lexem == Lexems::Operators::LESS_EQ)    return LESS_EQ;
        else if (lexem == Lexems::Operators::DIV)        return DIV;
        else if (lexem == Lexems::Operators::NOT_EQ)     return NOT_EQ;
    }
    else if (token_type == TokenTypes::PUNCTUATORS) {
        if      (lexem == Lexems::Punctuators::DOT)           return DOT;
        else if (lexem == Lexems::Punctuators::RANGER)        return RANGER;
        else if (lexem == Lexems::Punctuators::COLON)         return COLON;
        else if (lexem == Lexems::Punctuators::COMMA)         return COMMA;
        else if (lexem == Lexems::Punctuators::PARENTHESES_L) return PARENTHESES_L;
        else if (lexem == Lexems::Punctuators::PARENTHESES_R) return PARENTHESES_R;
        else if (lexem == Lexems::Punctuators::BRACKETS_L)    return BRACKETS_L;
        else if (lexem == Lexems::Punctuators::BRACKETS_R)    return BRACKETS_R;
    }
    else if (token_type == TokenTypes::IDENTIFIERS) {
        yyextra_strings.push_back(lexem);
        return IDENTIFIER;
    }
    else if (token_type == TokenTypes::INTEGER_LITERALS) {
        int yyextra_int = 0;
        for (int i = 0; i < lexem.size(); yyextra_int = yyextra_int * 10 + lexem[i] - '0', i++);
        yyextra_ints.push_back(yyextra_int);
        return INTEGER_LITERAL;
    }
    else if (token_type == TokenTypes::FLOATING_LITERALS) {
        float yyextra_float = 0;
        float exp = 0.1;
        bool flag = 0;
        for (int i = 0; i < lexem.size(); i++) {
            if (lexem[i] == '.') {
                flag = 1;
                continue;
            }
            if (!flag) {
                yyextra_float *= 10;
                yyextra_float += lexem[i] - '0';
            }
            else {
                yyextra_float += (lexem[i] - '0') * exp;
                exp /= 10;
            }
        }
        yyextra_floats.push_back(yyextra_float);
        return REAL_LITERAL;
    }
    if (lexem != "\0") yyerror("Mystery character\n");
    return 0;
}

void yyerror(char const *s){
    fflush(stdout);
    printf("\n%*s\n%*s\n", column, "^", column, s);
}

string get_yyextra_string() {
    if (yyextra_strings.empty()) {
        cout << "Lexer error. Trying to get non-existent yyextra_string.";
        exit(1);
    }
    string yyextra_string = yyextra_strings.back();
    yyextra_strings.pop_back();
    return yyextra_string;
}

int get_yyextra_int() {
    if (yyextra_ints.empty()) {
        cout << "Lexer error. Trying to get non-existent yyextra_int.";
        exit(1);
    }
    int yyextra_int = yyextra_ints.back();
    yyextra_ints.pop_back();
    return yyextra_int;
}

float get_yyextra_float() {
    if (yyextra_floats.empty()) {
        cout << "Lexer error. Trying to get non-existent yyextra_float.";
        exit(1);
    }
    float yyextra_float = yyextra_floats.back();
    yyextra_floats.pop_back();
    return yyextra_float;
}