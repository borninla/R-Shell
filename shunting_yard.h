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
    ShuntingYard(string command);
    void run();
    stack<string> getReversePolish();
private:
    string delivery;
    stack<string> command_stack, connector_stack;

//    const int AND_PRECEDENCE = 2;
//    const int OR_PRECEDENCE = 1;
};


#endif //SHUNTING_YARD_H
