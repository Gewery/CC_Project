#include <iostream>
#include <map>
#include <vector>

#include "../SyntaxAnalysis/AST.h"
using namespace std;

struct Identifier {
    string identifier_type; // "Variable", "Function" or "Type"
    string value_type;
    bool read_only;
    map<string, Identifier*> subidentifiers;
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


void check_Program(Program *program, map<string, Identifier*> declared_identifiers);
map<string, Identifier*> check_Declaration(Declaration *declaration, map<string, Identifier*> declared_identifiers);
map<string, Identifier*> check_SimpleDeclaration(SimpleDeclaration *simpleDeclaration, map<string, Identifier*> declared_identifiers);
map<string, Identifier*> check_VariableDeclaration(VariableDeclaration *variabledeclaration, map<string, Identifier* > declared_identifiers, Identifier *parent = nullptr);
string check_InitialValue(InitialValue *initialvalue, map<string, Identifier* > declared_identifiers);
map<string, Identifier*> check_TypeDeclaration(TypeDeclaration *typedeclaration, map<string, Identifier*> declared_identifiers);
string check_Type(Type *type, map<string, Identifier*> declared_identifiers, Identifier* ident = nullptr, bool is_param=false);
string check_ArrayType(ArrayType *arraytype, map<string, Identifier*> declared_identifiers, bool is_param);
string check_RecordType(RecordType *recordtype, map<string, Identifier*> declared_identifiers, Identifier *ident);
string check_VariableDeclarations(VariableDeclarations *variabledeclarations, map<string, Identifier*> declared_identifiers, Identifier* ident);
map<string, Identifier* > check_RoutineDeclaration(RoutineDeclaration *routinedeclaration, map<string, Identifier* > declared_identifiers);
string check_ReturnInRoutine(ReturnInRoutine *returnInRoutine, map<string, Identifier* > declared_identifiers);
map<string, Identifier*> check_Parameters(Parameters *parameters, map<string, Identifier* > declared_identifiers);
map<string, Identifier* > check_ParameterDeclaration(ParameterDeclaration *parameterdeclaration, map<string, Identifier* > declared_identifiers);
map<string, Identifier* > check_ParametersDeclaration(ParametersDeclaration *parametersdeclaration, map<string, Identifier* > declared_identifiers);
string check_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration, map<string, Identifier*> declared_identifiers);
map<string, Identifier*> check_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration, map<string, Identifier*> declared_identifiers);
map<string, Identifier*> check_Body(Body *body, map<string, Identifier*> declared_identifiers);
void check_Statement(Statement *statement, map<string, Identifier* > declared_identifiers);
void check_Assignment(Assignment *assignment, map<string, Identifier*> declared_identifiers);
void check_RoutineCall(RoutineCall *routinecall, map<string, Identifier*> declared_identifiers);
void check_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall, map<string, Identifier*> declared_identifiers);
void check_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall, map<string, Identifier*> declared_identifiers);
void check_WhileLoop(WhileLoop *whileloop, map<string, Identifier*> declared_identifiers);
void check_ForLoop(ForLoop *forloop, map<string, Identifier*> declared_identifiers);
void check_Range(Range *range, map<string, Identifier*> declared_identifiers);
void check_IfStatement(IfStatement *ifstatement, map<string, Identifier*> declared_identifiers);
void check_ElseInIfStatement(ElseInIfStatement *elseinifstatement, map<string, Identifier*> declared_identifiers);
string check_Expression(Expression *expression, map<string, Identifier*> declared_identifiers);
void check_MultipleRelationsInExpression(MultipleRelationsInExpression *multiplerelationsinexpression, map<string, Identifier*> declared_identifiers);
string check_Relation(Relation *relation, map<string, Identifier*> declared_identifiers);
void check_ComparisonInRelation(ComparisonInRelation *comparisoninrelation, map<string, Identifier*> declared_identifiers);
string check_Simple(Simple *simple, map<string, Identifier*> declared_identifiers);
string check_Factors(Factors *factors, map<string, Identifier*> declared_identifiers);
string check_Factor(Factor *factor, map<string, Identifier*> declared_identifiers);
string check_Summands(Summands *summands, map<string, Identifier*> declared_identifiers);
string check_Summand(Summand *summand, map<string, Identifier*> declared_identifiers);
string check_Primary(Primary *primary, map<string, Identifier*> declared_identifiers);
string check_ModifiablePrimary(ModifiablePrimary *modifiableprimary, map<string, Identifier*> declared_identifiers);
string check_Identifiers(Identifiers *identifiers, Identifier* ident, map<string, Identifier*> declared_identifiers);