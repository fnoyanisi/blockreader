# blockreader
Reads program blocks and generates appropriate symbol table entries.
This is a simple implementation of the program snippet given the Chapter2 of Compilers Principles Techniques and Tools, 2nd Edition (aka the Dragon Book).

An input of the form
```
{ Int x; Char y; { Bool y; x; y;} x; y;}
```
is converted into
```
{{ x:Int; y:Bool; } x:Int; y:Char; }
```

The input gives declaration of some symbols and demostrates their usage in (sometimes nested) program blocks. The output gives the type of particular symbol within each program block.

# The Grammar
`E` stands for the epsilon symbol, which denotes the empty set.
```
        program --> block

        block   --> '{' decls stmts '}'

        decls   --> decls decl
                 | E

        decl    --> type id ;

        stmts   --> stmts stmt
                 | E

        stmt    --> block
                 | factor ;

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