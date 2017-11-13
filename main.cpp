#include <iostream>
#include "manager.h"
#include "shunting_yard.h"

using namespace std;

int main()
{
//    Manager m;
//    m.run();

    ShuntingYard sy("a || b && c && d || e || f");
    cout << sy.getReversePolish() << endl;

    return 0;
};