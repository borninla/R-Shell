#include <iostream>
#include "../header/manager.h"
#include "../header/shunting_yard.h"
#include "../header/util.h"

using namespace std;

int main()
{
    Manager m;
    m.run();

    //ShuntingYard sy("a || b && c && d || e || f");
    //ShuntingYard sy("echo a || echo b && echo c && echo d || echo e || echo f");

    //queue<string> my_queue = returnParsedData("a || b && c && d || e || f");
    //ShuntingYard sy(my_queue) ;

    return 0;
}