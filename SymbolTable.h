#include <iostream>
#include <unordered_map>
#include <vector>

#include "AST.h"
using namespace std;


struct Variable {
    string type;
    bool scope; // 0 - global, 1 - local
    float value;
    Variable(string type_of_var, bool scope_of_var, float value_of_var) {
        type = type_of_var;
        scope = scope_of_var;
        value = value_of_var;
    }
};

struct Function {
    string return_type;
    float return_value;
    int number_of_args;
    vector<Variable*> arguments;
    Function(string return_type_of_func, float return_value_of_func, int number_of_args_of_func, vector<Variable*> &arguments_of_func) {
        return_type = return_type_of_func;
        return_value = return_value_of_func;
        number_of_args = number_of_args_of_func;
        arguments = arguments_of_func;
    }
};

void check_variabledeclaration(VariableDeclaration *variabledeclaration);
auto check_expression(Expression *expression);
auto check_relation(Relation *relation);
auto check_simple(Simple *simple);
auto check_factor(Factor *factor);
auto check_summand(Summand *summand);
string check_type(Type *type);
auto check_primary(Primary *primary);
string check_modifiable_primary(ModifiablePrimary *modifiablePrimary);
void check_assignment(Assignment *assignment);

void check_Program(Program *program);
void check_Declaration(Declaration *declaration);
void check_SimpleDeclaration(SimpleDeclaration *simpledeclaration);
void check_VariableDeclaration(VariableDeclaration *variabledeclaration);
void check_InitialValue(InitialValue *initialvalue);
void check_TypeDeclaration(TypeDeclaration *typedeclaration);
void check_Type(Type *type);
void check_PrimitiveType(PrimitiveType *primitivetype);
void check_ArrayType(ArrayType *arraytype);
void check_RecordType(RecordType *recordtype);
void check_VariableDeclarations(VariableDeclarations *variabledeclarations);
void check_RoutineDeclaration(RoutineDeclaration *routinedeclaration);
void check_Parameters(Parameters *parameters);
void check_ParameterDeclaration(ParameterDeclaration *parameterdeclaration);
void check_ParametersDeclaration(ParametersDeclaration *parametersdeclaration);
void check_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration);
void check_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration);
void check_Body(Body *body);
void check_Statement(Statement *statement);
void check_Assignment(Assignment *assignment);
void check_RoutineCall(RoutineCall *routinecall);
void check_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall);
void check_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall);
void check_WhileLoop(WhileLoop *whileloop);
void check_ForLoop(ForLoop *forloop);
void check_Range(Range *range);
void check_Reverse(Reverse *reverse);
void check_IfStatement(IfStatement *ifstatement);
void check_ElseInIfStatement(ElseInIfStatement *elseinifstatement);
void check_Expression(Expression *expression);
void check_MultipleRelationsInExpression(MultipleRelationsInExpression *multiplerelationsinexpression);
void check_LogicalOperator(LogicalOperator *logicaloperator);
void check_Relation(Relation *relation);
void check_ComparisonInRelation(ComparisonInRelation *comparisoninrelation);
void check_ComparisonOperator(ComparisonOperator *comparisonoperator);
void check_Simple(Simple *simple);
void check_Factors(Factors *factors);
void check_SimpleOperator(SimpleOperator *simpleoperator);
void check_Factor(Factor *factor);
void check_Summands(Summands *summands);
void check_Summand(Summand *summand);
void check_Primary(Primary *primary);
void check_Sign(Sign *sign);
void check_ModifiablePrimary(ModifiablePrimary *modifiableprimary);
void check_Identifiers(Identifiers *identifiers);