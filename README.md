## CC_Project 

#### DESCRIPTION
* Compiler for Imperative language for .Net target platform implemented by Irek Nazmiev, Danil Kabirov, Dmitriy Kochetov and Aygul Malikova
Frontend - C++, Backend - C#

* Firstly, specify the input program according to the grammar described in ![](Project%20I.pdf) in input.txt file. For example:
    ```
    var b : Integer is 6
    routine main() is
        var c : Real is 8.7
        if (b < c) then
            b := 10
        end
    end
    ```

#### RUNNING

* To run the frontend part of the compiler (Lexical, Syntax and Semantic parts) run the following command:
    ```
    sh run.sh
    ```

* Run in docker if you have compilation problems
    ```
    docker-compose up -d 
    docker exec -it cc_project_compiler_1 bash
    sh run.sh
    ```

* You will see the following output:
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
  
    ```
    {
      "name": null,
      "type": "Program",
      "value": null,
      "children": [
        {
          "name": null,
          "type": "Declaration",
          "value": null,
          "children": [
            {
              "name": null,
              "type": "SimpleDeclaration",
              "value": null,
              "children": [
                {
                  "name": "b",
                  "type": "VariableDeclaration",
                  "value": null,
                  "children": [
                    {
                      "name": "Integer",
                      "type": "type",
                      "value": null,
                      "children": []
                    },
                    {
                      "name": null,
                      "type": "InitialValue",
                      "value": null,
                      "children": [
                        {
                          "name": null,
                          "type": "Expression",
                          "value": null,
                          "children": [
                            {
                              "name": null,
                              "type": "Relation",
                              "value": null,
                              "children": [
                                {
                                  "name": null,
                                  "type": "Simple",
                                  "value": null,
                                  "children": [
                                    {
                                      "name": null,
                                      "type": "Factor",
                                      "value": null,
                                      "children": [
                                        {
                                          "name": null,
                                          "type": "Summand",
                                          "value": null,
                                          "children": [
                                            {
                                              "name": "integer",
                                              "type": "Primary",
                                              "value": "6.000000",
                                              "children": []
                                            }
                                          ]
                                        }
                                      ]
                                    }
                                  ]
                                }
                              ]
                            }
                          ]
                        }
                      ]
                    }
                  ]
                }
              ]
            }
          ]
        },
        {
          "name": null,
          "type": "Declaration",
          "value": null,
          "children": [
            {
              "name": "main",
              "type": "RoutineDeclaration",
              "value": null,
              "children": [
                {
                  "name": null,
                  "type": "BodyInRoutineDeclaration",
                  "value": null,
                  "children": [
                    {
                      "name": null,
                      "type": "Body",
                      "value": null,
                      "children": [
                        {
                          "name": null,
                          "type": "SimpleDeclaration",
                          "value": null,
                          "children": [
                            {
                              "name": "c",
                              "type": "VariableDeclaration",
                              "value": null,
                              "children": [
                                {
                                  "name": "Real",
                                  "type": "type",
                                  "value": null,
                                  "children": []
                                },
                                {
                                  "name": null,
                                  "type": "InitialValue",
                                  "value": null,
                                  "children": [
                                    {
                                      "name": null,
                                      "type": "Expression",
                                      "value": null,
                                      "children": [
                                        {
                                          "name": null,
                                          "type": "Relation",
                                          "value": null,
                                          "children": [
                                            {
                                              "name": null,
                                              "type": "Simple",
                                              "value": null,
                                              "children": [
                                                {
                                                  "name": null,
                                                  "type": "Factor",
                                                  "value": null,
                                                  "children": [
                                                    {
                                                      "name": null,
                                                      "type": "Summand",
                                                      "value": null,
                                                      "children": [
                                                        {
                                                          "name": "real",
                                                          "type": "Primary",
                                                          "value": "8.700000",
                                                          "children": []
                                                        }
                                                      ]
                                                    }
                                                  ]
                                                }
                                              ]
                                            }
                                          ]
                                        }
                                      ]
                                    }
                                  ]
                                }
                              ]
                            }
                          ]
                        },
                        {
                          "name": null,
                          "type": "Body",
                          "value": null,
                          "children": [
                            {
                              "name": null,
                              "type": "Statement",
                              "value": null,
                              "children": [
                                {
                                  "name": null,
                                  "type": "IfStatement",
                                  "value": null,
                                  "children": [
                                    {
                                      "name": null,
                                      "type": "Expression",
                                      "value": null,
                                      "children": [
                                        {
                                          "name": null,
                                          "type": "Relation",
                                          "value": null,
                                          "children": [
                                            {
                                              "name": null,
                                              "type": "Simple",
                                              "value": null,
                                              "children": [
                                                {
                                                  "name": null,
                                                  "type": "Factor",
                                                  "value": null,
                                                  "children": [
                                                    {
                                                      "name": null,
                                                      "type": "Summand",
                                                      "value": null,
                                                      "children": [
                                                        {
                                                          "name": null,
                                                          "type": "Expression",
                                                          "value": null,
                                                          "children": [
                                                            {
                                                              "name": null,
                                                              "type": "Relation",
                                                              "value": null,
                                                              "children": [
                                                                {
                                                                  "name": null,
                                                                  "type": "Simple",
                                                                  "value": null,
                                                                  "children": [
                                                                    {
                                                                      "name": null,
                                                                      "type": "Factor",
                                                                      "value": null,
                                                                      "children": [
                                                                        {
                                                                          "name": null,
                                                                          "type": "Summand",
                                                                          "value": null,
                                                                          "children": [
                                                                            {
                                                                              "name": "",
                                                                              "type": "Primary",
                                                                              "value": "0.000000",
                                                                              "children": [
                                                                                {
                                                                                  "name": null,
                                                                                  "type": "ModifiablePrimary",
                                                                                  "value": "b",
                                                                                  "children": []
                                                                                }
                                                                              ]
                                                                            }
                                                                          ]
                                                                        }
                                                                      ]
                                                                    }
                                                                  ]
                                                                },
                                                                {
                                                                  "name": null,
                                                                  "type": "ComparisonInRelation",
                                                                  "value": null,
                                                                  "children": [
                                                                    {
                                                                      "name": null,
                                                                      "type": "ComparisonOperator",
                                                                      "value": "<",
                                                                      "children": []
                                                                    },
                                                                    {
                                                                      "name": null,
                                                                      "type": "Simple",
                                                                      "value": null,
                                                                      "children": [
                                                                        {
                                                                          "name": null,
                                                                          "type": "Factor",
                                                                          "value": null,
                                                                          "children": [
                                                                            {
                                                                              "name": null,
                                                                              "type": "Summand",
                                                                              "value": null,
                                                                              "children": [
                                                                                {
                                                                                  "name": "",
                                                                                  "type": "Primary",
                                                                                  "value": "0.000000",
                                                                                  "children": [
                                                                                    {
                                                                                      "name": null,
                                                                                      "type": "ModifiablePrimary",
                                                                                      "value": "c",
                                                                                      "children": []
                                                                                    }
                                                                                  ]
                                                                                }
                                                                              ]
                                                                            }
                                                                          ]
                                                                        }
                                                                      ]
                                                                    }
                                                                  ]
                                                                }
                                                              ]
                                                            }
                                                          ]
                                                        }
                                                      ]
                                                    }
                                                  ]
                                                }
                                              ]
                                            }
                                          ]
                                        }
                                      ]
                                    },
                                    {
                                      "name": null,
                                      "type": "Body",
                                      "value": null,
                                      "children": [
                                        {
                                          "name": null,
                                          "type": "Statement",
                                          "value": null,
                                          "children": [
                                            {
                                              "name": null,
                                              "type": "Assignment",
                                              "value": null,
                                              "children": [
                                                {
                                                  "name": null,
                                                  "type": "ModifiablePrimary",
                                                  "value": "b",
                                                  "children": []
                                                },
                                                {
                                                  "name": null,
                                                  "type": "Expression",
                                                  "value": null,
                                                  "children": [
                                                    {
                                                      "name": null,
                                                      "type": "Relation",
                                                      "value": null,
                                                      "children": [
                                                        {
                                                          "name": null,
                                                          "type": "Simple",
                                                          "value": null,
                                                          "children": [
                                                            {
                                                              "name": null,
                                                              "type": "Factor",
                                                              "value": null,
                                                              "children": [
                                                                {
                                                                  "name": null,
                                                                  "type": "Summand",
                                                                  "value": null,
                                                                  "children": [
                                                                    {
                                                                      "name": "integer",
                                                                      "type": "Primary",
                                                                      "value": "10.000000",
                                                                      "children": []
                                                                    }
                                                                  ]
                                                                }
                                                              ]
                                                            }
                                                          ]
                                                        }
                                                      ]
                                                    }
                                                  ]
                                                }
                                              ]
                                            }
                                          ]
                                        }
                                      ]
                                    }
                                  ]
                                }
                              ]
                            }
                          ]
                        }
                      ]
                    }
                  ]
                }
              ]
            }
          ]
        }
      ]
    }
    ```
  
    <img src="./src/ThirdParty/architecture.jpg" width="400">
