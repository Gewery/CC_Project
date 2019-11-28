#ifndef CC_PROJECT_AST_H
#define CC_PROJECT_AST_H

#include <string>
#include <vector>
#include <iostream>

#include <math.h>
#include <bits/stdc++.h>

#include "../SemanticAnalysis/SymbolTable.h"
#include "../TraceLogger/TraceLogger.h"

using namespace std;

#define NEW_PREFIX (prefix + (is_last ? "   " : "│  "))
#define PRINT_INFO(INFO) printf("%s%s%s\n", prefix.c_str(), (is_last ? "└──" : "├──"), INFO)

string format(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::vector<char> v(1024);
    while (true) {
        va_list args2;
        va_copy(args2, args);
        int res = vsnprintf(v.data(), v.size(), fmt, args2);
        if ((res >= 0) && (res < static_cast<int>(v.size()))) {
            va_end(args);
            va_end(args2);
            return std::string(v.data());
        }
        size_t size;
        if (res < 0)
            size = v.size() * 2;
        else
            size = static_cast<size_t>(res) + 1;
        v.clear();
        v.resize(size);
        va_end(args2);
    }
}


void print_Tree(Program *program) {
    print_Program("", program, true);
}

void print_Program(string prefix, Program *program, bool is_last) {
    if (!program) return;
    PRINT_INFO("Program");

    print_Declaration(
            NEW_PREFIX,
            program->declaration,
            !program->program);
    print_Program(
            NEW_PREFIX,
            program->program, 1);
}

void print_Declaration(string prefix, Declaration *declaration, bool is_last) {
    if (!declaration) return;
    PRINT_INFO("Declaration");

    print_SimpleDeclaration(
            NEW_PREFIX,
            declaration->simpledeclaration,
            !declaration->routinedeclaration);
    print_RoutineDeclaration(
            NEW_PREFIX,
            declaration->routinedeclaration, 1);
}

void print_SimpleDeclaration(string prefix, SimpleDeclaration *simpledeclaration, bool is_last) {
    if (!simpledeclaration) return;
    PRINT_INFO("SimpleDeclaration");

    print_VariableDeclaration(
            NEW_PREFIX,
            simpledeclaration->variabledeclaration,
            !simpledeclaration->typedeclaration);
    print_TypeDeclaration(
            NEW_PREFIX,
            simpledeclaration->typedeclaration, 1);
}

void print_VariableDeclaration(string prefix, VariableDeclaration *variabledeclaration, bool is_last) {
    if (!variabledeclaration) return;
    PRINT_INFO(format("VariableDeclaration (%s)", variabledeclaration->name.c_str()).c_str());

    print_Type(
            NEW_PREFIX,
            variabledeclaration->type,
            !variabledeclaration->initialvalue && !variabledeclaration->expression);
    print_InitialValue(
            NEW_PREFIX,
            variabledeclaration->initialvalue,
            !variabledeclaration->expression);
    print_Expression(
            NEW_PREFIX,
            variabledeclaration->expression, 1);
}

void print_InitialValue(string prefix, InitialValue *initialvalue, bool is_last) {
    if (!initialvalue) return;
    PRINT_INFO("InitialValue");

    print_Expression(
            NEW_PREFIX,
            initialvalue->expression, 1);
}

void print_TypeDeclaration(string prefix, TypeDeclaration *typedeclaration, bool is_last) {
    if (!typedeclaration) return;
    PRINT_INFO(format("TypeDeclaration (%s)", typedeclaration->name.c_str()).c_str());

    print_Type(
            NEW_PREFIX,
            typedeclaration->type, 1);
}

void print_Type(string prefix, Type *type, bool is_last) {
    if (!type) return;
    PRINT_INFO(format("Type (%s)", type->name.c_str()).c_str());

    print_PrimitiveType(
            NEW_PREFIX,
            type->primitivetype,
            !type->arraytype && !type->recordtype);
    print_ArrayType(
            NEW_PREFIX,
            type->arraytype,
            !type->recordtype);
    print_RecordType(
            NEW_PREFIX,
            type->recordtype, 1);
}

void print_PrimitiveType(string prefix, PrimitiveType *primitivetype, bool is_last) {
    if (!primitivetype) return;
    PRINT_INFO(format("PrimitiveType (%s)", primitivetype->isint ? "int" :
                                            primitivetype->isreal ? "real" :
                                            primitivetype->isboolean ? "bool" : "undef").c_str());
}

void print_ArrayType(string prefix, ArrayType *arraytype, bool is_last) {
    if (!arraytype) return;
    PRINT_INFO("ArrayType");

    print_Expression(
            NEW_PREFIX,
            arraytype->expression,
            !arraytype->type);
    print_Type(
            NEW_PREFIX,
            arraytype->type, 1);
}

void print_RecordType(string prefix, RecordType *recordtype, bool is_last) {
    if (!recordtype) return;
    PRINT_INFO("RecordType");

    print_VariableDeclarations(
            NEW_PREFIX,
            recordtype->variabledeclarations, 1);
}

void print_VariableDeclarations(string prefix, VariableDeclarations *variabledeclarations, bool is_last) {
    if (!variabledeclarations) return;
    PRINT_INFO("VariableDeclarations");

    print_VariableDeclaration(
            NEW_PREFIX,
            variabledeclarations->variabledeclaration,
            !variabledeclarations->variabledeclarations);
    print_VariableDeclarations(
            NEW_PREFIX,
            variabledeclarations->variabledeclarations, 1);
}

void print_RoutineDeclaration(string prefix, RoutineDeclaration *routinedeclaration, bool is_last) {
    if (!routinedeclaration) return;
    PRINT_INFO(format("RoutineDeclaration (%s)", routinedeclaration->name.c_str()).c_str());

    print_Parameters(
            NEW_PREFIX,
            routinedeclaration->parameters,
            !routinedeclaration->typeinroutinedeclaration && !routinedeclaration->bodyinroutinedeclaration);
    print_TypeInRoutineDeclaration(
            NEW_PREFIX,
            routinedeclaration->typeinroutinedeclaration,
            !routinedeclaration->bodyinroutinedeclaration);
    print_BodyInRoutineDeclaration(
            NEW_PREFIX,
            routinedeclaration->bodyinroutinedeclaration, 1);
}

void print_Parameters(string prefix, Parameters *parameters, bool is_last) {
    if (!parameters) return;
    PRINT_INFO("Parameters");

    print_ParameterDeclaration(
            NEW_PREFIX,
            parameters->parameterdeclaration,
            !parameters->parametersdeclaration);
    print_ParametersDeclaration(
            NEW_PREFIX,
            parameters->parametersdeclaration, 1);
}

void print_ParameterDeclaration(string prefix, ParameterDeclaration *parameterdeclaration, bool is_last) {
    if (!parameterdeclaration) return;
    PRINT_INFO(format("ParameterDeclaration (%s)", parameterdeclaration->name.c_str()).c_str());

    print_Type(
            NEW_PREFIX,
            parameterdeclaration->type, 1);
}

void print_ParametersDeclaration(string prefix, ParametersDeclaration *parametersdeclaration, bool is_last) {
    if (!parametersdeclaration) return;
    PRINT_INFO("ParametersDeclaration");

    print_ParameterDeclaration(
            NEW_PREFIX,
            parametersdeclaration->parameterdeclaration,
            !parametersdeclaration->parametersdeclaration);
    print_ParametersDeclaration(
            NEW_PREFIX,
            parametersdeclaration->parametersdeclaration, 1);
}

void print_TypeInRoutineDeclaration(string prefix, TypeInRoutineDeclaration *typeinroutinedeclaration, bool is_last) {
    if (!typeinroutinedeclaration) return;
    PRINT_INFO("TypeInRoutineDeclaration");

    print_Type(
            NEW_PREFIX,
            typeinroutinedeclaration->type, 1);
}

void print_BodyInRoutineDeclaration(string prefix, BodyInRoutineDeclaration *bodyinroutinedeclaration, bool is_last) {
    if (!bodyinroutinedeclaration) return;
    PRINT_INFO("BodyInRoutineDeclaration");

    print_Body(
            NEW_PREFIX,
            bodyinroutinedeclaration->body, 1);
}

void print_Body(string prefix, Body *body, bool is_last) {
    if (!body) return;
    PRINT_INFO("Body");

    print_SimpleDeclaration(
            NEW_PREFIX,
            body->simpledeclaration,
            !body->statement && !body->body);
    print_Statement(
            NEW_PREFIX,
            body->statement,
            !body->body);
    print_Body(
            NEW_PREFIX,
            body->body, 1);
}

void print_Statement(string prefix, Statement *statement, bool is_last) {
    if (!statement) return;
    PRINT_INFO("Statement");

    print_Assignment(
            NEW_PREFIX,
            statement->assignment,
            !statement->routinecall && !statement->whileloop && !statement->forloop && !statement->ifstatement);
    print_RoutineCall(
            NEW_PREFIX,
            statement->routinecall,
            !statement->whileloop && !statement->forloop && !statement->ifstatement);
    print_WhileLoop(
            NEW_PREFIX,
            statement->whileloop,
            !statement->forloop && !statement->ifstatement);
    print_ForLoop(
            NEW_PREFIX,
            statement->forloop,
            !statement->ifstatement);
    print_IfStatement(
            NEW_PREFIX,
            statement->ifstatement, 1);
}

void print_Assignment(string prefix, Assignment *assignment, bool is_last) {
    if (!assignment) return;
    PRINT_INFO("Assignment");

    print_ModifiablePrimary(
            NEW_PREFIX,
            assignment->modifiableprimary,
            !assignment->expression);
    print_Expression(
            NEW_PREFIX,
            assignment->expression, 1);
}

void print_RoutineCall(string prefix, RoutineCall *routinecall, bool is_last) {
    if (!routinecall) return;
    PRINT_INFO(format("RoutineCall (%s)", routinecall->name.c_str()).c_str());

    print_ExpressionInRoutineCall(
            NEW_PREFIX,
            routinecall->expressioninroutinecall, 1);
}

void print_ExpressionInRoutineCall(string prefix, ExpressionInRoutineCall *expressioninroutinecall, bool is_last) {
    if (!expressioninroutinecall) return;
    PRINT_INFO("ExpressionInRoutineCall");

    print_Expression(
            NEW_PREFIX,
            expressioninroutinecall->expression,
            !expressioninroutinecall->expressionsinroutinecall);
    print_ExpressionsInRoutineCall(
            NEW_PREFIX,
            expressioninroutinecall->expressionsinroutinecall, 1);
}

void print_ExpressionsInRoutineCall(string prefix, ExpressionsInRoutineCall *expressionsinroutinecall, bool is_last) {
    if (!expressionsinroutinecall) return;
    PRINT_INFO("ExpressionsInRoutineCall");

    print_Expression(
            NEW_PREFIX,
            expressionsinroutinecall->expression,
            !expressionsinroutinecall->expressioninroutinecall);
    print_ExpressionInRoutineCall(
            NEW_PREFIX,
            expressionsinroutinecall->expressioninroutinecall, 1);
}

void print_WhileLoop(string prefix, WhileLoop *whileloop, bool is_last) {
    if (!whileloop) return;
    PRINT_INFO("WhileLoop");

    print_Expression(
            NEW_PREFIX,
            whileloop->expression,
            !whileloop->body);
    print_Body(
            NEW_PREFIX,
            whileloop->body, 1);
}

void print_ForLoop(string prefix, ForLoop *forloop, bool is_last) {
    if (!forloop) return;
    PRINT_INFO(format("ForLoop (%s)", forloop->name.c_str()).c_str());

    print_Reverse(
            NEW_PREFIX,
            forloop->reverse,
            !forloop->range && !forloop->body);
    print_Range(
            NEW_PREFIX,
            forloop->range,
            !forloop->body);
    print_Body(
            NEW_PREFIX,
            forloop->body, 1);
}

void print_Range(string prefix, Range *range, bool is_last) {
    if (!range) return;
    PRINT_INFO("Range");

    print_Expression(
            NEW_PREFIX,
            range->expression2,
            !range->expression1);
    print_Expression(
            NEW_PREFIX,
            range->expression1, 1);
}

void print_Reverse(string prefix, Reverse *reverse, bool is_last) {
    if (!reverse) return;
    PRINT_INFO(format("RoutineCall (%s)", reverse->isreverse ? "is_inverse" : "not_inverse").c_str());
}

void print_IfStatement(string prefix, IfStatement *ifstatement, bool is_last) {
    if (!ifstatement) return;
    PRINT_INFO("IfStatement");

    print_Expression(
            NEW_PREFIX,
            ifstatement->expression,
            !ifstatement->body && !ifstatement->elseinifstatement);
    print_Body(
            NEW_PREFIX,
            ifstatement->body,
            !ifstatement->elseinifstatement);
    print_ElseInIfStatement(
            NEW_PREFIX,
            ifstatement->elseinifstatement, 1);
}

void print_ElseInIfStatement(string prefix, ElseInIfStatement *elseinifstatement, bool is_last) {
    if (!elseinifstatement) return;
    PRINT_INFO("ElseInIfStatement");

    print_Body(
            NEW_PREFIX,
            elseinifstatement->body, 1);
}

void print_Expression(string prefix, Expression *expression, bool is_last) {
    if (!expression) return;
    PRINT_INFO("Expression");

    print_Relation(
            NEW_PREFIX,
            expression->relation,
            !expression->multiplerelationsinexpression);
    print_MultipleRelationsInExpression(
            NEW_PREFIX,
            expression->multiplerelationsinexpression, 1);
}

void print_MultipleRelationsInExpression(string prefix, MultipleRelationsInExpression *multiplerelationsinexpression,
                                         bool is_last) {
    if (!multiplerelationsinexpression) return;
    PRINT_INFO("MultipleRelationsInExpression");

    print_LogicalOperator(
            NEW_PREFIX,
            multiplerelationsinexpression->logicaloperator,
            !multiplerelationsinexpression->relation && !multiplerelationsinexpression->multiplerelationsinexpression);
    print_Relation(
            NEW_PREFIX,
            multiplerelationsinexpression->relation,
            !multiplerelationsinexpression->multiplerelationsinexpression);
    print_MultipleRelationsInExpression(
            NEW_PREFIX,
            multiplerelationsinexpression->multiplerelationsinexpression, 1);
}

void print_LogicalOperator(string prefix, LogicalOperator *logicaloperator, bool is_last) {
    if (!logicaloperator) return;
    PRINT_INFO(format("LogicalOperator (%s)", logicaloperator->op.c_str()).c_str());
}

void print_Relation(string prefix, Relation *relation, bool is_last) {
    if (!relation) return;
    PRINT_INFO("Relation");

    print_Simple(
            NEW_PREFIX,
            relation->simple,
            !relation->comparisoninrelation);
    print_ComparisonInRelation(
            NEW_PREFIX,
            relation->comparisoninrelation, 1);
}

void print_ComparisonInRelation(string prefix, ComparisonInRelation *comparisoninrelation, bool is_last) {
    if (!comparisoninrelation) return;
    PRINT_INFO("ComparisonInRelation");

    print_ComparisonOperator(
            NEW_PREFIX,
            comparisoninrelation->comparisonoperator,
            !comparisoninrelation->simple);
    print_Simple(
            NEW_PREFIX,
            comparisoninrelation->simple, 1);
}

void print_ComparisonOperator(string prefix, ComparisonOperator *comparisonoperator, bool is_last) {
    if (!comparisonoperator) return;
    PRINT_INFO(format("ComparisonOperator (%s)", comparisonoperator->op.c_str()).c_str());
}

void print_Simple(string prefix, Simple *simple, bool is_last) {
    if (!simple) return;
    PRINT_INFO("Simple");

    print_Factor(
            NEW_PREFIX,
            simple->factor,
            !simple->factors);
    print_Factors(
            NEW_PREFIX,
            simple->factors, 1);
}

void print_Factors(string prefix, Factors *factors, bool is_last) {
    if (!factors) return;
    PRINT_INFO("Factors");

    print_SimpleOperator(
            NEW_PREFIX,
            factors->simpleOperator,
            !factors->factor && !factors->factors);
    print_Factor(
            NEW_PREFIX,
            factors->factor,
            !factors->factors);
    print_Factors(
            NEW_PREFIX,
            factors->factors, 1);
}

void print_SimpleOperator(string prefix, SimpleOperator *simpleoperator, bool is_last) {
    if (!simpleoperator) return;
    PRINT_INFO(format("SimpleOperator (%s)", simpleoperator->op.c_str()).c_str());
}

void print_Factor(string prefix, Factor *factor, bool is_last) {
    if (!factor) return;
    PRINT_INFO("Factor");

    print_Summand(
            NEW_PREFIX,
            factor->summand,
            !factor->summands);
    print_Summands(
            NEW_PREFIX,
            factor->summands, 1);
}

void print_Summands(string prefix, Summands *summands, bool is_last) {
    if (!summands) return;
    PRINT_INFO("Summands");

    print_Sign(
            NEW_PREFIX,
            summands->sign,
            !summands->summand && !summands->summands);
    print_Summand(
            NEW_PREFIX,
            summands->summand,
            !summands->summands);
    print_Summands(
            NEW_PREFIX,
            summands->summands, 1);
}

void print_Summand(string prefix, Summand *summand, bool is_last) {
    if (!summand) return;
    PRINT_INFO("Summand");

    print_Primary(
            NEW_PREFIX,
            summand->primary,
            !summand->expression);
    print_Expression(
            NEW_PREFIX,
            summand->expression, 1);
}

void print_Primary(string prefix, Primary *primary, bool is_last) {
    if (!primary) return;
    PRINT_INFO(format("Primary (%s, %f, %s)",
                      primary->type.c_str(), primary->value, primary->isNot ? "is_not" : "not_is_not").c_str());

    print_Sign(
            NEW_PREFIX,
            primary->sign,
            !primary->modifiablePrimary);
    print_ModifiablePrimary(
            NEW_PREFIX,
            primary->modifiablePrimary, 1);
}

void print_Sign(string prefix, Sign *sign, bool is_last) {
    if (!sign) return;
    PRINT_INFO(format("Sign (%s)", sign->op.c_str()).c_str());
}

void print_ModifiablePrimary(string prefix, ModifiablePrimary *modifiableprimary, bool is_last) {
    if (!modifiableprimary) return;
    PRINT_INFO(format("ModifiablePrimary (%s)", modifiableprimary->name.c_str()).c_str());

    print_Identifiers(
            NEW_PREFIX,
            modifiableprimary->identifiers, 1);
}

void print_Identifiers(string prefix, Identifiers *identifiers, bool is_last) {
    if (!identifiers) return;
    PRINT_INFO(format("Identifiers (%s)", identifiers->name.c_str()).c_str());

    print_Expression(
            NEW_PREFIX,
            identifiers->expression,
            !identifiers->identifiers);
    print_Identifiers(
            NEW_PREFIX,
            identifiers->identifiers, 1);
}


#endif //CC_PROJECT_AST_H
