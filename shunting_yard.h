#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include <iostream>
#include <stack>
#include <queue>

#include "util.h"

using namespace std;

class ShuntingYard
{
public:
    ShuntingYard(string command);
    void run();
private:
    string delivery;
    queue<char*> command_queue;
    stack<char*> connector_stack;

//    const int AND_PRECEDENCE = 2;
//    const int OR_PRECEDENCE = 1;
};


#endif //SHUNTING_YARD_H
