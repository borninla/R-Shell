#include "../header/manager.h"

void Manager::run() {

    char line[1024];

    while(true)
    {
        memset(line, 0, sizeof(line));  //clears line for future input

        cout << "$ ";
        cin.getline(line, 1024);
        line[cin.gcount()] = '\0';  //null-terminated
        cout << endl;

        Token thisLine;
        queue<Token> dd_token_queue, main_token_queue;
        Delim d(line, ';', false, false);  //Constructor delimits line

        while(!d.done())
        {
            d >> thisLine;

            Delim dd(thisLine.toString(), ' ', true, true);
            dd.compressTokens();
            dd.checkFlagsAndReinitStatus();

            while(!dd.done())
            {
                Token thisToken;
                dd >> thisToken;

                dd_token_queue.push(thisToken);
            }

            //main_token_queue = combineCommands(dd_token_queue);

            if(!parenthesisChecker(dd_token_queue))
            {
                cerr << "ERROR: Uneven amount of parenthesis" << endl;
                continue;
            }

            ShuntingYard sy(dd_token_queue);
            queue<Token> evalQueue = sy.getReversePolish();
            evalPostFix(evalQueue);

            clearQueue(dd_token_queue);

            cout << endl;
        }
    }
}

void Manager::execute(char **command)
{
    pid_t process_id;
    int status;

    if (equals(*command, "exit", false)){

        //cerr << "Exiting!!" << endl;
        exit(0);
    }

    //cerr << "Would be running execute() here!!" << endl;
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

        if(WEXITSTATUS(status)) //if it wasn't successful
            wasSuccess = false;
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
    while (*line != '\0')  //While you're not at the end of the cstring
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
void Manager::evalPostFix(queue<Token>& token_postfix_queue)
{
    stack<Token> token_eval_stack;
    vector<Token> vectorToEval;

    while(!token_postfix_queue.empty())
    {
        if (token_postfix_queue.front().getStatus() != Token::connector)
        {
            token_eval_stack.push(token_postfix_queue.front());
            token_postfix_queue.pop();
        }
        else //if token is an operator
        {
            //Prepare the binary expression

            Token op2 = token_eval_stack.top();
            token_eval_stack.pop();

            Token connector = token_postfix_queue.front();
            token_postfix_queue.pop();

            Token op1 = token_eval_stack.top();
            token_eval_stack.pop();

            // [command] [connector] [command]
            vectorToEval.push_back(op1);
            vectorToEval.push_back(connector);
            vectorToEval.push_back(op2);

            //Evaluate the binary expression!
            evaluate(vectorToEval); //updates wasSuccess
            vectorToEval.clear();   //clears vector for next bin expression

            //Push result onto the stack
            if (wasSuccess) {
                Token t("", Token::successfulCmd);
                token_eval_stack.push(t);
            } else {
                Token t("", Token::failedCmd);
                token_eval_stack.push(t);
            }
        }
    }

    if (token_eval_stack.size() == 1 && !token_eval_stack.top().toString().empty())
    {
        ifstream path(token_eval_stack.top().toString().c_str());

        switch(token_eval_stack.top().getStatus())
        {
            case Token::notYetRunCmd:
                execute(token_eval_stack.top().toString());
                break;
            case Token::testE:

                if(path.good())
                    cout << "(True)" << endl;
                else
                    cout << "(False)" << endl;

                break;
            case Token::testD:

                if(isThisADirectory(token_eval_stack.top().toString()))
                    cout << "(True)" << endl;
                else
                    cout << "(False)" << endl;

                break;
            case Token::testF:

                if(isThisAFile(token_eval_stack.top().toString()))
                    cout << "(True)" << endl;
                else
                    cout << "(False)" << endl;

                break;
            default:
                cerr << "ERROR: Incorrect token type to evaluate. Token is enum "
                     << token_eval_stack.top().getStatus() << endl;
                exit(7);
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
void Manager::evaluate(vector<Token> binExpression)
{
    assert(binExpression.size() == 3);  //first command, connector, last command

    ifstream path(binExpression[0].toString().c_str());

    switch (binExpression[0].getStatus())
    {
        case Token::notYetRunCmd:

            execute(binExpression[0].toString());

            break;
        case Token::testE:

            wasSuccess = path.good();

            if (wasSuccess)
                cout << "(True)" << endl;
            else
                cout << "(False)" << endl;

            break;
        case Token::testD:

            wasSuccess = isThisADirectory(binExpression[0].toString());

            if (wasSuccess)
                cout << "(True)" << endl;
            else
                cout << "(False)" << endl;

            break;
        case Token::testF:

            wasSuccess = isThisAFile(binExpression[0].toString());

            if (wasSuccess)
                cout << "(True)" << endl;
            else
                cout << "(False)" << endl;

            break;
        default:
            cerr << "ERROR: Incorrect token type to evaluate. Token is enum "
                 << binExpression[0].getStatus() << endl;
            exit(7);

    }

    path.close();

    binExpression[0].setStatus(wasSuccess);

    if(shouldExecute(binExpression))
        execute(binExpression[2].toString());
}

/**
 * @brief Determines if connectors permit the execution of a command
 * @param vector of Tokens of either form "<cmd> ** <cmd>" or "** <cmd>" (where ** denotes a connector, && or ||)
 * NOTE: wasSuccess must be properly updated from the previous command!!!!!!!!!!!!!
**/
bool Manager::shouldExecute(vector<Token> expr)
{

    if ( (expr.size() != 1 && expr.size() != 3) ||
         (expr[0].getStatus() != Token::notYetRunCmd && expr[2].getStatus() != Token::notYetRunCmd) )
        return false;

    return ( (expr[0].getStatus() == Token::successfulCmd && expr[1].toString() == "&&")
             || (expr[0].getStatus() == Token::failedCmd && expr[1].toString() == "||") );
}

queue<Token> Manager::combineCommands(queue<Token>& old_token_queue)
{
    queue<Token> new_token_queue;

    while(!old_token_queue.empty())
    {
        Token t("", Token::notYetRunCmd);

        if(old_token_queue.front().getStatus() == Token::notYetRunCmd)
        {
            while (old_token_queue.front().getStatus() == Token::notYetRunCmd
                   || old_token_queue.front().getStatus() == Token::quote)
            {
                t += old_token_queue.front();
                old_token_queue.pop();
            }

            new_token_queue.push(t);
        }
        else
        {
            new_token_queue.push(old_token_queue.front());
            old_token_queue.pop();
        }
    }

    return new_token_queue;
}

bool Manager::isThisADirectory(string pathname)
{
    struct stat sb;

    stat(pathname.c_str(), &sb);

    if ( access(pathname.c_str(),0) == 0 )
    {
        if (sb.st_mode & S_IFDIR) {
            wasSuccess = true;  //directory exists
            return true;
        }
        else {
            wasSuccess = false; //directory is not found
            return false;
        }
    }

    wasSuccess = false; //path is not found
    return false;
}

bool Manager::isThisAFile(string pathname)
{
    struct stat sb;

    stat(pathname.c_str(), &sb);

    if ( access(pathname.c_str(),0) == 0 )
    {
        if (sb.st_mode & S_IFREG) {
            wasSuccess = true;  //file exists
            return true;
        }
        else {
            wasSuccess = false; //file is not found
            return false;
        }
    }

    wasSuccess = false; //path is not found
    return false;
}
