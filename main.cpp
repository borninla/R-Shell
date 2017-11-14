#include <iostream>
#include "manager.h"
#include "shunting_yard.h"
#include "util.h"

using namespace std;

int main()
{
//    Manager m;
//    m.run();

//    ShuntingYard sy("a || b && c && d || e || f");
//    ShuntingYard sy("echo hello || git status && ls -a && cat hi || echo dude || git push");
//    ShuntingYard sy("echo a || echo b && echo c && echo d || echo e || echo f");

    queue<string> my_queue = returnParsedData("echo hello || git status && ls -a && cat hi || echo dude || git push");

    return 0;
};