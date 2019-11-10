%{
    #include <stdio.h>
    #include "AST.h"
    int yylex();
    void yyerror(const char *s);
%}

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
    : Declaration DECLARATION_SEPARATOR Program     //{ $$ = Program($1, $2); root = $$; }
    | DECLARATION_SEPARATOR Program                 //{ $$ = $1; }
    | Declaration                                  // { $$ = Program($1, NULL); root = $$; }
    |                                              // { $$ = Program(NULL, NULL); root = $$; }
    ;

Declaration
    : SimpleDeclaration                             //{ $$ = Declaration($1, NULL); }
    | RoutineDeclaration                           // { $$ = Declaration(NULL, $1); }
    ;

SimpleDeclaration
    : VariableDeclaration                           //{ $$ = SimpleDeclaration($1, NULL); }
    | TypeDeclaration                               //{ $$ = SimpleDeclaration(NULL, $1); }
    ;

VariableDeclaration
    : VAR IDENTIFIER COLON Type InitialValue        //{ $$ = VariableDeclaration($2, $4, $5, NULL); }
    | VAR IDENTIFIER IS Expression                  //{ $$ = VariableDeclaration($2, NULL, NULL, $4); }
    ;

InitialValue
    : IS Expression                                 //{ $$ = InitialValue($2); }
    |
    ;

TypeDeclaration
    : TYPE IDENTIFIER IS Type                       //{ $$ = TypeDeclaration($2, $4); }
    ;

Type
    : PrimitiveType                                 //{ $$ = Type($1, NULL, NULL, NULL); }
    | ArrayType                                     //{ $$ = Type(NULL, $1, NULL, NULL); }
    | RecordType                                    //{ $$ = Type(NULL, NULL, $1, NULL); }
    | IDENTIFIER                                    //{ $$ = Type(NULL, NULL, NULL, $1); }
    ;

PrimitiveType
    : INTEGER                                       //{ $$ = PrimitiveType(true, false, false); }
    | REAL                                          //{ $$ = PrimitiveType(false, true, false); }
    | BOOLEAN                                       //{ $$ = PrimitiveType(false, false, true); }
    ;

ArrayType
    : ARRAY BRACKETS_L Expression BRACKETS_R Type   //{ $$ = ArrayType($3, $5); }
    ;

RecordType
    : RECORD VariableDeclarations END               //{ $$ = RecordType($2); }
    ;

VariableDeclarations
    : VariableDeclaration VariableDeclarations      //{ $$ = VariableDeclarations($1, $2); }
    |
    ;

RoutineDeclaration
    : ROUTINE IDENTIFIER Parameters TypeInRoutineDeclaration BodyInRoutineDeclaration   //{ $$ = RoutineDeclaration($2, $3, $4, $5); }
    ;

Parameters
    : PARENTHESES_L ParameterDeclaration ParametersDeclaration PARENTHESES_R    //{ $$ = Parameters($2, $3); }
    |
    ;

ParameterDeclaration
    : IDENTIFIER COLON Type                                   //{ $$ = ParameterDeclaration($1, $3); }
    ;

ParametersDeclaration
    : COMMA ParameterDeclaration ParametersDeclaration        //{ $$ = ParametersDeclaration($2, $3); }
    |
    ;

TypeInRoutineDeclaration
    : COLON Type                                      //{ $$ = TypeInRoutineDeclaration($2); }
    |
    ;

BodyInRoutineDeclaration
    : IS Body END                                   //{ $$ = BodyInRoutineDeclaration($2); }
    |
    ;

Body
    : SimpleDeclaration Body                        //{ $$ = Body($1, NULL, $2); }
    | Statement Body                                //{ $$ = Body(NULL, $1, $2); }
    |
    ;

Statement
    : Assignment                                    //{ $$ = Statement($1, NULL, NULL, NULL, NULL); }
    | RoutineCall                                   //{ $$ = Statement(NULL, $1, NULL, NULL, NULL); }
    | WhileLoop                                     //{ $$ = Statement(NULL, NULL, $1, NULL, NULL); }
    | ForLoop                                       //{ $$ = Statement(NULL, NULL, NULL, $1, NULL); }
    | IfStatement                                   //{ $$ = Statement(NULL, NULL, NULL, NULL, $1); }
    ;

Assignment
    : ModifiablePrimary ASSIGN Expression             //{ $$ = Assignment($1, $3); }
    ;

RoutineCall
    : IDENTIFIER ExpressionInRoutineCall            //{ $$ = RoutineCall($1, $2); }
    ;

ExpressionInRoutineCall
    : PARENTHESES_L Expression ExpressionsInRoutineCall PARENTHESES_R   //{ $$ = ExpressionInRoutineCall($2, $3); }
    |
    ;

ExpressionsInRoutineCall
    : COMMA Expression ExpressionInRoutineCall        //{ $$ = ExpressionsInRoutineCall($2, $3); }
    |
    ;

WhileLoop
    : WHILE Expression LOOP Body END                //{ $$ = WhileLoop($2, $4); }
    ;

ForLoop
    : FOR IDENTIFIER IN Reverse Range LOOP Body END //{ $$ = ForLoop($2, $4, $5, $7); }
    ;

Range
    : Expression RANGER Expression                      //{ $$ = Range($1, $3); }
    ;

Reverse
    : REVERSE                                       //{ $$ = Reverse(true); }
    |
    ;

IfStatement
    : IF Expression THEN Body ElseInIfStatement END //{ $$ = IfStatement($2, $4, $5); }
    ;

ElseInIfStatement
    : ELSE Body                                     //{ $$ = IfStatement($2); }
    |
    ;

Expression
    : Relation MultipleRelationsInExpression        //{ $$ = Expression($1, $2); }
    ;

MultipleRelationsInExpression
    : LogicalOperator Relation MultipleRelationsInExpression    //{ $$ = MultipleRelationsInExpression($1, $2, $3); }
    |
    ;

LogicalOperator
    : AND                                           //{ $$ = LogicalOperator("and"); }
    | OR                                            //{ $$ = LogicalOperator("or"); }
    | XOR                                           //{ $$ = LogicalOperator("xor"); }
    ;

Relation
    : Simple ComparisonInRelation                   //{ $$ = Relation($1, $2); }
    ;

ComparisonInRelation
    : ComparisonOperator Simple                     //{ $$ = ComparisonInRelation($1, $2); }
    |
    ;

ComparisonOperator
    : LESS                                           //{ $$ = ComparisonOperator(LESS); }
    | LESS_EQ                                          //{ $$ = ComparisonOperator(LESS_EQ); }
    | GREATER                                           //{ $$ = ComparisonOperator(GREATER); }
    | GREATER_EQ                                          //{ $$ = ComparisonOperator(GREATER_EQ); }
    | EQ                                           //{ $$ = ComparisonOperator(EQ); }
    | NOT_EQ                                          //{ $$ = ComparisonOperator(NOT_EQ); }
    ;

Simple
    : Factor Factors                                //{ $$ = Simple($1, $2); }
    ;

Factors
    : SimpleOperator Factor Factors                 //{ $$ = Factors($1, $2, $3); }
    |
    ;

SimpleOperator
    : MULT                                           //{ $$ = SimpleOperator("*"); }
    | DIV                                            //{ $$ = SimpleOperator("/"); }
    | REMAINDER                                      //{ $$ = SimpleOperator("%"); }
    ;

Factor
    : Summand Summands                               //{ $$ = Factor($1, $2); }
    ;

Summands
    : Sign Summand Summands                         //{ $$ = Summands($1, $2, $3); }
    |
    ;

Summand
    : Primary                                       //{ $$ = Summand($1, NULL); }
    | PARENTHESES_L Expression PARENTHESES_R        //{ $$ = Summand(NULL, $2); }
    ;

//TODO
Primary
    : INTEGER_LITERAL
    | Sign INTEGER_LITERAL
    | NOT INTEGER_LITERAL
    | REAL_LITERAL
    | Sign REAL_LITERAL
    | TRUE
    | FALSE
    | ModifiablePrimary
    ;

Sign
    : PLUS                                           //{ $$ = Sign("+"); }
    | MINUS                                          //{ $$ = Sign("-"); }
    ;

ModifiablePrimary
    : IDENTIFIER Identifiers                         //{ $$ = ModifiablePrimary($1, $2); }
    ;

Identifiers
    : DOT IDENTIFIER Identifiers                     //{ $$ = Identifiers($2, NULL, $3); }
    | BRACKETS_L Expression BRACKETS_R Identifiers   //{ $$ = Identifiers(NULL, $2, $4); }
    |
    ;

%%

//subroutines

#include <stdio.h>

extern char yytext[];
extern int column;

int main(int argc, char **argv){
    yyparse();
    return 0;
}

void yyerror(char const *s){
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}