# blockreader
Reads program blocks and generates appropriate symbol table entries.
This is a simple implementation of the program snippet given the Chapter2 of Compilers Principles Techniques and Tools, 2nd Edition (aka the Dragon Book).

An input of the form
```
{ int x; char y; { bool y; x; y;} x; y;}
```
is converted into
```
{{x:int; y:bool; } x:int; y:char; }
```

The input gives declaration of some symbols and demostrates their usage in (sometimes nested) program blocks. The output gives the type of particular symbol within each program block.
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