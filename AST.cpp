#include "Tree.h"
#include <string>

extern void yyerror(char const *s);

struct Program *Program(struct Declaration *Declaration,
                        struct Program *Program) {
    struct Program *node = malloc(sizeof(struct Program));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Declaration = Declaration;
    node->Program = Program;
    return node;
}

struct Declaration *Declaration(struct SimpleDeclaration *SimpleDeclaration,
                                struct RoutineDeclaration *RoutineDeclaration) {
    struct Declaration *node = malloc(sizeof(struct Declaration));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->SimpleDeclaration = SimpleDeclaration;
    node->RoutineDeclaration = RoutineDeclaration;
    return node;
}

struct SimpleDeclaration *SimpleDeclaration(struct VariableDeclaration *VariableDeclaration,
                                            struct TypeDeclaration *TypeDeclaration) {
    struct SimpleDeclaration *node = malloc(sizeof(struct SimpleDeclaration));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->VariableDeclaration = VariableDeclaration;
    node->TypeDeclaration = TypeDeclaration;
    return node;
}

struct VariableDeclaration *VariableDeclaration(string name,
                                                struct Type *Type,
                                                struct InitialValue *InitialValue,
                                                struct Expression *Expression) {
    struct VariableDeclaration *node = malloc(sizeof(struct VariableDeclaration));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->name = name;
    node->Type = Type;
    node->InitialValue = InitialValue;
    node->Expression = Expression;
    return node;
}

struct InitialValue *InitialValue(struct Expression *Expression) {
    struct InitialValue *node = malloc(sizeof(struct InitialValue));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Expression = Expression;
    return node;
}

struct TypeDeclaration *TypeDeclaration(string name,
                                        struct Type *Type) {
    struct TypeDeclaration *node = malloc(sizeof(struct TypeDeclaration));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->name = name;
    node->Type = Type;
    return node;
}

struct Type *Type(struct PrimitiveType *PrimitiveType,
                  struct ArrayType *ArrayType,
                  struct RecordType *RecordType,
                  string name) {
    struct Type *node = malloc(sizeof(struct Type));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }

    node->PrimitiveType = PrimitiveType;
    node->ArrayType = ArrayType;
    node->RecordType = v;
    node->name = name;
    return node;
}

struct PrimitiveType *PrimitiveType(bool isInt, bool isReal, bool isBoolean) {
    struct PrimitiveType *node = malloc(sizeof(struct PrimitiveType));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }

    node->isInt = isInt;
    node->isReal = isReal;
    node->isBoolean = isBoolean;
    return node;
}

struct ArrayType *ArrayType(struct Expression *Expression,
                            struct Type *Type) {
    struct ArrayType *node = malloc(sizeof(struct ArrayType));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Expression = Expression;
    node->Type = Type;
    return node;
}

struct RecordType *RecordType(struct VariableDeclarations *VariableDeclarations) {
    struct RecordType *node = malloc(sizeof(struct RecordType));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->VariableDeclarations = VariableDeclarations;
    return node;
}

struct VariableDeclarations *VariableDeclarations(struct VariableDeclaration *VariableDeclaration,
                                                  struct VariableDeclarations *VariableDeclarations) {
    struct VariableDeclarations *node = malloc(sizeof(struct VariableDeclarations));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->VariableDeclaration = VariableDeclaration;
    node->VariableDeclarations = VariableDeclarations;
    return node;
}

struct RoutineDeclaration *RoutineDeclaration(string name,
                                              struct Parameters *Parameters,
                                              struct TypeInRoutineDeclaration *TypeInRoutineDeclaration,
                                              struct BodyInRoutineDeclaration *BodyInRoutineDeclaration) {
    struct RoutineDeclaration *node = malloc(sizeof(struct RoutineDeclaration));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->name = name;
    node->Parameters = Parameters;
    node->TypeInRoutineDeclaration = TypeInRoutineDeclaration;
    node->BodyInRoutineDeclaration = BodyInRoutineDeclaration;
    return node;
}

struct Parameters *Parameters(struct ParameterDeclaration *ParameterDeclaration,
                              struct ParametersDeclaration *ParametersDeclaration) {
    struct Parameters *node = malloc(sizeof(struct Parameters));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->ParameterDeclaration = ParameterDeclaration;
    node->ParametersDeclaration = ParametersDeclaration;
    return node;
}

struct ParameterDeclaration *ParameterDeclaration(string name,
                                                  struct Type *Type) {
    struct ParameterDeclaration *node = malloc(sizeof(struct ParameterDeclaration));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->name = name;
    node->Type = Type;
    return node;
}


struct ParametersDeclaration *ParametersDeclaration(struct ParameterDeclaration *ParameterDeclaration,
                                                    struct ParametersDeclaration *ParametersDeclaration) {
    struct ParametersDeclaration *node = malloc(sizeof(struct ParametersDeclaration));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->ParameterDeclaration = ParameterDeclaration;
    node->ParametersDeclaration = ParametersDeclaration;
    return node;
}

struct TypeInRoutineDeclaration *TypeInRoutineDeclaration(struct Type *Type) {
    struct TypeInRoutineDeclaration *node = malloc(sizeof(struct TypeInRoutineDeclaration));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Type = Type;
    return node;
}

struct BodyInRoutineDeclaration *BodyInRoutineDeclaration(struct Body *Body) {
    struct BodyInRoutineDeclaration *node = malloc(sizeof(struct BodyInRoutineDeclaration));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Body = Body;
    return node;
}

struct Body *Body(struct SimpleDeclaration *SimpleDeclaration,
                  struct Statement *Statement,
                  struct Body *Body) {
    struct Body *node = malloc(sizeof(struct Body));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->SimpleDeclaration = SimpleDeclaration;
    node->Statement = Statement;
    node->Body = Body;
    return node;
}

struct Statement *Statement(struct Assignment *Assignment,
                            struct RoutineCall *RoutineCall,
                            struct WhileLoop *WhileLoop,
                            struct ForLoop *ForLoop,
                            struct IfStatement *IfStatement) {
    struct Statement *node = malloc(sizeof(struct Statement));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Assignment = Assignment;
    node->RoutineCall = RoutineCall;
    node->WhileLoop = WhileLoop;
    node->ForLoop = ForLoop;
    node->IfStatement = IfStatement;
    return node;
}

struct Assignment *Assignment(struct ModifiablePrimary *ModifiablePrimary,
                              struct Expression *Expression) {
    struct Assignment *node = malloc(sizeof(struct Assignment));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->ModifiablePrimary = ModifiablePrimary;
    node->Expression = Expression;
    return node;
}

struct RoutineCall *RoutineCall(string name,
                                struct ExpressionInRoutineCall *ExpressionInRoutineCall) {
    struct RoutineCall *node = malloc(sizeof(struct RoutineCall));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->name = name;
    node->ExpressionInRoutineCall = ExpressionInRoutineCall;
    return node;
}


struct ExpressionInRoutineCall *ExpressionInRoutineCall(struct Expression *Expression,
                                                        struct ExpressionsInRoutineCall *ExpressionsInRoutineCall) {
    struct ExpressionInRoutineCall *node = malloc(sizeof(struct ExpressionInRoutineCall));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Expression = Expression;
    node->ExpressionsInRoutineCall = ExpressionsInRoutineCall;
    return node;
}

struct ExpressionsInRoutineCall *ExpressionsInRoutineCall(struct Expression *Expression,
                                                          struct ExpressionInRoutineCall *ExpressionInRoutineCall) {
    struct ExpressionsInRoutineCall *node = malloc(sizeof(struct ExpressionsInRoutineCall));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Expression = Expression;
    node->ExpressionInRoutineCall = ExpressionInRoutineCall;
    return node;
}


struct WhileLoop *WhileLoop(struct Expression *Expression,
                            struct Body *Body) {
    struct WhileLoop *node = malloc(sizeof(struct WhileLoop));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Expression = Expression;
    node->Body = Body;
    return node;
}

struct ForLoop *ForLoop(string name,
                        struct Reverse *Reverse,
                        struct Range *Range,
                        struct Body *Body) {
    struct ForLoop *node = malloc(sizeof(struct ForLoop));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->name = name;
    node->Reverse = Reverse;
    node->Range = Range;
    node->Body = Body;
    return node;
}

struct Range *Range(struct Expression *Expression1,
                    struct Expression *Expression2) {
    struct Range *node = malloc(sizeof(struct Range));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Expression = Expression1;
    node->Expression2 = Expression2;
    return node;
}

struct Reverse *Reverse(bool isReverse) {
    struct Reverse *node = malloc(sizeof(struct Reverse));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->isReverse = isReverse;
    return node;
}

struct IfStatement *IfStatement(struct Expression *Expression1,
                                struct Body *Body,
                                struct ElseInIfStatement *ElseInIfStatement) {
    struct IfStatement *node = malloc(sizeof(struct IfStatement));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Expression = Expression;
    node->Body = Body;
    node->ElseInIfStatement = ElseInIfStatement;
    return node;
}

struct ElseInIfStatement *ElseInIfStatement(struct Body *Body) {
    struct ElseInIfStatement *node = malloc(sizeof(struct ElseInIfStatement));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Body = Body;
    return node;
}

struct Expression *Expression(struct Relation *Relation,
                              struct MultipleRelationsInExpression *MultipleRelationsInExpression) {
    struct Expression *node = malloc(sizeof(struct Expression));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Relation = Relation;
    node->MultipleRelationsInExpression = MultipleRelationsInExpression;
    return node;
}

struct MultipleRelationsInExpression *MultipleRelationsInExpression(struct LogicalOperator *LogicalOperator,
                                                                    struct Relation *Relation,
                                                                    struct MultipleRelationsInExpression *MultipleRelationsInExpression) {
    struct MultipleRelationsInExpression *node = malloc(sizeof(struct MultipleRelationsInExpression));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->LogicalOperator = LogicalOperator;
    node->Relation = Relation;
    node->MultipleRelationsInExpression = MultipleRelationsInExpression;
    return node;
}

struct LogicalOperator *LogicalOperator(string op) {
    struct LogicalOperator *node = malloc(sizeof(struct LogicalOperator));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->op = op;
    return node;
}

struct Relation *Relation(struct Simple *Simple,
                          struct ComparisonInRelation *ComparisonInRelation) {
    struct Relation *node = malloc(sizeof(struct Relation));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Simple = Simple;
    node->ComparisonInRelation = ComparisonInRelation;
    return node;
}


struct ComparisonInRelation *ComparisonInRelation(struct ComparisonOperator *ComparisonOperator,
                                                  struct Simple *Simple) {
    struct ComparisonInRelation *node = malloc(sizeof(struct ComparisonInRelation));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->ComparisonOperator = ComparisonOperator;
    node->Simple = Simple;
    return node;
}

struct ComparisonOperator *ComparisonOperator(string op) {
    struct ComparisonOperator *node = malloc(sizeof(struct ComparisonOperator));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->op = op;
    return node;
}

struct Simple *Simple(struct Factor *Factor,
                      struct Factors *Factors) {
    struct Simple *node = malloc(sizeof(struct Simple));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Factor = Factor;
    node->Factors = Factors;
    return node;
}

struct Factors *Factors(struct SimpleOperator *SimpleOperator,
                        struct Factor *Factor,
                        struct Factors *Factors) {
    struct Factors *node = malloc(sizeof(struct Factors));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->SimpleOperator = SimpleOperator;
    node->Factor = Factor;
    node->Factors = Factors;
    return node;
}

struct SimpleOperator *SimpleOperator(string op) {
    struct SimpleOperator *node = malloc(sizeof(struct SimpleOperator));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->op = op;
    return node;
}

struct Factor *Factor(struct Summand *Summand,
                      struct Summands *Summands) {
    struct Factor *node = malloc(sizeof(struct Factor));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Summand = Summand;
    node->Summands = Summands;
    return node;
}


struct Summands *Summands(struct Sign *Sign,
                          struct Summand *Summand,
                          struct Summands *Summands) {
    struct Summands *node = malloc(sizeof(struct Summands));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Sign = Sign;
    node->Summand = Summand;
    node->Summands = Summands;
    return node;
}

struct Summand *Summand(struct Primary *Primary,
                        struct Expression *Expression) {
    struct Summand *node = malloc(sizeof(struct Summand));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->Primary = Primary;
    node->Expression = Expression;
    return node;
}

//TODO Primary

struct Sign *Sign(string op) {
    struct Sign *node = malloc(sizeof(struct Sign));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->op = op;
    return node;
}

struct ModifiablePrimary *ModifiablePrimary(string name,
                                            struct Identifiers *Identifiers) {
    struct ModifiablePrimary *node = malloc(sizeof(struct ModifiablePrimary));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->name = name;
    node->Identifiers = Identifiers;
    return node;
}

struct Identifiers *Identifiers(string name,
                                struct Expression *Expression,
                                struct Identifiers *Identifiers) {
    struct Identifiers *node = malloc(sizeof(struct Identifiers));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->name = name;
    node->Expression = Expression;
    node->Identifiers = Identifiers;
    return node;
}





