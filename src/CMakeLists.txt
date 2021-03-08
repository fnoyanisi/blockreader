project(blockreader_source)

set(BLOCKREADER_SRC
        Environment.cc Environment.h
        Symbol.cc Symbol.h
        Parser.cc Parser.h
        Lexer.cc Lexer.h
        Type.h
        Exception.h
        Main.cc)

add_executable(blockreader ${BLOCKREADER_SRC})