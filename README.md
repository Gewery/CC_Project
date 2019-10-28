# CC_Project

Structure:

```
.
├── README.md
├── input.txt
├── lex.cpp
├── run.sh
└── yacc.y
```

run.sh does the following:

> Process the yacc grammar file:\
> `yacc.y` -> (bison) -> `yacc.cpp`, `yacc.hpp`
>
> Compile and link the two C language source files:\
> `yacc.cpp`, `lex.cpp`  -> `parser`
>
> Run the program\
> `./parser input.txt`

Input (input.txt):
```
var VAR1 is 1 <= 2
```

Output:
```
===== Tree =====

└──123
    └──123
        ├──123
        ├──123
        │   └──123
        │       └──123
        └──123
            └──123
                ├──123
                └──123
```

