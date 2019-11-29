#include <string>
#include <vector>
#include <iostream>

#include <math.h>
#include <bits/stdc++.h>

#include "SymbolTable.h"

using namespace std;

map<string, string > types = {
    {"integer", "integer"},
    {"real", "real"},
    {"boolean", "boolean"}
};
map<string, Function* > functions;

bool isEqual(string str1, string str2) {
    return str1.compare(str2) == 0;
}

map<string, Variable* > add_to_symbol_table(string name, string type, map<string, Variable* > table) {
    Variable *v1 = new Variable(type, 0);
    table[name] = v1;
    return table;
}

bool is_record_in_table(string name, auto table) {
    for (auto x : table) {
        if (isEqual(x.first, name)) {
            return true;
        }
    }
    return false;
}

map<string, Variable* > update_table(map<string, Variable* > source, map<string, Variable* > target) {
    for (auto x : source) {
        if (is_record_in_table(x.first, target)) {
            target[x.first] = x.second;
        }
    }
    return target;
};

string returned_type(string a, string b) {
    if (a == "float" || b == "float") return "float";
    else if (a == "int" || b == "int") return "int";
    else if (a == "bool" || b == "bool") return "bool";
    else {
        cout << "Wrong type (" << a << ", " << b << ")\n";
        exit(1);
    }
}


void check_Identifiers(Identifiers *identifiers) {

}

string check_ModifiablePrimary(ModifiablePrimary *modifiableprimary) {
    if (!(modifiableprimary->name).empty()) {
        return modifiableprimary->name;
    }
    if (modifiableprimary->identifiers) {
        //check identifiers
    }
}

void check_Sign(Sign *sign) {

}

/**
 * Check if type is already in the map of custom types.
 * @param name  Type name
 * @return      Boolean value storing true if type exists, false - otherwise
 */
bool type_exists(string name) {
    return types.find(name) != types.end();
}

/**
 * Add a type to the custom map list.
 *
 * @param name  Name of the type
 * @param type  Actual type of a given type
 * @return      Nothing
 */
void add_type(string name, string type) {
    if (!type_exists(name))
        types[name] = type;
    // cout << "\n\nAdded type '" << name << "' of actual type '" << type << "'\n\n"; // LOG
}

//DONE
string check_Primary(Primary *primary) {
    return primary->type;
}

//DONE
string check_Summand(Summand *summand) {
    if (summand->expression) {
        return check_Expression(summand->expression);
    } else {
        return check_Primary(summand->primary);
    }
}

//DONE
string check_Summands(Summands *summands) {
    string summand_type = check_Summand(summands->summand);
    if (summands->summands) {
        return returned_type(summand_type, check_Summands(summands->summands));
    } else {
        return summand_type;
    }
}

//DONE
string check_Factor(Factor *factor) {
    string summand_type = check_Summand(factor->summand);
    if (factor->summands) {
        return returned_type(summand_type, check_Summands(factor->summands));
    } else {
        return summand_type;
    }
}

//DONE
string check_Factors(Factors *factors) {
    string factor_type = check_Factor(factors->factor);
    if (factors->factors) {
        return returned_type(factor_type, check_Factors(factors->factors));
    } else {
        return factor_type;
    }
}

//DONE
string check_Simple(Simple *simple) {
    string factor_type = check_Factor(simple->factor);
    if (simple->factors) {
        return returned_type(factor_type, check_Factors(simple->factors));
    } else {
        return factor_type;
    }
}

//DONE
string check_Relation(Relation *relation) {
    if (relation->comparisoninrelation) return "bool";
    else return check_Simple(relation->simple);
}

//DONE
string check_Expression(Expression *expression) {
    if (expression->multiplerelationsinexpression) return "bool";
    else return check_Relation(expression->relation);
}

pair <map<string, Variable* >, map<string, Variable* >> check_ElseInIfStatement(ElseInIfStatement *elseinifstatement, map<string, Variable* > global_variables, map<string, Variable* > local_variables) {
    map<string, Variable* > new_global_variables;
    map<string, Variable* > new_local_variables;

    new_global_variables.insert(global_variables.begin(), global_variables.end());
    new_global_variables.insert(local_variables.begin(), local_variables.end());

    if (elseinifstatement->body) {
        auto result = check_Body(elseinifstatement->body, new_global_variables, new_local_variables);
        new_global_variables = result.first;
        new_local_variables = result.second;
    }
    global_variables = update_table(new_global_variables, global_variables);
    local_variables = update_table(new_global_variables, local_variables);
    return make_pair(global_variables, local_variables);
}

pair <map<string, Variable* >, map<string, Variable* >>  check_IfStatement(IfStatement *ifstatement, map<string, Variable* > global_variables, map<string, Variable* > local_variables) {
    map<string, Variable* > new_global_variables;
    map<string, Variable* > new_local_variables;

    new_global_variables.insert(global_variables.begin(), global_variables.end());
    new_global_variables.insert(local_variables.begin(), local_variables.end());

    if (ifstatement->expression) {

    }
    if (ifstatement->body) {
        auto result = check_Body(ifstatement->body, new_global_variables, new_local_variables);
        new_global_variables = result.first;
        new_local_variables = result.second;
        global_variables = update_table(new_global_variables, global_variables);
        local_variables = update_table(new_global_variables, local_variables);
    }
    if (ifstatement->elseinifstatement) {
        auto result = check_ElseInIfStatement(ifstatement->elseinifstatement, global_variables, local_variables);
        global_variables = result.first;
        local_variables = result.second;
    }
    return make_pair(global_variables, local_variables);
}

void check_Reverse(Reverse *reverse) {

}

void check_Range(Range *range) {

}

pair <map<string, Variable* >, map<string, Variable* >> check_ForLoop(ForLoop *forloop, map<string, Variable* > global_variables, map<string, Variable* > local_variables) {
    map<string, Variable* > new_global_variables;
    map<string, Variable* > new_local_variables;

    new_global_variables.insert(global_variables.begin(), global_variables.end());
    new_global_variables.insert(local_variables.begin(), local_variables.end());

    struct result_with_sign {string type;  float value; bool isNot; string sign;};

    if (!(forloop->name).empty()) {
//        new_local_variables = add_to_symbol_table(forloop->name, result_with_sign {"integer", 0, 0, ""}, new_local_variables);
    }
    if (forloop->range) {

    }
    if (forloop->reverse) {

    }
    if (forloop->body) {
        auto result = check_Body(forloop->body, new_global_variables, new_local_variables);
        new_global_variables = result.first;
        new_local_variables = result.second;
    }
    global_variables = update_table(new_global_variables, global_variables);
    local_variables = update_table(new_global_variables, local_variables);
    return make_pair(global_variables, local_variables);
}

pair <map<string, Variable* >, map<string, Variable* >> check_WhileLoop(WhileLoop *whileloop, map<string, Variable* > global_variables, map<string, Variable* > local_variables) {
    map<string, Variable* > new_global_variables;
    map<string, Variable* > new_local_variables;

    new_global_variables.insert(global_variables.begin(), global_variables.end());
    new_global_variables.insert(local_variables.begin(), local_variables.end());
    if (whileloop->expression) {

    }
    if (whileloop->body) {
        auto result = check_Body(whileloop->body, new_global_variables, new_local_variables);
        new_global_variables = result.first;
        new_local_variables = result.second;
    }
    global_variables = update_table(new_global_variables, global_variables);
    local_variables = update_table(new_global_variables, local_variables);
    return make_pair(global_variables, local_variables);
}

void check_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall) {

}

void check_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall) {

}

void check_RoutineCall(RoutineCall *routinecall, map<string, Variable* > global_variables, map<string, Variable* > local_variables) {
    if (!(routinecall->name).empty()) {
        if (!is_record_in_table(routinecall->name, functions)) {
            cout << "\n\nFunction " << routinecall->name << " is not declared!\n";
            exit(0);
        }
    }
    if (routinecall->expressioninroutinecall) {

    }
}

pair <map<string, Variable* >, map<string, Variable* >>  check_Assignment(Assignment *assignment, map<string, Variable* > global_variables, map<string, Variable* > local_variables) {
    string name;
    if (assignment->modifiableprimary) {
        name = check_ModifiablePrimary(assignment->modifiableprimary);
    }
    bool is_local_variable= is_record_in_table(name, local_variables);

    if ((!is_record_in_table(name, global_variables) && (!is_record_in_table(name, local_variables)))){
        cout << "\n\nASSVariable " << name << " was not declared!\n";
        exit(0);
    }

    //getting the type of record
    string record_type;
    if (!is_local_variable) {
        for (auto x : global_variables) {
            if (isEqual(x.first, name)) {
                record_type = x.second->type;
            }
        }
    }
    else {
        for (auto x : local_variables) {
            if (isEqual(x.first, name)) {
                record_type = x.second->type;
            }
        }
    }

    if (assignment->expression) {
        string actual_type = check_Expression(assignment->expression);
//        if (isEqual(actual_type, record_type)) {
//            if (is_local_variable) {
//                local_variables =  add_to_symbol_table(name, actual_type, local_variables);
//            }
//            else {
//                global_variables =  add_to_symbol_table(name, actual_type, global_variables);
//            }
//        }
//        // int - real
//        else if (isEqual(record_type, "integer") && isEqual(actual_type, "real")) {
//            if (is_local_variable) {
//                local_variables =  add_to_symbol_table(name, record_type, local_variables);
//            }
//            else {
//                global_variables =  add_to_symbol_table(name, record_type, global_variables);
//            }
//        }
//        //int - boolean, real - int, real - bool
//        else if ((isEqual(record_type, "integer") && isEqual(actual_type, "boolean")) ||
//                (isEqual(record_type, "real") && isEqual(actual_type, "integer")) ||
//                (isEqual(record_type, "real") && isEqual(actual_type, "boolean"))) {
//            if (is_local_variable) {
//                local_variables =  add_to_symbol_table(name, record_type, local_variables);
//            }
//            else {
//                global_variables =  add_to_symbol_table(name, record_type, global_variables);
//            }
//        }

        // bool - int
//        else if (isEqual(record_type, "boolean") && isEqual(actual_type, "integer")) {
//            if (result.value == 0 || result.value == 1) {
//                is_local_variable ? add_to_symbol_table(name, record_type, local_variables)
//                                  : add_to_symbol_table(name, record_type, global_variables);
//            }
//            else {
//                cout << "\n\nVariable " << name << " can not be casted to boolean!\n";
//                exit(0);
//            }
//        }
        if (isEqual(record_type, "boolean") && isEqual(actual_type, "real")) {
            cout << "\n\nVariable " << name << " can not be casted to boolean!\n";
            exit(0);
        }
    }
    return make_pair(global_variables, local_variables);
}

pair <map<string, Variable* >, map<string, Variable* >>  check_Statement(Statement *statement, map<string, Variable* > global_variables, map<string, Variable* > local_variables) {
    if (statement->assignment) {
        auto result = check_Assignment(statement->assignment, global_variables, local_variables);
        global_variables = result.first;
        local_variables = result.second;
    }
    else if (statement->routinecall) {
        check_RoutineCall(statement->routinecall, global_variables, local_variables);
//        global_variables = result.first;
//        local_variables = result.second;

    }
    else if (statement->whileloop) {
        auto result = check_WhileLoop(statement->whileloop, global_variables, local_variables);
        global_variables = result.first;
        local_variables = result.second;
    }
    else if (statement->forloop) {
        auto result = check_ForLoop(statement->forloop, global_variables, local_variables);
        global_variables = result.first;
        local_variables = result.second;
    }
    else if (statement->ifstatement) {
        auto result = check_IfStatement(statement->ifstatement, global_variables, local_variables);
        global_variables = result.first;
        local_variables = result.second;
    }
    return make_pair(global_variables, local_variables);
}

//DONE
pair <map<string, Variable* >, map<string, Variable* >> check_Body(Body *body, map<string, Variable* > global_variables, map<string, Variable* > local_variables) {
    if (body->simpledeclaration) {
        auto result = check_SimpleDeclaration(body->simpledeclaration, global_variables, local_variables, true);
        global_variables = result.first;
        local_variables = result.second;
    }
    if (body->statement) {
        auto result = check_Statement(body->statement, global_variables, local_variables);
        global_variables = result.first;
        local_variables = result.second;
    }
    if (body->body) {
        auto result = check_Body(body->body, global_variables, local_variables);
        global_variables = result.first;
        local_variables = result.second;
    }
    return make_pair(global_variables, local_variables);
}

//DONE
pair <map<string, Variable* >, map<string, Variable* >> check_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration, map<string, Variable* > global_variables, map<string, Variable* > local_variables) {
    if (bodyinroutinedeclaration->body) {
        auto result = check_Body(bodyinroutinedeclaration->body, global_variables, local_variables);
        global_variables = result.first;
        local_variables = result.second;
    }
    return make_pair(global_variables, local_variables);
}

//DONE
string check_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration) {
    if (typeinroutinedeclaration->type) {
        return check_Type(typeinroutinedeclaration->type);
    }
}

//DONE
map<string, Variable* > check_ParametersDeclaration(ParametersDeclaration *parametersdeclaration, map<string, Variable* > local_variables) {
    if (parametersdeclaration->parameterdeclaration) {
        local_variables = check_ParameterDeclaration(parametersdeclaration->parameterdeclaration, local_variables);
    }
    if (parametersdeclaration->parametersdeclaration) {
        local_variables = check_ParametersDeclaration(parametersdeclaration->parametersdeclaration, local_variables);
    }
    return local_variables;
}

//DONE
map<string, Variable* > check_ParameterDeclaration(ParameterDeclaration *parameterdeclaration, map<string, Variable* > local_variables) {
    string name;
    string type;
    if (!(parameterdeclaration->name).empty()) {
        name = parameterdeclaration->name;
    }
    if (parameterdeclaration->type) {
        type = check_Type(parameterdeclaration->type);
    }
    Variable *v1 = new Variable(type, 1);
    local_variables[name] = v1;
    return local_variables;
}

//DONE
map<string, Variable* > check_Parameters(Parameters *parameters, map<string, Variable* > local_variables) {
    if (parameters->parameterdeclaration) {
        local_variables = check_ParameterDeclaration(parameters->parameterdeclaration, local_variables);
    }
    if (parameters->parametersdeclaration) {
        local_variables = check_ParametersDeclaration(parameters->parametersdeclaration, local_variables);
    }
    return local_variables;
}

map<string, Variable* > check_ReturnInRoutine(ReturnInRoutine *returnInRoutine, map<string, Variable* > global_variables, map<string, Variable* > local_variables) {
    if (returnInRoutine->expression) {

    }
};

map<string, Variable* > check_RoutineDeclaration(RoutineDeclaration *routinedeclaration, map<string, Variable* > global_variables) {
    map<string, Variable* > local_variables;
    string function_name;
    string type;
    vector<Variable*> parameters;

    if (!(routinedeclaration->name).empty()) {
        function_name = routinedeclaration->name;
    }
    if (routinedeclaration->parameters) {
        local_variables = check_Parameters(routinedeclaration->parameters, local_variables);
        for (std::pair<const std::__cxx11::basic_string<char>, Variable*> x : local_variables) {
            parameters.push_back(x.second);
            cout << "\n" << x.first << " " << x.second->type  << endl;
        }
    }
    if (routinedeclaration->typeinroutinedeclaration) {
        type = check_TypeInRoutineDeclaration(routinedeclaration->typeinroutinedeclaration);
    }
    if (routinedeclaration->bodyinroutinedeclaration) {
        auto result = check_BodyInRoutineDeclaration(routinedeclaration->bodyinroutinedeclaration, global_variables, local_variables);
        global_variables = result.first;
        local_variables = result.second;
    }

    Function *f1 = new Function(type, INFINITY, parameters);
    functions[function_name] = f1;

    cout << "LOCAL VARS\n";
    for (auto x : local_variables)
        cout << "\n" << x.first << " " << x.second->type << endl;
    cout << "====";

    return global_variables;
}

void check_VariableDeclarations(string type_name, VariableDeclarations *variabledeclarations, map<string, Variable* > global_variables, map<string, Variable* > local_variables, bool scope) {
    if (variabledeclarations->variabledeclaration) {
        variabledeclarations->variabledeclaration->name = type_name + variabledeclarations->variabledeclaration->name;
        check_VariableDeclaration(variabledeclarations->variabledeclaration, global_variables, local_variables, scope);
    }
    
    if (variabledeclarations->variabledeclarations)
        check_VariableDeclarations(type_name, variabledeclarations->variabledeclarations, global_variables, local_variables, scope);
}

void check_RecordType(string type_name, RecordType *recordtype, map<string, Variable* > global_variables, map<string, Variable* > local_variables, bool scope) {
    // add typename to variabledeclaration
    if (recordtype->variabledeclarations) {
        check_VariableDeclarations(type_name, recordtype->variabledeclarations, global_variables, local_variables, scope);
    }
}

void check_ArrayType(ArrayType *arraytype) {

}

void check_PrimitiveType(PrimitiveType *primitivetype) {

}

string check_Type(Type *type) {
    string user_type;
    if (type->arraytype) {

    }
    else if (type->primitivetype) {
        if (type->primitivetype->isint)
            return "integer";
        else if (type->primitivetype->isreal)
            return "real";
        else if (type->primitivetype->isboolean)
            return "boolean";
    }
    else if (type->recordtype) {
        //check_RecordType(type->name, )
    }
    else if (!(type->name).empty()) {
        user_type = type->name;
        transform(user_type.begin(), user_type.end(), user_type.begin(), ::tolower);
        return user_type;
    }
}

void check_TypeDeclaration(TypeDeclaration *typedeclaration, bool scope) {
    string name = typedeclaration->name,
           type = check_Type(typedeclaration->type);

    transform(name.begin(), name.end(), name.begin(), ::tolower);

    // check if type was already declared
    if (type_exists(name)) {
        cout << "\n######\nERROR! Type is primitive or already declared: '" << typedeclaration->name << "'\n######\n";
        exit(EXIT_FAILURE);
    }

    // check if actual type exists
    if (!type_exists(type)) {
        cout << "\n######\nERROR! Actual type for your custom type doesn't exist or is out of scope: '" << type << "'\n######\n";
        exit(EXIT_FAILURE);
    }

    add_type(name, type);
}

void check_InitialValue(InitialValue *initialvalue) {

}

pair <map<string, Variable* >, map<string, Variable* >> check_VariableDeclaration(VariableDeclaration *variabledeclaration, map<string, Variable* > global_variables, map<string, Variable* > local_variables,
                               bool scope) {
    // firstly, checking whether variable was already declared

    if ((!scope && (is_record_in_table(variabledeclaration->name, global_variables)) || (scope && is_record_in_table(variabledeclaration->name, local_variables)))) {
        cout << "\n\nVariable " << variabledeclaration->name << " already declared!\n";
        exit(0);
    }

    string user_type;
    float user_value = INFINITY;

    //getting var type
    if (variabledeclaration->type) {
        user_type = check_Type(variabledeclaration->type);
    }
    // type is setting by the value of expression
    else {
        // TODO
    }

    // getting initial value
    if (variabledeclaration->initialvalue) {
        if (variabledeclaration->initialvalue->expression) {
            string actual_type = check_Expression(variabledeclaration->initialvalue->expression);
            if ((isEqual(actual_type, user_type)) ||
                (isEqual(actual_type, "integer") && isEqual(user_type, "real")) ||
                ((isEqual(actual_type, "integer") && isEqual(user_type,"boolean")))) {
                if (!scope) {
                    global_variables = add_to_symbol_table(variabledeclaration->name, actual_type, global_variables);
                }
                else {
                    local_variables = add_to_symbol_table(variabledeclaration->name, actual_type, local_variables);
                }

            }
            else {
                cout << "\n\nType error!\n";
                exit(0);
            }
        }
    }
    // case of initialization without initial value
    else {
        // initial value for bool is false
        if (isEqual(user_type, "boolean")) {
            if (!scope) {
                global_variables = add_to_symbol_table(variabledeclaration->name, user_type, global_variables);
            }
            else {
                local_variables = add_to_symbol_table(variabledeclaration->name, user_type, local_variables);
            }
        }
        //initial value for float and int is INF
        else {
            if (!scope) {
                global_variables = add_to_symbol_table(variabledeclaration->name, user_type, global_variables);
            }
            else {
                local_variables = add_to_symbol_table(variabledeclaration->name, user_type, local_variables);
            }
        }
    }

    return make_pair(global_variables, local_variables);
}

pair <map<string, Variable* >, map<string, Variable* >> check_SimpleDeclaration(SimpleDeclaration *simpleDeclaration, map<string, Variable* > global_variables, map<string, Variable* > local_variables, bool scope) {
    if (simpleDeclaration->variabledeclaration) {
        auto result = check_VariableDeclaration(simpleDeclaration->variabledeclaration, global_variables, local_variables, scope);
        global_variables = result.first;
        local_variables = result.second;
    }
    if (simpleDeclaration->typedeclaration) {
        check_TypeDeclaration(simpleDeclaration->typedeclaration, scope);
    }

    return make_pair(global_variables, local_variables);
}

map<string, Variable* > check_Declaration(Declaration *declaration, map<string, Variable* > global_variables) {
    bool scope = false; //global
    if (declaration->simpledeclaration) {
        auto result = check_SimpleDeclaration(declaration->simpledeclaration, global_variables, {}, scope);
        global_variables = result.first;
    }
    if (declaration->routinedeclaration){
        global_variables = check_RoutineDeclaration(declaration->routinedeclaration, global_variables);
    }
    return global_variables;
}

map<string, Variable* > glob_variables;
void check_Program(Program *program) {
    if (program->declaration) {
        glob_variables = check_Declaration(program->declaration, glob_variables);
    }
    if (program->program) {
        check_Program(program->program);
    }

    for (auto x : glob_variables)
        cout << "\n" << x.first << " " << x.second->type << endl;

    for (auto x : functions) {
        cout << "\n" << x.first << " " << x.second->return_type  << " " << endl;
        for (auto param : x.second->arguments) {
            cout << param->type << endl;
        }
    }
}