#include <iostream>
#include <map>
#include <vector>

#include "../SyntaxAnalysis/AST.h"
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
    vector<Variable*> arguments;
    Function(string return_type_of_func, float return_value_of_func, vector<Variable*> &arguments_of_func) {
        return_type = return_type_of_func;
        return_value = return_value_of_func;
        arguments = arguments_of_func;
    }
};


void check_Program(Program *program);
map<string, Variable* > check_Declaration(Declaration *declaration);
pair <map<string, Variable* >, map<string, Variable* >> check_SimpleDeclaration(SimpleDeclaration *simpledeclaration, map<string, Variable* > global_variables, map<string, Variable* > local_variables, bool scope);
pair <map<string, Variable* >, map<string, Variable* >> check_VariableDeclaration(VariableDeclaration *variabledeclaration, map<string, Variable* > global_variables, map<string, Variable* > local_variables, bool scope);
void check_InitialValue(InitialValue *initialvalue);
void check_TypeDeclaration(TypeDeclaration *typedeclaration, bool scope);
string check_Type(Type *type);
void check_PrimitiveType(PrimitiveType *primitivetype);
void check_ArrayType(ArrayType *arraytype);
void check_RecordType(RecordType *recordtype);
void check_VariableDeclarations(VariableDeclarations *variabledeclarations);
map<string, Variable* > check_RoutineDeclaration(RoutineDeclaration *routinedeclaration, map<string, Variable* > global_variables);
map<string, Variable* > check_ReturnInRoutine(ReturnInRoutine *returnInRoutine, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
map<string, Variable* > check_Parameters(Parameters *parameters, map<string, Variable* > local_variables);
map<string, Variable* > check_ParameterDeclaration(ParameterDeclaration *parameterdeclaration, map<string, Variable* > local_variables);
map<string, Variable* > check_ParametersDeclaration(ParametersDeclaration *parametersdeclaration, map<string, Variable* > local_variables);
string check_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration);
pair <map<string, Variable* >, map<string, Variable* >> check_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
pair <map<string, Variable* >, map<string, Variable* >> check_Body(Body *body,  map<string, Variable* > global_variables, map<string, Variable* > local_variables);
pair <map<string, Variable* >, map<string, Variable* >> check_Statement(Statement *statement, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
pair <map<string, Variable* >, map<string, Variable* >> check_Assignment(Assignment *assignment, map<string, Variable* > global_variable, map<string, Variable* > local_variables);
void check_RoutineCall(RoutineCall *routinecall);
void check_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall);
void check_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall);
pair <map<string, Variable* >, map<string, Variable* >> check_WhileLoop(WhileLoop *whileloop, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
pair <map<string, Variable* >, map<string, Variable* >> check_ForLoop(ForLoop *forloop, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
void check_Range(Range *range);
void check_Reverse(Reverse *reverse);
pair <map<string, Variable* >, map<string, Variable* >> check_IfStatement(IfStatement *ifstatement, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
pair <map<string, Variable* >, map<string, Variable* >> check_ElseInIfStatement(ElseInIfStatement *elseinifstatement, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
auto check_Expression(Expression *expression);
void check_MultipleRelationsInExpression(MultipleRelationsInExpression *multiplerelationsinexpression);
void check_LogicalOperator(LogicalOperator *logicaloperator);
auto check_Relation(Relation *relation);
void check_ComparisonInRelation(ComparisonInRelation *comparisoninrelation);
void check_ComparisonOperator(ComparisonOperator *comparisonoperator);
auto check_Simple(Simple *simple);
void check_Factors(Factors *factors);
void check_SimpleOperator(SimpleOperator *simpleoperator);
auto check_Factor(Factor *factor);
void check_Summands(Summands *summands);
auto check_Summand(Summand *summand);
auto check_Primary(Primary *primary);
void check_Sign(Sign *sign);
string check_ModifiablePrimary(ModifiablePrimary *modifiableprimary);
void check_Identifiers(Identifiers *identifiers);