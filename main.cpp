#include <iostream>
#include "manager.h"
#include "shunting_yard.h"

using namespace std;

int main()
{
    //Manager m;
    //m.run();

    ShuntingYard sy("echo hello || ls -a && git status");
    sy.run();

    return 0;
};
