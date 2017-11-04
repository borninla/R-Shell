#include <iostream>
#include <string>
#include "delim.h"
#include <cstdlib>

using namespace std;

void testDelim() {

    string str = "man cat; man man";
    str += '\0';
    char * c = new char[str.size() + 1];
    for (unsigned int i = 0; i < str.size() + 1; i++) {
        c[i] = str[i];
    }
//    c[str.size()] = '\0';


    Delim d(c, ';');

    while (!d.done()) {

        string printMe;
        d >> printMe;
        cout << printMe << endl;
    }

//    d.dumpToConsole();
}

int main(/*int argc, char *argv[]*/)
{
    cout << "Hello World!" << endl;

    testDelim();


    return EXIT_SUCCESS;
}
