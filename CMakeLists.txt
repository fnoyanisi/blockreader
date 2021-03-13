cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(blockreader
        VERSION 1.0
        DESCRIPTION "Reads program blocks and generates symbo tables"
        LANGUAGES CXX)

add_subdirectory(src)