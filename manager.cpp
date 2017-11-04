#include "manager.h"

void Manager::run()
{
    char line[1024];
    char *command[64];

    while (true)
    {
        cout << "$ ";
        cin.getline(line, 1024);
        cout << endl;

        //line has all words including whitespace
        parse(line, command);
        //now, command has all words, tokenized using whitespace

        if (strcmp(command[0], "exit") == 0)
            exit(0);
        execute(command);

        memset(command, 0, sizeof(command));    // clears c-string to avoid later command interference
    }
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
