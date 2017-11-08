#include "manager.h"
//#include "delim.h"
//#include <string>



void Manager::run() {

    char line[1024];
    char *command[64];

    while (true) {

        isFirstToken = true;

        cout << "$ ";
        cin.getline(line, 1024 + 1);
        line[cin.gcount()] = '\0';  //null-terminated
        cout << endl;

        string str;
        Delim d(line, ';');    //Constructor delimits line

        while(!d.done() || !_isConnector(str.substr(0, 2)) ) {

            //Extract from d and prepare a char*
            if(!d.done())
            {
                d >> str;
            }

            str = str.substr(0, str.find('#') - 1);  //handle comments
            str = trim(str);                         //handle leading and trailing whitespace

            if (str == "#")
                continue;
            //char* currentLine = _copyStrToCharPtr(str);
            char* currentLine = _copyStrToCharPtr(_parseUntilConnector(str));

            memset(command, 0, sizeof(command));     //prepare command for parse()

            //@TODO connector parsing should happen here

            //currentLine is a single command, ;-delimited
            parse(currentLine, command);
            //now, command has all words, tokenized using whitespace

            //Execute commands (or exit)
            if (strcmp(command[0], "exit") == 0)
                exit(0);
            if (_shouldExecute(currentLine, isFirstToken))
                execute(command);

            //Memory cleanup for future iterations
            memset(command, 0, sizeof(command));
            delete [] currentLine;
            cout << endl;

            isFirstToken = false;
        }

/*      //outdated code
        //line has all words including whitespace
        parse(line, command);
        //now, command has all words, tokenized using whitespace

        if (strcmp(command[0], "exit") == 0)
            exit(0);
        execute(command);

        memset(command, 0, sizeof(command));    // clears c-string to avoid later command interference*/
    }
}

//@TODO
/**
 * @brief Determines if connectors permit the execution of a command
 * @param str
**/
bool Manager::_shouldExecute(string str, bool isFirstToken) {

    str = trim(str);

    //if there are no connectors to test, return true
    if (str.size() < 2)
        return true;

    string firstTwo = str.substr(0, 2);

    //error check for connector being first token
    if (_isConnector(firstTwo) && isFirstToken)
        cerr << "Cannot execute commands with connectors as the first token" << endl;

    if (!_isConnector(firstTwo))
        return true;

    //Now, we know that there is a connector present

    //Next step requires that wasSuccess has been updated!
    return (wasSuccess && firstTwo == "&&") || (!wasSuccess && firstTwo == "||");

}

bool Manager::_isConnector(const string& str) {

    return str == "&&" || str == "||";

}

/*
char* Manager::_copyStrToCharPtr(const string& str) {

    char * c = new char[str.size() + 1];

    for (size_t i = 0; i < str.size(); i++) {

        c[i] = str[i];
    }

    c[str.size()] = '\0';

    return c;
}
*/

void Manager::execute(char **command)
{
    pid_t process_id;
    int status;

    if((process_id = fork()) < 0)   // if something went wrong with forking the process
    {
        cerr << "ERROR: child process forking failed" << endl;
        exit(1);
    }
    else if (process_id == 0)       // if child process was created
    {
        if(execvp(*command, command) < 0)
        {
            cerr << "ERROR: command failed to execute" << endl;
            wasSuccess = false;
        }
        else
        {
            wasSuccess = true;
        }
    }
    else
    {
        while(wait(&status) != process_id);
    }
}

void Manager::parse(char *line, char **command)
{
    while (*line != '\0')  //While you're not at the end of the cstring,
    {

        //Replace whitespace with \0
        while (*line == ' ' || *line == '\t' || *line == '\n')
        {
            *line = '\0';
            line++;
        }

        //Save line to its place in command, and move command walker forwward
        *command = line;
        command++;

        //Advance line to the next non-whitespace character
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
        {
            line++;
        }
    }
}

string Manager::_parseUntilConnector(string& parseThis)
{
    string parsedStr;
    size_t foundAnd, foundOr, foundSmallest;

    foundAnd = parseThis.find("&&");
    foundOr = parseThis.find("||");

    if(foundAnd != UINT_MAX && foundOr != UINT_MAX && foundAnd < foundOr)
        foundSmallest = foundAnd;
    else if(foundAnd != UINT_MAX && foundOr != UINT_MAX && foundOr < foundAnd)
        foundSmallest = foundOr;
    else
        return parseThis;

    if(_isConnector(parseThis.substr(0, 2)))
    {
        parsedStr = parseThis.substr(3, parseThis.size() - 3);
        parsedStr = _parseUntilConnector(parsedStr);

        return parsedStr;
    }

    parsedStr = parseThis.substr(0, foundSmallest - 1);
    parseThis.erase(0, foundSmallest - 1);

    return parsedStr;
}
