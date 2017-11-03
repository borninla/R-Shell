#ifndef MANAGER_H
#define MANAGER_H


#include <iostream>
#include <queue>
#include <unistd.h>
#include "token.h"
#include "str_tokenizer.h"

using namespace std;

class Manager
{
public:
    void run();
private:
    void execute(char **command);
    void parse(char *line, char **command);

    bool wasSuccess;
    queue<Token> tokens;

};


#endif //MANAGER_H
