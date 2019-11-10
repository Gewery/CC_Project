#ifndef CC_PROJECT_AST_H
#define CC_PROJECT_AST_H

#include <string>

using namespace std;


struct Program {
    struct Declaration* Declaration;
    struct Program* Program;
};

struct Declaration {
    struct SimpleDeclaration* SimpleDeclaration;
    struct RoutineDeclaration* RoutineDeclaration;
};

struct SimpleDeclaration {
    struct VariableDeclaration* VariableDeclaration;
    struct TypeDeclaration* TypeDeclaration;
};

struct VariableDeclaration {
    string name;
    struct Type* Type;
    struct InitialValue* InitialValue;
    struct Expression* Expression;
};

struct InitialValue {
    struct Expression* Expression;
};

struct TypeDeclaration {
    string name;
    struct Type* Type;
};

struct Type {
    struct PrimitiveType* PrimitiveType;
    struct ArrayType* ArrayType;
    struct RecordType* RecordType;
    string name;
};

struct PrimitiveType {
    bool isInt;
    bool isReal;
    bool isBoolean;
};

struct ArrayType {
    struct Expression* Expression;
    struct Type* Type;
};

struct RecordType {
    struct VariableDeclarations* VariableDeclarations;
};

struct VariableDeclarations {
    struct VariableDeclaration* VariableDeclaration;
    struct VariableDeclarations* VariableDeclarations;
};

struct RoutineDeclaration {
    string name;
    struct Parameters* Parameters;
    struct TypeInRoutineDeclaration* TypeInRoutineDeclaration;
    struct BodyInRoutineDeclaration* BodyInRoutineDeclaration;
};

struct Parameters {
    struct ParameterDeclaration* ParameterDeclaration;
    struct ParametersDeclaration* ParametersDeclaration;
};

struct ParameterDeclaration {
    string name;
    struct Type* Type;
};

struct ParametersDeclaration {
    struct ParameterDeclaration* ParameterDeclaration;
    struct ParametersDeclaration* ParametersDeclaration;
};

struct TypeInRoutineDeclaration {
    struct Type* Type;
};

struct BodyInRoutineDeclaration {
    struct Body* Body;
};

struct Body {
    struct SimpleDeclaration* SimpleDeclaration;
    struct Statement* Statement;
    struct Body* Body;
};

struct Statement {
    struct Assignment* Assignment;
    struct RoutineCall* RoutineCall;
    struct WhileLoop* WhileLoop;
    struct ForLoop* ForLoop;
    struct IfStatement* IfStatement;
};

struct Assignment {
    struct ModifiablePrimary* ModifiablePrimary;
    struct Expression* Expression;
};

struct RoutineCall {
    string name;
    struct ExpressionInRoutineCall* ExpressionInRoutineCall;
};

struct ExpressionInRoutineCall {
    struct Expression* Expression;
    struct ExpressionsInRoutineCall* ExpressionsInRoutineCall;
};

struct ExpressionsInRoutineCall {
    struct Expression* Expression;
    struct ExpressionInRoutineCall* ExpressionInRoutineCall;
};

struct WhileLoop {
    struct Expression* Expression;
    struct Body* Body;
};

struct ForLoop {
    string name;
    struct Reverse* Reverse;
    struct Range* Range;
    struct Body* Body;
};

struct Range {
    struct Expression* Expression1;
    struct Expression* Expression2;
};

struct Reverse {
   bool isReverse;
};

struct IfStatement {
    struct Expression* Expression1;
    struct Body* Body;
    struct ElseInIfStatement* ElseInIfStatement;
};

struct ElseInIfStatement {
    struct Body* Body;
};

struct Expression {
    struct Relation* Relation;
    struct MultipleRelationsInExpression* MultipleRelationsInExpression;
};

struct MultipleRelationsInExpression {
    struct LogicalOperator* LogicalOperator;
    struct Relation* Relation;
    struct MultipleRelationsInExpression* MultipleRelationsInExpression;
};

struct LogicalOperator {
    string op; // and,or,xor
};

struct Relation {
    struct Simple* Simple;
    struct ComparisonInRelation* ComparisonInRelation;
};

struct ComparisonInRelation {
    struct ComparisonOperator* ComparisonOperator;
    struct Simple* Simple;
};

struct ComparisonOperator {
    string op;
};

struct Simple {
    struct Factor* Factor;
    struct Factors* Factors;
};

struct Factors {
    struct SimpleOperator* SimpleOperator;
    struct Factor* Factor;
    struct Factors* Factors;
};

struct SimpleOperator {
    string op;
};

struct Factor {
    struct Summand* Summand;
    struct Summands* Summands;
};

struct Summands {
    struct Sign* Sign;
    struct Summand* Summand;
    struct Summands* Summands;
};

struct Summand {
    struct Primary* Primary;
    struct Expression* Expression;
};

//TODO
struct Primary {

};

struct Sign {
    string op;
};

struct ModifiablePrimary {
    string name;
    struct Identifiers* Identifiers;
};

struct Identifiers {
    string name;
    struct Expression* Expression;
    struct Identifiers* Identifiers;
};


#endif //CC_PROJECT_AST_H
