## CC_Project 

#### Compiler for Imperative language for .Net target platform implemented by Irek Nazmiev, Danil Kabirov, Dmitriy Kochetov and Aygul Malikova
##### Frontend - C++, Backend - C#

Firstly, specify the input program according to the grammar described in ![](Project%20I.pdf) in input.txt file. For example:
```
var b : Integer is 6
routine main() is
    var c : Real is 8.7
    if (b < c) then
        b := 10
    end
end
```

To run the frontend part of the compiler (Lexical, Syntax and Semantic parts) run the following command:

```
sh run.sh
```

You will see the following output:
```
Lexical Analyzer
======###========

var | keyword
b | identifier
: | operator
Integer | identifier
is | keyword
6 | integer literal
routine | keyword
main | identifier
( | punctuator
) | punctuator
is | keyword
var | keyword
c | identifier
: | operator
Real | identifier
is | keyword
8.7 | floating literal
if | keyword
( | punctuator
b | identifier
< | operator
c | identifier
) | punctuator
then | keyword
b | identifier
:= | operator
10 | integer literal
end | keyword
end | keyword

```

```
AST
======###========

└──Program
   ├──Declaration
   │  └──SimpleDeclaration
   │     └──VariableDeclaration (b)
   │        ├──Type (Integer)
   │        └──InitialValue
   │           └──Expression
   │              └──Relation
   │                 └──Simple
   │                    └──Factor
   │                       └──Summand
   │                          └──Primary (integer, 6.000000, not_is_not)
   └──Program
      └──Declaration
         └──RoutineDeclaration (main)
            └──BodyInRoutineDeclaration
               └──Body
                  ├──SimpleDeclaration
                  │  └──VariableDeclaration (c)
                  │     ├──Type (Real)
                  │     └──InitialValue
                  │        └──Expression
                  │           └──Relation
                  │              └──Simple
                  │                 └──Factor
                  │                    └──Summand
                  │                       └──Primary (real, 8.700000, not_is_not)
                  └──Body
                     └──Statement
                        └──IfStatement
                           ├──Expression
                           │  └──Relation
                           │     └──Simple
                           │        └──Factor
                           │           └──Summand
                           │              └──Expression
                           │                 └──Relation
                           │                    ├──Simple
                           │                    │  └──Factor
                           │                    │     └──Summand
                           │                    │        └──Primary (, 0.000000, not_is_not)
                           │                    │           └──ModifiablePrimary (b)
                           │                    └──ComparisonInRelation
                           │                       ├──ComparisonOperator (<)
                           │                       └──Simple
                           │                          └──Factor
                           │                             └──Summand
                           │                                └──Primary (, 0.000000, not_is_not)
                           │                                   └──ModifiablePrimary (c)
                           └──Body
                              └──Statement
                                 └──Assignment
                                    ├──ModifiablePrimary (b)
                                    └──Expression
                                       └──Relation
                                          └──Simple
                                             └──Factor
                                                └──Summand
                                                   └──Primary (integer, 10.000000, not_is_not)

```

```
Semantic Analyzer
======###========

Everything is correct
```
