#ifndef MANAGER_H
#define MANAGER_H


#include <iostream>
#include <queue>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
//#include <wait.h> //uncomment for hammer
//>>>>>>> 0185e0bc0d66ea43c5efd18a0b527f00db3dbde8

using namespace std;

class Manager
{
public:
    void run();
private:
    void execute(char **command);

    /**
 * @brief Populates command (an array of cstrings) with cstrings delimited by whitespace
 * @param line Beginning of cstring containing all args
 * @param command array of cstrings which this function will populate
 */
    void parse(char *line, char **command);
    void populateQueue();

    bool wasSuccess;
    queue<string> tokens;

};


#endif //MANAGER_H
