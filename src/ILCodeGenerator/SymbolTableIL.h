#include <iostream>
#include <map>
#include <vector>

#include "../SyntaxAnalysis/AST.h"
using namespace std;


struct Value_Commands {
    string type;
    float value;
    vector<string> commands;
    Value_Commands(string type = "", float value = 0) : type(type), value(value) {}
};


bool run_IL_Code_Generator(Program *program);
vector<string> generate_Program(Program *program, map<string, Identifier*> declared_identifiers);
pair<map<string, Identifier*>, Value_Commands*> generate_Declaration(Declaration *declaration, map<string, Identifier*> declared_identifiers);
pair<map<string, Identifier*>, Value_Commands*> generate_SimpleDeclaration(SimpleDeclaration *simpleDeclaration, map<string, Identifier*> declared_identifiers, bool global_declaration);
pair<map<string, Identifier*>, Value_Commands*> generate_VariableDeclaration(VariableDeclaration *variabledeclaration, map<string, Identifier* > declared_identifiers, bool global_declaration=false, Identifier *parent = nullptr);
Value_Commands* generate_InitialValue(InitialValue *initialvalue, map<string, Identifier* > declared_identifiers);
map<string, Identifier*> generate_TypeDeclaration(TypeDeclaration *typedeclaration, map<string, Identifier*> declared_identifiers);
string generate_Type(Type *type, map<string, Identifier*> declared_identifiers, Identifier* ident = nullptr, bool is_param=false);
string generate_ArrayType(ArrayType *arraytype, map<string, Identifier*> declared_identifiers, bool is_param);
string generate_RecordType(RecordType *recordtype, map<string, Identifier*> declared_identifiers, Identifier *ident);
string generate_VariableDeclarations(VariableDeclarations *variabledeclarations, map<string, Identifier*> declared_identifiers, Identifier* ident);
map<string, Identifier* > generate_RoutineDeclaration(RoutineDeclaration *routinedeclaration, map<string, Identifier* > declared_identifiers);
string generate_ReturnInRoutine(ReturnInRoutine *returnInRoutine, map<string, Identifier* > declared_identifiers);
map<string, Identifier*> generate_Parameters(Parameters *parameters, map<string, Identifier* > declared_identifiers);
map<string, Identifier* > generate_ParameterDeclaration(ParameterDeclaration *parameterdeclaration, map<string, Identifier* > declared_identifiers);
map<string, Identifier* > generate_ParametersDeclaration(ParametersDeclaration *parametersdeclaration, map<string, Identifier* > declared_identifiers);
string generate_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration, map<string, Identifier*> declared_identifiers);
string generate_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration, map<string, Identifier*> declared_identifiers);
map<string, Identifier*> generate_Body(Body *body, map<string, Identifier*> declared_identifiers);
void generate_Statement(Statement *statement, map<string, Identifier* > declared_identifiers);
void generate_Assignment(Assignment *assignment, map<string, Identifier*> declared_identifiers);
void generate_RoutineCall(RoutineCall *routinecall, map<string, Identifier*> declared_identifiers);
void generate_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall, map<string, Identifier*> declared_identifiers);
void generate_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall, map<string, Identifier*> declared_identifiers);
void generate_WhileLoop(WhileLoop *whileloop, map<string, Identifier*> declared_identifiers);
void generate_ForLoop(ForLoop *forloop, map<string, Identifier*> declared_identifiers);
void generate_Range(Range *range, map<string, Identifier*> declared_identifiers);
void generate_IfStatement(IfStatement *ifstatement, map<string, Identifier*> declared_identifiers);
void generate_ElseInIfStatement(ElseInIfStatement *elseinifstatement, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_Expression(Expression *expression, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_MultipleRelationsInExpression(MultipleRelationsInExpression *multiplerelationsinexpression, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_Relation(Relation *relation, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_ComparisonInRelation(ComparisonInRelation *comparisoninrelation, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_Simple(Simple *simple, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_Factors(Factors *factors, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_Factor(Factor *factor, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_Summands(Summands *summands, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_Summand(Summand *summand, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_Primary(Primary *primary, map<string, Identifier*> declared_identifiers);
Value_Commands* generate_ModifiablePrimary(ModifiablePrimary *modifiableprimary, map<string, Identifier*> declared_identifiers, bool generate_read_only=false, string type_change_to="");
string generate_Identifiers(Identifiers *identifiers, Identifier* ident, map<string, Identifier*> declared_identifiers, string type_change_to = "");