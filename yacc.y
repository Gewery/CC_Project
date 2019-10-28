Program : { SimpleDeclaration | RoutineDeclaration }
SimpleDeclaration
 : VariableDeclaration | TypeDeclaration
VariableDeclaration
 : var Identifier : Type [ is Expression ]
 | var Identifier is Expression
TypeDeclaration
 : type Identifier is Type
RoutineDeclaration
 : routine Identifier [ Parameters ] [ : Type ]
 [ is Body end ]
Parameters : ( ParameterDeclaration { , ParameterDeclaration } )
ParameterDeclaration
 : Identifier : Identifier
Type : PrimitiveType | ArrayType | RecordType | Identifier
PrimitiveType: integer | real | boolean
RecordType : record { VariableDeclaration } end
ArrayType : array [ Expression ] Type
Body : { SimpleDeclaration | Statement }
Statement : Assignment | RoutineCall | WhileLoop | ForLoop
 | ForeachLoop | IfStatement
Assignment : ModifiablePrimary := Expression
RoutineCall : Identifier [ ( Expression { , Expression } ) ]
WhileLoop : while Expression loop Body end
ForLoop : for Identifier Range loop Body end
Range : in [ reverse ] Expression .. Expression
ForeachLoop : foreach Identifier from ModifiablePrimary loop Body end
IfStatement : if Expression then Body [ else Body ] end
Expression : Relation { ( and | or | xor ) Relation }
Relation : Simple [ ( < | <= | > | >= | = | /= ) Simple ]
Simple : Factor { ( * | / | % ) Factor }
Factor : Summand { ( + | - ) Summand }
Summand : Primary | ( Expression )
Primary : IntegralLiteral | RealLiteral | true | false
 | ModifiablePrimary
ModifiablePrimary
 : Identifier { . Identifier | [ Expression ]
 | ( Expression { , Expression } ) ]