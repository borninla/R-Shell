#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H


#include <iostream>
#include <stack>
#include <queue>
#include <cstring>

#include "util.h"

using namespace std;

class ShuntingYard
{
public:
    ShuntingYard(queue<string> command);
    void run();
    queue<string> getReversePolish();
private:
    queue<string> delivery, command_queue;
    stack<string> connector_stack;
};


#endif //SHUNTING_YARD_H
