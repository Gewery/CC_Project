%{
    #include <stdio.h>
    int yylex();
    void yyerror(const char *s);
%}

%token NUMBER VAR IS IDENTIFIER END IN REVERSE WHILE FOR FOREACH FROM LOOP IF THEN ELSE TRUE FALSE

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
    | VariableDeclaration
    ;

InitialValue
    : IS Expression
    |
    ;

TypeDeclaration
    : type IDENTIFIER IS Type
    ;

RoutineDeclaration
    : routine IDENTIFIER Parameters TypeInRoutineDeclaration BodyInRoutineDeclaration
    ;

Parameters
    : "(" ParameterDeclaration ")"
    |
    ;

ParameterDeclaration
    : IDENTIFIER ":" Type
    | "," ParameterDeclaration
    ;

TypeInRoutineDeclaration:
    : ":" Type
    |
    ;

Type
    : PrimitiveType
    | ArrayType
    | RecordType
    | IDENTIFIER
    ;

PrimitiveType
    : integer
    | real
    | boolean
    ;

RecordType
    : record VariableDeclarationInRecordType END
    ;

VariableDeclarationInRecordType
    : VariableDeclaration VariableDeclarationInRecordType
    |
    ;

ArrayType
    : array ExpressionInArrayType Type
    ;

ExpressionInArrayType
    : Expression ExpressionInArrayType
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
    | ForeachLoop
    | IfStatement
    ;

Assignment
    : ModifiablePrimary ":" "=" Expression
    ;

RoutineCall
    : IDENTIFIER ExpressionInRoutineCall
    ;

ExpressionInRoutineCall
    : "(" Expression MultipleExpressionInRoutineCall ")"
    |
    ;

MultipleExpressionInRoutineCall
    : "," Expression
    |
    ;

WhileLoop
    : WHILE Expression loop Body END
    ;

ForLoop
    : FOR IDENTIFIER Range loop Body END
    ;

Range
    : IN ReverseInRange Expression ".""." Expression
    ;

ReverseInRange
    : REVERSE
    |
    ;

ForeachLoop
    : FOREACH IDENTIFIER FROM ModifiablePrimary LOOP Body END
    ;

IfStatement
    : IF Expression THEN Body ElseInIfStatement END
    ;

ElseInIfStatement
    : ELSE Body
    |
    ;

Expression
    : Relation { ( and | or | xor ) Relation }
    ;

RelationInExpression
    : LogicalOperators Relation RelationInExpression
    :
    ;

LogicalOperators
    : and
    | or
    | xor
    ;

Relation
    : Simple SimpleInRelation
    ;

SimpleInRelation
    : RelationalOperators Simple
    |
    ;

RelationalOperators
    : <
    | <=
    | >
    | >=
    | =
    | /=
    ;

Simple
    : Factor FactorInSimple
    ;

FactorInSimple
    : SimpleOperators Factor
    |
    ;

SimpleOperators
    : *
    | /
    | %
    ;

Factor
    : Summand SummandInFactor
    ;

SummandInFactor
    : FactorOperators Summand
    |
    ;

FactorOperators
    : +
    | -
    ;

// TODO
Summand
    : Primary
    | ( Expression )
    ;

Primary
    : IntegralLiteral
    | RealLiteral
    | TRUE
    | FALSE
    | ModifiablePrimary
    ;

// TODO
ModifiablePrimary
    : IDENTIFIER { . IDENTIFIER | [ Expression ]
    | ( Expression { , Expression } ) ]
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

