project(blockreader_source)

set(BLOCKREADER_SRC
        Env.cc Env.h
        Main.cc)

add_executable(blockreader ${BLOCKREADER_SRC})