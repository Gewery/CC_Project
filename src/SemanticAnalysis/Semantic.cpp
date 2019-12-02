#include <string>
#include <vector>
#include <iostream>

#include <math.h>
#include <bits/stdc++.h>

#include "SymbolTable.h"

using namespace std;

#define VARIABLE "Variable"
#define FUNCTION "Function"
#define TYPE "Type" 

/**
 * Choose dominant type - the one which should be returned after two primaries' cast.
 * 
 * @param type1  Type of the first primary
 * @param type2  Type of the second primary
 * @return       String indicating the output dominant type
 */
string cast_types(string type1, string type2) {
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

map<string, Identifier*> allow_redeclaration(map<string, Identifier*> declared_identifiers) {
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
string check_Identifiers(Identifiers *identifiers, Identifier* ident, map<string, Identifier*> declared_identifiers) {

    if (identifiers->expression) {
        check_Expression(identifiers->expression, declared_identifiers);
    }

    if (!ident->subidentifiers[identifiers->name]) {
        cout << identifiers->name << "Was not declared in this scope";
        exit(EXIT_FAILURE);
    }

    if (identifiers->identifiers)
        return check_Identifiers(identifiers->identifiers, ident->subidentifiers[identifiers->name], declared_identifiers);
    else
        return ident->subidentifiers[identifiers->name]->value_type;
}

/**
 * Check identifiers' usage correctness, return actual type of modifiable primary.
 * 
 * @param modifiableprimary  Structure contatining modifiable primary's data
 * @return                   Actual type of modifiable primary
 * DanyaDone
 */
string check_ModifiablePrimary(ModifiablePrimary *modifiableprimary, map<string, Identifier*> declared_identifiers, bool check_read_only/*=false*/) {
    string ident_name = modifiableprimary->name;

    if (!declared_identifiers[ident_name]) {
        cout << ident_name << " was not declared in this scope\n";
        exit(EXIT_FAILURE);
    }
    if (check_read_only && declared_identifiers[ident_name]->read_only) {
        cout << ident_name << " is read-only in this scope\n";
        exit(EXIT_FAILURE);
    }

    string type = declared_identifiers[ident_name]->value_type;

    if (modifiableprimary->identifiers)
        return check_Identifiers(modifiableprimary->identifiers, declared_identifiers[ident_name], declared_identifiers);
    else
        return type;
}

// DanyaDone
string check_Primary(Primary *primary, map<string, Identifier*> declared_identifiers) {
    if (primary->type != "")
        return primary->type;
    else
        return check_ModifiablePrimary(primary->modifiablePrimary, declared_identifiers);
}

// DanyaDone
string check_Summand(Summand *summand, map<string, Identifier*> declared_identifiers) {
    if (summand->expression)
        return check_Expression(summand->expression, declared_identifiers);
    else
        return check_Primary(summand->primary, declared_identifiers);
}

// DanyaDone
string check_Summands(Summands *summands, map<string, Identifier*> declared_identifiers) {
    string summand_type = check_Summand(summands->summand, declared_identifiers);

    if (summands->summands)
        return cast_types(summand_type, check_Summands(summands->summands, declared_identifiers));
    else
        return summand_type;
}

// DanyaDone
string check_Factor(Factor *factor, map<string, Identifier*> declared_identifiers) {
    string summand_type = check_Summand(factor->summand, declared_identifiers);

    if (factor->summands)
        return cast_types(summand_type, check_Summands(factor->summands, declared_identifiers));
    else
        return summand_type;
}

// DanyaDone
string check_Factors(Factors *factors, map<string, Identifier*> declared_identifiers) {
    string factor_type = check_Factor(factors->factor, declared_identifiers);

    if (factors->factors)
        return cast_types(factor_type, check_Factors(factors->factors, declared_identifiers));
    else
        return factor_type;
}

// DanyaDone
string check_Simple(Simple *simple, map<string, Identifier*> declared_identifiers) {
    string factor_type = check_Factor(simple->factor, declared_identifiers);

    if (simple->factors)
        return cast_types(factor_type, check_Factors(simple->factors, declared_identifiers));
    else
        return factor_type;
}

// DanyaDone
void check_ComparisonInRelation(ComparisonInRelation *comparisoninrelation, map<string, Identifier*> declared_identifiers) {
    if (comparisoninrelation->simple)
        check_Simple(comparisoninrelation->simple, declared_identifiers);
}

// DanyaDone
string check_Relation(Relation *relation, map<string, Identifier*> declared_identifiers) {
    if (relation->comparisoninrelation) {
        check_ComparisonInRelation(relation->comparisoninrelation, declared_identifiers);
        return "boolean";
    }
    else
        return check_Simple(relation->simple, declared_identifiers);
}

// DanyaDone
void check_MultipleRelationsInExpression(MultipleRelationsInExpression *multiplerelationsinexpression, map<string, Identifier*> declared_identifiers) {
    if (multiplerelationsinexpression->relation)
        check_Relation(multiplerelationsinexpression->relation, declared_identifiers);
    if (multiplerelationsinexpression->multiplerelationsinexpression)
        check_MultipleRelationsInExpression(multiplerelationsinexpression->multiplerelationsinexpression, declared_identifiers);
}

// DanyaDone
string check_Expression(Expression *expression, map<string, Identifier*> declared_identifiers) {
    if (expression->multiplerelationsinexpression) {
        check_MultipleRelationsInExpression(expression->multiplerelationsinexpression, declared_identifiers);
        return "boolean";
    }
    else
        return check_Relation(expression->relation, declared_identifiers);
}

// DanyaDone
void check_ElseInIfStatement(ElseInIfStatement *elseinifstatement, map<string, Identifier*> declared_identifiers) {
    if (elseinifstatement->body)
        check_Body(elseinifstatement->body, allow_redeclaration(declared_identifiers));
}

// DanyaDone
void check_IfStatement(IfStatement *ifstatement, map<string, Identifier*> declared_identifiers) {
    if (ifstatement->expression)
        check_Expression(ifstatement->expression, declared_identifiers);

    if (ifstatement->body)
        check_Body(ifstatement->body, allow_redeclaration(declared_identifiers));

    if (ifstatement->elseinifstatement)
        check_ElseInIfStatement(ifstatement->elseinifstatement, declared_identifiers);
}

// DanyaDone
void check_Range(Range *range, map<string, Identifier*> declared_identifiers) {
    string type_1, type_2;
    if (range->expression1)
        type_1 = check_Expression(range->expression1, declared_identifiers);
    if (range->expression2)
        type_2 = check_Expression(range->expression2, declared_identifiers);
    
    if (type_1 != "integer" || type_2 != "integer") {
        cout << "ERROR Both expressions in ForLoop must to be integer, but found: " << type_1 << " and " << type_2 << "\n";
        exit(EXIT_FAILURE);
    }
}

// DanyaDone
void check_ForLoop(ForLoop *forloop, map<string, Identifier*> declared_identifiers) { // TODO Read-only thing
    map<string, Identifier*> redeclaration_allowed = allow_redeclaration(declared_identifiers);
    if (!(forloop->name).empty()) {
        redeclaration_allowed[forloop->name] = new Identifier(VARIABLE, "integer", 1);
    }
    if (forloop->range) {
        check_Range(forloop->range, redeclaration_allowed);
    }
    if (forloop->body) {
        check_Body(forloop->body, redeclaration_allowed);
    }
}

// DanyaDone
void check_WhileLoop(WhileLoop *whileloop, map<string, Identifier*> declared_identifiers) {
    if (whileloop->expression)
        check_Expression(whileloop->expression, declared_identifiers);

    if (whileloop->body)
        check_Body(whileloop->body, allow_redeclaration(declared_identifiers));
}

// DanyaDone
void check_ExpressionsInRoutineCall(ExpressionsInRoutineCall *expressionsinroutinecall, map<string, Identifier*> declared_identifiers) {
    if (expressionsinroutinecall->expression)
        check_Expression(expressionsinroutinecall->expression, declared_identifiers);
    if (expressionsinroutinecall->expressionsinroutinecall)
        check_ExpressionsInRoutineCall(expressionsinroutinecall->expressionsinroutinecall, declared_identifiers);
}

// DanyaDone
void check_ExpressionInRoutineCall(ExpressionInRoutineCall *expressioninroutinecall, map<string, Identifier*> declared_identifiers) {
    if (expressioninroutinecall->expression)
        check_Expression(expressioninroutinecall->expression, declared_identifiers);
    if (expressioninroutinecall->expressionsinroutinecall)
        check_ExpressionsInRoutineCall(expressioninroutinecall->expressionsinroutinecall, declared_identifiers);
}

// DanyaDone
void check_RoutineCall(RoutineCall *routinecall, map<string, Identifier*> declared_identifiers) {
    if (!declared_identifiers[routinecall->name]) {
        cout << "\n\nFunction " << routinecall->name << " was not declared!\n";
        exit(EXIT_FAILURE);
    }
    if (routinecall->expressioninroutinecall) {
        check_ExpressionInRoutineCall(routinecall->expressioninroutinecall, declared_identifiers);
    }
}

// DanyaDone
void check_Assignment(Assignment *assignment, map<string, Identifier*> declared_identifiers) {
    string left_part_type, right_part_type;
    if (assignment->modifiableprimary)
        left_part_type = check_ModifiablePrimary(assignment->modifiableprimary, declared_identifiers, true);
    
    if (assignment->expression)
        right_part_type = check_Expression(assignment->expression, declared_identifiers);
    
    if (left_part_type == "boolean" && right_part_type == "real") {
        cout << "Impossible to cast real to boolean\n";
        exit(EXIT_FAILURE);
    }
}

// DanyaDone
void check_Statement(Statement *statement, map<string, Identifier* > declared_identifiers) {
    if (statement->assignment) {
        check_Assignment(statement->assignment, declared_identifiers);
    }
    else if (statement->routinecall) {
        check_RoutineCall(statement->routinecall, declared_identifiers);
    }
    else if (statement->whileloop) {
        check_WhileLoop(statement->whileloop, declared_identifiers);
    }
    else if (statement->forloop) {
        check_ForLoop(statement->forloop, declared_identifiers);
    }
    else if (statement->ifstatement) {
        check_IfStatement(statement->ifstatement, declared_identifiers);
    }
}

// DanyaDone
map<string, Identifier*> check_Body(Body *body, map<string, Identifier*> declared_identifiers) {
    if (body->simpledeclaration)
        declared_identifiers = check_SimpleDeclaration(body->simpledeclaration, declared_identifiers);

    if (body->statement)
        check_Statement(body->statement, declared_identifiers);

    if (body->body)
        declared_identifiers = check_Body(body->body, declared_identifiers);

    return declared_identifiers;
}

// DanyaDone
map<string, Identifier*> check_BodyInRoutineDeclaration(BodyInRoutineDeclaration *bodyinroutinedeclaration, map<string, Identifier*> declared_identifiers) {
    if (bodyinroutinedeclaration->body)
        declared_identifiers = check_Body(bodyinroutinedeclaration->body, declared_identifiers);

    return declared_identifiers;
}

// DanyaDone
string check_TypeInRoutineDeclaration(TypeInRoutineDeclaration *typeinroutinedeclaration, map<string, Identifier*> declared_identifiers) {
    if (typeinroutinedeclaration->type) {
        return check_Type(typeinroutinedeclaration->type, declared_identifiers);
    }
    else
        return ""; // Impossible btw
}

// DanyaDone
map<string, Identifier* > check_ParametersDeclaration(ParametersDeclaration *parametersdeclaration, map<string, Identifier* > declared_identifiers) {
    if (parametersdeclaration->parameterdeclaration)
        declared_identifiers = check_ParameterDeclaration(parametersdeclaration->parameterdeclaration, declared_identifiers);

    if (parametersdeclaration->parametersdeclaration)
        declared_identifiers = check_ParametersDeclaration(parametersdeclaration->parametersdeclaration, declared_identifiers);

    return declared_identifiers;
}

// DanyaDone
map<string, Identifier* > check_ParameterDeclaration(ParameterDeclaration *parameterdeclaration, map<string, Identifier* > declared_identifiers) {
    string name;
    string type;
    if (!(parameterdeclaration->name).empty()) {
        name = parameterdeclaration->name;
    }
    if (parameterdeclaration->type) {
        type = check_Type(parameterdeclaration->type, declared_identifiers, nullptr, true);
    }
    declared_identifiers[name] = new Identifier(VARIABLE, type);
    return declared_identifiers;
}

// DanyaDone
map<string, Identifier*> check_Parameters(Parameters *parameters, map<string, Identifier* > declared_identifiers) {
    if (parameters->parameterdeclaration)
        declared_identifiers = check_ParameterDeclaration(parameters->parameterdeclaration, declared_identifiers);

    if (parameters->parametersdeclaration)
        declared_identifiers = check_ParametersDeclaration(parameters->parametersdeclaration, declared_identifiers);

    return declared_identifiers;
}

// DanyaDone
string check_ReturnInRoutine(ReturnInRoutine *returnInRoutine, map<string, Identifier* > declared_identifiers) {
    if (returnInRoutine->expression) {
        return check_Expression(returnInRoutine->expression, declared_identifiers);
    }
    else
        return ""; // Impossible btw
};

// DanyaDone
map<string, Identifier* > check_RoutineDeclaration(RoutineDeclaration *routinedeclaration, map<string, Identifier* > declared_identifiers) {
    string function_name;
    string return_type;

    map<string, Identifier*> declared_identifiers_in_function;

    function_name = routinedeclaration->name;
    if (routinedeclaration->parameters) {
        declared_identifiers_in_function = check_Parameters(routinedeclaration->parameters, allow_redeclaration(declared_identifiers)); // we can use this inside body
    }
    if (routinedeclaration->typeinroutinedeclaration) {
        return_type = check_TypeInRoutineDeclaration(routinedeclaration->typeinroutinedeclaration, declared_identifiers); // Because we have type declarations
        if (!routinedeclaration->returnInRoutine) {
            cout << "\n\nFunction " << function_name << " doesn't return a type!\n";
            exit(EXIT_FAILURE);
        }
    }
    else {
        if (routinedeclaration->returnInRoutine) {
            cout << "\n\nFunction " << function_name << "must not return a value!\n";
            exit(EXIT_FAILURE);
        }
    }

    declared_identifiers[function_name] = new Identifier(FUNCTION, return_type);

    declared_identifiers_in_function = check_BodyInRoutineDeclaration(routinedeclaration->bodyinroutinedeclaration, declared_identifiers_in_function);

    if (routinedeclaration->returnInRoutine) {
        string actual_type = check_ReturnInRoutine(routinedeclaration->returnInRoutine, declared_identifiers_in_function);
        if (return_type != actual_type) {
            cout << "\n\n" << function_name << "function's return value doesn't match the return type!\n";
            exit(EXIT_FAILURE);
        }
    }

    return declared_identifiers;
}


// DanyaDone
string check_VariableDeclarations(VariableDeclarations *variabledeclarations, map<string, Identifier*> declared_identifiers, Identifier* ident) {
    string result;
    if (variabledeclarations->variabledeclaration) {
        check_VariableDeclaration(variabledeclarations->variabledeclaration, declared_identifiers, ident);
        result = variabledeclarations->variabledeclaration->name + "-" + check_Type(variabledeclarations->variabledeclaration->type, declared_identifiers);
    }
    
    if (variabledeclarations->variabledeclarations)
        result += ":" + check_VariableDeclarations(variabledeclarations->variabledeclarations, declared_identifiers, ident);
    
    return result;
}

// DanyaDone
string check_RecordType(RecordType *recordtype, map<string, Identifier*> declared_identifiers, Identifier *ident) {
    if (recordtype->variabledeclarations)
        return check_VariableDeclarations(recordtype->variabledeclarations, declared_identifiers, ident);
    else 
        return ""; // Impossible btw
}

// DanyaDone
string check_ArrayType(ArrayType *arraytype, map<string, Identifier*> declared_identifiers, bool is_param) {
    if (arraytype->expression) {
        string expr_type = check_Expression(arraytype->expression, declared_identifiers);

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

    
    return check_Type(arraytype->type, declared_identifiers, nullptr, is_param);     // check type of the array
}

/**
 * Checks type correctness and return its string representation.
 * 
 * @param type      type structure containing type informationL is it array, record, primitive or user-defined
 * @param is_param  is type declared for routine parameter (false by default)
 * @return          string representation of given type (if it's a custom type, its actual type is returned)
 * DanyaDone
 */
string check_Type(Type *type, map<string, Identifier*> declared_identifiers, Identifier* ident /*= nullptr*/, bool is_param/*=false*/) {
    string user_type;
    if (type->arraytype) {
        return "array:" + check_ArrayType(type->arraytype, declared_identifiers, is_param);
    }
    else if (type->primitivetype)
        return type->primitivetype->isint     ? "integer" :
               type->primitivetype->isreal    ? "real"    :
               type->primitivetype->isboolean ? "boolean" : "IMPOSSIBLE";
    else if (type->recordtype) {
        return "record:" + check_RecordType(type->recordtype, declared_identifiers, ident);
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
map<string, Identifier*> check_TypeDeclaration(TypeDeclaration *typedeclaration, map<string, Identifier*> declared_identifiers) {
    string name = typedeclaration->name;
    transform(name.begin(), name.end(), name.begin(), ::tolower);

    // check if type was already declared
    if (declared_identifiers[name]) {
        cout << "\n######\nERROR! Type is primitive or already declared: '" << typedeclaration->name << "'\n######\n";
        exit(EXIT_FAILURE);
    }

    // check actual type and get its string representation
    string type = check_Type(typedeclaration->type, declared_identifiers);

    declared_identifiers[name] = new Identifier(TYPE, type);

    return declared_identifiers;
}

// DanyaDone
string check_InitialValue(InitialValue *initialvalue, map<string, Identifier* > declared_identifiers) {
    if (initialvalue->expression)
        return check_Expression(initialvalue->expression, declared_identifiers);
    else
        return ""; // Impossible btw
}

// DanyaDone
map<string, Identifier*> check_VariableDeclaration(VariableDeclaration *variabledeclaration, map<string, Identifier* > declared_identifiers, Identifier *parent /*= nullptr*/) {
    // firstly, checking whether variable was already declared
    if (declared_identifiers[variabledeclaration->name] && !declared_identifiers[variabledeclaration->name]->can_redeclare) {
        cout << "\n\nVariable " << variabledeclaration->name << " already declared!\n";
        exit(EXIT_FAILURE);
    }

    string user_type;

    Identifier *new_identifier = new Identifier(VARIABLE, user_type);

    //getting var type
    if (variabledeclaration->type) {
        user_type = check_Type(variabledeclaration->type, declared_identifiers, new_identifier);
        new_identifier->value_type = user_type;
    }
    // type is setting by the value of expression
    else if (variabledeclaration->expression) { // var b is 5
        user_type = check_Expression(variabledeclaration->expression, declared_identifiers);
        declared_identifiers[variabledeclaration->name] = new Identifier("Variable", user_type);
        return declared_identifiers;
    }
    else {
        cout << "Something is defenitely wrong in declaration of this variable: " << variabledeclaration->name << "\n";
        exit(EXIT_FAILURE);
    }

    // getting initial value
    if (variabledeclaration->initialvalue) {
        if (variabledeclaration->initialvalue) { // var b: Integer is 5, TODO: test this: var b : array[12] is arr2
            string actual_type = check_InitialValue(variabledeclaration->initialvalue, declared_identifiers);
            if (actual_type == user_type || 
            (user_type == "real" && actual_type == "integer") ||
            (user_type == "boolean" && actual_type == "integer")) {
                if (parent) {// Only in case of record declaration
                    parent->subidentifiers[variabledeclaration->name] = new_identifier;
                }
                else
                    declared_identifiers[variabledeclaration->name] = new_identifier; // we will not check sizes of arrays here. Lets do it in runtime
            }
            else {
                cout << "\n\nType error!\n";
                exit(EXIT_FAILURE);
            }
        }
        else { // case of initialization without initial value:  var b : Integer
            if (parent) {// Only in case of record declaration
                parent->subidentifiers[variabledeclaration->name] = new_identifier;
            }
            else
                declared_identifiers[variabledeclaration->name] = new_identifier;
        }
    }
    
    return declared_identifiers;
}

// DanyaDone
map<string, Identifier*> check_SimpleDeclaration(SimpleDeclaration *simpleDeclaration, map<string, Identifier*> declared_identifiers) {
    if (simpleDeclaration->variabledeclaration) {
        declared_identifiers = check_VariableDeclaration(simpleDeclaration->variabledeclaration, declared_identifiers);
    }
    if (simpleDeclaration->typedeclaration) {
        declared_identifiers = check_TypeDeclaration(simpleDeclaration->typedeclaration, declared_identifiers);
    }
    return declared_identifiers;
}

// DanyaDone
map<string, Identifier*> check_Declaration(Declaration *declaration, map<string, Identifier*> declared_identifiers) {
    if (declaration->simpledeclaration) {
        declared_identifiers = check_SimpleDeclaration(declaration->simpledeclaration, declared_identifiers);
    }
    if (declaration->routinedeclaration){
        declared_identifiers = check_RoutineDeclaration(declaration->routinedeclaration, declared_identifiers);
    }
    return declared_identifiers;
}

// DanyaDone
void check_Program(Program *program, map<string, Identifier*> declared_identifiers) {
    if (program->declaration)
        declared_identifiers = check_Declaration(program->declaration, declared_identifiers);

    if (program->program)
        check_Program(program->program, declared_identifiers);
}