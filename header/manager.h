#ifndef MANAGER_H
#define MANAGER_H


#include <iostream>
#include <stack>
#include <queue>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <climits>
#include <cassert>
#include <sys/wait.h> //uncomment for hammer

#include "token.h"
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
    void execute(string command);
    void parse(char *line, char **command);
    //void populateQueue();
    bool _shouldExecute(string str, bool isFirstToken);
    void evaluate(string binExpression);
    void evalPostFix(queue<string>& string_postfix_queue);

    bool isFirstToken;
    bool wasSuccess;
    stack<char*> tokens;

};


#endif //MANAGER_H
