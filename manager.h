#ifndef MANAGER_H
#define MANAGER_H


#include <iostream>
#include <queue>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <climits>
//#include <wait.h> //uncomment for hammer
#include "delim.h"
#include "util.h"

using namespace std;

class Manager
{
public:
    void run();
private:
    void execute(char **command);

/** parse
 * @brief Populates command (an array of cstrings) with cstrings delimited by whitespace
 * @param line Beginning of cstring containing all args
 * @param command array of cstrings which this function will populate
**/
    void parse(char *line, char **command);
    //void populateQueue();
    bool _shouldExecute(string str, bool isFirstToken);
    bool _isConnector(const string& str);
    string _parseUntilConnector(string& parseThis);

    bool wasSuccess;
    queue<string> tokens;

};


#endif //MANAGER_H
