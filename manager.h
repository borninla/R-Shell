//
// Created by Andrew Lvovsky on 11/1/17.
//

#ifndef MANAGER_H
#define MANAGER_H


#include <iostream>
#include <queue>
#include "token.h"

using namespace std;

class Manager
{
public:
    Manager();
    void run();
private:
    queue<Token*> prompt;
};


#endif //MANAGER_H
