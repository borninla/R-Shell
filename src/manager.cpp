#include "../header/manager.h"

void Manager::run() {

    char line[1024];
    char *command[64];

    while (true) {

        cout << "$ ";
        cin.getline(line, 1024 + 1);
        line[cin.gcount()] = '\0';  //null-terminated
        cout << endl;

        string str;
        Delim d(line, ';');    //Constructor delimits line

        while(!d.done()) {

            bool isFirstToken = true;

            //prepare command for parse()
            memset(command, 0, sizeof(command);

            //Extract from d and prepare a char*
            d >> str;
            str = str.substr(0, str.find('#') - 1);  //handle comments
            str = trim(str);                         //handle leading and trailing whitespace
            if (str == "#")
                continue;

            ShuntingYard sy(returnParsedData(str));
            queue<string> cmdAndConnectorQueue = sy.getReversePolish();
            //following queue might need to take CompToken* instead of CompToken
            //queue<CompToken> TokenQueue = convToTokenQueue(cmdAndConnectorQueue);

            //Now, cmdAndConnectorQueue is in Reverse Polish
            //@TODO: evaluate!!


            // get stack to take tokens from package queue to evaluate postfix notation

            /*
	        VVVVV DEPRACATED VVVVV		(but don't delete until properly replaced!)
            //Execute commands (or exit)
            if (strcmp(command[0], "exit") == 0)
                exit(0);

            //if (_shouldExecute(str, isFirstToken))
            //     execute(command);
            */



            //Memory cleanup for future iterations
            memset(command, 0, sizeof(command));
            //delete [] currentToken;
            cout << endl;

            isFirstToken = false;
        }
    }
}

/**
 * @brief Determines if connectors permit the execution of a command
 * @param str String of either form "<cmd> ** <cmd>" or "** <cmd>" (where ** denotes a connector, && or ||)
 * NOTE: wasSuccess must be properly updated from the previous command!!!!!!!!!!!!!
**/
bool Manager::_shouldExecute(string str, bool isFirstToken) {

    str = trim(str);

    //if there are no connectors to test, return true
    if (str.size() < 2)
        return true;

    string firstTwo = str.substr(0, 2);

    //error check for connector being first token
    if (_isConnector(firstTwo) && isFirstToken)
    {
        cerr << "Cannot execute commands with connectors as the first token" << endl;
        assert(!_isConnector(firstTwo) || !isFirstToken);
    }

    if (!_isConnector(firstTwo))
        return true;

    //Now, we know that there is a connector present

    //Next step requires that wasSuccess has been updated!
    return (wasSuccess && firstTwo == "&&") || (!wasSuccess && firstTwo == "||");

}

void Manager::execute(char **command)
{
    pid_t process_id;
    int status;

    if (equals(*command, "exit", false))
        exit(0);


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
        while(wait(&status) != process_id); //not sure what this does yet
    }
}

void Manager::execute(string commandStr) {

    char * cStr = _copyStrToCharPtr(commandStr);
    char * cmd[64];
    memset(cmd, 0, sizeof(cmd));

    parse(cStr, cmd);

    execute(cmd);

    delete [] cStr;
}

/** parse
 * @brief Populates command (an array of cstrings) with cstrings delimited by whitespace
 * @param line Beginning of cstring containing all args
 * @param command array of cstrings which this function will populate
**/
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

/**
 * @brief Evaluates the queue (already in postfix notation)
 * @param string_postfix_queue a queue that contains an expression in postfix notation
 */
void Manager::evalPostFix(queue<string>& string_postfix_queue)
{
    stack<Token> token_eval_stack;
    string stringToEval;
    queue<Token> token_postfix_queue = stringsToTokens(string_postfix_queue);

    //wasSuccess = true;  //sets to true before going into the loop

    while(!token_postfix_queue.empty())
    {
        if(token_postfix_queue.front() != "&&" && token_postfix_queue.front() != "||")  //if not connector
        {
            token_eval_stack.push(token_postfix_queue.front());
            token_postfix_queue.pop();
        }
        else
        {
            assert(token_eval_stack.top().getStatus() == Token::notYetRunCmd);
            string op2 = token_eval_stack.top().toString();
            token_eval_stack.pop();
            string connector = token_postfix_queue.front().toString();
            token_postfix_queue.pop();
            assert(token_eval_stack.top().getStatus() == Token::notYetRunCmd);
            string op1 = token_eval_stack.top().toString();
            token_eval_stack.pop();

            stringToEval = op1 + " " + connector + " " + op2;   // [command] [connector] [command]
            //token_eval_stack.pop();

            evaluate(stringToEval);

            if(token_eval_stack.top().getStatus() != Token::successfulCmd)
            {
                //@TODO: LEFT OFF HERE
            }
        }
    }
}

/**
 * @brief Evaluates a binary expression according to rules for && and ||
 * @param binExpression - expression of the form A ** B
 *        (where A and B are individually executable commands,
 *        and where ** is connector && or ||)
 * NOTE: Modifies Manager::wasSuccess
 */
void Manager::evaluate(string binExpression) {

    queue<string> q = returnParsedData(binExpression);

    assert(q.size() == 3);  //first command, connector, last command

    string firstArg = q.front();
    q.pop();

    execute(firstArg);   //modifies wasSuccess

    string restOfExpression = toSpaceDelimitedString(q, " ");  //does not modify q

    if(_shouldExecute(restOfExpression, false)) {

        q.pop(); //get rid of connector
        execute(q.front()); //q.front() is the last command
    }
}
