#ifndef MANAGER_H
#define MANAGER_H


#include <iostream>
#include <queue>
#include <unistd.h>

using namespace std;

class Manager
{
public:
    void run();
private:
    void execute(char **command);
    void parse(char *line, char **command);

    bool wasSuccess;
    queue<string> tokens;

};


#endif //MANAGER_H
