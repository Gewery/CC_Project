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

    regex_tokens.push_back(t_regex_pair("keywords", "^\\s*(alignas|alignof|asm|auto|bool|break|case|catch|char|char16_t|char32_t|class|const|constexpr|const_cast|continue|"
                                                    "decltype|default|delete|do|double|dynamic_cast|else|enum|explicit|export|extern|float|for|friend|goto|if|inline|int|long|mutable|namespace|new|noexcept|"
                                                    "operator|private|protected|public|register|reinterpret_cast|return|short|signed|sizeof|static|static_assert|static_cast|struct|switch|template|this|thread_local|throw|"
                                                    "try|typedef|typeid|typename|union|unsigned|using|virtual|void|volatile|wchar_t|while)(?![a-zA-Z0-9_])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("boolean literals", "^\\s*(false|true)(?![a-zA-Z0-9_])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("pointer literals", "^\\s*(nullptr)(?![a-zA-Z0-9_])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("comments", "^\\s*(//.*|/\\*(?:(?!\\*/)[\\s\\S])*\\*/)([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("operators", "^\\s*((?:bitand|bitor|compl|(?:and|not|x?or)(?:_eq)?)(?![a-zA-Z0-9_])|(?:[><*%+&|^=/\\-]?)=|\\.\\*|->\\*|\\?:|::|->|"
                                                     "\\+{1,2}|-{1,2}|&{1,2}|<(?![:%])|<<=?|>{1,2}=?|\\|{1,2}|%(?![:>])|\\.(?!\\.)|[?~!^\\*\\/])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("identifiers", "^\\s*([a-zA-Z_][a-zA-Z0-9_]*)([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("punctuators", "^\\s*([\\(){}\\[\\]\\\\;:,]|#{1,2}|<:|:>|<%|%>|(?:%:){1,2}|\\.{3})([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("floating literals", "^\\s*((?:(?:(?:[0-9](?:'?[0-9])*)*\\.[0-9](?:'?[0-9])*|[0-9](?:'?[0-9])*\\.)(?:[Ee][-+]?[0-9](?:'?[0-9])*)?|"
                                                             "(?:[0-9](?:'?[0-9])*)(?:[Ee][-+]?[0-9](?:'?[0-9])*))[FfLl]?)(?![a-zA-Z0-9_\\.])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("integer literals", "^\\s*((?:0[Bb](?:[01]'?)*[01]|0(?:[0-7]'?)*[0-7]|(?:[0-9]'?)*[0-9]|0[Xx](?:[0-9A-Fa-f]'?)*[0-9A-Fa-f])"
                                                            "(?:[Uu](?:l{1,2}|L{1,2})?|(?:l{1,2}|L{1,2})[Uu]?)?)(?![a-zA-Z0-9_\\.])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("character literals", "^\\s*((?:u8|u|U|L)?'(?:[^'\\\\\\n]|\\\\['\"?\\\\abfnrtv]|\\\\[0-8]{1,3}|\\\\x[0-9A-Fa-f]+)')(?![a-zA-Z0-9_])([\\s\\S]*)"));
    regex_tokens.push_back(t_regex_pair("string literals", "^\\s*((?:u8|u|U|L)?\"(?:[^\"\\\\\\n]|\\\\['\"?\\\\abfnrtv]|\\\\[0-8]{1,3}|\\\\x[0-9A-Fa-f]+)*\")(?![a-zA-Z0-9_])([\\s\\S]*)"));
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
    else if (lexem == "foreach")
        return FOREACH;
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
    else if (lexem == "true")
        return TRUE;
    else if (lexem == "false")
        return FALSE;
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
    else if (token_name != "identifiers") {
        string yylval = lexem;
        return IDENTIFIER;
    }
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