CC = g++
CC_FLAGS = -Wall -Werror -ansi -pedantic

all:
	rm -rf ./bin
	mkdir -p ./bin
	g++ $(CC_FLAGS) ./src/*.cpp -o /bin/rshell
rshell:
	rm -rf ./bin
	mkdir -p ./bin
	g++ $(CC_FLAGS)  ./src/*.cpp -o /bin/rshell
clean:
	rm -rf ./bin
