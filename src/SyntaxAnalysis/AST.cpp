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

string prefix;

void add_spaces_to_prefix(int a) {
    for (int i = 0; i < a; i++, prefix += ' ');
}

void rem_from_prefix(int a) {
    for (int i = 0; i < a; i++, prefix.pop_back());
}

void print_bars(bool isLast) {
    cout << " |" << "\n";
    if (isLast) cout << prefix + " └──";
    else cout << prefix +  " ├──";

    if (!isLast) prefix += " │";
    else prefix += "  ";
    return;
}

void print_margine(int number) {
    printf("\n");
    for (int i = 0; i < number; i++)
        cout << "--";
    return;
}


void print_Program(Program *program, bool isLast) {
    if (!program) return;
    check_Program(program);
    print_bars(isLast);
    cout << "Program";
    add_spaces_to_prefix(2 + 7);
    if (!program->program) print_Declaration(program->declaration, 1);
    else print_Declaration(program->declaration, 0);
    print_Program(program->program, 0);
    rem_from_prefix(6 + 7);
    cout << "\n" + prefix;
    return;
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
    return;
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
    return;
}

void print_VariableDeclaration(VariableDeclaration *variabledeclaration, bool isLast) {
    if (!variabledeclaration) return;;
    print_bars(isLast);

    cout << "VariableDeclaration";

    add_spaces_to_prefix(2 + 19);
//    check_VariableDeclaration(variabledeclaration);
    cout << " |\n" + prefix  << variabledeclaration->name << "\n" + prefix;
    if (!variabledeclaration->initialvalue && !variabledeclaration->expression)
        print_Type(variabledeclaration->type, 1);
    else print_Type(variabledeclaration->type, 0);
    if (!variabledeclaration->expression) print_InitialValue(variabledeclaration->initialvalue, 1);
    else print_InitialValue(variabledeclaration->initialvalue, 0);
    print_Expression(variabledeclaration->expression, 0);
    rem_from_prefix(6 + 19);
    cout << "\n" + prefix;
    return;
}

void print_InitialValue(InitialValue *initialvalue, bool isLast) {
    if (!initialvalue) return;
    print_bars(isLast);

    cout << "InitialValue";

    add_spaces_to_prefix(2 + 12);
    print_Expression(initialvalue->expression, 0);
    rem_from_prefix(6 + 12);
    cout << "\n" + prefix;
    return;
}

void print_TypeDeclaration(TypeDeclaration *typedeclaration, bool isLast) {
    if (!typedeclaration) return;
    // check_TypeDeclaration(typedeclaration);
    print_bars(isLast);

    cout << "TypeDeclaration";

    add_spaces_to_prefix(2 + 15);
    cout << " |\n" + prefix << typedeclaration->name << "\n" + prefix;

    print_Type(typedeclaration->type, 0);
    rem_from_prefix(6 + 15);
    cout << "\n" + prefix;
    return;
}

void print_Type(Type *type, bool isLast) {
    if (!type) return;
    print_bars(isLast);

    cout << "Type";

    add_spaces_to_prefix(2 + 4);
    if (!type->arraytype && !type->recordtype && (type->name).empty()) print_PrimitiveType(type->primitivetype, 1);
    else print_PrimitiveType(type->primitivetype, 0);
    if (!type->recordtype && (type->name).empty()) print_ArrayType(type->arraytype, 1);
    else print_ArrayType(type->arraytype, 0);
    if ((type->name).empty()) print_RecordType(type->recordtype, 1);
    else print_RecordType(type->recordtype, 0);
    cout << " |\n" + prefix << type->name << "\n" + prefix;

    rem_from_prefix(6 + 4);
    cout << "\n" + prefix;
    return;
}

void print_PrimitiveType(PrimitiveType *primitivetype, bool isLast) {
    if (!primitivetype) return;
    print_bars(isLast);

    cout << "PrimitiveType";

    add_spaces_to_prefix(2 + 13);
    cout << " |\n" + prefix << primitivetype->isint << "\n" + prefix;

    cout << " |\n" + prefix << primitivetype->isreal << "\n" + prefix;

    cout << " |\n" + prefix << primitivetype->isboolean << "\n" + prefix;

    rem_from_prefix(6 + 13);
    cout << "\n" + prefix;
    return;
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
    return;
}

void print_RecordType(RecordType *recordtype, bool isLast) {
    if (!recordtype) return;
    print_bars(isLast);

    cout << "RecordType";

    add_spaces_to_prefix(2 + 10);
    print_VariableDeclarations(recordtype->variabledeclarations, 0);
    rem_from_prefix(6 + 10);
    cout << "\n" + prefix;
    return;
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
    return;
}

void print_RoutineDeclaration(RoutineDeclaration *routinedeclaration, bool isLast) {
    if (!routinedeclaration) return;
    print_bars(isLast);

    cout << "RoutineDeclaration";

    add_spaces_to_prefix(2 + 18);
    cout << " |\n" + prefix << routinedeclaration->name << "\n" + prefix;

    if (!routinedeclaration->typeinroutinedeclaration && !routinedeclaration->bodyinroutinedeclaration)
        print_Parameters(routinedeclaration->parameters, 1);
    else print_Parameters(routinedeclaration->parameters, 0);
    if (!routinedeclaration->bodyinroutinedeclaration)
        print_TypeInRoutineDeclaration(routinedeclaration->typeinroutinedeclaration, 1);
    else print_TypeInRoutineDeclaration(routinedeclaration->typeinroutinedeclaration, 0);
    print_BodyInRoutineDeclaration(routinedeclaration->bodyinroutinedeclaration, 0);
    rem_from_prefix(6 + 18);
    cout << "\n" + prefix;
    return;
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
    return;
}

void print_ParameterDeclaration(ParameterDeclaration *parameterdeclaration, bool isLast) {
    if (!parameterdeclaration) return;
    print_bars(isLast);

    cout << "ParameterDeclaration";

    add_spaces_to_prefix(2 + 20);
    cout << " |\n" + prefix << parameterdeclaration->name << "\n" + prefix;

    print_Type(parameterdeclaration->type, 0);
    rem_from_prefix(6 + 20);
    cout << "\n" + prefix;
    return;
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
    return;
}

void print_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration, bool isLast) {
    if (!typeinroutinedeclaration) return;
    print_bars(isLast);

    cout << "TypeInRoutineDeclaration";

    add_spaces_to_prefix(2 + 24);
    print_Type(typeinroutinedeclaration->type, 0);
    rem_from_prefix(6 + 24);
    cout << "\n" + prefix;
    return;
}

void print_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration, bool isLast) {
    if (!bodyinroutinedeclaration) return;
    print_bars(isLast);

    cout << "BodyInRoutineDeclaration";

    add_spaces_to_prefix(2 + 24);
    print_Body(bodyinroutinedeclaration->body, 0);
    rem_from_prefix(6 + 24);
    cout << "\n" + prefix;
    return;
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
    return;
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
    return;
}

void print_Assignment(Assignment *assignment, bool isLast) {
    if (!assignment) return;
    print_bars(isLast);

    cout << "Assignment";
//    check_Assignment(assignment);
    add_spaces_to_prefix(2 + 10);
    if (!assignment->expression) print_ModifiablePrimary(assignment->modifiableprimary, 1);
    else print_ModifiablePrimary(assignment->modifiableprimary, 0);
    print_Expression(assignment->expression, 0);
    rem_from_prefix(6 + 10);
    cout << "\n" + prefix;
    return;
}

void print_RoutineCall(RoutineCall *routinecall, bool isLast) {
    if (!routinecall) return;
    print_bars(isLast);

    cout << "RoutineCall";

    add_spaces_to_prefix(2 + 11);
    cout << " |\n" + prefix << routinecall->name << "\n" + prefix;

    print_ExpressionInRoutineCall(routinecall->expressioninroutinecall, 0);
    rem_from_prefix(6 + 11);
    cout << "\n" + prefix;
    return;
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
    return;
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
    return;
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
    return;
}

void print_ForLoop(ForLoop *forloop, bool isLast) {
    if (!forloop) return;
    print_bars(isLast);

    cout << "ForLoop";

    add_spaces_to_prefix(2 + 7);
    cout << " |\n" + prefix << forloop->name << "\n" + prefix;

    if (!forloop->range && !forloop->body) print_Reverse(forloop->reverse, 1);
    else print_Reverse(forloop->reverse, 0);
    if (!forloop->body) print_Range(forloop->range, 1);
    else print_Range(forloop->range, 0);
    print_Body(forloop->body, 0);
    rem_from_prefix(6 + 7);
    cout << "\n" + prefix;
    return;
}

void print_Range(Range *range, bool isLast) {
    if (!range) return;
    print_bars(isLast);

    cout << "Range";

    add_spaces_to_prefix(2 + 5);
    if (!range->expression1) print_Expression(range->expression2, 1);
    else print_Expression(range->expression1, 0);
    print_Expression(range->expression1, 0);
    rem_from_prefix(6 + 5);
    cout << "\n" + prefix;
    return;
}

void print_Reverse(Reverse *reverse, bool isLast) {
    if (!reverse) return;
    print_bars(isLast);

    cout << "Reverse";

    add_spaces_to_prefix(2 + 7);
    cout << " |\n" + prefix << reverse->isreverse << "\n" + prefix;

    rem_from_prefix(6 + 7);
    cout << "\n" + prefix;
    return;
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
    return;
}

void print_ElseInIfStatement(ElseInIfStatement *elseinifstatement, bool isLast) {
    if (!elseinifstatement) return;
    print_bars(isLast);

    cout << "ElseInIfStatement";

    add_spaces_to_prefix(2 + 17);
    print_Body(elseinifstatement->body, 0);
    rem_from_prefix(6 + 17);
    cout << "\n" + prefix;
    return;
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
    return;
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
    return;
}

void print_LogicalOperator(LogicalOperator *logicaloperator, bool isLast) {
    if (!logicaloperator) return;
    print_bars(isLast);

    cout << "LogicalOperator";

    add_spaces_to_prefix(2 + 15);
    cout << " |\n" + prefix << logicaloperator->op << "\n" + prefix;

    rem_from_prefix(6 + 15);
    cout << "\n" + prefix;
    return;
}

void print_Relation(Relation *relation, bool isLast) {
    if (!relation) return;
    print_bars(isLast);

    cout << "Relation";

    add_spaces_to_prefix(2 + 8);
    if (!relation->comparisoninrelation) print_Simple(relation->simple, 1);
    else print_Simple(relation->simple, 0);
    print_ComparisonInRelation(relation->comparisoninrelation, 0);
    rem_from_prefix(6 + 8);
    cout << "\n" + prefix;
    return;
}

void print_ComparisonInRelation(ComparisonInRelation *comparisoninrelation, bool isLast) {
    if (!comparisoninrelation) return;
    print_bars(isLast);

    cout << "ComparisonInRelation";

    add_spaces_to_prefix(2 + 20);
    if (!comparisoninrelation->simple) print_ComparisonOperator(comparisoninrelation->comparisonoperator, 1);
    else print_ComparisonOperator(comparisoninrelation->comparisonoperator, 0);
    print_Simple(comparisoninrelation->simple, 0);
    rem_from_prefix(6 + 20);
    cout << "\n" + prefix;
    return;
}

void print_ComparisonOperator(ComparisonOperator *comparisonoperator, bool isLast) {
    if (!comparisonoperator) return;
    print_bars(isLast);

    cout << "ComparisonOperator";

    add_spaces_to_prefix(2 + 18);
    cout << " |\n" + prefix << comparisonoperator->op << "\n" + prefix;

    rem_from_prefix(6 + 18);
    cout << "\n" + prefix;
    return;
}

void print_Simple(Simple *simple, bool isLast) {
    if (!simple) return;
    print_bars(isLast);

    cout << "Simple";

    add_spaces_to_prefix(2 + 6);
    if (!simple->factors) print_Factor(simple->factor, 1);
    else print_Factor(simple->factor, 0);
    print_Factors(simple->factors, 0);
    rem_from_prefix(6 + 6);
    cout << "\n" + prefix;
    return;
}

void print_Factors(Factors *factors, bool isLast) {
    if (!factors) return;
    print_bars(isLast);

    cout << "Factors";

    add_spaces_to_prefix(2 + 7);
    if (!factors->factor && !factors->factors) print_SimpleOperator(factors->simpleOperator, 1);
    else print_SimpleOperator(factors->simpleOperator, 0);
    if (!factors->factors) print_Factor(factors->factor, 1);
    else print_Factor(factors->factor, 0);
    print_Factors(factors->factors, 0);
    rem_from_prefix(6 + 7);
    cout << "\n" + prefix;
    return;
}

void print_SimpleOperator(SimpleOperator *simpleoperator, bool isLast) {
    if (!simpleoperator) return;
    print_bars(isLast);

    cout << "SimpleOperator";

    add_spaces_to_prefix(2 + 14);
    cout << " |\n" + prefix << simpleoperator->op << "\n" + prefix;

    rem_from_prefix(6 + 14);
    cout << "\n" + prefix;
    return;
}

void print_Factor(Factor *factor, bool isLast) {
    if (!factor) return;
    print_bars(isLast);

    cout << "Factor";

    add_spaces_to_prefix(2 + 6);
    if (!factor->summands) print_Summand(factor->summand, 1);
    else print_Summand(factor->summand, 0);
    print_Summands(factor->summands, 0);
    rem_from_prefix(6 + 6);
    cout << "\n" + prefix;
    return;
}

void print_Summands(Summands *summands, bool isLast) {
    if (!summands) return;
    print_bars(isLast);

    cout << "Summands";

    add_spaces_to_prefix(2 + 8);
    if (!summands->summand && !summands->summands) print_Sign(summands->sign , 1);
    else print_Sign(summands->sign , 0);
    if (!summands->summands) print_Summand(summands->summand , 1);
    else print_Summand(summands->summand , 0);
    print_Summands(summands->summands , 0);
    rem_from_prefix(6 + 8);
    cout << "\n" + prefix;
    return;
}

void print_Summand(Summand *summand, bool isLast) {
    if (!summand) return;
    print_bars(isLast);

    cout << "Summand";

    add_spaces_to_prefix(2 + 7);
    if (!summand->expression) print_Primary(summand->primary, 1);
    else print_Primary(summand->primary, 0);
    print_Expression(summand->expression, 0);
    rem_from_prefix(6 + 7);
    cout << "\n" + prefix;
    return;
}

void print_Primary(Primary *primary, bool isLast) {
    if (!primary) return;
    print_bars(isLast);

    cout << "Primary";

    add_spaces_to_prefix(2 + 7);
    if (!primary->sign && !primary->modifiablePrimary) cout << " |\n" + prefix << primary->type <<
    "\n" + prefix;
    else  cout << " |\n" + prefix << primary->type << "\n" + prefix;

    if (!primary->sign && !primary->modifiablePrimary) cout << " |\n" + prefix << primary->value <<
    "\n" + prefix;
    else  cout << " |\n" + prefix  << primary->value << "\n" + prefix;

    if (!primary->sign && !primary->modifiablePrimary) cout << " |\n" + prefix << primary->isNot <<
    "\n" + prefix;
    else  cout << " |\n" + prefix << primary->isNot << "\n" + prefix;

    if (!primary->modifiablePrimary) print_Sign(primary->sign , 1);
    else print_Sign(primary->sign , 0);
    print_ModifiablePrimary(primary->modifiablePrimary , 0);
    rem_from_prefix(6 + 7);
    cout << "\n" + prefix;
    return;
}

void print_Sign(Sign *sign, bool isLast) {
    if (!sign) return;
    print_bars(isLast);

    cout << "Sign";

    add_spaces_to_prefix(2 + 4);
    cout << " |\n" + prefix << sign->op << "\n" + prefix;

    rem_from_prefix(6 + 4);
    cout << "\n" + prefix;
    return;
}

void print_ModifiablePrimary(ModifiablePrimary *modifiableprimary, bool isLast) {
    if (!modifiableprimary) return;
    print_bars(isLast);

    cout << "ModifiablePrimary";

    add_spaces_to_prefix(2 + 17);
    cout << " |\n" + prefix << modifiableprimary->name << "\n" + prefix;

    print_Identifiers(modifiableprimary->identifiers, 0);
    rem_from_prefix(6 + 17);
    cout << "\n" + prefix;
    return;
}

void print_Identifiers(Identifiers *identifiers, bool isLast) {
    if (!identifiers) return;
    print_bars(isLast);

    cout << "Identifiers";

    add_spaces_to_prefix(2 + 11);
    cout << " |\n" + prefix << identifiers->name << "\n" + prefix;

    if (!identifiers->identifiers) print_Expression(identifiers->expression, 1);
    else print_Expression(identifiers->expression, 0);
    print_Identifiers(identifiers->identifiers, 0);
    rem_from_prefix(6 + 11);
    cout << "\n" + prefix;
    return;
}


#endif //CC_PROJECT_AST_H
