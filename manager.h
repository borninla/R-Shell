#ifndef MANAGER_H
#define MANAGER_H


#include <iostream>
#include <queue>
#include "token.h"
#include "str_tokenizer.h"

using namespace std;

class Manager
{
public:
    Manager();
    void run();
private:
    queue<Token> tokens;
};


#endif //MANAGER_H
