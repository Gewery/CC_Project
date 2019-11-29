#include <string>
#include <vector>
#include <iostream>


using namespace std;


struct Program {
    struct Declaration *declaration;
    struct Program *program;
    Program(Declaration *declaration, Program *program) :
            declaration(declaration),
            program(program) {}
};

struct Declaration {
    struct SimpleDeclaration *simpledeclaration;
    struct RoutineDeclaration *routinedeclaration;
    Declaration(SimpleDeclaration *simpledeclaration, RoutineDeclaration *routinedeclaration) :
            simpledeclaration(simpledeclaration),
            routinedeclaration(routinedeclaration) {}
};


struct SimpleDeclaration {
    struct VariableDeclaration *variabledeclaration;
    struct TypeDeclaration *typedeclaration;
    SimpleDeclaration(VariableDeclaration *variabledeclaration, TypeDeclaration *typedeclaration) :
            variabledeclaration(variabledeclaration),
            typedeclaration(typedeclaration) {};
};

struct VariableDeclaration {
    string name;
    struct Type *type;
    struct InitialValue *initialvalue;
    struct Expression *expression;
    VariableDeclaration(string name, Type *type, InitialValue *initialvalue, Expression *expression):
            name(name),
            type(type),
            initialvalue(initialvalue),
            expression(expression) {};
};

struct InitialValue {
    struct Expression *expression;
    InitialValue(Expression *expression):
            expression(expression) {};
};

struct TypeDeclaration {
    string name;
    struct Type *type;
    TypeDeclaration(string name, Type *type):
            name(name),
            type(type) {};
};

struct Type {
    struct PrimitiveType *primitivetype;
    struct ArrayType *arraytype;
    struct RecordType *recordtype;
    string name;
    Type(PrimitiveType *primitivetype, ArrayType *arraytype, RecordType *recordtype, string name):
            primitivetype(primitivetype),
            arraytype(arraytype),
            recordtype(recordtype),
            name(name) {};
};

struct PrimitiveType {
    bool isint;
    bool isreal;
    bool isboolean;
    PrimitiveType(bool isint, bool isreal, bool isboolean):
            isint(isint),
            isreal(isreal),
            isboolean(isboolean) {};
};

struct ArrayType {
    struct Expression *expression;
    struct Type *type;
    ArrayType(Expression *expression, Type *type):
            expression(expression),
            type(type) {};
};

struct RecordType {
    struct VariableDeclarations *variabledeclarations;
    RecordType(VariableDeclarations *variabledeclarations):
            variabledeclarations(variabledeclarations) {};
};

struct VariableDeclarations {
    struct VariableDeclaration *variabledeclaration;
    struct VariableDeclarations *variabledeclarations;
    VariableDeclarations(VariableDeclaration *variabledeclaration, VariableDeclarations *variabledeclarations):
            variabledeclaration(variabledeclaration),
            variabledeclarations(variabledeclarations) {};
};

struct RoutineDeclaration {
    string name;
    struct Parameters *parameters;
    struct TypeInRoutineDeclaration *typeinroutinedeclaration;
    struct BodyInRoutineDeclaration *bodyinroutinedeclaration;
    struct ReturnInRoutine *returnInRoutine;
    RoutineDeclaration(string name, Parameters *parameters, TypeInRoutineDeclaration *typeinroutinedeclaration,
                       BodyInRoutineDeclaration *bodyinroutinedeclaration, struct ReturnInRoutine *returnInRoutine):
            name(name),
            parameters(parameters),
            typeinroutinedeclaration(typeinroutinedeclaration),
            bodyinroutinedeclaration(bodyinroutinedeclaration),
            returnInRoutine(returnInRoutine) {};
};

struct ReturnInRoutine {
    struct Expression *expression;
    ReturnInRoutine(struct Expression *expression):
            expression(expression) {};
};

struct Parameters {
    struct ParameterDeclaration *parameterdeclaration;
    struct ParametersDeclaration *parametersdeclaration;
    Parameters(ParameterDeclaration *parameterdeclaration, ParametersDeclaration *parametersdeclaration):
            parameterdeclaration(parameterdeclaration),
            parametersdeclaration(parametersdeclaration) {};
};

struct ParameterDeclaration {
    string name;
    struct Type *type;

    ParameterDeclaration(string name, Type *type):
            name(name),
            type(type) {};
};

struct ParametersDeclaration {
    struct ParameterDeclaration *parameterdeclaration;
    struct ParametersDeclaration *parametersdeclaration;

    ParametersDeclaration(ParameterDeclaration *parameterdeclaration, ParametersDeclaration *parametersdeclaration):
            parameterdeclaration(parameterdeclaration),
            parametersdeclaration(parametersdeclaration) {};
};

struct TypeInRoutineDeclaration {
    struct Type *type;
    TypeInRoutineDeclaration(Type *type):
            type(type) {};
};

struct BodyInRoutineDeclaration {
    struct Body *body;
    BodyInRoutineDeclaration(Body *body):
            body(body) {};
};

struct Body {
    struct SimpleDeclaration *simpledeclaration;
    struct Statement *statement;
    struct Body *body;
    Body(SimpleDeclaration *simpledeclaration, Statement *statement, Body *body):
            simpledeclaration(simpledeclaration),
            statement(statement),
            body(body) {};
};

struct Statement {
    struct Assignment *assignment;
    struct RoutineCall *routinecall;
    struct WhileLoop *whileloop;
    struct ForLoop *forloop;
    struct IfStatement *ifstatement;
    Statement(Assignment *assignment, RoutineCall *routinecall, WhileLoop *whileloop, ForLoop *forloop,
              IfStatement *ifstatement):
            assignment(assignment),
            routinecall(routinecall),
            whileloop(whileloop),
            forloop(forloop),
            ifstatement(ifstatement) {};
};

struct Assignment {
    struct ModifiablePrimary *modifiableprimary;
    struct Expression *expression;
    Assignment(ModifiablePrimary *modifiableprimary, Expression *expression):
            modifiableprimary(modifiableprimary),
            expression(expression) {};
};

struct RoutineCall {
    string name;
    struct ExpressionInRoutineCall *expressioninroutinecall;
    RoutineCall(string name, ExpressionInRoutineCall *expressioninroutinecall):
            name(name),
            expressioninroutinecall(expressioninroutinecall) {};
};

struct ExpressionInRoutineCall {
    struct Expression *expression;
    struct ExpressionsInRoutineCall *expressionsinroutinecall;
    ExpressionInRoutineCall(Expression *expression, ExpressionsInRoutineCall *expressionsinroutinecall):
            expression(expression),
            expressionsinroutinecall(expressionsinroutinecall) {};
};

struct ExpressionsInRoutineCall {
    struct Expression *expression;
    struct ExpressionInRoutineCall *expressioninroutinecall;
    ExpressionsInRoutineCall(Expression *expression, ExpressionInRoutineCall *expressioninroutinecall):
            expression(expression),
            expressioninroutinecall(expressioninroutinecall) {};
};

struct WhileLoop {
    struct Expression *expression;
    struct Body *body;
    WhileLoop(Expression *expression, Body *body):
            expression(expression),
            body(body) {};
};

struct ForLoop {
    string name;
    struct Reverse *reverse;
    struct Range *range;
    struct Body *body;
    ForLoop(string name, Reverse *reverse, Range *range, Body *body):
            name(name),
            reverse(reverse),
            range(range),
            body(body) {};
};

struct Range {
    struct Expression *expression1;
    struct Expression *expression2;
    Range(Expression *expression1, Expression *expression2):
            expression1(expression1),
            expression2(expression2) {};
};

struct Reverse {
    bool isreverse;
    Reverse(bool isreverse):
            isreverse(isreverse) {};
};

struct IfStatement {
    struct Expression *expression;
    struct Body *body;
    struct ElseInIfStatement *elseinifstatement;
    IfStatement(Expression *expression, Body *body, ElseInIfStatement *elseinifstatement):
            expression(expression),
            body(body),
            elseinifstatement(elseinifstatement) {};
};

struct ElseInIfStatement {
    struct Body *body;
    ElseInIfStatement(Body *body):
            body(body) {};
};

struct Expression {
    struct Relation *relation;
    struct MultipleRelationsInExpression *multiplerelationsinexpression;
    Expression(Relation *relation, MultipleRelationsInExpression *multiplerelationsinexpression):
            relation(relation),
            multiplerelationsinexpression(multiplerelationsinexpression) {};
};

struct MultipleRelationsInExpression {
    struct LogicalOperator *logicaloperator;
    struct Relation *relation;
    struct MultipleRelationsInExpression *multiplerelationsinexpression;
    MultipleRelationsInExpression(LogicalOperator *logicaloperator, Relation *relation,
                                  MultipleRelationsInExpression *multiplerelationsinexpression):
            logicaloperator(logicaloperator),
            relation(relation),
            multiplerelationsinexpression(multiplerelationsinexpression) {};
};

struct LogicalOperator {
    string op;
    LogicalOperator(string op):
            op(op) {};
};

struct Relation {
    struct Simple *simple;
    struct ComparisonInRelation *comparisoninrelation;
    Relation(Simple *simple, ComparisonInRelation *comparisoninrelation):
            simple(simple),
            comparisoninrelation(comparisoninrelation){};
};

struct ComparisonInRelation {
    struct ComparisonOperator *comparisonoperator;
    struct Simple *simple;
    ComparisonInRelation(ComparisonOperator *comparisonoperator, Simple *simple):
            comparisonoperator(comparisonoperator),
            simple(simple) {};
};


struct ComparisonOperator {
    string op;
    ComparisonOperator(string op):
            op(op) {};
};

struct Simple {
    struct Factor* factor;
    struct Factors* factors;
    Simple(Factor* factor, Factors* factors):
            factor(factor), factors(factors) {};
};

struct Factors {
    struct SimpleOperator* simpleOperator;
    struct Factor* factor;
    struct Factors* factors;
    Factors(SimpleOperator* simpleOperator, Factor* factor, Factors* factors):
            simpleOperator(simpleOperator),
            factor(factor),
            factors(factors) {};
};

struct SimpleOperator {
    string op;
    SimpleOperator(string op):
            op(op) {};
};

struct Factor {
    struct Summand* summand;
    struct Summands* summands;
    Factor(Summand* summand, Summands* summands):
            summand(summand),
            summands(summands) {};
};

struct Summands {
    struct Sign* sign;
    struct Summand* summand;
    struct Summands* summands;
    Summands(Sign* sign, Summand* summand, Summands* summands):
            sign(sign),
            summand(summand),
            summands(summands) {};
};

struct Summand {
    struct Primary* primary;
    struct Expression* expression;
    Summand(Primary* primary, Expression* expression):
            primary(primary),
            expression(expression) {};
};

struct Primary {
    string type;
    float value;
    bool isNot;
    struct Sign* sign;
    struct ModifiablePrimary* modifiablePrimary;
    Primary(string type, float value,  bool isNot, struct Sign* sign, struct ModifiablePrimary* modifiablePrimary):
            type(type),
            value(value),
            isNot(isNot),
            sign(sign),
            modifiablePrimary(modifiablePrimary) {};
};

struct Sign {
    string op;
    Sign(string op):
            op(op) {};
};

struct ModifiablePrimary {
    string name;
    struct Identifiers* identifiers;
    ModifiablePrimary(string name, struct Identifiers* identifiers):
            name(name),
            identifiers(identifiers) {};
};

struct Identifiers {
    string name;
    struct Expression* expression;
    struct Identifiers* identifiers;
    Identifiers(string name, struct Expression* expression, struct Identifiers* identifiers):
            name(name),
            expression(expression),
            identifiers(identifiers) {};
};

void print_Tree(Program *program);
void print_Program(string prefix, Program *program, bool is_last);
void print_Declaration(string prefix, Declaration *declaration, bool is_last);
void print_SimpleDeclaration(string prefix, SimpleDeclaration *simpledeclaration, bool isLast);
void print_VariableDeclaration(string prefix, VariableDeclaration *variabledeclaration, bool isLast);
void print_InitialValue(string prefix, InitialValue *initialvalue, bool isLast);
void print_TypeDeclaration(string prefix, TypeDeclaration *typedeclaration, bool isLast);
void print_Type(string prefix, Type *type, bool isLast);
void print_PrimitiveType(string prefix, PrimitiveType *primitivetype, bool isLast);
void print_ArrayType(string prefix, ArrayType *arraytype, bool isLast);
void print_RecordType(string prefix, RecordType *recordtype, bool isLast);
void print_VariableDeclarations(string prefix, VariableDeclarations *variabledeclarations, bool isLast);
void print_RoutineDeclaration(string prefix, RoutineDeclaration *routinedeclaration, bool isLast);
void print_ReturnInRoutine(string prefix, ReturnInRoutine *returnInRoutine, bool is_last);
void print_Parameters(string prefix, Parameters *parameters, bool isLast);
void print_ParameterDeclaration(string prefix, ParameterDeclaration *parameterdeclaration, bool isLast);
void print_ParametersDeclaration(string prefix, ParametersDeclaration *parametersdeclaration, bool isLast);
void print_TypeInRoutineDeclaration(string prefix, TypeInRoutineDeclaration *typeinroutinedeclaration, bool isLast);
void print_BodyInRoutineDeclaration(string prefix, BodyInRoutineDeclaration *bodyinroutinedeclaration, bool isLast);
void print_Body(string prefix, Body *body, bool isLast);
void print_Statement(string prefix, Statement *statement, bool isLast);
void print_Assignment(string prefix, Assignment *assignment, bool isLast);
void print_RoutineCall(string prefix, RoutineCall *routinecall, bool isLast);
void print_ExpressionInRoutineCall(string prefix, ExpressionInRoutineCall *expressioninroutinecall, bool isLast);
void print_ExpressionsInRoutineCall(string prefix, ExpressionsInRoutineCall *expressionsinroutinecall, bool isLast);
void print_WhileLoop(string prefix, WhileLoop *whileloop, bool isLast);
void print_ForLoop(string prefix, ForLoop *forloop, bool isLast);
void print_Range(string prefix, Range *range, bool isLast);
void print_Reverse(string prefix, Reverse *reverse, bool isLast);
void print_IfStatement(string prefix, IfStatement *ifstatement, bool isLast);
void print_ElseInIfStatement(string prefix, ElseInIfStatement *elseinifstatement, bool isLast);
void print_Expression(string prefix, Expression *expression, bool isLast);
void print_MultipleRelationsInExpression(string prefix, MultipleRelationsInExpression *multiplerelationsinexpression, bool isLast);
void print_LogicalOperator(string prefix, LogicalOperator *logicaloperator, bool isLast);
void print_Relation(string prefix, Relation *relation, bool isLast);
void print_ComparisonInRelation(string prefix, ComparisonInRelation *comparisoninrelation, bool isLast);
void print_ComparisonOperator(string prefix, ComparisonOperator *comparisonoperator, bool isLast);
void print_Simple(string prefix, Simple *simple, bool isLast);
void print_Factors(string prefix, Factors *factors, bool isLast);
void print_SimpleOperator(string prefix, SimpleOperator *simpleoperator, bool isLast);
void print_Factor(string prefix, Factor *factor, bool isLast);
void print_Summands(string prefix, Summands *summands, bool isLast);
void print_Summand(string prefix, Summand *summand, bool isLast);
void print_Primary(string prefix, Primary *primary, bool isLast);
void print_Sign(string prefix, Sign *sign, bool isLast);
void print_ModifiablePrimary(string prefix, ModifiablePrimary *modifiableprimary, bool isLast);
void print_Identifiers(string prefix, Identifiers *identifiers, bool isLast);
