cmake_minimum_required(VERSION 3.8)
project(rshell_the_werbenjagermanjensens)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/delim.cpp
        header/delim.h
        src/main.cpp
        src/manager.cpp
        header/manager.h
        src/util.cpp
        header/util.h
        src/shunting_yard.cpp
        header/shunting_yard.h
        src/token.cpp
        header/token.h)

add_executable(rshell_the_werbenjagermanjensens ${SOURCE_FILES})