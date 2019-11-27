#include <string>
#include <vector>
#include <iostream>

#include <math.h>
#include <bits/stdc++.h>

#include "SymbolTable.h"

using namespace std;

unordered_map<string, Variable* > global_variables;
unordered_map<string, Function* > functions;

bool isEqual(string str1, string str2) {
    return str1.compare(str2) == 0;
}

unordered_map<string, Variable* > add_to_symbol_table(string name, auto result, unordered_map<string, Variable* > table) {
    Variable *v1 = new Variable(result.type, 0, result.value);
    table[name] = v1;
    return table;
}

bool is_record_in_table(string name, unordered_map<string, Variable* > table) {
    for (auto x : table) {
        if (isEqual(x.first, name)) {
            return true;
        }
    }
    return false;
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

//TODO peredelat' na calculator!!!
auto check_Primary(Primary *primary) {
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
auto check_Summand(Summand *summand) {
    if (summand->expression) {

    }
    else if (summand->primary) {
        return check_Primary(summand->primary);
    }
}

void check_Summands(Summands *summands) {

}

//TODO peredelat' na calculator!!!
auto check_Factor(Factor *factor) {
    if (factor->summand) {
        return check_Summand(factor->summand);
    }
    if (factor->summands) {

    }
}

void check_SimpleOperator(SimpleOperator *simpleoperator) {

}

void check_Factors(Factors *factors) {

}

//TODO peredelat' na calculator!!!
auto check_Simple(Simple *simple) {
    if (simple->factor) {
        return check_Factor(simple->factor);
    }
    if (simple->factors) {

    }
}

void check_ComparisonOperator(ComparisonOperator *comparisonoperator) {

}

void check_ComparisonInRelation(ComparisonInRelation *comparisoninrelation) {

}

//TODO peredelat' na calculator!!!
auto check_Relation(Relation *relation) {
    if (relation->simple) {
        return check_Simple(relation->simple);
    }
    if (relation->comparisoninrelation) {

    }
}

void check_LogicalOperator(LogicalOperator *logicaloperator) {

}

void check_MultipleRelationsInExpression(MultipleRelationsInExpression *multiplerelationsinexpression) {

}

//TODO peredelat' na calculator!!!
auto check_Expression(Expression *expression) {
    if (expression->relation) {
        return check_Relation(expression->relation);
    }
    if (expression->multiplerelationsinexpression) {

    }
}

void check_ElseInIfStatement(ElseInIfStatement *elseinifstatement) {

}

void check_IfStatement(IfStatement *ifstatement) {
    if (ifstatement->expression) {

    }
    if (ifstatement->body) {

    }
    if (ifstatement->elseinifstatement) {
        check_ElseInIfStatement(ifstatement->elseinifstatement);
    }
}

void check_Reverse(Reverse *reverse) {

}

void check_Range(Range *range) {

}

void check_ForLoop(ForLoop *forloop) {

}

void check_WhileLoop(WhileLoop *whileloop) {

}

void check_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall) {

}

void check_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall) {

}

void check_RoutineCall(RoutineCall *routinecall) {

}

void check_Assignment(Assignment *assignment, unordered_map<string, Variable* > local_variables) {
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
        auto result = check_Expression(assignment->expression);
        if (isEqual(result.type, record_type)) {
            if (is_local_variable) {
                local_variables =  add_to_symbol_table(name, result, local_variables);
            }
            else {
                global_variables =  add_to_symbol_table(name, result, global_variables);
            }
        }
        // int - real
        else if (isEqual(record_type, "integer") && isEqual(result.type, "real")) {
            result.value = round(result.value);
            result.type = record_type;
            if (is_local_variable) {
                local_variables =  add_to_symbol_table(name, result, local_variables);
            }
            else {
                global_variables =  add_to_symbol_table(name, result, global_variables);
            }
        }
        //int - boolean, real - int, real - bool
        else if ((isEqual(record_type, "integer") && isEqual(result.type, "boolean")) ||
                (isEqual(record_type, "real") && isEqual(result.type, "integer")) ||
                (isEqual(record_type, "real") && isEqual(result.type, "boolean"))) {
            result.type = record_type;
            if (is_local_variable) {
                local_variables =  add_to_symbol_table(name, result, local_variables);
            }
            else {
                global_variables =  add_to_symbol_table(name, result, global_variables);
            }
        }

        // bool - int
        else if (isEqual(record_type, "boolean") && isEqual(result.type, "integer")) {
            if (result.value == 0 || result.value == 1) {
                result.type = record_type;
                is_local_variable ? add_to_symbol_table(name, result, local_variables)
                                  : add_to_symbol_table(name, result, global_variables);
            }
            else {
                cout << "\n\nVariable " << name << " can not be casted to boolean!\n";
                exit(0);
            }
        }
        else if (isEqual(record_type, "boolean") && isEqual(result.type, "real")) {
            cout << "\n\nVariable " << name << " can not be casted to boolean!\n";
            exit(0);
        }
    }
}

void check_Statement(Statement *statement, unordered_map<string, Variable* > local_variables) {
    if (statement->assignment) {
        check_Assignment(statement->assignment, local_variables);
    }
    else if (statement->routinecall) {

    }
    else if (statement->whileloop) {

    }
    else if (statement->forloop) {

    }
    else if (statement->ifstatement) {
        check_IfStatement(statement->ifstatement);
    }
}

void check_Body(Body *body, unordered_map<string, Variable* > local_variables) {
    if (body->simpledeclaration) {
        check_SimpleDeclaration(body->simpledeclaration, local_variables, true);

    }
    if (body->statement) {
        check_Statement(body->statement, local_variables);
    }
    if (body->body) {
        check_Body(body->body, local_variables);
    }
}

void check_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration, unordered_map<string, Variable* > local_variables) {
    if (bodyinroutinedeclaration->body) {
        check_Body(bodyinroutinedeclaration->body, local_variables);
    }
}

string check_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration) {
    if (typeinroutinedeclaration->type) {
        return check_Type(typeinroutinedeclaration->type);
    }
}

unordered_map<string, Variable* > check_ParametersDeclaration(ParametersDeclaration *parametersdeclaration, unordered_map<string, Variable* > local_variables) {
    if (parametersdeclaration->parameterdeclaration) {
        unordered_map<string, Variable* > temp = check_ParameterDeclaration(parametersdeclaration->parameterdeclaration, local_variables);
        local_variables.insert(temp.begin(), temp.end());
    }
    if (parametersdeclaration->parametersdeclaration) {
        unordered_map<string, Variable* > temp = check_ParametersDeclaration(parametersdeclaration->parametersdeclaration, local_variables);
        local_variables.insert(temp.begin(), temp.end());
    }
    return local_variables;
}

unordered_map<string, Variable* > check_ParameterDeclaration(ParameterDeclaration *parameterdeclaration, unordered_map<string, Variable* > local_variables) {
    string name;
    string type;
    if (!(parameterdeclaration->name).empty()) {
        name = parameterdeclaration->name;
    }
    if (parameterdeclaration->type) {
        type = check_Type(parameterdeclaration->type);
    }
    Variable *v1 = new Variable(type, 1, INFINITY);
    vector<Variable*> vect;
    vect.push_back(v1);
    local_variables[name] = v1;
    return local_variables;
}

unordered_map<string, Variable* > check_Parameters(Parameters *parameters, unordered_map<string, Variable* > local_variables) {
    if (parameters->parameterdeclaration) {
        local_variables = check_ParameterDeclaration(parameters->parameterdeclaration, local_variables);
    }
    if (parameters->parametersdeclaration) {
        unordered_map<string, Variable* > temp = check_ParametersDeclaration(parameters->parametersdeclaration, local_variables);
        local_variables.insert(temp.begin(), temp.end());
    }
    return local_variables;
}

void check_RoutineDeclaration(RoutineDeclaration *routinedeclaration) {
    unordered_map<string, Variable* > local_variables;
    string function_name;
    string type;
//    vector<Variable*> parameters;
    if (!(routinedeclaration->name).empty()) {
        function_name = routinedeclaration->name;
    }
    if (routinedeclaration->parameters) {
        local_variables = check_Parameters(routinedeclaration->parameters, local_variables);
//        for (auto x : local_variables)
//            parameters.push_back(x);
//            cout << "\n" << x.first << " " << x.second->type  << " " << x.second->value << endl;

    }
    if (routinedeclaration->typeinroutinedeclaration) {
        type = check_TypeInRoutineDeclaration(routinedeclaration->typeinroutinedeclaration);
    }
    if (routinedeclaration->bodyinroutinedeclaration) {
        check_BodyInRoutineDeclaration(routinedeclaration->bodyinroutinedeclaration, local_variables);
    }
//    Function *f1 = new Function(type, INFINITY, parameters);
//    functions[function_name] = f1;
}

void check_VariableDeclarations(VariableDeclarations *variabledeclarations) {

}

void check_RecordType(RecordType *recordtype) {

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

    }
    else if (type->recordtype) {

    }
    else if (!(type->name).empty()) {
        user_type = type->name;
        transform(user_type.begin(), user_type.end(), user_type.begin(), ::tolower);
        return user_type;
    }
}

string check_TypeDeclaration(TypeDeclaration *typedeclaration) {

}

void check_InitialValue(InitialValue *initialvalue) {

}

void check_VariableDeclaration(VariableDeclaration *variabledeclaration, unordered_map<string, Variable* > local_variables,
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
    if (variabledeclaration->initialvalue->expression) {
        auto result = check_Expression(variabledeclaration->initialvalue->expression);
        if ((isEqual(result.type, user_type)) ||
            (isEqual(result.type, "integer") && isEqual(user_type, "real")) ||
            ((isEqual(result.type, "integer") && isEqual(user_type,"boolean") && (result.value == 0 || result.value == 1)))) {
            if (!scope) {
                global_variables = add_to_symbol_table(variabledeclaration->name, result, global_variables);
            }
            else {
                local_variables = add_to_symbol_table(variabledeclaration->name, result, local_variables);
            }

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
            if (!scope) {
                global_variables = add_to_symbol_table(variabledeclaration->name, result_with_sign {user_type, 0, 0, ""}, global_variables);
            }
            else {
                local_variables = add_to_symbol_table(variabledeclaration->name, result_with_sign {user_type, 0, 0, ""}, local_variables);
            }
        }
        //initial value for float and int is INF
        else {
            if (!scope) {
                global_variables =  add_to_symbol_table(variabledeclaration->name, result_with_sign {user_type, INFINITY, 0, ""}, global_variables);
            }
            else {
                local_variables = add_to_symbol_table(variabledeclaration->name, result_with_sign {user_type, INFINITY, 0, ""}, local_variables);
            }
        }
    }
}

void check_SimpleDeclaration(SimpleDeclaration *simpleDeclaration, unordered_map<string, Variable* > local_variables, bool scope) {
    if (simpleDeclaration->variabledeclaration) {
        check_VariableDeclaration(simpleDeclaration->variabledeclaration, local_variables, scope);
    }
    if (simpleDeclaration->typedeclaration) {
//        check_TypeDeclaration(simpleDeclaration->typedeclaration)
    }
}


void check_Declaration(Declaration *declaration) {
    bool scope = false; //global
    if (declaration->simpledeclaration) {
        check_SimpleDeclaration(declaration->simpledeclaration, {}, scope);
    }
    if (declaration->routinedeclaration){
        check_RoutineDeclaration(declaration->routinedeclaration);
    }
}

void check_Program(Program *program) {
    if (program->declaration) {
        check_Declaration(program->declaration);
    }
    if (program->program) {
        check_Program(program->program);
    }
    for (auto x : global_variables)
        cout << "\n" << x.first << " " << x.second->type  << " " << x.second->value << endl;

    for (auto x : functions)
        cout << "\n" << x.first << " " << x.second->return_type  << " " << endl;


}