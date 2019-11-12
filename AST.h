#ifndef CC_PROJECT_AST_H
#define CC_PROJECT_AST_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct Node {
    const char *nodeName = "DEFAULT_NODE_NAME_PLS_OVERRIDE";
    vector<Node *> children;
    virtual void printTree();
    void _print(const string &prefix, bool isLast);
};

struct Program : Node {
    const char *nodeName = "Program";
    struct Declaration *declaration;
    struct Program *program;
    Program(Declaration *declaration, Program *program);
};

struct Declaration : Node {
    const char *nodeName = "Declaration";
    struct SimpleDeclaration *simpledeclaration;
    struct RoutineDeclaration *routinedeclaration;
    Declaration(SimpleDeclaration *simpledeclaration, RoutineDeclaration *routinedeclaration);
};


struct SimpleDeclaration {
    struct VariableDeclaration *variabledeclaration;
    struct TypeDeclaration *typedeclaration;
    SimpleDeclaration(VariableDeclaration *variabledeclaration, TypeDeclaration *typedeclaration);
};

struct VariableDeclaration {
    string name;
    struct Type *type;
    struct InitialValue *initialvalue;
    struct Expression *expression;
    VariableDeclaration(string name, Type *type, InitialValue *initialvalue, Expression *expression);
};

struct InitialValue {
    struct Expression *expression;
    InitialValue(Expression *expression);
};

struct TypeDeclaration {
    string name;
    struct Type *type;
    TypeDeclaration(string name, Type *type);
};

struct Type {
    struct PrimitiveType *primitivetype;
    struct ArrayType *arraytype;
    struct RecordType *recordtype;
    string name;
    Type(PrimitiveType *primitivetype, ArrayType *arraytype, RecordType *recordtype, string name);
};

struct PrimitiveType {
    bool isint;
    bool isreal;
    bool isboolean;
    PrimitiveType(bool isint, bool isreal, bool isboolean);
};

struct ArrayType {
    struct Expression *expression;
    struct Type *type;
    ArrayType(Expression *expression, Type *type);
};

struct RecordType {
    struct VariableDeclarations *variabledeclarations;
    RecordType(VariableDeclarations *variabledeclarations);
};

struct VariableDeclarations {
    struct VariableDeclaration *variabledeclaration;
    struct VariableDeclarations *variabledeclarations;
    VariableDeclarations(VariableDeclaration *variabledeclaration, VariableDeclarations *variabledeclarations);
};

struct RoutineDeclaration {
    string name;
    struct Parameters *parameters;
    struct TypeInRoutineDeclaration *typeinroutinedeclaration;
    struct BodyInRoutineDeclaration *bodyinroutinedeclaration;
    RoutineDeclaration(string name, Parameters *parameters, TypeInRoutineDeclaration *typeinroutinedeclaration,
                       BodyInRoutineDeclaration *bodyinroutinedeclaration);
};

struct Parameters {
    struct ParameterDeclaration *parameterdeclaration;
    struct ParametersDeclaration *parametersdeclaration;
    Parameters(ParameterDeclaration *parameterdeclaration, ParametersDeclaration *parametersdeclaration);
};

struct ParameterDeclaration {
    string name;
    struct Type *type;

    ParameterDeclaration(string name, Type *type);
};

struct ParametersDeclaration {
    struct ParameterDeclaration *parameterdeclaration;
    struct ParametersDeclaration *parametersdeclaration;

    ParametersDeclaration(ParameterDeclaration *parameterdeclaration, ParametersDeclaration *parametersdeclaration);
};

struct TypeInRoutineDeclaration {
    struct Type *type;
    TypeInRoutineDeclaration(Type *type);
};

struct BodyInRoutineDeclaration {
    struct Body *body;
    BodyInRoutineDeclaration(Body *body);
};

struct Body {
    struct SimpleDeclaration *simpledeclaration;
    struct Statement *statement;
    struct Body *body;
    Body(SimpleDeclaration *simpledeclaration, Statement *statement, Body *body);
};

struct Statement {
    struct Assignment *assignment;
    struct RoutineCall *routinecall;
    struct WhileLoop *whileloop;
    struct ForLoop *forloop;
    struct IfStatement *ifstatement;
    Statement(Assignment *assignment, RoutineCall *routinecall, WhileLoop *whileloop, ForLoop *forloop,
              IfStatement *ifstatement);
};

struct Assignment {
    struct ModifiablePrimary *modifiableprimary;
    struct Expression *expression;
    Assignment(ModifiablePrimary *modifiableprimary, Expression *expression);
};

struct RoutineCall {
    string name;
    struct ExpressionInRoutineCall *expressioninroutinecall;
    RoutineCall(string name, ExpressionInRoutineCall *expressioninroutinecall);
};

struct ExpressionInRoutineCall {
    struct Expression *expression;
    struct ExpressionsInRoutineCall *expressionsinroutinecall;
    ExpressionInRoutineCall(Expression *expression, ExpressionsInRoutineCall *expressionsinroutinecall);
};

struct ExpressionsInRoutineCall {
    struct Expression *expression;
    struct ExpressionInRoutineCall *expressioninroutinecall;
    ExpressionsInRoutineCall(Expression *expression, ExpressionInRoutineCall *expressioninroutinecall);
};

struct WhileLoop {
    struct Expression *expression;
    struct Body *body;
    WhileLoop(Expression *expression, Body *body);
};

struct ForLoop {
    string name;
    struct Reverse *reverse;
    struct Range *range;
    struct Body *body;
    ForLoop(string name, Reverse *reverse, Range *range, Body *body);
};

struct Range {
    struct Expression *expression1;
    struct Expression *expression2;
    Range(Expression *expression1, Expression *expression2);
};

struct Reverse {
    bool isreverse;
    Reverse(bool isreverse);
};

struct IfStatement {
    struct Expression *expression1;
    struct Body *body;
    struct ElseInIfStatement *elseinifstatement;
    IfStatement(Expression *expression1, Body *body, ElseInIfStatement *elseinifstatement);
};

struct ElseInIfStatement {
    struct Body *body;
    ElseInIfStatement(Body *body);
};

struct Expression {
    struct Relation *relation;
    struct MultipleRelationsInExpression *multiplerelationsinexpression;
    Expression(Relation *relation, MultipleRelationsInExpression *multiplerelationsinexpression);
};

struct MultipleRelationsInExpression {
    struct LogicalOperator *logicaloperator;
    struct Relation *relation;
    struct MultipleRelationsInExpression *multiplerelationsinexpression;
    MultipleRelationsInExpression(LogicalOperator *logicaloperator, Relation *relation,
                                  MultipleRelationsInExpression *multiplerelationsinexpression);
};

struct LogicalOperator {
    string op;
    LogicalOperator(string op);
};

struct Relation {
    struct Simple *simple;
    struct ComparisonInRelation *comparisoninrelation;
    Relation(Simple *simple, ComparisonInRelation *comparisoninrelation);
};

struct ComparisonInRelation {
    struct ComparisonOperator *comparisonoperator;
    ComparisonInRelation(ComparisonOperator *comparisonoperator);
};


struct ComparisonOperator {
    string op;
    ComparisonOperator(string op);
};

struct Simple {
    struct Factor* factor;
    struct Factors* factors;
    Simple(Factor* factor, Factors* factors);
};

struct Factors {
    struct SimpleOperator* simpleOperator;
    struct Factor* factor;
    struct Factors* factors;
    Factors(SimpleOperator* simpleOperator, Factor* factor, Factors* factors);
};

struct SimpleOperator {
    string op;
    SimpleOperator(string op);
};

struct Factor {
    struct Summand* summand;
    struct Summands* summands;
    Factor(Summand* summand, Summands* summands);
};

struct Summands {
    struct Sign* sign;
    struct Summand* summand;
    struct Summands* summands;
    Summands(Sign* sign, Summand* summand, Summands* summands);
};

struct Summand {
    struct Primary* primary;
    struct Expression* expression;
    Summand(Primary* primary, Expression* expression);
};


struct Primary {
    string type;
    float value;
    string sign;
    struct ModifiablePrimary* modifiablePrimary;
    Primary(string type, float value,  string sign, struct ModifiablePrimary* modifiablePrimary);
};

struct Sign {
    string op;
    Sign(string op);
};

struct ModifiablePrimary {
    string name;
    struct Identifiers* identifiers;
    ModifiablePrimary(string name, struct Identifiers* identifiers);
};

struct Identifiers {
    string name;
    struct Expression* expression;
    struct Identifiers* identifiers;
    Identifiers(string name, struct Expression* expression, struct Identifiers* identifiers);
};
#endif //CC_PROJECT_AST_H
