#include <iostream>
#include "manager.h"
#include "shunting_yard.h"

using namespace std;

int main()
{
    //Manager m;
    //m.run();

    ShuntingYard sy("echo hello && git status || ls -a");
    sy.run();

    return 0;
};
