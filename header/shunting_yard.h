#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H


#include <iostream>
#include <stack>
#include <queue>
#include <cstring>
#include <cassert>

#include "util.h"

using namespace std;

class ShuntingYard
{
public:
    ShuntingYard(queue<Token> command);
    void run();
    queue<Token> getReversePolish();
private:
    queue<Token> delivery, command_queue;
    stack<Token> connector_stack;
};


#endif //SHUNTING_YARD_H
