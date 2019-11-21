#ifndef CC_PROJECT_AST_H
#define CC_PROJECT_AST_H

#include <string>
#include <vector>
#include <iostream>
#include "AST.h"

using namespace std;

struct Node {
    const char *nodeName = "DEFAULT_NODE_NAME_PLS_OVERRIDE";
    vector<Node *> children;

    virtual void printTree() {
        this->_print("", true);
    }

    void _print(const string &prefix, bool isLast) {
        cout << prefix;
        cout << (isLast ? "└──" : "├──");
        cout << this->nodeName << endl;

        // todo: ne rabotayet fffffffffff
        // AST.h:29:42: error: cannot convert 'std::__cxx11::basic_string<char>::iterator' {aka '__gnu_cxx::__normal_iterator<char*, std::__cxx11::basic_string<char> >'} to 'const char*'
        //         str1.erase(std::remove(str1.begin(), str1.end(), ' '), str1.end());
        //
        //this->children.erase(remove(this->children.begin(), this->children.end(), nullptr), this->children.end());

        for (auto const &child: this->children) {
            child->_print(
                    prefix + (isLast ? "    " : "│   "),
                    children.back() == child);
        }
    }
};

struct ParametersDeclaration {
    struct ParameterDeclaration *parameterdeclaration;
    struct ParametersDeclaration *parametersdeclaration;

    ParametersDeclaration(ParameterDeclaration *parameterdeclaration, ParametersDeclaration *parametersdeclaration) :
            parameterdeclaration(parameterdeclaration),
            parametersdeclaration(parametersdeclaration) {}
};

struct ParameterDeclaration {
    string name;
    struct Type *type;

    ParameterDeclaration(string name, Type *type) :
            name(name),
            type(type) {}
};

struct Parameters {
    struct ParameterDeclaration *parameterdeclaration;
    struct ParametersDeclaration *parametersdeclaration;

    Parameters(ParameterDeclaration *parameterdeclaration, ParametersDeclaration *parametersdeclaration) :
            parameterdeclaration(parameterdeclaration),
            parametersdeclaration(parametersdeclaration) {}
};

struct RoutineDeclaration {
    string name;
    struct Parameters *parameters;
    struct TypeInRoutineDeclaration *typeinroutinedeclaration;
    struct BodyInRoutineDeclaration *bodyinroutinedeclaration;

    RoutineDeclaration(string name, Parameters *parameters, TypeInRoutineDeclaration *typeinroutinedeclaration,
                       BodyInRoutineDeclaration *bodyinroutinedeclaration) :
            name(name),
            parameters(parameters),
            typeinroutinedeclaration(typeinroutinedeclaration),
            bodyinroutinedeclaration(bodyinroutinedeclaration) {}
};

struct VariableDeclarations {
    struct VariableDeclaration *variabledeclaration;
    struct VariableDeclarations *variabledeclarations;

    VariableDeclarations(VariableDeclaration *variabledeclaration, VariableDeclarations *variabledeclarations) :
            variabledeclaration(variabledeclaration),
            variabledeclarations(variabledeclarations) {}
};


struct RecordType {
    struct VariableDeclarations *variabledeclarations;

    RecordType(VariableDeclarations *variabledeclarations) :
            variabledeclarations(variabledeclarations) {}
};

struct ArrayType {
    struct Expression *expression;
    struct Type *type;

    ArrayType(Expression *expression, Type *type) :
            expression(expression),
            type(type) {}
};

struct PrimitiveType {
    bool isint;
    bool isreal;
    bool isboolean;

    PrimitiveType(bool isint, bool isreal, bool isboolean) :
            isint(isint),
            isreal(isreal),
            isboolean(isboolean) {}
};

struct Type {
    struct PrimitiveType *primitivetype;
    struct ArrayType *arraytype;
    struct RecordType *recordtype;
    string name;

    Type(PrimitiveType *primitivetype, ArrayType *arraytype, RecordType *recordtype, string name) :
            primitivetype(primitivetype),
            arraytype(arraytype),
            recordtype(recordtype),
            name(name) {}
};

struct TypeDeclaration {
    string name;
    struct Type *type;

    TypeDeclaration(string name, Type *type) :
            name(name),
            type(type) {}
};

struct InitialValue {
    struct Expression *expression;

    InitialValue(Expression *expression) :
            expression(expression) {}
};

struct VariableDeclaration {
    string name;
    struct Type *type;
    struct InitialValue *initialvalue;
    struct Expression *expression;

    VariableDeclaration(string name, Type *type, InitialValue *initialvalue, Expression *expression) :
            name(name),
            type(type),
            initialvalue(initialvalue),
            expression(expression) {}
};

struct SimpleDeclaration {
    struct VariableDeclaration *variabledeclaration;
    struct TypeDeclaration *typedeclaration;

    SimpleDeclaration(VariableDeclaration *variabledeclaration, TypeDeclaration *typedeclaration) :
            variabledeclaration(variabledeclaration),
            typedeclaration(typedeclaration) {}
};

struct Declaration : Node {
    const char *nodeName = "Declaration";

    struct SimpleDeclaration *simpledeclaration;
    struct RoutineDeclaration *routinedeclaration;

    Declaration(SimpleDeclaration *simpledeclaration, RoutineDeclaration *routinedeclaration) :
            simpledeclaration(simpledeclaration),
            routinedeclaration(routinedeclaration) {
        // this->children.push_back(simpledeclaration);
        this->children.push_back((Node*)(routinedeclaration));
    }
};

struct Program : Node {
    const char *nodeName = "Program";

    struct Declaration *declaration;
    struct Program *program;

    Program(Declaration *declaration, Program *program) :
            declaration(declaration),
            program(program) {
        // this->children.push_back(program);
        // this->children.push_back(declaration);  // todo: nuzhno `Declaration` obyavlyat vishe, ili vinesti cod v cpp
    }
};

struct TypeInRoutineDeclaration {
    struct Type *type;

    TypeInRoutineDeclaration(Type *type) :
            type(type) {}
};

struct BodyInRoutineDeclaration {
    struct Body *body;

    BodyInRoutineDeclaration(Body *body) :
            body(body) {}
};

struct Body {
    struct SimpleDeclaration *simpledeclaration;
    struct Statement *statement;
    struct Body *body;

    Body(SimpleDeclaration *simpledeclaration, Statement *statement, Body *body) :
            simpledeclaration(simpledeclaration),
            statement(statement),
            body(body) {}
};

struct Statement {
    struct Assignment *assignment;
    struct RoutineCall *routinecall;
    struct WhileLoop *whileloop;
    struct ForLoop *forloop;
    struct IfStatement *ifstatement;

    Statement(Assignment *assignment, RoutineCall *routinecall, WhileLoop *whileloop, ForLoop *forloop,
              IfStatement *ifstatement) :
            assignment(assignment),
            routinecall(routinecall),
            whileloop(whileloop),
            forloop(forloop),
            ifstatement(ifstatement) {}
};

struct Assignment {
    struct ModifiablePrimary *modifiableprimary;
    struct Expression *expression;

    Assignment(ModifiablePrimary *modifiableprimary, Expression *expression) :
            modifiableprimary(modifiableprimary),
            expression(expression) {}
};

struct RoutineCall {
    string name;
    struct ExpressionInRoutineCall *expressioninroutinecall;

    RoutineCall(string name, ExpressionInRoutineCall *expressioninroutinecall) :
            name(name),
            expressioninroutinecall(expressioninroutinecall) {}
};

struct ExpressionInRoutineCall {
    struct Expression *expression;
    struct ExpressionsInRoutineCall *expressionsinroutinecall;

    ExpressionInRoutineCall(Expression *expression, ExpressionsInRoutineCall *expressionsinroutinecall) :
            expression(expression),
            expressionsinroutinecall(expressionsinroutinecall) {}
};

struct ExpressionsInRoutineCall {
    struct Expression *expression;
    struct ExpressionInRoutineCall *expressioninroutinecall;

    ExpressionsInRoutineCall(Expression *expression, ExpressionInRoutineCall *expressioninroutinecall) :
            expression(expression),
            expressioninroutinecall(expressioninroutinecall) {}
};

struct WhileLoop {
    struct Expression *expression;
    struct Body *body;

    WhileLoop(Expression *expression, Body *body) :
            expression(expression),
            body(body) {}
};

struct ForLoop {
    string name;
    struct Reverse *reverse;
    struct Range *range;
    struct Body *body;

    ForLoop(string name, Reverse *reverse, Range *range, Body *body) :
            name(name),
            reverse(reverse),
            range(range),
            body(body) {}
};

struct Range {
    struct Expression *expression1;
    struct Expression *expression2;

    Range(Expression *expression1, Expression *expression2) :
            expression1(expression1),
            expression2(expression2) {}
};

struct Reverse {
    bool isreverse;

    Reverse(bool isreverse) :
            isreverse(isreverse) {}
};

struct IfStatement {
    struct Expression *expression1;
    struct Body *body;
    struct ElseInIfStatement *elseinifstatement;

    IfStatement(Expression *expression1, Body *body, ElseInIfStatement *elseinifstatement) :
            expression1(expression1),
            body(body),
            elseinifstatement(elseinifstatement) {}
};

struct ElseInIfStatement {
    struct Body *body;

    ElseInIfStatement(Body *body) :
            body(body) {}
};

struct Expression {
    struct Relation *relation;
    struct MultipleRelationsInExpression *multiplerelationsinexpression;

    Expression(Relation *relation, MultipleRelationsInExpression *multiplerelationsinexpression) :
            relation(relation),
            multiplerelationsinexpression(multiplerelationsinexpression) {}
};

struct MultipleRelationsInExpression {
    struct LogicalOperator *logicaloperator;
    struct Relation *relation;
    struct MultipleRelationsInExpression *multiplerelationsinexpression;

    MultipleRelationsInExpression(LogicalOperator *logicaloperator, Relation *relation,
                                  MultipleRelationsInExpression *multiplerelationsinexpression) :
            logicaloperator(logicaloperator),
            relation(relation),
            multiplerelationsinexpression(multiplerelationsinexpression) {}
};

struct LogicalOperator {
    string op;

    LogicalOperator(string op) :
            op(op) {}
};

struct Relation {
    struct Simple *simple;
    struct ComparisonInRelation *comparisoninrelation;

    Relation(Simple *simple, ComparisonInRelation *comparisoninrelation) :
            simple(simple),
            comparisoninrelation(comparisoninrelation) {}
};

struct ComparisonInRelation {
    struct ComparisonOperator *comparisonoperator;
    struct Simple *simple;

    ComparisonInRelation(ComparisonOperator *comparisonoperator, Simple *simple) :
            comparisonoperator(comparisonoperator),
            simple(simple) {}
};


struct ComparisonOperator {
    string op;

    ComparisonOperator(string op) :
            op(op) {}
};

struct Simple {
    struct Factor *factor;
    struct Factors *factors;

    Simple(Factor *factor, Factors *factors) :
            factor(factor),
            factors(factors) {}
};

struct Factors {
    struct SimpleOperator *simpleOperator;
    struct Factor *factor;
    struct Factors *factors;

    Factors(SimpleOperator *simpleOperator, Factor *factor, Factors *factors) :
            simpleOperator(simpleOperator),
            factor(factor),
            factors(factors) {}
};

struct SimpleOperator {
    string op;

    SimpleOperator(string op) :
            op(op) {}
};

struct Factor {
    struct Summand *summand;
    struct Summands *summands;

    Factor(Summand *summand, Summands *summands) :
            summand(summand),
            summands(summands) {}
};

struct Summands {
    struct Sign *sign;
    struct Summand *summand;
    struct Summands *summands;

    Summands(Sign *sign, Summand *summand, Summands *summands) :
            sign(sign),
            summand(summand),
            summands(summands) {}
};

struct Summand {
    struct Primary *primary;
    struct Expression *expression;

    Summand(Primary *primary, Expression *expression) :
            primary(primary),
            expression(expression) {}
};


struct Primary {
    string type;
    float value;
    bool isNot;
    struct Sign *sign;
    struct ModifiablePrimary *modifiablePrimary;

    Primary(string type, float value, bool isNot, struct Sign *sign, struct ModifiablePrimary *modifiablePrimary) :
            type(type),
            value(value),
            isNot(isNot),
            sign(sign),
            modifiablePrimary(modifiablePrimary) {}
};

struct Sign {
    string op;

    Sign(string op) :
            op(op) {}
};

struct ModifiablePrimary {
    string name;
    struct Identifiers *identifiers;

    ModifiablePrimary(string name, struct Identifiers *identifiers) :
            name(name),
            identifiers(identifiers) {}
};

struct Identifiers {
    string name;
    struct Expression *expression;
    struct Identifiers *identifiers;

    Identifiers(string name, struct Expression *expression, struct Identifiers *identifiers) :
            name(name),
            expression(expression),
            identifiers(identifiers) {}
};

#endif //CC_PROJECT_AST_H
