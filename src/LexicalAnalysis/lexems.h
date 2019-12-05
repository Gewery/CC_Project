using namespace std;

enum TokenTypes {
    UNDEFINED = -1,
    DECLARATION_SEPARATORS, KEYWORDS, BOOLEAN_LITERALS, OPERATORS,
    PUNCTUATORS, IDENTIFIERS, FLOATING_LITERALS, INTEGER_LITERALS
};
static constexpr const char *TokenTypeNames[] = {"declaration separator", "keyword", "boolean literal", "operator",
                                                 "punctuator", "identifier", "floating literal", "integer literal"};

struct Lexems {
    struct Keywords {
        static constexpr const char *VAR     = "var";
        static constexpr const char *IS      = "is";
        static constexpr const char *END     = "end";
        static constexpr const char *IN      = "in";
        static constexpr const char *REVERSE = "reverse";
        static constexpr const char *WHILE   = "while";
        static constexpr const char *FOR     = "for";
        static constexpr const char *FROM    = "from";
        static constexpr const char *LOOP    = "loop";
        static constexpr const char *IF      = "if";
        static constexpr const char *THEN    = "then";
        static constexpr const char *ELSE    = "else";
        static constexpr const char *REAL    = "real";
        static constexpr const char *BOOLEAN = "boolean";
        static constexpr const char *INTEGER = "integer";
        static constexpr const char *TYPE    = "type";
        static constexpr const char *RECORD  = "record";
        static constexpr const char *ARRAY   = "array";
        static constexpr const char *ROUTINE = "routine";
        static constexpr const char *RETURN = "return";
    };
    struct BooleanLiterals {
        static constexpr const char *TRUE  = "true";
        static constexpr const char *FALSE = "false";
    };
    struct Operators {
        static constexpr const char *NOT        = "not";
        static constexpr const char *AND        = "and";
        static constexpr const char *OR         = "or";
        static constexpr const char *XOR        = "xor";
        static constexpr const char *MULT       = "*";
        static constexpr const char *PLUS       = "+";
        static constexpr const char *MINUS      = "-";
        static constexpr const char *EQ         = "=";
        static constexpr const char *REMAINDER  = "%";
        static constexpr const char *COLON      = ":";
        static constexpr const char *ASSIGN     = ":=";
        static constexpr const char *GREATER    = ">";
        static constexpr const char *GREATER_EQ = ">=";
        static constexpr const char *LESS       = "<";
        static constexpr const char *LESS_EQ    = "<=";
        static constexpr const char *DIV        = "/";
        static constexpr const char *NOT_EQ     = "/=";
    };
    struct Punctuators {
        static constexpr const char *DOT           = ".";
        static constexpr const char *RANGER        = "..";
        static constexpr const char *COLON         = ":";
        static constexpr const char *COMMA         = ",";
        static constexpr const char *PARENTHESES_L = "(";
        static constexpr const char *PARENTHESES_R = ")";
        static constexpr const char *BRACKETS_L    = "[";
        static constexpr const char *BRACKETS_R    = "]";
    };
};