#include <stdio.h>
#include <algorithm>
#include <ctype.h>
#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include "fb1-5.tab.h"

using namespace std;

FILE *yyin;
static int seeneof = 0;
string source;

typedef std::pair<std::string, std::regex> t_regex_pair;
std::vector<t_regex_pair> regex_tokens;

void read_file() {
    string st;
    while (getline(cin, st))
        source += st + "\n";

    regex_tokens.push_back(t_regex_pair("keywords", "^\\s*(var|is|end|in|reverse|while|for|from|loop|if|then|else|real|boolean|integer|type|record|routine)(?![a-zA-Z0-9_])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("boolean literals", "^\\s*(false|true)(?![a-zA-Z0-9_])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("operators", "^\\s*((?:not|and|x?or)(?![a-zA-Z0-9_])|(?:[:><\\/])=?|(?:[\\*\\+\\-=%]))([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("identifiers", "^\\s*([a-zA-Z_][a-zA-Z0-9_]*)([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("punctuators", "^\\s*(\\.{1,2}|[\\()\\;:,])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("floating literals", "^\\s*([0-9]+\\.(?:[0-9])*|\\.[0-9]+)(?![a-zA-Z0-9_\\.])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("integer literals", "^\\s*([0-9]+)(?![a-zA-Z0-9_\\.])([\\s\\S]*)"));
}

pair<string, pair<string, string>> find_next_token(std::string input_string) {
  smatch match;
  for (const auto& regex_token: regex_tokens) {
    if (regex_match(input_string, match, regex_token.second)) {
      cout << match[1].str() << " | " << regex_token.first << std::endl;
      return {match[2].str(), {match[1].str(), regex_token.first}};
    }
  }
  return {string(), {"", ""}};
}

int yylex(void)
{
    if (!yyin)
        yyin = stdin;
    if (seeneof)
        return 0;

    auto res = find_next_token(source);
    source = res.first;
    string lexem = res.second.first;
    string token_name = res.second.second;

    //IDENTIFIER
    if (token_name == "keywords") {
        if (lexem == "var")
            return VAR;
        else if (lexem == "is")
            return IS;
        else if (lexem == "end")
            return END;
        else if (lexem == "in")
            return IN;
        else if (lexem == "reverse")
            return REVERSE;
        else if (lexem == "while")
            return WHILE;
        else if (lexem == "for")
            return FOR;
        else if (lexem == "from")
            return FROM;
        else if (lexem == "loop")
            return LOOP;
        else if (lexem == "if")
            return IF;
        else if (lexem == "then")
            return THEN;
        else if (lexem == "else")
            return ELSE;
        else if (lexem == "real")
            return REAL;
        else if (lexem == "boolean")
            return BOOLEAN;
        else if (lexem == "integer")
            return INTEGER;
        else if (lexem == "type")
            return TYPE;
        else if (lexem == "record")
            return RECORD;
        else if (lexem == "routine")
            return ROUTINE;
    }
    else if (token_name == "boolean literals") {
        if (lexem == "true")
            return TRUE;
        else if (lexem == "false")
            return FALSE;
    }
    else if (token_name == "operators") {
        if (lexem == "not")
            return NOT;
        else if (lexem == "and")
            return AND;
        else if (lexem == "or")
            return OR;
        else if (lexem == "xor")
            return XOR;
        else
            string yylval = lexem;  // todo make new tokens for the remaining operators (>= / + / -  etc)
            return IDENTIFIER;
    }
    else if (token_name == "punctuators") {
        string yylval = lexem;  // todo make new tokens for punctuators (.. / ; / , / :  etc)
        return IDENTIFIER;
    }
    else if (token_name == "integer literals") {
        int yylval = 0;
        for (int i = 0; i < lexem.size(); yylval = yylval * 10 + lexem[i] - '0', i++);
        return INTEGER_LITERAL;
    }
    else if (token_name == "floating literals") {
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
//    else if (token_name != "identifiers") {
//        string yylval = lexem;
//        return IDENTIFIER;
//    }
    else {
        yyerror("Mystery character %c\n", c);
    }
}

int main()
{
    yyin = fopen("input.txt", "r");
    int res = 1;
    while (res != 0)
    {
        res = yylex();
        cout << res << " next ";
    }
}