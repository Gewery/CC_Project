%{
    #include <stdio.h>
    #include <string>
    #include <map>
    #include <iostream>
    #include <fstream>
    #include "../SemanticAnalysis/SymbolTable.h"
    #include "../LexicalAnalysis/lexems.h"
    #include "../ILCodeGenerator/SymbolTableIL.h"

    int yylex();
    extern void yyerror(const char *);
    extern char yytext[];
    extern int column;
    extern string get_yyextra_string();
    extern float get_yyextra_float();
    extern int get_yyextra_int();

    Program *root;
%}
%union {
    char *str;
    char character;
    int integer;
    float float_value;

    struct Program *Program;
    struct Declaration *Declaration;
    struct SimpleDeclaration *SimpleDeclaration;
    struct VariableDeclaration *VariableDeclaration;
    struct InitialValue *InitialValue;
    struct TypeDeclaration *TypeDeclaration;
    struct Type *Type;
    struct PrimitiveType *PrimitiveType;
    struct ArrayType *ArrayType;
    struct RecordType *RecordType;
    struct VariableDeclarations *VariableDeclarations;
    struct RoutineDeclaration *RoutineDeclaration;
    struct ReturnInRoutine *ReturnInRoutine;
    struct Parameters *Parameters;
    struct ParameterDeclaration *ParameterDeclaration;
    struct ParametersDeclaration *ParametersDeclaration;
    struct TypeInRoutineDeclaration *TypeInRoutineDeclaration;
    struct BodyInRoutineDeclaration *BodyInRoutineDeclaration;
    struct Body *Body;
    struct Statement *Statement;
    struct Assignment *Assignment;
    struct RoutineCall *RoutineCall;
    struct ExpressionInRoutineCall *ExpressionInRoutineCall;
    struct ExpressionsInRoutineCall *ExpressionsInRoutineCall;
    struct WhileLoop *WhileLoop;
    struct ForLoop *ForLoop;
    struct Range *Range;
    struct Reverse *Reverse;
    struct IfStatement *IfStatement;
    struct ElseInIfStatement *ElseInIfStatement;
    struct Expression *Expression;
    struct MultipleRelationsInExpression *MultipleRelationsInExpression;
    struct LogicalOperator *LogicalOperator;
    struct Relation *Relation;
    struct ComparisonInRelation *ComparisonInRelation;
    struct ComparisonOperator *ComparisonOperator;
    struct Simple *Simple;
    struct Factors *Factors;
    struct SimpleOperator *SimpleOperator;
    struct Factor *Factor;
    struct Summands *Summands;
    struct Summand *Summand;
    struct Primary *Primary;
    struct Sign *Sign;
    struct ModifiablePrimary *ModifiablePrimary;
    struct Identifiers *Identifiers;
}

// ======== TOKENS ========

%token DECLARATION_SEPARATOR

%token VAR IS END IN REVERSE WHILE RETURN
%token FOR FROM LOOP IF THEN ELSE
%token REAL BOOLEAN INTEGER TYPE
%token RECORD ROUTINE ARRAY

%token TRUE FALSE

%token PLUS MINUS MULT DIV REMAINDER
%token NOT AND OR XOR
%token EQ NOT_EQ GREATER GREATER_EQ LESS LESS_EQ
%token ASSIGN

%token DOT RANGER COLON COMMA
%token PARENTHESES_L PARENTHESES_R
%token BRACKETS_L BRACKETS_R

%token IDENTIFIER INTEGER_LITERAL REAL_LITERAL

%type  <float_value> INTEGER_LITERAL REAL_LITERAL TRUE FALSE
%type  <str> IDENTIFIER LESS LESS_EQ EQ NOT_EQ GREATER GREATER_EQ NOT PLUS MINUS
%type  <Program> Program
%type  <Declaration> Declaration
%type  <SimpleDeclaration> SimpleDeclaration
%type  <VariableDeclaration> VariableDeclaration
%type  <InitialValue> InitialValue
%type  <TypeDeclaration> TypeDeclaration
%type  <Type> Type
%type  <PrimitiveType> PrimitiveType
%type  <ArrayType> ArrayType
%type  <RecordType> RecordType
%type  <VariableDeclarations> VariableDeclarations
%type  <RoutineDeclaration> RoutineDeclaration
%type  <ReturnInRoutine> ReturnInRoutine
%type  <Parameters> Parameters
%type  <ParameterDeclaration> ParameterDeclaration
%type  <ParametersDeclaration> ParametersDeclaration
%type  <TypeInRoutineDeclaration> TypeInRoutineDeclaration
%type  <BodyInRoutineDeclaration> BodyInRoutineDeclaration
%type  <Body> Body
%type  <Statement> Statement
%type  <Assignment> Assignment
%type  <RoutineCall> RoutineCall
%type  <ExpressionInRoutineCall> ExpressionInRoutineCall
%type  <ExpressionsInRoutineCall> ExpressionsInRoutineCall
%type  <WhileLoop> WhileLoop
%type  <ForLoop> ForLoop
%type  <Range> Range
%type  <Reverse> Reverse
%type  <IfStatement> IfStatement
%type  <ElseInIfStatement> ElseInIfStatement
%type  <Expression> Expression
%type  <MultipleRelationsInExpression> MultipleRelationsInExpression
%type  <LogicalOperator> LogicalOperator
%type  <Relation> Relation
%type  <ComparisonInRelation> ComparisonInRelation
%type  <ComparisonOperator> ComparisonOperator
%type  <Simple> Simple
%type  <Factors> Factors
%type  <SimpleOperator> SimpleOperator
%type  <Factor> Factor
%type  <Summands> Summands
%type  <Summand> Summand
%type  <Primary> Primary
%type  <Sign> Sign
%type  <ModifiablePrimary> ModifiablePrimary
%type  <Identifiers> Identifiers

// ======== OPERATOR PRECEDENCE ========

%nonassoc RANGER

%left AND OR
%left PLUS MINUS
%left MULT DIV REMAINDER

%right EQ ASSIGN

%start Program

%%

// ======== RULES ========

Program
    : Declaration Program                           { $$ = new Program($1, $2); root = $$; }
    | DECLARATION_SEPARATOR Program                 { $$ = $2; root = $$; }
    |                                               { $$ = NULL; root = $$; }
    ;

Declaration
    : SimpleDeclaration                             { $$ = new Declaration($1, NULL); }
    | RoutineDeclaration                            { $$ = new Declaration(NULL, $1); }
    ;

SimpleDeclaration
    : VariableDeclaration                           { $$ = new SimpleDeclaration($1, NULL); }
    | TypeDeclaration                               { $$ = new SimpleDeclaration(NULL, $1); }
    ;

VariableDeclaration
    : VAR IDENTIFIER COLON Type InitialValue        { $$ = new VariableDeclaration(get_yyextra_string(), $4, $5, NULL); }
    | VAR IDENTIFIER IS Expression                  { $$ = new VariableDeclaration(get_yyextra_string(), NULL, NULL, $4); }
    ;

InitialValue
    : IS Expression                                 { $$ = new InitialValue($2); }
    |                                               { $$ = NULL; }
    ;

TypeDeclaration
    : TYPE IDENTIFIER IS Type                       { $$ = new TypeDeclaration(get_yyextra_string(), $4); }
    ;

Type
    : PrimitiveType                                 { $$ = new Type($1, NULL, NULL, ""); }
    | ArrayType                                     { $$ = new Type(NULL, $1, NULL, ""); }
    | RecordType                                    { $$ = new Type(NULL, NULL, $1, ""); }
    | IDENTIFIER                                    { $$ = new Type(NULL, NULL, NULL, get_yyextra_string()); }
    ;

PrimitiveType
    : INTEGER                                       { $$ = new PrimitiveType(true, false, false); }
    | REAL                                          { $$ = new PrimitiveType(false, true, false); }
    | BOOLEAN                                       { $$ = new PrimitiveType(false, false, true); }
    ;

ArrayType
    : ARRAY BRACKETS_L Expression BRACKETS_R Type   { $$ = new ArrayType($3, $5); }
    | ARRAY BRACKETS_L BRACKETS_R Type              { $$ = new ArrayType(NULL, $4); }
    ;

RecordType
    : RECORD VariableDeclarations END               { $$ = new RecordType($2); }
    ;

VariableDeclarations
    : VariableDeclaration VariableDeclarations      { $$ = new VariableDeclarations($1, $2); }
    |                                               { $$ = NULL; }
    ;

RoutineDeclaration
    : ROUTINE IDENTIFIER PARENTHESES_L Parameters PARENTHESES_R TypeInRoutineDeclaration BodyInRoutineDeclaration { $$ = new RoutineDeclaration(get_yyextra_string(), $4, $6, $7); }
    | ROUTINE IDENTIFIER PARENTHESES_L Parameters PARENTHESES_R BodyInRoutineDeclaration  { $$ = new RoutineDeclaration(get_yyextra_string(), $4, NULL, $6); }
    ;

ReturnInRoutine
    : RETURN Expression                                                          { $$ = new ReturnInRoutine($2); }
    |                                                                            { $$ = NULL; }
    ;

Parameters
    :  ParameterDeclaration ParametersDeclaration     { $$ = new Parameters($1, $2); }
    |                                                 { $$ = NULL; }
    ;

ParameterDeclaration
    : IDENTIFIER COLON Type                                   { $$ = new ParameterDeclaration(get_yyextra_string(), $3); }
    ;

ParametersDeclaration
    : COMMA ParameterDeclaration ParametersDeclaration        { $$ = new ParametersDeclaration($2, $3); }
    |                                                         { $$ = NULL; }
    ;

TypeInRoutineDeclaration
    : COLON Type                                      { $$ = new TypeInRoutineDeclaration($2); }
    ;

BodyInRoutineDeclaration
    : IS Body ReturnInRoutine END                                   { $$ = new BodyInRoutineDeclaration($2, $3); }
    |                                                               { $$ = NULL; }
    ;

Body
    : SimpleDeclaration Body                        { $$ = new Body($1, NULL, $2); }
    | Statement Body                                { $$ = new Body(NULL, $1, $2); }
    |                                               { $$ = NULL; }
    ;

Statement
    : Assignment                                    { $$ = new Statement($1, NULL, NULL, NULL, NULL); }
    | RoutineCall                                   { $$ = new Statement(NULL, $1, NULL, NULL, NULL); }
    | WhileLoop                                     { $$ = new Statement(NULL, NULL, $1, NULL, NULL); }
    | ForLoop                                       { $$ = new Statement(NULL, NULL, NULL, $1, NULL); }
    | IfStatement                                   { $$ = new Statement(NULL, NULL, NULL, NULL, $1); }
    ;

Assignment
    : ModifiablePrimary ASSIGN Expression             { $$ = new Assignment($1, $3); }
    ;

RoutineCall
    : IDENTIFIER ExpressionInRoutineCall            { $$ = new RoutineCall(get_yyextra_string(), $2); }
    ;

ExpressionInRoutineCall
    : PARENTHESES_L Expression ExpressionsInRoutineCall PARENTHESES_R   { $$ = new ExpressionInRoutineCall($2, $3); }
    |                                                                   { $$ = NULL; }
    ;

ExpressionsInRoutineCall
    : COMMA Expression ExpressionsInRoutineCall        { $$ = new ExpressionsInRoutineCall($2, $3); }
    |                                                 { $$ = NULL; }
    ;

WhileLoop
    : WHILE Expression LOOP Body END                { $$ = new WhileLoop($2, $4); }
    ;

ForLoop
    : FOR IDENTIFIER IN Reverse Range LOOP Body END { $$ = new ForLoop(get_yyextra_string(), $4, $5, $7); }
    ;

Range
    : Expression RANGER Expression                      { $$ = new Range($1, $3); }
    ;

Reverse
    : REVERSE                                       { $$ = new Reverse(true); }
    |                                               { $$ = new Reverse(false); }
    ;

IfStatement
    : IF Expression THEN Body ElseInIfStatement END { $$ = new IfStatement($2, $4, $5); }
    ;

ElseInIfStatement
    : ELSE Body                                     { $$ = new ElseInIfStatement($2); }
    |                                               { $$ = NULL; }
    ;

Expression
    : Relation MultipleRelationsInExpression        { $$ = new Expression($1, $2); }
    ;

MultipleRelationsInExpression
    : LogicalOperator Relation MultipleRelationsInExpression    { $$ = new MultipleRelationsInExpression($1, $2, $3); }
    |                                                           { $$ = NULL; }
    ;

LogicalOperator
    : AND                                           { $$ = new LogicalOperator(Lexems::Operators::AND); }
    | OR                                            { $$ = new LogicalOperator(Lexems::Operators::OR); }
    | XOR                                           { $$ = new LogicalOperator(Lexems::Operators::XOR); }
    ;

Relation
    : Simple ComparisonInRelation                   { $$ = new Relation($1, $2); }
    ;

ComparisonInRelation
    : ComparisonOperator Simple                     { $$ = new ComparisonInRelation($1, $2); }
    |                                               { $$ = NULL; }
    ;

ComparisonOperator
    : LESS                                           { $$ = new ComparisonOperator(Lexems::Operators::LESS); }
    | LESS_EQ                                        { $$ = new ComparisonOperator(Lexems::Operators::LESS_EQ); }
    | GREATER                                        { $$ = new ComparisonOperator(Lexems::Operators::GREATER); }
    | GREATER_EQ                                     { $$ = new ComparisonOperator(Lexems::Operators::GREATER_EQ); }
    | EQ                                             { $$ = new ComparisonOperator(Lexems::Operators::EQ); }
    | NOT_EQ                                         { $$ = new ComparisonOperator(Lexems::Operators::NOT_EQ); }
    ;

Simple
    : Factor Factors                                { $$ = new Simple($1, $2); }
    ;

Factors
    : SimpleOperator Factor Factors                 { $$ = new Factors($1, $2, $3); }
    |                                               { $$ = NULL; }
    ;

SimpleOperator
    : MULT                                           { $$ = new SimpleOperator(Lexems::Operators::MULT); }
    | DIV                                            { $$ = new SimpleOperator(Lexems::Operators::DIV); }
    | REMAINDER                                      { $$ = new SimpleOperator(Lexems::Operators::REMAINDER); }
    ;

Factor
    : Summand Summands                               { $$ = new Factor($1, $2); }
    ;

Summands
    : Sign Summand Summands                         { $$ = new Summands($1, $2, $3); }
    |                                               { $$ = NULL; }
    ;

Summand
    : Primary                                       { $$ = new Summand($1, NULL); }
    | PARENTHESES_L Expression PARENTHESES_R        { $$ = new Summand(NULL, $2); }
    ;


Primary
    : INTEGER_LITERAL                               { $$ = new Primary(Lexems::Keywords::INTEGER, (float)get_yyextra_int(), false, NULL,  NULL); }
    | Sign INTEGER_LITERAL                          { $$ = new Primary(Lexems::Keywords::INTEGER, (float)get_yyextra_int(), false, $1, NULL); }
    | NOT INTEGER_LITERAL                           { $$ = new Primary(Lexems::Keywords::INTEGER, (float)get_yyextra_int(), true, NULL, NULL); }
    | REAL_LITERAL                                  { $$ = new Primary(Lexems::Keywords::REAL, get_yyextra_float(), false, NULL, NULL); }
    | Sign REAL_LITERAL                             { $$ = new Primary(Lexems::Keywords::REAL, get_yyextra_float(), false, $1, NULL); }
    | TRUE                                          { $$ = new Primary(Lexems::Keywords::BOOLEAN, (float)true, false, NULL, NULL); }
    | FALSE                                         { $$ = new Primary(Lexems::Keywords::BOOLEAN, (float)false, false, NULL, NULL); }
    | ModifiablePrimary                             { $$ = new Primary("", 0, false, NULL, $1); }
    ;

Sign
    : PLUS                                           { $$ = new Sign(Lexems::Operators::PLUS); }
    | MINUS                                          { $$ = new Sign(Lexems::Operators::MINUS); }
    ;

ModifiablePrimary
    : IDENTIFIER Identifiers                         { $$ = new ModifiablePrimary(get_yyextra_string(), $2); }
    ;

Identifiers
    : DOT IDENTIFIER Identifiers                     { $$ = new Identifiers(get_yyextra_string(), NULL, $3); }
    | BRACKETS_L Expression BRACKETS_R Identifiers   { $$ = new Identifiers("", $2, $4); }
    |                                                { $$ = NULL; }
    ;

%%

int main(int argc, char **argv){
    cout << "\nLexical Analyzer\n======###========\n\n";
    if (yyparse())
        exit(EXIT_FAILURE);

    if (root) {
        cout << "\n\nAST\n======###========\n\n";
        print_Tree(root);

        cout << "\n\nSemantic Analyzer\n======###========\n\n";
        if (run_Semantic_Analyzer(root))
            cout << "Everything is correct\n";

        cout << "\n\nCode Generation\n======###========\n\n";
        if (run_IL_Code_Generator(root))
            cout << "IL Code successfullty generated\n";

        cout << "\n\nAST JSON\n======###========\n\n";
        string serialized_json = serialize_Tree(root);
        std::cout << serialized_json << std::endl;

        ofstream file("tree.json");
        file << serialized_json;
        file.close();

        file.open("tree_ru.json");
        std::replace(serialized_json.begin(), serialized_json.end(), '.', ',');
        file << serialized_json;
        file.close();
    } else {
    	cout << "FAILED BUILDING A TREE\n";
    }
    return 0;
}
