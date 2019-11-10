#include <stdio.h>
#include <algorithm>
#include <ctype.h>
#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include "yacc.hpp"

using namespace std;

extern void yyerror(const char *);
FILE *yyin;
static int seeneof = 0;
int column = 0;
string source;
vector<pair<string, regex> > regex_tokens;
bool flag = 0;

void read_file() {
	if (flag)
		return;
	flag = 1;
    string st;
    while (getline(cin, st))
        source += st + "\n";

    regex_tokens.push_back(make_pair(string("declaration_separators"),
            regex("^\\s*([;\\n])([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(string("keywords"),
            regex("^\\s*(var|is|end|in|reverse|while|for|from|loop|if|then|else|real|boolean|integer|type|record|routine)(?![a-zA-Z0-9_])([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(string("boolean_literals"),
            regex("^\\s*(false|true)(?![a-zA-Z0-9_])([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(string("operators"),
            regex("^\\s*((?:not|and|x?or)(?![a-zA-Z0-9_])|(?:[:><\\/])=?|(?:[\\*\\+\\-=%]))([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(string("punctuators"),
            regex("^\\s*(\\.{1,2}|[\\()\\[\\]:,])([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(string("identifiers"),
            regex("^\\s*([a-zA-Z_][a-zA-Z0-9_]*)([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(string("floating_literals"),
            regex("^\\s*([0-9]+\\.(?:[0-9])*|\\.[0-9]+)(?![a-zA-Z0-9_\\.])([\\s\\S]*)")));
    regex_tokens.push_back(make_pair(string("integer_literals"),
            regex("^\\s*([0-9]+)(?![a-zA-Z0-9_\\.])([\\s\\S]*)")));
}

pair<string, pair<string, string> > find_next_token(string input_string) {
  smatch match;
  for (const auto& regex_token: regex_tokens) {
    if (regex_match(input_string, match, regex_token.second)) {
      cout << match[1].str() << " | " << regex_token.first << endl;
      return make_pair(match[2].str(), make_pair(match[1].str(), regex_token.first));
    }
  }
  return make_pair(string(), make_pair("", ""));
}

extern int yylex(void)
{
	read_file();
    if (!yyin)
        yyin = stdin;
    if (seeneof)
        return 0;

    auto res = find_next_token(source);
    source = res.first;
    string lexem = res.second.first;
    string token_name = res.second.second;

    //TOKENS
    if (token_name == "declaration_separators") {
        return DECLARATION_SEPARATOR;
    }
    else if (token_name == "keywords") {
        if      (lexem == "var")     return VAR;
        else if (lexem == "is")      return IS;
        else if (lexem == "end")     return END;
        else if (lexem == "in")      return IN;
        else if (lexem == "reverse") return REVERSE;
        else if (lexem == "while")   return WHILE;
        else if (lexem == "for")     return FOR;
        else if (lexem == "from")    return FROM;
        else if (lexem == "loop")    return LOOP;
        else if (lexem == "if")      return IF;
        else if (lexem == "then")    return THEN;
        else if (lexem == "else")    return ELSE;
        else if (lexem == "real")    return REAL;
        else if (lexem == "boolean") return BOOLEAN;
        else if (lexem == "integer") return INTEGER;
        else if (lexem == "type")    return TYPE;
        else if (lexem == "record")  return RECORD;
        else if (lexem == "array")  return ARRAY;
        else if (lexem == "routine") return ROUTINE;
    }
    else if (token_name == "boolean_literals") {
        if      (lexem == "true")  return TRUE;
        else if (lexem == "false") return FALSE;
    }
    else if (token_name == "operators") {
        if      (lexem == "not") return NOT;
        else if (lexem == "and") return AND;
        else if (lexem == "or")  return OR;
        else if (lexem == "xor") return XOR;
        else if (lexem == "*")   return MULT;
        else if (lexem == "+")   return PLUS;
        else if (lexem == "-")   return MINUS;
        else if (lexem == "=")   return EQ;
        else if (lexem == "%")   return REMAINDER;
        else if (lexem == ":")   return COLON;
        else if (lexem == ":=")  return ASSIGN;
        else if (lexem == ">")   return GREATER;
        else if (lexem == ">=")  return GREATER_EQ;
        else if (lexem == "<")   return LESS;
        else if (lexem == "<=")  return LESS_EQ;
        else if (lexem == "/")   return DIV;
        else if (lexem == "/=")  return NOT_EQ;
    }
    else if (token_name == "punctuators") {
        if      (lexem == ".")  return DOT;
        else if (lexem == "..") return RANGER;
        else if (lexem == ":")  return COLON;
        else if (lexem == ",")  return COMMA;
        else if (lexem == "(")  return PARENTHESES_L;
        else if (lexem == ")")  return PARENTHESES_R;
        else if (lexem == "[")  return BRACKETS_L;
        else if (lexem == "]")  return BRACKETS_R;
    }
    else if (token_name == "identifiers") {
        string yylval = lexem;
        return IDENTIFIER;
    }
    else if (token_name == "integer_literals") {
        int yylval = 0;
        for (int i = 0; i < lexem.size(); yylval = yylval * 10 + lexem[i] - '0', i++);
        return INTEGER_LITERAL;
    }
    else if (token_name == "floating_literals") {
        float yylval = 0;
        float exp = 0.1;
        bool flag = 0;
        for (int i = 0; i < lexem.size(); i++) {
            if (lexem[i] == '.') {
                flag = 1;
                continue;
            }
            if (!flag) {
                yylval *= 10;
                yylval += lexem[i] - '0';
            }
            else {
                yylval += (lexem[i] - '0') * exp;
                exp /= 10;
            }
        }
        return REAL_LITERAL;
    }
    yyerror("Mystery character\n");
    return 0;
}
//
//int main()
//{
//    yyin = fopen("input.txt", "r");
//    int res = 1;
//    while (res != 0)
//    {
//        res = yylex();
//        cout << res << " next ";
//    }
//}