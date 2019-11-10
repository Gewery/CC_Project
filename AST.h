#ifndef CC_PROJECT_AST_H
#define CC_PROJECT_AST_H

#include <string>

using namespace std;

struct Program {
    struct Declaration *declaration;
    struct Program *program;

    Program(Declaration *declaration, Program *program) : declaration(declaration), program(program) {}
};

struct Declaration {
    struct SimpleDeclaration *simpledeclaration;
    struct RoutineDeclaration *routinedeclaration;

    Declaration(SimpleDeclaration *simpledeclaration, RoutineDeclaration *routinedeclaration) : simpledeclaration(
            simpledeclaration), routinedeclaration(routinedeclaration) {}
};

struct SimpleDeclaration {
    struct VariableDeclaration *variabledeclaration;
    struct TypeDeclaration *typedeclaration;

    SimpleDeclaration(VariableDeclaration *variabledeclaration, TypeDeclaration *typedeclaration) : variabledeclaration(
            variabledeclaration), typedeclaration(typedeclaration) {}
};

struct VariableDeclaration {
    string name;
    struct Type *type;
    struct InitialValue *initialvalue;
    struct Expression *expression;

    VariableDeclaration(string name, Type *type, InitialValue *initialvalue, Expression *expression) : name(name),
                                                                                                       type(type),
                                                                                                       initialvalue(
                                                                                                               initialvalue),
                                                                                                       expression(
                                                                                                               expression) {}
};

struct InitialValue {
    struct Expression *expression;

    InitialValue(Expression *expression) : expression(expression) {}
};

struct TypeDeclaration {
    string name;
    struct Type *type;

    TypeDeclaration(string name, Type *type) : name(name), type(type) {}
};

struct Type {
    struct PrimitiveType *primitivetype;
    struct ArrayType *arraytype;
    struct RecordType *recordtype;
    string name;

    Type(PrimitiveType *primitivetype, ArrayType *arraytype, RecordType *recordtype, string name) : primitivetype(
            primitivetype), arraytype(arraytype), recordtype(recordtype), name(name) {}
};

struct PrimitiveType {
    bool isint;
    bool isreal;
    bool isboolean;

    PrimitiveType(bool isint, bool isreal, bool isboolean) : isint(isint), isreal(isreal), isboolean(isboolean) {}
};

struct ArrayType {
    struct Expression *expression;
    struct Type *type;

    ArrayType(Expression *expression, Type *type) : expression(expression), type(type) {}
};

struct RecordType {
    struct VariableDeclarations *variabledeclarations;

    RecordType(VariableDeclarations *variabledeclarations) : variabledeclarations(variabledeclarations) {}
};

struct VariableDeclarations {
    struct VariableDeclaration *variabledeclaration;
    struct VariableDeclarations *variabledeclarations;

    VariableDeclarations(VariableDeclaration *variabledeclaration, VariableDeclarations *variabledeclarations)
            : variabledeclaration(variabledeclaration), variabledeclarations(variabledeclarations) {}
};

struct RoutineDeclaration {
    string name;
    struct Parameters *parameters;
    struct TypeInRoutineDeclaration *typeinroutinedeclaration;
    struct BodyInRoutineDeclaration *bodyinroutinedeclaration;

    RoutineDeclaration(string name, Parameters *parameters, TypeInRoutineDeclaration *typeinroutinedeclaration,
                       BodyInRoutineDeclaration *bodyinroutinedeclaration) : name(name), parameters(parameters),
                                                                             typeinroutinedeclaration(
                                                                                     typeinroutinedeclaration),
                                                                             bodyinroutinedeclaration(
                                                                                     bodyinroutinedeclaration) {}
};

struct Parameters {
    struct ParameterDeclaration *parameterdeclaration;
    struct ParametersDeclaration *parametersdeclaration;

    Parameters(ParameterDeclaration *parameterdeclaration, ParametersDeclaration *parametersdeclaration)
            : parameterdeclaration(parameterdeclaration), parametersdeclaration(parametersdeclaration) {}
};

struct ParameterDeclaration {
    string name;
    struct Type *type;

    ParameterDeclaration(string name, Type *type) : name(name), type(type) {}
};

struct ParametersDeclaration {
    struct ParameterDeclaration *parameterdeclaration;
    struct ParametersDeclaration *parametersdeclaration;

    ParametersDeclaration(ParameterDeclaration *parameterdeclaration, ParametersDeclaration *parametersdeclaration)
            : parameterdeclaration(parameterdeclaration), parametersdeclaration(parametersdeclaration) {}
};

struct TypeInRoutineDeclaration {
    struct Type *type;

    TypeInRoutineDeclaration(Type *type) : type(type) {}
};

struct BodyInRoutineDeclaration {
    struct Body *body;

    BodyInRoutineDeclaration(Body *body) : body(body) {}
};

struct Body {
    struct SimpleDeclaration *simpledeclaration;
    struct Statement *statement;
    struct Body *body;

    Body(SimpleDeclaration *simpledeclaration, Statement *statement, Body *body) : simpledeclaration(simpledeclaration),
                                                                                   statement(statement), body(body) {}
};

struct Statement {
    struct Assignment *assignment;
    struct RoutineCall *routinecall;
    struct WhileLoop *whileloop;
    struct ForLoop *forloop;
    struct IfStatement *ifstatement;

    Statement(Assignment *assignment, RoutineCall *routinecall, WhileLoop *whileloop, ForLoop *forloop,
              IfStatement *ifstatement) : assignment(assignment), routinecall(routinecall), whileloop(whileloop),
                                          forloop(forloop), ifstatement(ifstatement) {}
};

struct Assignment {
    struct ModifiablePrimary *modifiableprimary;
    struct Expression *expression;

    Assignment(ModifiablePrimary *modifiableprimary, Expression *expression) : modifiableprimary(modifiableprimary),
                                                                               expression(expression) {}
};

struct RoutineCall {
    string name;
    struct ExpressionInRoutineCall *expressioninroutinecall;

    RoutineCall(string name, ExpressionInRoutineCall *expressioninroutinecall) : name(name), expressioninroutinecall(
            expressioninroutinecall) {}
};

struct ExpressionInRoutineCall {
    struct Expression *expression;
    struct ExpressionsInRoutineCall *expressionsinroutinecall;

    ExpressionInRoutineCall(Expression *expression, ExpressionsInRoutineCall *expressionsinroutinecall) : expression(
            expression), expressionsinroutinecall(expressionsinroutinecall) {}
};

struct ExpressionsInRoutineCall {
    struct Expression *expression;
    struct ExpressionInRoutineCall *expressioninroutinecall;

    ExpressionsInRoutineCall(Expression *expression, ExpressionInRoutineCall *expressioninroutinecall) : expression(
            expression), expressioninroutinecall(expressioninroutinecall) {}
};

struct WhileLoop {
    struct Expression *expression;
    struct Body *body;

    WhileLoop(Expression *expression, Body *body) : expression(expression), body(body) {}
};

struct ForLoop {
    string name;
    struct Reverse *reverse;
    struct Range *range;
    struct Body *body;

    ForLoop(string name, Reverse *reverse, Range *range, Body *body) : name(name), reverse(reverse), range(range),
                                                                       body(body) {}
};

struct Range {
    struct Expression *expression1;
    struct Expression *expression2;

    Range(Expression *expression1, Expression *expression2) : expression1(expression1), expression2(expression2) {}
};

struct Reverse {
    bool isreverse;

    Reverse(bool isreverse) : isreverse(isreverse) {}
};

struct IfStatement {
    struct Expression *expression1;
    struct Body *body;
    struct ElseInIfStatement *elseinifstatement;

    IfStatement(Expression *expression1, Body *body, ElseInIfStatement *elseinifstatement) : expression1(expression1),
                                                                                             body(body),
                                                                                             elseinifstatement(
                                                                                                     elseinifstatement) {}
};

struct ElseInIfStatement {
    struct Body *body;

    ElseInIfStatement(Body *body) : body(body) {}
};

struct Expression {
    struct Relation *relation;
    struct MultipleRelationsInExpression *multiplerelationsinexpression;

    Expression(Relation *relation, MultipleRelationsInExpression *multiplerelationsinexpression) : relation(relation),
                                                                                                   multiplerelationsinexpression(
                                                                                                           multiplerelationsinexpression) {}
};

struct MultipleRelationsInExpression {
    struct LogicalOperator *logicaloperator;
    struct Relation *relation;
    struct MultipleRelationsInExpression *multiplerelationsinexpression;

    MultipleRelationsInExpression(LogicalOperator *logicaloperator, Relation *relation,
                                  MultipleRelationsInExpression *multiplerelationsinexpression) : logicaloperator(
            logicaloperator), relation(relation), multiplerelationsinexpression(multiplerelationsinexpression) {}
};

struct LogicalOperator {
    string op;

    LogicalOperator(string op) : op(op) {}
};

struct Relation {
    struct Simple *simple;
    struct ComparisonInRelation *comparisoninrelation;

    Relation(Simple *simple, ComparisonInRelation *comparisoninrelation) : simple(simple),
                                                                           comparisoninrelation(comparisoninrelation) {}
};

struct ComparisonInRelation {
    struct ComparisonOperator *comparisonoperator;
};

#endif //CC_PROJECT_AST_H
