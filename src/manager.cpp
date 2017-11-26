#include "../header/manager.h"

template <typename T>
void printQueue(queue<T> printMe) {

    cout << "PopFirst <====> PopLast" << endl;
    cout << "{ ";

    while (!printMe.empty()) {

        T& printStr = printMe.front();

        cout << "\"" << printStr << "\", ";

        printMe.pop();
    }

    cout << "}";
}

void Manager::run() {

    char line[1024];
    //char *command[64];

    while (true) {

        cout << "$ ";
        cin.getline(line, 1024 + 1);
        line[cin.gcount()] = '\0';  //null-terminated
        cout << endl;

        string str;
        Delim d(line, ';');    //Constructor delimits line

        while(!d.done()) {

            bool isFirstToken = true;

            //Extract from d into str
            d >> str;
            str = str.substr(0, str.find('#') - 1);  //handle comments
            str = trim(str);                         //handle leading and trailing whitespace
            if (str == "#")
                continue;

            str = padDelim(str,'(');
            str = padDelim(str, ')');

            if(str.substr(0, 4) == "exit")
                exit(0);

            if(parenthesisChecker(str))
            {
                ShuntingYard sy(returnParsedData(str));
                queue<string> cmdAndConnectorQueue = sy.getReversePolish();
                //printQueue(cmdAndConnectorQueue); //@TODO test cmdAndConnectorQueue
                evalPostFix(cmdAndConnectorQueue);
            } else
                cerr << "ERROR: Uneven number of parenthesis" << endl;

            cout << endl;
            //isFirstToken = false;
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
        assert(!_isConnector(firstTwo) || !isFirstToken);
    }

    if (!_isConnector(firstTwo))
        return true;

    //Now, we know that there is a connector present
    //cout << "was it succ? " << wasSuccess << " " << "was it f: " << (firstTwo == "&&") << endl;
    //Next step requires that wasSuccess has been updated!
    bool runAnd = wasSuccess && (firstTwo == "&&");
    bool runOr = !wasSuccess && firstTwo == "||";

    return runAnd || runOr;

}

void Manager::execute(char **command)
{
    pid_t process_id;
    int status;
    isFirstToken = false;//Whatever the outcome, all future tokens will not be the first
    //cerr << "Setting isFirstToken = false !" << endl;

    if (equals(*command, "exit", false)){

        //cerr << "Exiting!!" << endl;
        exit(0);
    }


    if((process_id = fork()) < 0)   // if something went wrong with forking the process
    {
        //cerr << "ERROR: child process forking failed" << endl;
        //cerr << "In first block in execute()" << endl;
        exit(1);
    }
    else if (process_id == 0)       // if child process was created
    {
        //cerr << "In second block in execute()" << endl;

        if(execvp(*command, command) < 0)
        {
            cerr << "ERROR: command failed to execute()" << endl;
            wasSuccess = false;
        }
        else
        {
            //cerr << "hey I ran, wasSuccess should be true" << endl;
            wasSuccess = true;
        }
    }
    else
    {
        //cerr << "In third block in execute()" << endl;
        wasSuccess = true;
        while(wait(&status) != process_id);

        if(WEXITSTATUS(status))
            wasSuccess = false;
    } //not sure what this does yet
}

void Manager::execute(string commandStr) {

    char * cStr = _copyStrToCharPtr(commandStr);
    char * cmd[64];
    memset(cmd, 0, sizeof(cmd));

    parse(cStr, cmd);

    //cerr << "Calling execute() with \"" << commandStr << "\"" << endl;

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
//    cout << "Printing string_postfix_queue:" << endl;
//    printQueue(string_postfix_queue);
//    cout << endl
//         << "Printing token_postfix_queue:" << endl;
//    printQueue(token_postfix_queue);

    while(!token_postfix_queue.empty())
    {
        if(token_postfix_queue.front() != "&&" && token_postfix_queue.front() != "||")  //if not connector
        {
            token_eval_stack.push(token_postfix_queue.front());
            token_postfix_queue.pop();
        }
        else
        {
            //Prepare the binary expression
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

            //Evaluate the binary expression!
            evaluate(stringToEval);
        }
    }

    //@TODO: Replace with better code lol
    if (token_eval_stack.size() == 1)
    {
        execute(token_eval_stack.top().toString());
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

    bool shouldIExecute = _shouldExecute(restOfExpression, isFirstToken);

    if(shouldIExecute) {

        q.pop(); //get rid of connector
        execute(q.front()); //q.front() is the last command
    }
}

