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
/* ================================ */
/* ========== PRINT TREE ========== */
/* ================================ */

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
            routinedeclaration->bodyinroutinedeclaration,
            !routinedeclaration->returnInRoutine);
    print_ReturnInRoutine(
            NEW_PREFIX,
            routinedeclaration->returnInRoutine, 1);
}

void print_ReturnInRoutine(string prefix, ReturnInRoutine *returnInRoutine, bool is_last) {
    if (!returnInRoutine) return;
    PRINT_INFO("ReturnInRoutine");
    print_Expression(
            NEW_PREFIX,
            returnInRoutine->expression, 1);
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
            !expressionsinroutinecall->expressionsinroutinecall);
    print_ExpressionsInRoutineCall(
            NEW_PREFIX,
            expressionsinroutinecall->expressionsinroutinecall, 1);
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
    PRINT_INFO(format("Reverse (%s)", reverse->isreverse ? "is_inverse" : "not_inverse").c_str());
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




/* ======================================= */
/* ========== SERIALISE TO JSON ========== */
/* ======================================= */

string serialize_Tree(Program *program) {
    return json{{"Program", to_json_Program(program)}}.dump(2);
}

json to_json_Program(Program *program) {
    if (!program) return {};
    Program *cur = program;
    std::vector <json> declarations;
    while (cur) {
        declarations.push_back(to_json_Declaration(cur->declaration));
        cur = cur->program;
    }
    return json(declarations);
}

json to_json_Declaration(Declaration *declaration) {
    if (!declaration) return {};
    return {
            {"SimpleDeclaration",  to_json_SimpleDeclaration(declaration->simpledeclaration)},
            {"RoutineDeclaration", to_json_RoutineDeclaration(declaration->routinedeclaration)}
    };
}

json to_json_SimpleDeclaration(SimpleDeclaration *simpledeclaration) {
    if (!simpledeclaration) return {};
    return {
            {"VariableDeclaration", to_json_VariableDeclaration(simpledeclaration->variabledeclaration)},
            {"TypeDeclaration",     to_json_TypeDeclaration(simpledeclaration->typedeclaration)}
    };
}

json to_json_VariableDeclaration(VariableDeclaration *variabledeclaration) {
    if (!variabledeclaration) return {};
    return {
            {"name",         variabledeclaration->name},
            {"Type",         to_json_Type(variabledeclaration->type)},
            {"InitialValue", to_json_InitialValue(variabledeclaration->initialvalue)},
            {"Expression",   to_json_Expression(variabledeclaration->expression)}
    };
}

json to_json_InitialValue(InitialValue *initialvalue) {
    if (!initialvalue) return {};
    return {
            {"Expression", to_json_Expression(initialvalue->expression)}
    };
}

json to_json_TypeDeclaration(TypeDeclaration *typedeclaration) {
    if (!typedeclaration) return {};
    return {
            {"name", typedeclaration->name},
            {"Type", to_json_Type(typedeclaration->type)}
    };
}

json to_json_Type(Type *type) {
    if (!type) return {};
    return {
            {"name",          type->name},
            {"PrimitiveType", to_json_PrimitiveType(type->primitivetype)},
            {"ArrayType",     to_json_ArrayType(type->arraytype)},
            {"RecordType",    to_json_RecordType(type->recordtype)}
    };
}

json to_json_PrimitiveType(PrimitiveType *primitivetype) {
    if (!primitivetype) return {};
    return {
            {"is_int",  primitivetype->isint},
            {"is_real", primitivetype->isreal},
            {"is_bool", primitivetype->isboolean}
    };
}

json to_json_ArrayType(ArrayType *arraytype) {
    if (!arraytype) return {};
    return {
            {"Expression", to_json_Expression(arraytype->expression)},
            {"Type",       to_json_Type(arraytype->type)}
    };
}

json to_json_RecordType(RecordType *recordtype) {
    if (!recordtype) return {};
    return {
            {"VariableDeclarations", to_json_VariableDeclarations(recordtype->variabledeclarations)},
    };
}

json to_json_VariableDeclarations(VariableDeclarations *variabledeclarations) {
    if (!variabledeclarations) return {};
    return {
            {"VariableDeclaration",  to_json_VariableDeclaration(variabledeclarations->variabledeclaration)},
            {"VariableDeclarations", to_json_VariableDeclarations(variabledeclarations->variabledeclarations)}
    };
}

json to_json_RoutineDeclaration(RoutineDeclaration *routinedeclaration) {
    if (!routinedeclaration) return {};
    return {
            {"name",                     routinedeclaration->name},
            {"Parameters",               to_json_Parameters(routinedeclaration->parameters)},
            {"TypeInRoutineDeclaration", to_json_TypeInRoutineDeclaration(
                    routinedeclaration->typeinroutinedeclaration)},
            {"BodyInRoutineDeclaration", to_json_BodyInRoutineDeclaration(
                    routinedeclaration->bodyinroutinedeclaration)},
            {"ReturnInRoutine",          to_json_ReturnInRoutine(routinedeclaration->returnInRoutine)}
    };
}

json to_json_ReturnInRoutine(ReturnInRoutine *returnInRoutine) {
    if (!returnInRoutine) return {};
    return {
            {"Expression", to_json_Expression(returnInRoutine->expression)}
    };
}

json to_json_Parameters(Parameters *parameters) {
    if (!parameters) return {};
    return {
            {"ParameterDeclaration",  to_json_ParameterDeclaration(parameters->parameterdeclaration)},
            {"ParametersDeclaration", to_json_ParametersDeclaration(parameters->parametersdeclaration)}
    };
}

json to_json_ParameterDeclaration(ParameterDeclaration *parameterdeclaration) {
    if (!parameterdeclaration) return {};
    return {
            {"name", parameterdeclaration->name},
            {"Type", to_json_Type(parameterdeclaration->type)}
    };
}

json to_json_ParametersDeclaration(ParametersDeclaration *parametersdeclaration) {
    if (!parametersdeclaration) return {};
    return {
            {"ParameterDeclaration",  to_json_ParameterDeclaration(parametersdeclaration->parameterdeclaration)},
            {"ParametersDeclaration", to_json_ParametersDeclaration(parametersdeclaration->parametersdeclaration)}
    };
}

json to_json_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration) {
    if (!typeinroutinedeclaration) return {};
    return {
            {"Type", to_json_Type(typeinroutinedeclaration->type)}
    };
}

json to_json_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration) {
    if (!bodyinroutinedeclaration) return {};
    return {
            {"Body", to_json_Body(bodyinroutinedeclaration->body)}
    };
}

json to_json_Body(Body *body) {
    if (!body) return {};
    return {
            {"SimpleDeclaration", to_json_SimpleDeclaration(body->simpledeclaration)},
            {"Statement",         to_json_Statement(body->statement)},
            {"Body",              to_json_Body(body->body)}
    };
}

json to_json_Statement(Statement *statement) {
    if (!statement) return {};
    return {
            {"Assignment",  to_json_Assignment(statement->assignment)},
            {"RoutineCall", to_json_RoutineCall(statement->routinecall)},
            {"WhileLoop",   to_json_WhileLoop(statement->whileloop)},
            {"ForLoop",     to_json_ForLoop(statement->forloop)},
            {"IfStatement", to_json_IfStatement(statement->ifstatement)}
    };
}

json to_json_Assignment(Assignment *assignment) {
    if (!assignment) return {};
    return {
            {"ModifiablePrimary", to_json_ModifiablePrimary(assignment->modifiableprimary)},
            {"Expression",        to_json_Expression(assignment->expression)}
    };
}

json to_json_RoutineCall(RoutineCall *routinecall) {
    if (!routinecall) return {};
    return {
            {"name",                    routinecall->name},
            {"ExpressionInRoutineCall", to_json_ExpressionInRoutineCall(routinecall->expressioninroutinecall)}
    };
}

json to_json_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall) {
    if (!expressioninroutinecall) return {};
    return {
            {"Expression",               to_json_Expression(expressioninroutinecall->expression)},
            {"ExpressionsInRoutineCall", to_json_ExpressionsInRoutineCall(
                    expressioninroutinecall->expressionsinroutinecall)}
    };
}

json to_json_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall) {
    if (!expressionsinroutinecall) return {};
    return {
            {"Expression",               to_json_Expression(expressionsinroutinecall->expression)},
            {"ExpressionsInRoutineCall", to_json_ExpressionsInRoutineCall(
                    expressionsinroutinecall->expressionsinroutinecall)
            }
    };
}

json to_json_WhileLoop(WhileLoop *whileloop) {
    if (!whileloop) return {};
    return {
            {"Expression", to_json_Expression(whileloop->expression)},
            {"Body",       to_json_Body(whileloop->body)}
    };
}

json to_json_ForLoop(ForLoop *forloop) {
    if (!forloop) return {};
    return {
            {"name",    forloop->name},
            {"Reverse", to_json_Reverse(forloop->reverse)},
            {"Range",   to_json_Range(forloop->range)},
            {"Body",    to_json_Body(forloop->body)}
    };
}

json to_json_Range(Range *range) {
    if (!range) return {};
    return {
            {"Expression1", to_json_Expression(range->expression1)},
            {"Expression2", to_json_Expression(range->expression2)}
    };
}

json to_json_Reverse(Reverse *reverse) {
    if (!reverse) return {};
    return {
            {"is_reverse", reverse->isreverse}
    };
}

json to_json_IfStatement(IfStatement *ifstatement) {
    if (!ifstatement) return {};
    return {
            {"Expression",        to_json_Expression(ifstatement->expression)},
            {"Body",              to_json_Body(ifstatement->body)},
            {"ElseInIfStatement", to_json_ElseInIfStatement(ifstatement->elseinifstatement)}
    };
}

json to_json_ElseInIfStatement(ElseInIfStatement *elseinifstatement) {
    if (!elseinifstatement) return {};
    return {
            {"Body", to_json_Body(elseinifstatement->body)}
    };
}

json to_json_Expression(Expression *expression) {
    if (!expression) return {};
    return {
            {"Relation",                      to_json_Relation(expression->relation)},
            {"MultipleRelationsInExpression", to_json_MultipleRelationsInExpression(
                    expression->multiplerelationsinexpression)
            }
    };
}

json to_json_MultipleRelationsInExpression(MultipleRelationsInExpression *multiplerelationsinexpression) {
    if (!multiplerelationsinexpression) return {};
    return {
            {"LogicalOperator",               to_json_LogicalOperator(multiplerelationsinexpression->logicaloperator)},
            {"Relation",                      to_json_Relation(multiplerelationsinexpression->relation)},
            {"MultipleRelationsInExpression", to_json_MultipleRelationsInExpression(
                    multiplerelationsinexpression->multiplerelationsinexpression)}
    };
}

json to_json_LogicalOperator(LogicalOperator *logicaloperator) {
    if (!logicaloperator) return {};
    return {
            {"op", logicaloperator->op}
    };
}

json to_json_Relation(Relation *relation) {
    if (!relation) return {};
    return {
            {"Simple",               to_json_Simple(relation->simple)},
            {"ComparisonInRelation", to_json_ComparisonInRelation(relation->comparisoninrelation)}
    };
}

json to_json_ComparisonInRelation(ComparisonInRelation *comparisoninrelation) {
    if (!comparisoninrelation) return {};
    return {
            {"ComparisonOperator", to_json_ComparisonOperator(comparisoninrelation->comparisonoperator)},
            {"Simple",             to_json_Simple(comparisoninrelation->simple)}
    };
}

json to_json_ComparisonOperator(ComparisonOperator *comparisonoperator) {
    if (!comparisonoperator) return {};
    return {
            {"op", comparisonoperator->op},
    };
}

json to_json_Simple(Simple *simple) {
    if (!simple) return {};
    return {
            {"Factor",  to_json_Factor(simple->factor)},
            {"Factors", to_json_Factors(simple->factors)}
    };
}

json to_json_Factors(Factors *factors) {
    if (!factors) return {};
    return {
            {"SimpleOperator", to_json_SimpleOperator(factors->simpleOperator)},
            {"Factor",         to_json_Factor(factors->factor)},
            {"Factors",        to_json_Factors(factors->factors)}
    };
}

json to_json_SimpleOperator(SimpleOperator *simpleoperator) {
    if (!simpleoperator) return {};
    return {
            {"op", simpleoperator->op}
    };
}

json to_json_Factor(Factor *factor) {
    if (!factor) return {};
    return {
            {"Summand",  to_json_Summand(factor->summand)},
            {"Summands", to_json_Summands(factor->summands)}
    };
}

json to_json_Summands(Summands *summands) {
    if (!summands) return {};
    return {
            {"Sign",     to_json_Sign(summands->sign)},
            {"Summand",  to_json_Summand(summands->summand)},
            {"Summands", to_json_Summands(summands->summands)}
    };
}

json to_json_Summand(Summand *summand) {
    if (!summand) return {};
    return {
            {"Primary",    to_json_Primary(summand->primary)},
            {"Expression", to_json_Expression(summand->expression)}
    };
}

json to_json_Primary(Primary *primary) {
    if (!primary) return {};
    return {
            {"type",              primary->type},
            {"value",             primary->value},
            {"is_not",            primary->isNot},
            {"ModifiablePrimary", to_json_ModifiablePrimary(primary->modifiablePrimary)}
    };
}

json to_json_Sign(Sign *sign) {
    if (!sign) return {};
    return {
            {"op", sign->op}
    };
}

json to_json_ModifiablePrimary(ModifiablePrimary *modifiableprimary) {
    if (!modifiableprimary) return {};
    return {
            {"name",        modifiableprimary->name},
            {"Identifiers", to_json_Identifiers(modifiableprimary->identifiers)}
    };
}

json to_json_Identifiers(Identifiers *identifiers) {
    if (!identifiers) return {};
    return {
            {"name",        identifiers->name},
            {"Expression",  to_json_Expression(identifiers->expression)},
            {"Identifiers", to_json_Identifiers(identifiers->identifiers)}
    };
}


#endif //CC_PROJECT_AST_H
