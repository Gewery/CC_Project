%{
    #include <stdio.h>
    int yylex();
    void yyerror(const char *s);
%}

%token VAR IDENTIFIER IS TYPE INTEGER
%token REAL BOOLEAN ARRAY RECORD END
%token INTEGER_LITERAL, REAL_LITERAL
%token ROUTINE WHILE LOOP FOR IN
%token REVERSE IF THEN ELSE AND
%token OR XOR TRUE FALSE NOT
%token "<" "<=" ">" ">=" "=" "/="

%nonassoc ".." 

%left "+" "-" OR
%left "*" "/" "%" AND

%right "=" ":="

%start compilation_unit

%%
// rules

Program
    : SimpleDeclaration Program
    | RoutineDeclaration Program
    |
    ;

SimpleDeclaration
    : VariableDeclaration
    | TypeDeclaration
    ;

VariableDeclaration
    : VAR IDENTIFIER ":" Type InitialValue
    | VAR IDENTIFIER IS Expression
    ;

InitialValue
    : IS Expression
    |
    ;

TypeDeclaration
    : TYPE IDENTIFIER IS Type
    ;

Type
    : PrimitiveType
    | ArrayType
    | RecordType
    | IDENTIFIER
    ;

PrimitiveType
    : INTEGER
    | REAL
    | BOOLEAN
    ;

ArrayType
    : ARRAY "[" Expression "]" Type
    ;

RecordType
    : RECORD VariableDeclarations END
    ;

VariableDeclarations
    : VariableDeclaration VariableDeclarations
    |
    ;

RoutineDeclaration
    : ROUTINE IDENTIFIER Parameters TypeInRoutineDeclaration BodyInRoutineDeclaration
    ;

Parameters
    : "(" ParameterDeclaration ParametersDeclaration ")"
    |
    ;

ParameterDeclaration
    : IDENTIFIER ":" Type
    ;

ParametersDeclaration
    : "," ParameterDeclaration ParametersDeclaration
    |
    ;

TypeInRoutineDeclaration
    : ":" Type
    |
    ;

BodyInRoutineDeclaration
    : IS Body END
    |
    ;

Body
    : SimpleDeclaration Body
    | Statement Body
    |
    ;

Statement
    : Assignment
    | RoutineCall
    | WhileLoop
    | ForLoop
    | IfStatement
    ;

Assignment
    : ModifiablePrimary ":=" Expression
    ;

RoutineCall
    : IDENTIFIER ExpressionInRoutineCall
    ;

ExpressionInRoutineCall
    : "(" Expression ExpressionsInRoutineCall ")"
    |
    ;

ExpressionsInRoutineCall
    : "," Expression ExpressionInRoutineCall
    |
    ;

WhileLoop
    : WHILE Expression LOOP Body END
    ;

ForLoop
    : FOR IDENTIFIER IN Reverse Range LOOP Body END
    ;

Range
    : Expression .. Expression
    ;

Reverse
    : REVERSE
    |
    ;

IfStatement
    : IF Expression THEN Body ElseInIfStatement END
    ;

ElseInIfStatement
    : ELSE Body
    |
    ;

Expression
    : Relation MultipleRelationsInExpression
    ;

MultipleRelationsInExpression
    : LogicalOperator Relation MultipleRelationsInExpression
    |
    ;

LogicalOperator
    : AND
    | OR
    | XOR
    ;

Relation
    : Simple ComparisonInRelation
    ;

ComparisonInRelation
    : ComparisonOperator Simple
    |
    ;

ComparisonOperator
    : "<"
    | "<="
    | ">"
    | ">="
    | "="
    | "/="
    ;

Simple
    : Factor Factors
    ;

Factors
    : SimpleOperator Factor Factors
    |
    ;

SimpleOperator
    : "*"
    | "/"
    | "%"
    ;

Factor
    : Summand Summands
    ;

Summands
    : Sign Summand Summands
    |
    ;

Summand
    : Primary
    | "(" Expression ")"
    ;

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
    : "+"
    | "-"
    ;

ModifiablePrimary
    : IDENTIFIER Identifiers
    ;

Identifiers
    : "." IDENTIFIER Identifiers
    | "[" Expression "]" Identifiers
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