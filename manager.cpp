#include "manager.h"

void Manager::run() {

    char line[1024];
    char *command[64];

    while (true) {

        cout << "$ ";
        cin.getline(line, 1024 + 1);
        line[cin.gcount()] = '\0';  //null-terminated
        cout << endl;

        bool isFirstToken = true;

        string str;
        Delim d(line, ';');    //Constructor delimits line

        while(!d.done()) {

            //Extract from d and prepare a char*
            d >> str;
            str = str.substr(0, str.find('#') - 1);  //handle comments
            str = trim(str);                         //handle leading and trailing whitespace
            if (str == "#")
                continue;

            ShuntingYard sy(str);

            string reversePolishString = "hey";

            char* currentToken = strtok(_copyStrToCharPtr(reversePolishString), " "); //tokenize input

            memset(command, 0, sizeof(command));     //prepare command for parse()

            //currentToken is a single command, ;-delimited
            parse(currentToken, command);
            //now, command has all words, tokenized using whitespace

            //Execute commands (or exit)
            if (strcmp(command[0], "exit") == 0)
                exit(0);
            if (_shouldExecute(str, isFirstToken))
                execute(command);

            //Memory cleanup for future iterations
            memset(command, 0, sizeof(command));
            delete [] currentToken;
            cout << endl;

            isFirstToken = false;
        }
    }
}

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
    return (wasSuccess && firstTwo == "&&")
           || (!wasSuccess && firstTwo == "||");

}

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

//char *op1, *op2, *result;
//if command
//if(strncmp(currentToken, "&&", 2) != 0 && strncmp(currentToken, "||", 2) != 0)
//tokens.push(currentToken);
//else
//{
//op2 = tokens.top();
//tokens.pop();
//op1 = tokens.top();
//tokens.pop();
//result =