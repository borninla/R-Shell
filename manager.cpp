#include "manager.h"

void Manager::run()
{
    string userInput;

    while(userInput != "exit")
    {
        cout << "$ ";
        getline(cin, userInput);

        StrTokenizer stk(userInput);
        Token t;

        while(!stk.done())
        {
            stk >> t;
            tokens.push(t);
        }

    }
}