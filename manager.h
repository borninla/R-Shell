#ifndef MANAGER_H
#define MANAGER_H


#include <iostream>
#include <stack>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <climits>
//#include <wait.h> //uncomment for hammer
#include "delim.h"
#include "util.h"
#include "shunting_yard.h"

using namespace std;

class Manager
{
public:
    void run();
private:
    void execute(char **command);
    void parse(char *line, char **command);
    //void populateQueue();
    bool _shouldExecute(string str, bool isFirstToken);
    void evaluate(string binExpression);
    void evalPostFix(queue<string> postfix_queue);

    bool isFirstToken;
    bool wasSuccess;
    stack<char*> tokens;

};


#endif //MANAGER_H
