%{
    #include <stdio.h>
    #include "AST.h"
    #include <string>
    int yylex();
    void yyerror(const char *s);
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

%token VAR IS END IN REVERSE WHILE
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
    : Declaration DECLARATION_SEPARATOR Program     { $$ = new Program($1, $3); root = $$; }
    | DECLARATION_SEPARATOR Program                 { $$ = $2; }
    | Declaration                                   { $$ = new Program($1, NULL); root = $$; }
    |                                               { $$ = new Program(NULL, NULL); root = $$; }
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
    : VAR IDENTIFIER COLON Type InitialValue        { $$ = new VariableDeclaration($2, $4, $5, NULL); }
    | VAR IDENTIFIER IS Expression                  { $$ = new VariableDeclaration($2, NULL, NULL, $4); }
    ;

InitialValue
    : IS Expression                                 { $$ = new InitialValue($2); }
    |                                               { $$ = new InitialValue(NULL); }
    ;

TypeDeclaration
    : TYPE IDENTIFIER IS Type                       { $$ = new TypeDeclaration($2, $4); }
    ;

Type
    : PrimitiveType                                 { $$ = new Type($1, NULL, NULL, NULL); }
    | ArrayType                                     { $$ = new Type(NULL, $1, NULL, NULL); }
    | RecordType                                    { $$ = new Type(NULL, NULL, $1, NULL); }
    | IDENTIFIER                                    { $$ = new Type(NULL, NULL, NULL, $1); }
    ;

PrimitiveType
    : INTEGER                                       { $$ = new PrimitiveType(true, false, false); }
    | REAL                                          { $$ = new PrimitiveType(false, true, false); }
    | BOOLEAN                                       { $$ = new PrimitiveType(false, false, true); }
    ;

ArrayType
    : ARRAY BRACKETS_L Expression BRACKETS_R Type   { $$ = new ArrayType($3, $5); }
    ;

RecordType
    : RECORD VariableDeclarations END               { $$ = new RecordType($2); }
    ;

VariableDeclarations
    : VariableDeclaration VariableDeclarations      { $$ = new VariableDeclarations($1, $2); }
    |                                               { $$ = new VariableDeclarations(NULL, NULL); }
    ;

RoutineDeclaration
    : ROUTINE IDENTIFIER Parameters TypeInRoutineDeclaration BodyInRoutineDeclaration   { $$ = new RoutineDeclaration($2, $3, $4, $5); }
    ;

Parameters
    : PARENTHESES_L ParameterDeclaration ParametersDeclaration PARENTHESES_R    { $$ = new Parameters($2, $3); }
    |                                                                           { $$ = new Parameters(NULL, NULL); }
    ;

ParameterDeclaration
    : IDENTIFIER COLON Type                                   { $$ = new ParameterDeclaration($1, $3); }
    ;

ParametersDeclaration
    : COMMA ParameterDeclaration ParametersDeclaration        { $$ = new ParametersDeclaration($2, $3); }
    |                                                         { $$ = new ParametersDeclaration(NULL, NULL); }
    ;

TypeInRoutineDeclaration
    : COLON Type                                      { $$ = new TypeInRoutineDeclaration($2); }
    |                                                 { $$ = new TypeInRoutineDeclaration(NULL); }
    ;

BodyInRoutineDeclaration
    : IS Body END                                   { $$ = new BodyInRoutineDeclaration($2); }
    |                                               { $$ = new BodyInRoutineDeclaration(NULL); }
    ;

Body
    : SimpleDeclaration Body                        { $$ = new Body($1, NULL, $2); }
    | Statement Body                                { $$ = new Body(NULL, $1, $2); }
    |                                               { $$ = new Body(NULL, NULL, NULL); }
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
    : IDENTIFIER ExpressionInRoutineCall            { $$ = new RoutineCall($1, $2); }
    ;

ExpressionInRoutineCall
    : PARENTHESES_L Expression ExpressionsInRoutineCall PARENTHESES_R   { $$ = new ExpressionInRoutineCall($2, $3); }
    |                                                                   { $$ = new ExpressionInRoutineCall(NULL, NULL); }
    ;

ExpressionsInRoutineCall
    : COMMA Expression ExpressionInRoutineCall        { $$ = new ExpressionsInRoutineCall($2, $3); }
    |                                                 { $$ = new ExpressionsInRoutineCall(NULL, NULL); }
    ;

WhileLoop
    : WHILE Expression LOOP Body END                { $$ = new WhileLoop($2, $4); }
    ;

ForLoop
    : FOR IDENTIFIER IN Reverse Range LOOP Body END { $$ = new ForLoop($2, $4, $5, $7); }
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
    |                                               { $$ = new ElseInIfStatement(NULL); }
    ;

Expression
    : Relation MultipleRelationsInExpression        { $$ = new Expression($1, $2); }
    ;

MultipleRelationsInExpression
    : LogicalOperator Relation MultipleRelationsInExpression    { $$ = new MultipleRelationsInExpression($1, $2, $3); }
    |                                                           { $$ = new MultipleRelationsInExpression(NULL, NULL, NULL); }
    ;

LogicalOperator
    : AND                                           { $$ = new LogicalOperator("and"); }
    | OR                                            { $$ = new LogicalOperator("or"); }
    | XOR                                           { $$ = new LogicalOperator("xor"); }
    ;

Relation
    : Simple ComparisonInRelation                   { $$ = new Relation($1, $2); }
    ;

ComparisonInRelation
    : ComparisonOperator Simple                     { $$ = new ComparisonInRelation($1, $2); }
    |                                               { $$ = new ComparisonInRelation(NULL, NULL); }
    ;

ComparisonOperator
    : LESS                                           { $$ = new ComparisonOperator($1); }
    | LESS_EQ                                        { $$ = new ComparisonOperator($1); }
    | GREATER                                        { $$ = new ComparisonOperator($1); }
    | GREATER_EQ                                     { $$ = new ComparisonOperator($1); }
    | EQ                                             { $$ = new ComparisonOperator($1); }
    | NOT_EQ                                         { $$ = new ComparisonOperator($1); }
    ;

Simple
    : Factor Factors                                { $$ = new Simple($1, $2); }
    ;

Factors
    : SimpleOperator Factor Factors                 { $$ = new Factors($1, $2, $3); }
    |                                               { $$ = new Factors(NULL, NULL, NULL); }
    ;

SimpleOperator
    : MULT                                           { $$ = new SimpleOperator("*"); }
    | DIV                                            { $$ = new SimpleOperator("/"); }
    | REMAINDER                                      { $$ = new SimpleOperator("%"); }
    ;

Factor
    : Summand Summands                               { $$ = new Factor($1, $2); }
    ;

Summands
    : Sign Summand Summands                         { $$ = new Summands($1, $2, $3); }
    |                                               { $$ = new Summands(NULL, NULL, NULL); }
    ;

Summand
    : Primary                                       { $$ = new Summand($1, NULL); }
    | PARENTHESES_L Expression PARENTHESES_R        { $$ = new Summand(NULL, $2); }
    ;


Primary
    : INTEGER_LITERAL                               { $$ = new Primary("int", $1, false, NULL,  NULL); }
    | Sign INTEGER_LITERAL                          { $$ = new Primary("int", $2, false, $1, NULL); }
    | NOT INTEGER_LITERAL                           { $$ = new Primary("int", $2, true, NULL, NULL); }
    | REAL_LITERAL                                  { $$ = new Primary("real", $1, false, NULL, NULL); }
    | Sign REAL_LITERAL                             { $$ = new Primary("real", $2, false, NULL, NULL); }
    | TRUE                                          { $$ = new Primary("bool", $1, false, NULL, NULL); }
    | FALSE                                         { $$ = new Primary("bool", $1, false, NULL, NULL); }
    | ModifiablePrimary                             { $$ = new Primary("", 0, false, NULL, $1); }
    ;

Sign
    : PLUS                                           { $$ = new Sign("+"); }
    | MINUS                                          { $$ = new Sign("-"); }
    ;

ModifiablePrimary
    : IDENTIFIER Identifiers                         { $$ = new ModifiablePrimary($1, $2); }
    ;

Identifiers
    : DOT IDENTIFIER Identifiers                     { $$ = new Identifiers($2, NULL, $3); }
    | BRACKETS_L Expression BRACKETS_R Identifiers   { $$ = new Identifiers(NULL, $2, $4); }
    |                                                { $$ = new Identifiers(NULL, NULL, NULL); }
    ;

%%

//subroutines

#include <stdio.h>

extern char yytext[];
extern int column;

int main(int argc, char **argv){
    yyparse();
    if (root != nullptr) root->printTree();
    return 0;
}

void yyerror(char const *s){
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}