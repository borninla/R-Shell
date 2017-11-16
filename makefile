CC = g++
CC_FLAGS = -Wall -Werror -ansi -pedantic

all:
        rm -rf ./bin
        mkdir -p ./bin
        g++ -Wall -Werror -ansi pedantic ./src/main.cpp -o .bin/rshell
rshell:
        rm -rf ./bin
        mkdir -p ./bin
        g++ -Wall -Werror -ansi pedantic ./src/main.cpp -o .bin/rshell
clean:
        rm -rf ./bin