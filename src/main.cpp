#include <iostream>
#include "../header/manager.h"
#include "../header/shunting_yard.h"
#include "../header/util.h"

using namespace std;

void testXOR() {

    bool val1 = false;
    bool val2 = false;

    cout << "Testing XOR for values " << val1 << " and " << val2 << ":" << endl
         << "val1 XOR val2 = " << (val1 xor val2) << endl;
}

int main()
{
    Manager m;
    m.run();

    return 0;
}