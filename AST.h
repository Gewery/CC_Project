#ifndef CC_PROJECT_AST_H
#define CC_PROJECT_AST_H

struct Program {
    struct SimpleDeclaration* SimpleDeclaration;
    struct RoutineDeclaration* RoutineDeclaration;
    struct Program* Program;
};

struct SimpleDeclaration {
    struct VariableDeclaration* VariableDeclaration;
    struct TypeDeclaration* TypeDeclaration;
};

struct VariableDeclaration {
    string name;
    struct Type* Type;
    struct InitialValue* InitialValue;
    struct Expression* Expression;
    struct VariableDeclaration* VariableDeclaration;
};

struct InitialValue {
    struct Expression* Expression;
};

struct TypeDeclaration {
    string name;
    struct Type* Type;
};







#endif //CC_PROJECT_AST_H
