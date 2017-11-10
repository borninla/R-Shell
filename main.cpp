#include <iostream>
#include "manager.h"
#include "shunting_yard.h"

using namespace std;

int main()
{
//    Manager m;
//    m.run();

    ShuntingYard sy("Ryan || Andrew && Sean && Alex || Ross || Michael");
    cout << sy.getReversePolish() << endl;

    return 0;
};