%{
    #include <stdio.h>
    #include "AST.h"
    int yylex();
    void yyerror(const char *s);
%}

%token VAR IDENTIFIER IS TYPE INTEGER
%token REAL BOOLEAN ARRAY RECORD END
%token INTEGER_LITERAL REAL_LITERAL
%token ROUTINE WHILE LOOP FOR IN
%token REVERSE IF THEN ELSE AND
%token OR XOR TRUE FALSE NOT
%token "<" "<=" ">" ">=" "=" "/=" ";"

%nonassoc ".." 

%left "+" "-" OR
%left "*" "/" "%" AND

%right "=" ":="

%start Program

%%
// rules

Program
    : SimpleDeclaration Program                     //{ $$ = Program($1, NULL, $2); root = $$; }
    | RoutineDeclaration Program                    //{ $$ = Program(NULL, $1, $2); root = $$; }
    |
    ;

SimpleDeclaration
    : VariableDeclaration                           //{ $$ = SimpleDeclaration($1, NULL); }
    | TypeDeclaration                               //{ $$ = SimpleDeclaration(NULL, $1); }
    ;

VariableDeclaration
    : VAR IDENTIFIER ":" Type InitialValue          //{ $$ = SimpleDeclaration($2, $4, $5, NULL); }
    | VAR IDENTIFIER IS Expression                  //{ $$ = SimpleDeclaration($2, NULL, NULL, $4); }
    ;

InitialValue
    : IS Expression                                 //{ $$ = SimpleDeclaration($2); }
    |
    ;

TypeDeclaration
    : TYPE IDENTIFIER IS Type                       //{ $$ = TypeDeclaration($2, $4); }
    ;

Type
    : PrimitiveType                                 //{ $$ = TypeDeclaration($1, NULL, NULL, NULL); }
    | ArrayType                                     //{ $$ = TypeDeclaration(NULL, $1, NULL, NULL); }
    | RecordType                                    //{ $$ = TypeDeclaration(NULL, NULL, $1, NULL); }
    | IDENTIFIER                                    //{ $$ = TypeDeclaration(NULL, NULL, NULL, $1); }
    ;

PrimitiveType
    : INTEGER                                       //{ $$ = PrimitiveType(true, false, false); }
    | REAL                                          //{ $$ = PrimitiveType(false, true, false); }
    | BOOLEAN                                       //{ $$ = PrimitiveType(false, false, true); }
    ;

ArrayType
    : ARRAY "[" Expression "]" Type                 //{ $$ = ArrayType($3, $5); }
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
    : "(" ParameterDeclaration ParametersDeclaration ")"     //{ $$ = Parameters($2, $3); }
    |
    ;

ParameterDeclaration
    : IDENTIFIER ":" Type                                   //{ $$ = ParameterDeclaration($1, $3); }
    ;

ParametersDeclaration
    : "," ParameterDeclaration ParametersDeclaration        //{ $$ = ParametersDeclaration($2, $3); }
    |
    ;

TypeInRoutineDeclaration
    : ":" Type                                      //{ $$ = TypeInRoutineDeclaration($2); }
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
    : ModifiablePrimary ":=" Expression             //{ $$ = Assignment($1, $3); }
    ;

RoutineCall
    : IDENTIFIER ExpressionInRoutineCall            //{ $$ = RoutineCall($1, $2); }
    ;

ExpressionInRoutineCall
    : "(" Expression ExpressionsInRoutineCall ")"   //{ $$ = ExpressionInRoutineCall($2, $3); }
    |
    ;

ExpressionsInRoutineCall
    : "," Expression ExpressionInRoutineCall        //{ $$ = ExpressionsInRoutineCall($2, $3); }
    |
    ;

WhileLoop
    : WHILE Expression LOOP Body END                //{ $$ = WhileLoop($2, $4); }
    ;

ForLoop
    : FOR IDENTIFIER IN Reverse Range LOOP Body END //{ $$ = ForLoop($2, $4, $5, $7); }
    ;

Range
    : Expression .. Expression                      //{ $$ = Range($1, $3); }
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
    : "<"                                           //{ $$ = ComparisonOperator("<"); }
    | "<="                                          //{ $$ = ComparisonOperator("<="); }
    | ">"                                           //{ $$ = ComparisonOperator(">"); }
    | ">="                                          //{ $$ = ComparisonOperator(">="); }
    | "="                                           //{ $$ = ComparisonOperator("="); }
    | "/="                                          //{ $$ = ComparisonOperator("/="); }
    ;

Simple
    : Factor Factors                                //{ $$ = Simple($1, $2); }
    ;

Factors
    : SimpleOperator Factor Factors                 //{ $$ = Factors($1, $2, $3); }
    |
    ;

SimpleOperator
    : "*"                                            //{ $$ = SimpleOperator("*"); }
    | "/"                                            //{ $$ = SimpleOperator("/"); }
    | "%"                                            //{ $$ = SimpleOperator("%"); }
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
    | "(" Expression ")"                            //{ $$ = Summand(NULL, $2); }
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
    | RoutineCall
    ;

Sign
    : "+"                                            //{ $$ = Sign("+"); }
    | "-"                                            //{ $$ = Sign("-"); }
    ;

ModifiablePrimary
    : IDENTIFIER Identifiers                        //{ $$ = Summand($1, $2); }
    ;

Identifiers
    : "." IDENTIFIER Identifiers                    //{ $$ = Summand($2, NULL, $3); }
    | "[" Expression "]" Identifiers                //{ $$ = Summand(NULL, $2, $4); }
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