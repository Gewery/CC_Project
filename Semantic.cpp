#include <string>
#include <vector>
#include <iostream>

#include <math.h>
#include <bits/stdc++.h>

#include "SymbolTable.h"

using namespace std;

unordered_map<string, Variable* > variables;
unordered_map<string, Function* > functions;

bool isEqual(string str1, string str2) {
    return str1.compare(str2) == 0;
}

void add_to_symbol_table(string name, auto result) {
    float value = result.value;
//    if (isEqual(result.sign, "-")) {
//        value =  -1 * value;
//    }
    Variable *v1 = new Variable(result.type, 0, value);
    vector<Variable*> vect;
    vect.push_back(v1);
    variables[name] = v1;
}

bool is_record_in_table(string name) {
    for (auto x : variables) {
        if (isEqual(x.first, name)) {
            return true;
        }
    }
    return false;
}

//TODO peredelat' na calculator!!!
auto check_primary(Primary *primary) {
    string type;
    float value;
    bool isNot;
    string sign;
    struct result_with_sign {string type;  float value; bool isNot; string sign;};
    if (!(primary->type).empty()) {
        if (primary->sign) {
            return result_with_sign {primary->type, primary->value, primary->isNot, primary->sign->op};
        }
        else {
            return result_with_sign {primary->type, primary->value, primary->isNot, ""};
        }
    }
    else {
    }
}
//TODO peredelat' na calculator!!!
auto check_summand(Summand *summand) {
    if (summand->expression) {

    }
    else if (summand->primary) {
        return check_primary(summand->primary);
    }
}
//TODO peredelat' na calculator!!!
auto check_factor(Factor *factor) {
    if (factor->summand) {
        return check_summand(factor->summand);
    }
    if (factor->summands) {

    }
}
//TODO peredelat' na calculator!!!
auto check_simple(Simple *simple) {
    if (simple->factor) {
        return check_factor(simple->factor);
    }
    if (simple->factors) {

    }
}

//TODO peredelat' na calculator!!!
auto check_relation(Relation *relation) {
    if (relation->simple) {
        return check_simple(relation->simple);
    }
    if (relation->comparisoninrelation) {

    }
}

//TODO peredelat' na calculator!!!
auto check_expression(Expression *expression) {
    if (expression->relation) {
        return check_relation(expression->relation);
    }
    if (expression->multiplerelationsinexpression) {

    }
}

string check_type(Type *type) {
    string user_type;
    if (type->arraytype) {

    }
    else if (type->primitivetype) {

    }
    else if (type->recordtype) {

    }
    else if (!(type->name).empty()) {
        user_type = type->name;
        transform(user_type.begin(), user_type.end(), user_type.begin(), ::tolower);
        return user_type;
    }
}

void check_VariableDeclaration(VariableDeclaration *variabledeclaration) {
    cout << "YA TUT!";
    // firstly, checking whether variable was already declared
    if (is_record_in_table(variabledeclaration->name)) {
        cout << "\n\nVariable " << variabledeclaration->name << " already declared!\n";
        exit(0);
    }

    string user_type;
    float user_value = INFINITY;

    //getting var type
    if (variabledeclaration->type) {
        user_type = check_type(variabledeclaration->type);
    }
    else {
        // type is setting by the value of expression
    }

    // getting initial value
    if (variabledeclaration->initialvalue->expression) {
        auto result = check_expression(variabledeclaration->initialvalue->expression);
        if (isEqual(result.type, user_type)) {
            add_to_symbol_table(variabledeclaration->name, result);
        }
        else if (isEqual(result.type, "integer") && isEqual(user_type, "real")) {
            add_to_symbol_table(variabledeclaration->name, result);
        }
        else if (isEqual(result.type, "integer") && isEqual(user_type,"boolean") && (result.value == 0 || result.value == 1)) {
            add_to_symbol_table(variabledeclaration->name, result);
        }
        else {
            cout << "\n\nType error!\n";
            exit(0);
        }
    }
        // case of initialization without initial value
    else {
        struct result_with_sign {string type;  float value; bool isNot; string sign;};
        // initial value for bool is false
        if (isEqual(user_type, "boolean")) {
            add_to_symbol_table(variabledeclaration->name, result_with_sign {user_type, 0, 0, ""});
        }
            //initial value for float and int is INF
        else {
            add_to_symbol_table(variabledeclaration->name, result_with_sign {user_type, INFINITY, 0, ""});
        }
    }
}

string check_modifiable_primary(ModifiablePrimary *modifiablePrimary) {
    if (!(modifiablePrimary->name).empty()) {
        return modifiablePrimary->name;
    }
    if (modifiablePrimary->identifiers) {
        //check identifiers
    }
}

void check_Assignment(Assignment *assignment) {
    string name;
    if (assignment->modifiableprimary) {
        name = check_modifiable_primary(assignment->modifiableprimary);
    }
    if (assignment->expression) {
        auto result = check_expression(assignment->expression);
        add_to_symbol_table(name, result);
    }

    // check if modifiable primary was already declared or not
    if (!is_record_in_table(name)){
        cout << "\n\nVariable " << name << " was not declared!\n";
        exit(0);
    }
}

void check_SimpleDeclaration(SimpleDeclaration *simpleDeclaration) {
    if (simpleDeclaration->variabledeclaration) {
        check_VariableDeclaration(simpleDeclaration->variabledeclaration);
    }
    if (simpleDeclaration->typedeclaration) {
//        check_TypeDeclaration(simpleDeclaration->typedeclaration)
    }
}

void check_Declaration(Declaration *declaration) {
    if (declaration->simpledeclaration) {
        check_SimpleDeclaration(declaration->simpledeclaration);
    }
    if (declaration->routinedeclaration){
//        check_RoutineDeclaration(declaration->routinedeclaration);
    }
}

void check_Program(Program *program) {
    if (program->declaration) {
        check_Declaration(program->declaration);
    }
    if (program->program) {
        check_Program(program->program);
    }
    for (auto x : variables)
        cout << "\n" << x.first << " " << x.second->type  << " " << x.second->value << endl;
}
