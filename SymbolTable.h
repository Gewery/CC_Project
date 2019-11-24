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