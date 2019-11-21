#ifndef CC_PROJECT_AST_H
#define CC_PROJECT_AST_H

#include <string>
#include <vector>
#include <iostream>
#include "AST.h"

using namespace std;

string prefix;

void add_spaces_to_prefix(int a) {
    for (int i = 0; i < a; i++, prefix += ' ');
}

void rem_from_prefix(int a) {
    for (int i = 0; i < a; i++, prefix.pop_back());
}

void print_bars(bool isLast) {
    cout << " │\n";

    if (isLast) cout << prefix + " └──";
    else cout << prefix + " ├──";

    if (!isLast) prefix += " │";
    else prefix += "  ";
}

void print_Program(Program *program, bool isLast) {
    if (!program) return;
    print_bars(isLast);

    cout << "Program";

    add_spaces_to_prefix(2 + 7);
    if (!program->program) print_Declaration(program->declaration, 1);
    else print_Declaration(program->declaration, 0);
    print_Program(program->program, 0);
    rem_from_prefix(6 + 7);
    cout << "\n" + prefix;
}

void print_Declaration(Declaration *declaration, bool isLast) {
    if (!declaration) return;
    print_bars(isLast);

    cout << "Declaration";

    add_spaces_to_prefix(2 + 11);
    if (!declaration->routinedeclaration) print_SimpleDeclaration(declaration->simpledeclaration, 1);
    else print_SimpleDeclaration(declaration->simpledeclaration, 0);
    print_RoutineDeclaration(declaration->routinedeclaration, 0);
    rem_from_prefix(6 + 11);
    cout << "\n" + prefix;
}

void print_SimpleDeclaration(SimpleDeclaration *simpledeclaration, bool isLast) {
    if (!simpledeclaration) return;
    print_bars(isLast);

    cout << "SimpleDeclaration";

    add_spaces_to_prefix(2 + 17);
    if (!simpledeclaration->typedeclaration) print_VariableDeclaration(simpledeclaration->variabledeclaration, 1);
    else print_VariableDeclaration(simpledeclaration->variabledeclaration, 0);
    print_TypeDeclaration(simpledeclaration->typedeclaration, 0);
    rem_from_prefix(6 + 17);
    cout << "\n" + prefix;
}

void print_VariableDeclaration(VariableDeclaration *variabledeclaration, bool isLast) {
    if (!variabledeclaration) return;
    print_bars(isLast);

    cout << "VariableDeclaration";

    add_spaces_to_prefix(2 + 19);
    if (!variabledeclaration->type && !variabledeclaration->initialvalue && !variabledeclaration->expression)
        cout << " |\n" + prefix + " !AUGUL!" << variabledeclaration->name << "\n" + prefix;
    else  cout << " |\n" + prefix + " !AYGUL!" << variabledeclaration->name << "\n" + prefix;

    if (!variabledeclaration->initialvalue && !variabledeclaration->expression)
        print_Type(variabledeclaration->type, 1);
    else print_Type(variabledeclaration->type, 0);
    if (!variabledeclaration->expression) print_InitialValue(variabledeclaration->initialvalue, 1);
    else print_InitialValue(variabledeclaration->initialvalue, 0);
    print_Expression(variabledeclaration->expression, 0);
    rem_from_prefix(6 + 19);
    cout << "\n" + prefix;
}

void print_InitialValue(InitialValue *initialvalue, bool isLast) {
    if (!initialvalue) return;
    print_bars(isLast);

    cout << "InitialValue";

    add_spaces_to_prefix(2 + 12);
    print_Expression(initialvalue->expression, 0);
    rem_from_prefix(6 + 12);
    cout << "\n" + prefix;
}

void print_TypeDeclaration(TypeDeclaration *typedeclaration, bool isLast) {
    if (!typedeclaration) return;
    print_bars(isLast);

    cout << "TypeDeclaration";

    add_spaces_to_prefix(2 + 15);
    if (!typedeclaration->type) cout << " |\n" + prefix + " !AUGUL!" << typedeclaration->name << "\n" + prefix;
    else  cout << " |\n" + prefix + " !AYGUL!" << typedeclaration->name << "\n" + prefix;

    print_Type(typedeclaration->type, 0);
    rem_from_prefix(6 + 15);
    cout << "\n" + prefix;
}

void print_Type(Type *type, bool isLast) {
    if (!type) return;
    print_bars(isLast);

    cout << "Type";

    add_spaces_to_prefix(2 + 4);
    if (!type->arraytype && !type->recordtype && !type->name) print_PrimitiveType(type->primitivetype, 1);
    else print_PrimitiveType(type->primitivetype, 0);
    if (!type->recordtype && !type->name) print_ArrayType(type->arraytype, 1);
    else print_ArrayType(type->arraytype, 0);
    if (!type->name) print_RecordType(type->recordtype, 1);
    else print_RecordType(type->recordtype, 0);
    cout << " |\n" + prefix + " !AYGUL!" << type->name << "\n" + prefix;

    rem_from_prefix(6 + 4);
    cout << "\n" + prefix;
}

void print_PrimitiveType(PrimitiveType *primitivetype, bool isLast) {
    if (!primitivetype) return;
    print_bars(isLast);

    cout << "PrimitiveType";

    add_spaces_to_prefix(2 + 13);
    cout << " |\n" + prefix + " !AYGUL!" << primitivetype->isint << "\n" + prefix;

    cout << " |\n" + prefix + " !AYGUL!" << primitivetype->isreal << "\n" + prefix;

    cout << " |\n" + prefix + " !AYGUL!" << primitivetype->isboolean << "\n" + prefix;

    rem_from_prefix(6 + 13);
    cout << "\n" + prefix;
}

void print_ArrayType(ArrayType *arraytype, bool isLast) {
    if (!arraytype) return;
    print_bars(isLast);

    cout << "ArrayType";

    add_spaces_to_prefix(2 + 9);
    if (!arraytype->type) print_Expression(arraytype->expression, 1);
    else print_Expression(arraytype->expression, 0);
    print_Type(arraytype->type, 0);
    rem_from_prefix(6 + 9);
    cout << "\n" + prefix;
}

void print_RecordType(RecordType *recordtype, bool isLast) {
    if (!recordtype) return;
    print_bars(isLast);

    cout << "RecordType";

    add_spaces_to_prefix(2 + 10);
    print_VariableDeclarations(recordtype->variabledeclarations, 0);
    rem_from_prefix(6 + 10);
    cout << "\n" + prefix;
}

void print_VariableDeclarations(VariableDeclarations *variabledeclarations, bool isLast) {
    if (!variabledeclarations) return;
    print_bars(isLast);

    cout << "VariableDeclarations";

    add_spaces_to_prefix(2 + 20);
    if (!variabledeclarations->variabledeclarations)
        print_VariableDeclaration(variabledeclarations->variabledeclaration, 1);
    else print_VariableDeclaration(variabledeclarations->variabledeclaration, 0);
    print_VariableDeclarations(variabledeclarations->variabledeclarations, 0);
    rem_from_prefix(6 + 20);
    cout << "\n" + prefix;
}

void print_RoutineDeclaration(RoutineDeclaration *routinedeclaration, bool isLast) {
    if (!routinedeclaration) return;
    print_bars(isLast);

    cout << "RoutineDeclaration";

    add_spaces_to_prefix(2 + 18);
    if (!routinedeclaration->parameters && !routinedeclaration->typeinroutinedeclaration &&
        !routinedeclaration->bodyinroutinedeclaration)
        cout << " |\n" + prefix + " !AUGUL!" << routinedeclaration->name << "\n" + prefix;
    else  cout << " |\n" + prefix + " !AYGUL!" << routinedeclaration->name << "\n" + prefix;

    if (!routinedeclaration->typeinroutinedeclaration && !routinedeclaration->bodyinroutinedeclaration)
        print_Parameters(routinedeclaration->parameters, 1);
    else print_Parameters(routinedeclaration->parameters, 0);
    if (!routinedeclaration->bodyinroutinedeclaration)
        print_TypeInRoutineDeclaration(routinedeclaration->typeinroutinedeclaration, 1);
    else print_TypeInRoutineDeclaration(routinedeclaration->typeinroutinedeclaration, 0);
    print_BodyInRoutineDeclaration(routinedeclaration->bodyinroutinedeclaration, 0);
    rem_from_prefix(6 + 18);
    cout << "\n" + prefix;
}

void print_Parameters(Parameters *parameters, bool isLast) {
    if (!parameters) return;
    print_bars(isLast);

    cout << "Parameters";

    add_spaces_to_prefix(2 + 10);
    if (!parameters->parametersdeclaration) print_ParameterDeclaration(parameters->parameterdeclaration, 1);
    else print_ParameterDeclaration(parameters->parameterdeclaration, 0);
    print_ParametersDeclaration(parameters->parametersdeclaration, 0);
    rem_from_prefix(6 + 10);
    cout << "\n" + prefix;
}

void print_ParameterDeclaration(ParameterDeclaration *parameterdeclaration, bool isLast) {
    if (!parameterdeclaration) return;
    print_bars(isLast);

    cout << "ParameterDeclaration";

    add_spaces_to_prefix(2 + 20);
    if (!parameterdeclaration->type) cout << " |\n" + prefix + " !AUGUL!" << parameterdeclaration->name << "\n" +
                                                                                                           prefix;
    else  cout << " |\n" + prefix + " !AYGUL!" << parameterdeclaration->name << "\n" + prefix;

    print_Type(parameterdeclaration->type, 0);
    rem_from_prefix(6 + 20);
    cout << "\n" + prefix;
}

void print_ParametersDeclaration(ParametersDeclaration *parametersdeclaration, bool isLast) {
    if (!parametersdeclaration) return;
    print_bars(isLast);

    cout << "ParametersDeclaration";

    add_spaces_to_prefix(2 + 21);
    if (!parametersdeclaration->parametersdeclaration)
        print_ParameterDeclaration(parametersdeclaration->parameterdeclaration, 1);
    else print_ParameterDeclaration(parametersdeclaration->parameterdeclaration, 0);
    print_ParametersDeclaration(parametersdeclaration->parametersdeclaration, 0);
    rem_from_prefix(6 + 21);
    cout << "\n" + prefix;
}

void print_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration, bool isLast) {
    if (!typeinroutinedeclaration) return;
    print_bars(isLast);

    cout << "TypeInRoutineDeclaration";

    add_spaces_to_prefix(2 + 24);
    print_Type(typeinroutinedeclaration->type, 0);
    rem_from_prefix(6 + 24);
    cout << "\n" + prefix;
}

void print_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration, bool isLast) {
    if (!bodyinroutinedeclaration) return;
    print_bars(isLast);

    cout << "BodyInRoutineDeclaration";

    add_spaces_to_prefix(2 + 24);
    print_Body(bodyinroutinedeclaration->body, 0);
    rem_from_prefix(6 + 24);
    cout << "\n" + prefix;
}

void print_Body(Body *body, bool isLast) {
    if (!body) return;
    print_bars(isLast);

    cout << "Body";

    add_spaces_to_prefix(2 + 4);
    if (!body->statement && !body->body) print_SimpleDeclaration(body->simpledeclaration, 1);
    else print_SimpleDeclaration(body->simpledeclaration, 0);
    if (!body->body) print_Statement(body->statement, 1);
    else print_Statement(body->statement, 0);
    print_Body(body->body, 0);
    rem_from_prefix(6 + 4);
    cout << "\n" + prefix;
}

void print_Statement(Statement *statement, bool isLast) {
    if (!statement) return;
    print_bars(isLast);

    cout << "Statement";

    add_spaces_to_prefix(2 + 9);
    if (!statement->routinecall && !statement->whileloop && !statement->forloop &&
        !statement->ifstatement)
        print_Assignment(statement->assignment, 1);
    else print_Assignment(statement->assignment, 0);
    if (!statement->whileloop && !statement->forloop && !statement->ifstatement)
        print_RoutineCall(statement->routinecall, 1);
    else print_RoutineCall(statement->routinecall, 0);
    if (!statement->forloop && !statement->ifstatement) print_WhileLoop(statement->whileloop, 1);
    else print_WhileLoop(statement->whileloop, 0);
    if (!statement->ifstatement) print_ForLoop(statement->forloop, 1);
    else print_ForLoop(statement->forloop, 0);
    print_IfStatement(statement->ifstatement, 0);
    rem_from_prefix(6 + 9);
    cout << "\n" + prefix;
}

void print_Assignment(Assignment *assignment, bool isLast) {
    if (!assignment) return;
    print_bars(isLast);

    cout << "Assignment";

    add_spaces_to_prefix(2 + 10);
    if (!assignment->expression) print_ModifiablePrimary(assignment->modifiableprimary, 1);
    else print_ModifiablePrimary(assignment->modifiableprimary, 0);
    print_Expression(assignment->expression, 0);
    rem_from_prefix(6 + 10);
    cout << "\n" + prefix;
}

void print_RoutineCall(RoutineCall *routinecall, bool isLast) {
    if (!routinecall) return;
    print_bars(isLast);

    cout << "RoutineCall";

    add_spaces_to_prefix(2 + 11);
    if (!routinecall->expressioninroutinecall) cout << " |\n" + prefix + " !AUGUL!" << routinecall->name << "\n" +
                                                                                                            prefix;
    else  cout << " |\n" + prefix + " !AYGUL!" << routinecall->name << "\n" + prefix;

    print_ExpressionInRoutineCall(routinecall->expressioninroutinecall, 0);
    rem_from_prefix(6 + 11);
    cout << "\n" + prefix;
}

void print_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall, bool isLast) {
    if (!expressioninroutinecall) return;
    print_bars(isLast);

    cout << "ExpressionInRoutineCall";

    add_spaces_to_prefix(2 + 23);
    if (!expressioninroutinecall->expressionsinroutinecall) print_Expression(expressioninroutinecall->expression, 1);
    else print_Expression(expressioninroutinecall->expression, 0);
    print_ExpressionsInRoutineCall(expressioninroutinecall->expressionsinroutinecall, 0);
    rem_from_prefix(6 + 23);
    cout << "\n" + prefix;
}

void print_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall, bool isLast) {
    if (!expressionsinroutinecall) return;
    print_bars(isLast);

    cout << "ExpressionsInRoutineCall";

    add_spaces_to_prefix(2 + 24);
    if (!expressionsinroutinecall->expressioninroutinecall) print_Expression(expressionsinroutinecall->expression, 1);
    else print_Expression(expressionsinroutinecall->expression, 0);
    print_ExpressionInRoutineCall(expressionsinroutinecall->expressioninroutinecall, 0);
    rem_from_prefix(6 + 24);
    cout << "\n" + prefix;
}

void print_WhileLoop(WhileLoop *whileloop, bool isLast) {
    if (!whileloop) return;
    print_bars(isLast);

    cout << "WhileLoop";

    add_spaces_to_prefix(2 + 9);
    if (!whileloop->body) print_Expression(whileloop->expression, 1);
    else print_Expression(whileloop->expression, 0);
    print_Body(whileloop->body, 0);
    rem_from_prefix(6 + 9);
    cout << "\n" + prefix;
}

void print_ForLoop(ForLoop *forloop, bool isLast) {
    if (!forloop) return;
    print_bars(isLast);

    cout << "ForLoop";

    add_spaces_to_prefix(2 + 7);
    if (!forloop->reverse && !forloop->range && !forloop->body)
        cout << " |\n" + prefix + " !AUGUL!" << forloop->name << "\n" + prefix;
    else  cout << " |\n" + prefix + " !AYGUL!" << forloop->name << "\n" + prefix;

    if (!forloop->range && !forloop->body) print_Reverse(forloop->reverse, 1);
    else print_Reverse(forloop->reverse, 0);
    if (!forloop->body) print_Range(forloop->range, 1);
    else print_Range(forloop->range, 0);
    print_Body(forloop->body, 0);
    rem_from_prefix(6 + 7);
    cout << "\n" + prefix;
}

void print_Range(Range *range, bool isLast) {
    if (!range) return;
    print_bars(isLast);

    cout << "Range";

    add_spaces_to_prefix(2 + 5);
    if (!range->expression) print_Expression(range->expression, 1);
    else print_Expression(range->expression, 0);
    print_Expression(range->expression, 0);
    rem_from_prefix(6 + 5);
    cout << "\n" + prefix;
}

void print_Reverse(Reverse *reverse, bool isLast) {
    if (!reverse) return;
    print_bars(isLast);

    cout << "Reverse";

    add_spaces_to_prefix(2 + 7);
    cout << " |\n" + prefix + " !AYGUL!" << reverse->isreverse << "\n" + prefix;

    rem_from_prefix(6 + 7);
    cout << "\n" + prefix;
}

void print_IfStatement(IfStatement *ifstatement, bool isLast) {
    if (!ifstatement) return;
    print_bars(isLast);

    cout << "IfStatement";

    add_spaces_to_prefix(2 + 11);
    if (!ifstatement->body && !ifstatement->elseinifstatement) print_Expression(ifstatement->expression, 1);
    else print_Expression(ifstatement->expression, 0);
    if (!ifstatement->elseinifstatement) print_Body(ifstatement->body, 1);
    else print_Body(ifstatement->body, 0);
    print_ElseInIfStatement(ifstatement->elseinifstatement, 0);
    rem_from_prefix(6 + 11);
    cout << "\n" + prefix;
}

void print_ElseInIfStatement(ElseInIfStatement *elseinifstatement, bool isLast) {
    if (!elseinifstatement) return;
    print_bars(isLast);

    cout << "ElseInIfStatement";

    add_spaces_to_prefix(2 + 17);
    print_Body(elseinifstatement->body, 0);
    rem_from_prefix(6 + 17);
    cout << "\n" + prefix;
}

void print_Expression(Expression *expression, bool isLast) {
    if (!expression) return;
    print_bars(isLast);

    cout << "Expression";

    add_spaces_to_prefix(2 + 10);
    if (!expression->multiplerelationsinexpression) print_Relation(expression->relation, 1);
    else print_Relation(expression->relation, 0);
    print_MultipleRelationsInExpression(expression->multiplerelationsinexpression, 0);
    rem_from_prefix(6 + 10);
    cout << "\n" + prefix;
}

void print_MultipleRelationsInExpression(MultipleRelationsInExpression *multiplerelationsinexpression, bool isLast) {
    if (!multiplerelationsinexpression) return;
    print_bars(isLast);

    cout << "MultipleRelationsInExpression";

    add_spaces_to_prefix(2 + 29);
    if (!multiplerelationsinexpression->relation &&
        !multiplerelationsinexpression->multiplerelationsinexpression)
        print_LogicalOperator(multiplerelationsinexpression->logicaloperator, 1);
    else print_LogicalOperator(multiplerelationsinexpression->logicaloperator, 0);
    if (!multiplerelationsinexpression->multiplerelationsinexpression)
        print_Relation(multiplerelationsinexpression->relation, 1);
    else print_Relation(multiplerelationsinexpression->relation, 0);
    print_MultipleRelationsInExpression(multiplerelationsinexpression->multiplerelationsinexpression, 0);
    rem_from_prefix(6 + 29);
    cout << "\n" + prefix;
}

void print_LogicalOperator(LogicalOperator *logicaloperator, bool isLast) {
    if (!logicaloperator) return;
    print_bars(isLast);

    cout << "LogicalOperator";

    add_spaces_to_prefix(2 + 15);
    cout << " |\n" + prefix + " !AYGUL!" << logicaloperator->op << "\n" + prefix;

    rem_from_prefix(6 + 15);
    cout << "\n" + prefix;
}

void print_Relation(Relation *relation, bool isLast) {
    if (!relation) return;
    print_bars(isLast);

    cout << "Relation";

    add_spaces_to_prefix(2 + 8);
}


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

struct Declaration  {
    struct SimpleDeclaration *simpledeclaration;
    struct RoutineDeclaration *routinedeclaration;

    Declaration(SimpleDeclaration *simpledeclaration, RoutineDeclaration *routinedeclaration) :
            simpledeclaration(simpledeclaration),
            routinedeclaration(routinedeclaration) {}
};

struct Program  {
    struct Declaration *declaration;
    struct Program *program;

    Program(Declaration *declaration, Program *program) :
            declaration(declaration),
            program(program) {}
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
