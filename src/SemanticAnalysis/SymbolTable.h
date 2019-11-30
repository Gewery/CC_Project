#include <iostream>
#include <map>
#include <vector>

#include "../SyntaxAnalysis/AST.h"
using namespace std;


struct Identifier {
    string identifier_type; // "Variable", "Function" or "Type"
    string value_type;
    bool read_only;
    Identifier(string identifier_type, string value_type, bool read_only = false): identifier_type(identifier_type), value_type(value_type), read_only(read_only) {}
    // Variable *variable;
    // Function *funciton;
    // Type *type;
};

// struct Variable {
//     string type;
// };

// struct Function {
//     // string return_type;
//     // vector<Variable*> arguments;
//     // Function(string return_type_of_func, vector<Variable*> &arguments_of_func) {
//     //     return_type = return_type_of_func;
//     //     arguments = arguments_of_func;
//     // }
// };

// struct Type {

// };


void check_Program(Program *program);
map<string, Variable* > check_Declaration(Declaration *declaration);
pair <map<string, Variable* >, map<string, Variable* >> check_SimpleDeclaration(SimpleDeclaration *simpledeclaration, map<string, Variable* > global_variables, map<string, Variable* > local_variables, bool scope);
pair <map<string, Variable* >, map<string, Variable* >> check_VariableDeclaration(VariableDeclaration *variabledeclaration, map<string, Variable* > global_variables, map<string, Variable* > local_variables, bool scope);
void check_InitialValue(InitialValue *initialvalue);
void check_TypeDeclaration(TypeDeclaration *typedeclaration, bool scope);
string check_Type(Type *type, bool is_decl=false);
string check_ArrayType(ArrayType *arraytype, bool is_decl);
void check_RecordType(RecordType *recordtype);
void check_VariableDeclarations(VariableDeclarations *variabledeclarations);
map<string, Variable* > check_RoutineDeclaration(RoutineDeclaration *routinedeclaration, map<string, Variable* > global_variables);
string check_ReturnInRoutine(ReturnInRoutine *returnInRoutine);
map<string, Variable* > check_Parameters(Parameters *parameters, map<string, Variable* > local_variables);
map<string, Variable* > check_ParameterDeclaration(ParameterDeclaration *parameterdeclaration, map<string, Variable* > local_variables);
map<string, Variable* > check_ParametersDeclaration(ParametersDeclaration *parametersdeclaration, map<string, Variable* > local_variables);
string check_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration);
pair <map<string, Variable* >, map<string, Variable* >> check_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
pair <map<string, Variable* >, map<string, Variable* >> check_Body(Body *body,  map<string, Variable* > global_variables, map<string, Variable* > local_variables);
pair <map<string, Variable* >, map<string, Variable* >> check_Statement(Statement *statement, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
pair <map<string, Variable* >, map<string, Variable* >> check_Assignment(Assignment *assignment, map<string, Variable* > global_variable, map<string, Variable* > local_variables);
void check_RoutineCall(RoutineCall *routinecall, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
void check_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall);
void check_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall);
pair <map<string, Variable* >, map<string, Variable* >> check_WhileLoop(WhileLoop *whileloop, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
pair <map<string, Variable* >, map<string, Variable* >> check_ForLoop(ForLoop *forloop, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
void check_Range(Range *range);
void check_Reverse(Reverse *reverse);
pair <map<string, Variable* >, map<string, Variable* >> check_IfStatement(IfStatement *ifstatement, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
pair <map<string, Variable* >, map<string, Variable* >> check_ElseInIfStatement(ElseInIfStatement *elseinifstatement, map<string, Variable* > global_variables, map<string, Variable* > local_variables);
string check_Expression(Expression *expression);
string check_Relation(Relation *relation);
string check_Simple(Simple *simple);
string check_Factors(Factors *factors);
string check_Factor(Factor *factor);
string check_Summands(Summands *summands);
string check_Summand(Summand *summand);
string check_Primary(Primary *primary);
string check_ModifiablePrimary(ModifiablePrimary *modifiableprimary);
string check_Identifiers(Identifiers *identifiers, string path);