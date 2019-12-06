#include <string>
#include <vector>
#include <iostream>

#include <math.h>
#include <bits/stdc++.h>

#include "SymbolTableIL.h"

using namespace std;

#define VARIABLE "Variable"
#define FUNCTION "Function"
#define TYPE "Type" 

#define BOOLEAN "boolean"
#define REAL "real"
#define INTEGER "integer"

/**
 * Choose dominant type - the one which should be returned after two primaries' cast.
 * 
 * @param type1  Type of the first primary
 * @param type2  Type of the second primary
 * @return       String indicating the output dominant type
 */
string gen_cast_types(string type1, string type2) {
    if (type1 == "real" || type2 == "real")
        return "real";
    else if (type1 == "integer" || type2 == "integer")
        return "integer";
    else if (type1 == "boolean" || type2 == "boolean")
        return "boolean";
    else {
        cout << "Wrong type (" << type1 << ", " << type2 << ")\n";
        exit(1);
    }
}

string format_type(string st) {
    if (st == "integer")
        return "int32";
    else if (st == "real")
        return "float64";
    else if (st == "boolean")
        return "bool";
    return "???";
}

string IL_number(int n) {
    string res = "";
    while (n != 0)  {
        res.push_back((n % 10) + '0');
        n /= 10;
    }
    while (res.size() != 4) res.push_back('0');
    reverse(res.begin(), res.end());
    return "IL_" + res;
}

map<string, Identifier*> gen_allow_redeclaration(map<string, Identifier*> declared_identifiers) {
    for (auto elem : declared_identifiers)
        elem.second->can_redeclare = true;
    return declared_identifiers;
}

/**
 * Check identifier's usage correctness, return actual identifier's type.
 * 
 * @param identifiers  Structure contatining identifiers' data
 * @param path         Full identifier's name in case of records dot notation
 * @return             Actual type of identifiers
 * DanyaDone
 */
string generate_Identifiers(Identifiers *identifiers, Identifier* ident, map<string, Identifier*> declared_identifiers, string type_change_to/*=""*/) {

    if (identifiers->expression) {
        generate_Expression(identifiers->expression, declared_identifiers);
    }

    if (!ident->subidentifiers[identifiers->name]) {
        cout << identifiers->name << "Was not declared in this scope";
        exit(EXIT_FAILURE);
    }

    if (identifiers->identifiers)
        return generate_Identifiers(identifiers->identifiers, ident->subidentifiers[identifiers->name], declared_identifiers, type_change_to);
    else {
        if (type_change_to != "")
            ident->subidentifiers[identifiers->name]->value_type = type_change_to;
        return ident->subidentifiers[identifiers->name]->value_type;
    }
}

/**
 * Check identifiers' usage correctness, return actual type of modifiable primary.
 * 
 * @param modifiableprimary  Structure contatining modifiable primary's data
 * @return                   Actual type of modifiable primary
 * DanyaDone
 */
Value_Commands* generate_ModifiablePrimary(ModifiablePrimary *modifiableprimary, map<string, Identifier*> declared_identifiers, bool generate_read_only/*=false*/, string type_change_to/*=""*/) {
    return nullptr;
    // string ident_name = modifiableprimary->name;

    // if (!declared_identifiers[ident_name]) {
    //     cout << ident_name << " was not declared in this scope\n";
    //     exit(EXIT_FAILURE);
    // }
    // if (generate_read_only && declared_identifiers[ident_name]->read_only) {
    //     cout << ident_name << " is read-only in this scope\n";
    //     exit(EXIT_FAILURE);
    // }

    // string type = declared_identifiers[ident_name]->value_type;

    // if (modifiableprimary->identifiers)
    //     return generate_Identifiers(modifiableprimary->identifiers, declared_identifiers[ident_name], declared_identifiers, type_change_to);
    // else {
    //     if (type_change_to != "")
    //         return declared_identifiers[ident_name]->value_type = type_change_to;
    //     return type;
    // }
}

// DanyaDone
Value_Commands* generate_Primary(Primary *primary, map<string, Identifier*> declared_identifiers) {
    if (primary->type != "") {
        Value_Commands* res = new Value_Commands(primary->type, stof(primary->value));
        if (primary->type == REAL) {
            res->commands.push_back("ldc.r8 " + to_string((float)((int)(100 * res->value)) / 100));
        }
        else {
            res->commands.push_back("ldc.i4." + to_string((int)(res->value)));
        }
        return res;
    }
    else
        return generate_ModifiablePrimary(primary->modifiablePrimary, declared_identifiers);
}

// DanyaDone
Value_Commands* generate_Summand(Summand *summand, map<string, Identifier*> declared_identifiers) {
    if (summand->expression)
        return generate_Expression(summand->expression, declared_identifiers);
    else
        return generate_Primary(summand->primary, declared_identifiers);
}

// DanyaDone
Value_Commands* generate_Summands(Summands *summands, map<string, Identifier*> declared_identifiers) {
    Value_Commands* res1, *res2 = nullptr, *total = new Value_Commands();
    res1 = generate_Summand(summands->summand, declared_identifiers);

    if (summands->summands) {
        res2 = generate_Summands(summands->summands, declared_identifiers);
        string op = summands->summands->sign->op;
        if (res1->commands.size() == 0 && res2->commands.size() == 0) {
            if (op == "+") total->value = (int)(res1->value) + (int)(res2->value);
            else if (op == "-") total->value = (int)(res1->value) - (int)(res2->value);
        }
        else {
            total->commands = res1->commands;
            total->commands.insert(total->commands.end(), res2->commands.begin(), res2->commands.end());
            if (op == "+") total->commands.push_back("add");
            else if (op == "-") total->commands.push_back("sub");
        }
        total->type = gen_cast_types(res1->type, res2->type);
        return total;
    }
    else
        return res1;
}

// DanyaDone
Value_Commands* generate_Factor(Factor *factor, map<string, Identifier*> declared_identifiers) {
    Value_Commands* res1, *res2 = nullptr, *total = new Value_Commands();
    res1 = generate_Summand(factor->summand, declared_identifiers);

    if (factor->summands) {
        res2 = generate_Summands(factor->summands, declared_identifiers);
        string op = factor->summands->sign->op;
        if (res1->commands.size() == 0 && res2->commands.size() == 0) {
            if (op == "+") total->value = (int)(res1->value) + (int)(res2->value);
            else if (op == "-") total->value = (int)(res1->value) - (int)(res2->value);
        }
        else {
            total->commands = res1->commands;
            total->commands.insert(total->commands.end(), res2->commands.begin(), res2->commands.end());
            if (op == "+") total->commands.push_back("add");
            else if (op == "-") total->commands.push_back("sub");
        }
        total->type = gen_cast_types(res1->type, res2->type);
        return total;
    }
    else
        return res1;
}

// DanyaDone
Value_Commands* generate_Factors(Factors *factors, map<string, Identifier*> declared_identifiers) {
    Value_Commands* res1, *res2 = nullptr, *total = new Value_Commands();
    res1 = generate_Factor(factors->factor, declared_identifiers);

    if (factors->factors) {
        res2 = generate_Factors(factors->factors, declared_identifiers);
        string op = factors->factors->simpleOperator->op;
        if (res1->commands.size() == 0 && res2->commands.size() == 0) {
            if (op == "*") total->value = (int)(res1->value) * (int)(res2->value);
            else if (op == "/") total->value = (int)(res1->value) / (int)(res2->value);
            else if (op == "%") total->value = (int)(res1->value) % (int)(res2->value);
        }
        else {
            total->commands = res1->commands;
            total->commands.insert(total->commands.end(), res2->commands.begin(), res2->commands.end());
            if (op == "*") total->commands.push_back("mul");
            else if (op == "/") total->commands.push_back("div");
            else if (op == "%")  total->commands.push_back("rem");
        }
        total->type = gen_cast_types(res1->type, res2->type);
        return total;
    }
    else
        return res1;
}

// DanyaDone
Value_Commands* generate_Simple(Simple *simple, map<string, Identifier*> declared_identifiers) {
    Value_Commands* res1, *res2 = nullptr, *total = new Value_Commands();
    res1 = generate_Factor(simple->factor, declared_identifiers);

    if (simple->factors) {
        res2 = generate_Factors(simple->factors, declared_identifiers);
        string op = simple->factors->simpleOperator->op;
        if (res1->commands.size() == 0 && res2->commands.size() == 0) {
            if (op == "*") total->value = (int)(res1->value) * (int)(res2->value);
            else if (op == "/") total->value = (int)(res1->value) / (int)(res2->value);
            else if (op == "%") total->value = (int)(res1->value) % (int)(res2->value);
        }
        else {
            total->commands = res1->commands;
            total->commands.insert(total->commands.end(), res2->commands.begin(), res2->commands.end());
            if (op == "*") total->commands.push_back("mul");
            else if (op == "/") total->commands.push_back("div");
            else if (op == "%")  total->commands.push_back("rem");
        }
        total->type = gen_cast_types(res1->type, res2->type);
        return total;
    }
    else
        return res1;
}

// DanyaDone
Value_Commands* generate_ComparisonInRelation(ComparisonInRelation *comparisoninrelation, map<string, Identifier*> declared_identifiers) {
    if (comparisoninrelation->simple)
        return generate_Simple(comparisoninrelation->simple, declared_identifiers);
    else return nullptr;
}

// DanyaDone
Value_Commands* generate_Relation(Relation *relation, map<string, Identifier*> declared_identifiers) {
    Value_Commands* res1, *res2 = nullptr, *total = new Value_Commands();
    if (relation->comparisoninrelation)
        res2 = generate_ComparisonInRelation(relation->comparisoninrelation, declared_identifiers);
    else
        res1 = generate_Simple(relation->simple, declared_identifiers);
    
    if (res2) {
        string op = relation->comparisoninrelation->comparisonoperator->op;
        if (res1->commands.size() == 0 && res2->commands.size() == 0) {
            if (op == "<") total->value = (int)(res1->value) < (int)(res2->value);
            else if (op == ">") total->value = (int)(res1->value) > (int)(res2->value);
            else if (op == "=") total->value = (int)(res1->value) == (int)(res2->value);
            else if (op == ">=") total->value = (int)(res1->value) >= (int)(res2->value);
            else if (op == "<=") total->value = (int)(res1->value) <= (int)(res2->value);
            else if (op == "/=") total->value = (int)(res1->value) != (int)(res2->value);
        }
        else {
            total->commands = res1->commands;
            total->commands.insert(total->commands.end(), res2->commands.begin(), res2->commands.end());
            
            if (op == "<") total->commands.push_back("clt");
            else if (op == ">") total->commands.push_back("cgt");
            else if (op == "=")  total->commands.push_back("ceq");
            else if (op == ">=") {
                total->commands.push_back("clt");
                total->commands.push_back("ldc.i4.0");
                total->commands.push_back("ceq");
            }
            else if (op == "<=") {
                total->commands.push_back("cgt");
                total->commands.push_back("ldc.i4.0");
                total->commands.push_back("ceq");
            }
            else if (op == "/=") {
                total->commands.push_back("ceq");
                total->commands.push_back("ldc.i4.0");
                total->commands.push_back("ceq");
            }
        }
        total->type = BOOLEAN;
        return total;
    }
    else {
        return res1;
    }
}

// DanyaDone
Value_Commands* generate_MultipleRelationsInExpression(MultipleRelationsInExpression *multiplerelationsinexpression, map<string, Identifier*> declared_identifiers) {
    Value_Commands* res1, *res2 = nullptr, *total = new Value_Commands();
    if (multiplerelationsinexpression->relation)
        res1 = generate_Relation(multiplerelationsinexpression->relation, declared_identifiers);
    if (multiplerelationsinexpression->multiplerelationsinexpression)
        res2 = generate_MultipleRelationsInExpression(multiplerelationsinexpression->multiplerelationsinexpression, declared_identifiers);
    if (res2) {
        string lop = multiplerelationsinexpression->multiplerelationsinexpression->logicaloperator->op;
        if (res1->commands.size() == 0 && res2->commands.size() == 0) {
            if (lop == "and") total->value = (int)(res1->value) && (int)(res2->value);
            else if (lop == "or") total->value = (int)(res1->value) || (int)(res2->value);
            else if (lop == "xor") total->value = (int)(res1->value) != (int)(res2->value);
        }
        else {
            total->commands = res1->commands;
            total->commands.insert(total->commands.end(), res2->commands.begin(), res2->commands.end());
            if (lop == "and") {

            }
            else if (lop == "or") {

            }
            else if (lop == "xor") {

            }
        }
        total->type = BOOLEAN;
        return total;
    }
    else {
        return res1;
    }
}

// DanyaDone
Value_Commands* generate_Expression(Expression *expression, map<string, Identifier*> declared_identifiers) {
    Value_Commands* res1, *res2 = nullptr, *total = new Value_Commands();
    if (expression->multiplerelationsinexpression) {
        res2 = generate_MultipleRelationsInExpression(expression->multiplerelationsinexpression, declared_identifiers);
    }
    else {
        res1 = generate_Relation(expression->relation, declared_identifiers);
        if (res2) {
            string lop = expression->multiplerelationsinexpression->logicaloperator->op;
            if (res1->commands.size() == 0 && res2->commands.size() == 0) {
                if (lop == "and") total->value = (int)(res1->value) && (int)(res2->value);
                else if (lop == "or") total->value = (int)(res1->value) || (int)(res2->value);
                else if (lop == "xor") total->value = (int)(res1->value) != (int)(res2->value);
            }
            else {
                total->commands = res1->commands;
                total->commands.insert(total->commands.end(), res2->commands.begin(), res2->commands.end());
                if (lop == "and") {

                }
                else if (lop == "or") {

                }
                else if (lop == "xor") {

                }
            }
            total->type = BOOLEAN;
            return total;
        }
        else {
            return res1;
        }
    }
    return nullptr;
}

// DanyaDone
void generate_ElseInIfStatement(ElseInIfStatement *elseinifstatement, map<string, Identifier*> declared_identifiers) {
    if (elseinifstatement->body)
        generate_Body(elseinifstatement->body, gen_allow_redeclaration(declared_identifiers));
}

// DanyaDone
vector<string> generate_IfStatement(IfStatement *ifstatement, map<string, Identifier*> declared_identifiers) {
    vector<string> commands;
    if (ifstatement->expression) {
        Value_Commands* cms = generate_Expression(ifstatement->expression, declared_identifiers);
        commands = cms->commands;
    }

    vector<string> BodyCommands;
    if (ifstatement->body) {
        BodyCommands = generate_Body(ifstatement->body, gen_allow_redeclaration(declared_identifiers)).second;
        commands.push_back("brfalse.s " + IL_number(10*BodyCommands.size()));
        commands.insert(commands.end(), BodyCommands.begin(), BodyCommands.end());
    }

    if (ifstatement->elseinifstatement)
        generate_ElseInIfStatement(ifstatement->elseinifstatement, declared_identifiers);
    return commands;
}

// DanyaDone
void generate_Range(Range *range, map<string, Identifier*> declared_identifiers) {
    string type_1, type_2;
    if (range->expression1)
        type_1 = generate_Expression(range->expression1, declared_identifiers)->type;
    if (range->expression2)
        type_2 = generate_Expression(range->expression2, declared_identifiers)->type;
    
    if (type_1 != "integer" || type_2 != "integer") {
        cout << "ERROR Both expressions in ForLoop must to be integer, but found: " << type_1 << " and " << type_2 << "\n";
        exit(EXIT_FAILURE);
    }
}

// DanyaDone
void generate_ForLoop(ForLoop *forloop, map<string, Identifier*> declared_identifiers) {
    map<string, Identifier*> redeclaration_allowed = gen_allow_redeclaration(declared_identifiers);
    if (!(forloop->name).empty()) {
        redeclaration_allowed[forloop->name] = new Identifier(8, VARIABLE, "integer", 1);
    }
    if (forloop->range) {
        generate_Range(forloop->range, redeclaration_allowed);
    }
    if (forloop->body) {
        generate_Body(forloop->body, redeclaration_allowed);
    }
}

// DanyaDone
void generate_WhileLoop(WhileLoop *whileloop, map<string, Identifier*> declared_identifiers) {
    if (whileloop->expression)
        generate_Expression(whileloop->expression, declared_identifiers);

    if (whileloop->body)
        generate_Body(whileloop->body, gen_allow_redeclaration(declared_identifiers));
}

// DanyaDone
void generate_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall, map<string, Identifier*> declared_identifiers) {
    if (expressionsinroutinecall->expression)
        generate_Expression(expressionsinroutinecall->expression, declared_identifiers);
    if (expressionsinroutinecall->expressionsinroutinecall)
        generate_ExpressionsInRoutineCall(expressionsinroutinecall->expressionsinroutinecall, declared_identifiers);
}

// DanyaDone
void generate_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall, map<string, Identifier*> declared_identifiers) {
    if (expressioninroutinecall->expression)
        generate_Expression(expressioninroutinecall->expression, declared_identifiers);
    if (expressioninroutinecall->expressionsinroutinecall)
        generate_ExpressionsInRoutineCall(expressioninroutinecall->expressionsinroutinecall, declared_identifiers);
}

// DanyaDone
void generate_RoutineCall(RoutineCall *routinecall, map<string, Identifier*> declared_identifiers) {
    if (!declared_identifiers[routinecall->name]) {
        cout << "\n\nFunction " << routinecall->name << " was not declared!\n";
        exit(EXIT_FAILURE);
    }
    if (routinecall->expressioninroutinecall) {
        generate_ExpressionInRoutineCall(routinecall->expressioninroutinecall, declared_identifiers);
    }
}

// DanyaDone
void generate_Assignment(Assignment *assignment, map<string, Identifier*> declared_identifiers) {
    string left_part_type, right_part_type;

    if (assignment->modifiableprimary)
        left_part_type = generate_ModifiablePrimary(assignment->modifiableprimary, declared_identifiers, true)->type;
    
    if (assignment->expression)
        right_part_type = generate_Expression(assignment->expression, declared_identifiers)->type;
    
    if (left_part_type == "boolean" && right_part_type == "real") {
        cout << "Impossible to cast real to boolean\n";
        exit(EXIT_FAILURE);
    }
    if (left_part_type.substr(0, 6) == "record" && left_part_type != right_part_type) {
        cout << "Impossible to cast " << left_part_type << " to " << right_part_type << "\n";
        exit(EXIT_FAILURE);
    }

    // To change type of left part if needed
    if (assignment->modifiableprimary && left_part_type.substr(0, 5) == "array" && right_part_type.substr(0, 5) == "array") {
        left_part_type = generate_ModifiablePrimary(assignment->modifiableprimary, declared_identifiers, true, right_part_type)->type;
    }

    if ((left_part_type.substr(0, 5) == "array") != (right_part_type.substr(0, 5) == "array")) {
        cout << "Impossible to cast " << left_part_type << " to " << right_part_type << "\n";
        exit(EXIT_FAILURE);
    }

}

// DanyaDone
vector<string> generate_Statement(Statement *statement, map<string, Identifier* > declared_identifiers) {
    if (statement->assignment) {
        generate_Assignment(statement->assignment, declared_identifiers);
    }
    else if (statement->routinecall) {
        generate_RoutineCall(statement->routinecall, declared_identifiers);
    }
    else if (statement->whileloop) {
        generate_WhileLoop(statement->whileloop, declared_identifiers);
    }
    else if (statement->forloop) {
        generate_ForLoop(statement->forloop, declared_identifiers);
    }
    else if (statement->ifstatement) {
        return generate_IfStatement(statement->ifstatement, declared_identifiers);
    }
    vector<string> emp;
    return emp;
}

// DanyaDone
pair<map<string, Identifier*>, vector<string>> generate_Body(Body *body, map<string, Identifier*> declared_identifiers, int var_number) {
    vector<string> commands;

    if (body->simpledeclaration) {
        auto res = generate_SimpleDeclaration(body->simpledeclaration, declared_identifiers, 0, var_number);
        
        var_number++;

        commands.insert(commands.end(), res.second->commands.begin(), res.second->commands.end());
        declared_identifiers = res.first;
    }

    if (body->statement) {
        vector<string> tmp = generate_Statement(body->statement, declared_identifiers);
        commands.insert(commands.end(), tmp.begin(), tmp.end());
    }

    if (body->body) {
        auto res = generate_Body(body->body, declared_identifiers, var_number);
        declared_identifiers = res.first;
        commands.insert(commands.end(), res.second.begin(), res.second.end());
    }

    return {declared_identifiers, commands};
}

// DanyaDone
string generate_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration, map<string, Identifier*> declared_identifiers) {
    if (bodyinroutinedeclaration->body) {
        cout << ".locals init(\n";
        auto res = generate_Body(bodyinroutinedeclaration->body, declared_identifiers);
        int var_num = 0;
        for (auto elem : res.first) {
            if (!declared_identifiers[elem.first]) {
                cout << "[" << elem.second->var_number << "] " << format_type(elem.second->value_type) << " " << elem.first << "\n";
            }
        }

        cout << ")\n";

        for (int i = 0; i < res.second.size(); i++)
            cout << IL_number(10*i) << ": " << res.second[i] << "\n";
    }
    
    if (bodyinroutinedeclaration->returnInRoutine) {
        string actual_type = generate_ReturnInRoutine(bodyinroutinedeclaration->returnInRoutine, declared_identifiers);
        return actual_type;
    }

    return "";
}

// DanyaDone
string generate_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration, map<string, Identifier*> declared_identifiers) {
    if (typeinroutinedeclaration->type) {
        return generate_Type(typeinroutinedeclaration->type, declared_identifiers);
    }
    else
        return ""; // Impossible btw
}

// DanyaDone
map<string, Identifier* > generate_ParametersDeclaration(ParametersDeclaration *parametersdeclaration, map<string, Identifier* > declared_identifiers) {
    if (parametersdeclaration->parameterdeclaration)
        declared_identifiers = generate_ParameterDeclaration(parametersdeclaration->parameterdeclaration, declared_identifiers);

    if (parametersdeclaration->parametersdeclaration)
        declared_identifiers = generate_ParametersDeclaration(parametersdeclaration->parametersdeclaration, declared_identifiers);

    return declared_identifiers;
}

// DanyaDone
map<string, Identifier* > generate_ParameterDeclaration(ParameterDeclaration *parameterdeclaration, map<string, Identifier* > declared_identifiers) {
    string name;
    string type;
    if (!(parameterdeclaration->name).empty()) {
        name = parameterdeclaration->name;
    }
    if (parameterdeclaration->type) {
        type = generate_Type(parameterdeclaration->type, declared_identifiers, nullptr, true);
    }
    declared_identifiers[name] =  new Identifier(8, VARIABLE, type);
    return declared_identifiers;
}

// DanyaDone
map<string, Identifier*> generate_Parameters(Parameters *parameters, map<string, Identifier* > declared_identifiers) {
    if (parameters->parameterdeclaration)
        declared_identifiers = generate_ParameterDeclaration(parameters->parameterdeclaration, declared_identifiers);

    if (parameters->parametersdeclaration)
        declared_identifiers = generate_ParametersDeclaration(parameters->parametersdeclaration, declared_identifiers);

    return declared_identifiers;
}

// DanyaDone
string generate_ReturnInRoutine(ReturnInRoutine *returnInRoutine, map<string, Identifier* > declared_identifiers) {
    if (returnInRoutine->expression) {
        return generate_Expression(returnInRoutine->expression, declared_identifiers)->type;
    }
    else
        return ""; // Impossible btw
};

// DanyaDone
map<string, Identifier* > generate_RoutineDeclaration(RoutineDeclaration *routinedeclaration, map<string, Identifier* > declared_identifiers, bool entrypoint) {
    string function_name;
    string return_type;

    map<string, Identifier*> declared_identifiers_in_function = gen_allow_redeclaration(declared_identifiers);

    function_name = routinedeclaration->name;
    cout << ".method private hidebysig static void\n" << function_name << "(\n";
    if (entrypoint) {
        cout << "string[] args\n";
    }
    else {
        if (routinedeclaration->parameters) {
            //declared_identifiers_in_function = generate_Parameters(routinedeclaration->parameters, declared_identifiers_in_function); // we can use this inside body
        }
    }
    cout << ") cil managed\n";
    cout << "{\n";
    if (entrypoint)
        cout << ".entrypoint\n";
    
    cout << ".maxstack 100\n\n";

    if (routinedeclaration->typeinroutinedeclaration) { // ???
        // return_type = generate_TypeInRoutineDeclaration(routinedeclaration->typeinroutinedeclaration, declared_identifiers); // Because we have type declarations
        // if (routinedeclaration->bodyinroutinedeclaration && !routinedeclaration->bodyinroutinedeclaration->returnInRoutine) {
        //     cout << "\n\nFunction " << function_name << " doesn't return a type!\n";
        //     exit(EXIT_FAILURE);
        // }
    }

    Identifier *function_name_identifier =  new Identifier(8, FUNCTION, return_type); 
    declared_identifiers[function_name] = function_name_identifier;
    declared_identifiers_in_function[function_name] = function_name_identifier;

    string actual_type = generate_BodyInRoutineDeclaration(routinedeclaration->bodyinroutinedeclaration, declared_identifiers_in_function);

    cout << IL_number(880) << ": call   void [System.Console]System.Console::WriteLine(int32)\n";

    cout << IL_number(900) << ": ret";
    cout << "\n}\n";

    return declared_identifiers;
}


// DanyaDone
string generate_VariableDeclarations(VariableDeclarations *variabledeclarations, map<string, Identifier*> declared_identifiers, Identifier* ident) {
    string result;
    if (variabledeclarations->variabledeclaration) {
        generate_VariableDeclaration(variabledeclarations->variabledeclaration, declared_identifiers, ident);
        result = variabledeclarations->variabledeclaration->name + "-" + generate_Type(variabledeclarations->variabledeclaration->type, declared_identifiers);
    }
    
    if (variabledeclarations->variabledeclarations)
        result += ":" + generate_VariableDeclarations(variabledeclarations->variabledeclarations, declared_identifiers, ident);
    
    return result;
}

// DanyaDone
string generate_RecordType(RecordType *recordtype, map<string, Identifier*> declared_identifiers, Identifier *ident) {
    if (recordtype->variabledeclarations)
        return generate_VariableDeclarations(recordtype->variabledeclarations, declared_identifiers, ident);
    else 
        return ""; // Impossible btw
}

// DanyaDone
string generate_ArrayType(ArrayType *arraytype, map<string, Identifier*> declared_identifiers, bool is_param) {
    if (arraytype->expression) {
        string expr_type = generate_Expression(arraytype->expression, declared_identifiers)->type;

        // check if expression type is integer
        if (expr_type != "integer") {
            cout << "\n######\nERROR! Array size must be integer, but found: '" << expr_type << "'\n######\n";
            exit(EXIT_FAILURE);
        }
    }
    // if array size is not specified
    else {
        // check if it's inside a routine parameters declaration
        if (!is_param) {
            cout << "\n######\nERROR! Sizeless array declaration is not acceptable here!\n######\n";
            exit(EXIT_FAILURE);
        }
    }

    
    return generate_Type(arraytype->type, declared_identifiers, nullptr, is_param);     // check type of the array
}

/**
 * Checks type correctness and return its string representation.
 * 
 * @param type      type structure containing type informationL is it array, record, primitive or user-defined
 * @param is_param  is type declared for routine parameter (false by default)
 * @return          string representation of given type (if it's a custom type, its actual type is returned)
 * DanyaDone
 */
string generate_Type(Type *type, map<string, Identifier*> declared_identifiers, Identifier* ident /*= nullptr*/, bool is_param/*=false*/) {
    string user_type;
    if (type->arraytype) {
        return "array:" + generate_ArrayType(type->arraytype, declared_identifiers, is_param);
    }
    else if (type->primitivetype)
        return type->primitivetype->isint     ? "integer" :
               type->primitivetype->isreal    ? "real"    :
               type->primitivetype->isboolean ? "boolean" : "IMPOSSIBLE";
    else if (type->recordtype) {
        return "record:" + generate_RecordType(type->recordtype, declared_identifiers, ident);
    }
    else {
        user_type = type->name;
        transform(user_type.begin(), user_type.end(), user_type.begin(), ::tolower);
        
        // check if user type exists
        if (!declared_identifiers[user_type]) {
            cout << "\n######\nERROR! Type doesn't exist in this scope: '" << user_type << "'\n######\n";
            exit(EXIT_FAILURE);
        }

        // return actual type of a custom type
        return declared_identifiers[user_type]->value_type;
    }
}

/**
 * Check if a new type has been already declared, and store it if correct.
 * 
 * @param typedeclaration  Structure containing new type data
 * DanyaDone
 */
map<string, Identifier*> generate_TypeDeclaration(TypeDeclaration *typedeclaration, map<string, Identifier*> declared_identifiers, bool global_declaration) {
    string name = typedeclaration->name;
    transform(name.begin(), name.end(), name.begin(), ::tolower);

    // check if type was already declared
    if (declared_identifiers[name]) {
        cout << "\n######\nERROR! Type is primitive or already declared: '" << typedeclaration->name << "'\n######\n";
        exit(EXIT_FAILURE);
    }

    // check actual type and get its string representation
    string type = generate_Type(typedeclaration->type, declared_identifiers);

    declared_identifiers[name] =  new Identifier(8, TYPE, type);

    return declared_identifiers;
}

// DanyaDone
Value_Commands* generate_InitialValue(InitialValue *initialvalue, map<string, Identifier* > declared_identifiers) {
    if (initialvalue->expression)
        return generate_Expression(initialvalue->expression, declared_identifiers);
    else
        return nullptr; // Impossible btw
}

// DanyaDone
pair<map<string, Identifier*>, Value_Commands*> generate_VariableDeclaration(VariableDeclaration *variabledeclaration, map<string, Identifier* > declared_identifiers, bool global_declaration/*=false*/, int var_number/*=0*/, Identifier *parent /*= nullptr*/) {
    string user_type;

    Identifier *new_identifier =  new Identifier(8, VARIABLE, user_type, global_declaration, var_number);

    //getting var type
    if (variabledeclaration->type) {
        user_type = generate_Type(variabledeclaration->type, declared_identifiers, new_identifier);
        new_identifier->value_type = user_type;
    }
    // type is setting by the value of expression
    else if (variabledeclaration->expression) { // var b is 5
        Value_Commands* expression = generate_Expression(variabledeclaration->expression, declared_identifiers);
        user_type = expression->type;
        declared_identifiers[variabledeclaration->name] = new Identifier(8, VARIABLE, user_type, global_declaration, var_number);

        if (global_declaration) {
            cout << ".field public static " << format_type(new_identifier->value_type) << " " << variabledeclaration->name << "\n";
            expression->commands.push_back("stsfld " + format_type(new_identifier->value_type) + " ConsoleApp1.Program::" + variabledeclaration->name);
        }
        else {
            expression->commands.push_back("stloc." + to_string(declared_identifiers[variabledeclaration->name]->var_number));

        }

        return {declared_identifiers, expression};
    }
    else {
        cout << "Something is defenitely wrong in declaration of this variable: " << variabledeclaration->name << "\n";
        exit(EXIT_FAILURE);
    }

    // getting initial value
    
    Value_Commands* initialvalue = nullptr;
    if (variabledeclaration->initialvalue) { // var b: Integer is 5
        Value_Commands* initialvalue = generate_InitialValue(variabledeclaration->initialvalue, declared_identifiers);
        string actual_type = initialvalue->type;
        if (actual_type == user_type || 
        (user_type == "real" && actual_type == "integer") ||
        (user_type == "boolean" && actual_type == "integer")) {
            if (parent) {// Only in case of record declaration
                parent->subidentifiers[variabledeclaration->name] = new_identifier;
            }
            else {
                if (global_declaration) {
                    cout << ".field public static " << format_type(new_identifier->value_type) << " " << variabledeclaration->name << "\n";
                    initialvalue->commands.push_back("stsfld " + format_type(new_identifier->value_type) + " ConsoleApp1.Program::" + variabledeclaration->name);
                }
                else {
                    initialvalue->commands.push_back("stloc." + to_string(new_identifier->var_number));
                }
                declared_identifiers[variabledeclaration->name] = new_identifier;
            }
        }
        else if (actual_type.substr(0, 5) == "array" && user_type.substr(0, 5) == "array") {
            new_identifier->value_type = actual_type;
            declared_identifiers[variabledeclaration->name] = new_identifier;
            if (global_declaration) {
                cout << ".field public static " << format_type(new_identifier->value_type) << " " << variabledeclaration->name << "\n";
            }
            else {
                initialvalue->commands.push_back("stloc." + to_string(new_identifier->var_number));
            }
        }
        return make_pair(declared_identifiers, initialvalue);
    }
    else { // case of initialization without initial value:  var b : Integer
        if (parent) {// Only in case of record declaration
            parent->subidentifiers[variabledeclaration->name] = new_identifier;
        }
        else {
            if (global_declaration) {
                cout << ".field public static " << format_type(new_identifier->value_type) << " " << variabledeclaration->name << "\n";
            }
            else {
                initialvalue->commands.push_back("stloc." + new_identifier->var_number);
            }
            declared_identifiers[variabledeclaration->name] = new_identifier;
        }
        return {declared_identifiers, nullptr};
    }
}

// DanyaDone
pair<map<string, Identifier*>, Value_Commands*> generate_SimpleDeclaration(SimpleDeclaration *simpleDeclaration, map<string, Identifier*> declared_identifiers, bool global_declaration, int var_number) {
    if (simpleDeclaration->variabledeclaration) {
        return generate_VariableDeclaration(simpleDeclaration->variabledeclaration, declared_identifiers, global_declaration, var_number);
    }
    if (simpleDeclaration->typedeclaration) {
        declared_identifiers = generate_TypeDeclaration(simpleDeclaration->typedeclaration, declared_identifiers, global_declaration);
        return {declared_identifiers, nullptr};
    }
    return {declared_identifiers, nullptr};
}

// DanyaDone
pair<map<string, Identifier*>, Value_Commands*> generate_Declaration(Declaration *declaration, map<string, Identifier*> declared_identifiers) {
    if (declaration->simpledeclaration) {
        return generate_SimpleDeclaration(declaration->simpledeclaration, declared_identifiers, 1);
    }
    if (declaration->routinedeclaration){
        declared_identifiers = generate_RoutineDeclaration(declaration->routinedeclaration, declared_identifiers, 1);
        return {declared_identifiers, nullptr};
    }
    return {declared_identifiers, nullptr};
}

// DanyaDone
vector<string> generate_Program(Program *program, map<string, Identifier*> declared_identifiers) {
    vector<string> commands;
    if (program->declaration) {
        auto res = generate_Declaration(program->declaration, declared_identifiers).second;
        if (res)
            commands = res->commands;
    }

    if (program->program) {
        vector<string> temp = generate_Program(program->program, declared_identifiers);
        commands.insert(commands.end(), temp.begin(), temp.end());
    }

    return commands;
}

bool run_IL_Code_Generator(Program *program) {
    freopen("generated.il", "w", stdout);

    cout << ".assembly rabotai{}\n.assembly extern System.Console{}\n.assembly extern System.Runtime{}\n.assembly extern mscorlib{}\n\n";
    cout << ".class private auto ansi beforefieldinit ConsoleApp1.Program extends [System.Runtime]System.Object\n{\n";

    map<string, Identifier*> declared_identifiers;
    declared_identifiers["integer"] =  new Identifier(8, "Type", "integer", 1);
    declared_identifiers["real"] =  new Identifier(8, "Type", "real", 1);
    declared_identifiers["boolean"] =  new Identifier(8, "Type", "boolean", 1);
    vector<string> init_commands = generate_Program(program, declared_identifiers);

    cout << "\n.method private hidebysig static specialname rtspecialname void .cctor() cil managed\n{\n";
    cout << ".maxstack 100\n\n";
    for (int i = 0; i < init_commands.size(); i++)
        cout << IL_number(10*i) << ": " << init_commands[i] << "\n";

    cout << "\n";
    cout << IL_number(900) << ": ret";
    cout << "\n}\n";

    cout << "}";
    freopen("CON", "w", stdout);
    return true;
}