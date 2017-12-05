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
#include <vector>
#include <sys/types.h>
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
    bool shouldExecute(vector<Token> expr);
    void evaluate(vector<Token> binExpression);
    void evalPostFix(queue<Token>& string_postfix_queue);
    queue<Token> combineCommands(queue<Token>& old_token_queue);

    bool wasSuccess;
};


#endif //MANAGER_H
