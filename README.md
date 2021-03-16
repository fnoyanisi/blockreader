# blockreader
This is a simple compiler front-end with a lexer, a parser and a symbol table. An IR code generator is not included since the used language does not have any statements resulting in value assignments or retrieval of values from the memory.

The language has program blocks (defined by `{` and `}` symbols), identifier declarations and simple statements. Only three types of (`Int`,`Bool`, and `Char`) identifiers are supported. An input of the form
```
{ Int x; Char y; { Bool y; x; y;} x; y;}
```
is converted into the output below
```
{{ x:Int; y:Bool; } x:Int; y:Char; }
```

The input comprises of the declaration of some identifiers and demostrates their usage in (sometimes nested) program blocks. The output gives the type of particular identifier within each program block.

The code can be improved in several ways, such as adding more information into the symbol table or with more explanatory error messages or using more simple but efficient constructs.

This code is a simple implementation of the program snippet given the Chapter2 of Compilers Principles Techniques and Tools, 2nd Edition (aka the Dragon Book).

# The Grammar
`E` stands for the epsilon symbol, which denotes the empty set.
```
        program --> block

        block   --> '{' decls stmts '}'

        decls   --> decls decl | E

        decl    --> type id ;

        stmts   --> stmts stmt | E

        stmt    --> block | factor ;

        factor  --> id
```

The code uses a top-down recursive descent parser, hence we need to make a few of minor modifications to the grammar in order to eliminate left-recursion
```
        program --> block

        block   --> '{' decls stmts '}'

        decls   --> decl decls | E // changed

        decl    --> type id ';'

        type    --> 'Int' | 'Char' | 'Bool' // added

        stmts   --> stmt stmts | E // changed

        stmt    --> block | factor ';'

        factor  --> id
```
# Building the code
You need `cmake` to compile the code
```
$ git clone https://github.com/fnoyanisi/blockreader.git
$ cd blockreader
$ mkdir build
$ cd build 
$ cmake ..
$ cmake --build .
$ ./src/blockreader
```
