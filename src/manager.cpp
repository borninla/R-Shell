#include "../header/manager.h"

void Manager::run() {

    char line[1024];

    while(true)
    {
        cout << "$ ";
        cin.getline(line, 1024 /*+ 1*/);
        line[cin.gcount()] = '\0';  //null-terminated
        cout << endl;

        Token thisLine;
        queue<Token> dd_token_queue, main_token_queue;
        Delim d(line, ';', false);    //Constructor delimits line

        while(!d.done())
        {
            d >> thisLine;

            Delim dd(thisLine.toString(), ' ', true);

            while(!dd.done())
            {
                Token thisToken;
                dd >> thisToken;
                dd_token_queue.push(thisToken);
            }

//            @TODO: paren and comment check will be handled in delim constructor
//            delimits parenthesis to get ready for parenthesisChecker
//            str = padDelim(str, '(');
//            str = padDelim(str, ')');
//
//            str = str.substr(0, str.find('#') - 1);  //handle comments
//            str = trim(str);                         //handle leading and trailing whitespace
//            if (str == "#")
//                continue;

            main_token_queue = combineCommands(dd_token_queue);

            ShuntingYard sy(main_token_queue);
            queue<Token> evalQueue = sy.getReversePolish();
            evalPostFix(evalQueue);

            cout << endl;
        }
    }
}

/**
 * @brief Determines if connectors permit the execution of a command
 * @param vector of Tokens of either form "<cmd> ** <cmd>" or "** <cmd>" (where ** denotes a connector, && or ||)
 * NOTE: wasSuccess must be properly updated from the previous command!!!!!!!!!!!!!
**/
bool Manager::shouldExecute(vector<Token> expr) {

    if (expr.size() != 1 && expr.size() != 3)
        return false;

    if(expr.size() == 1) //Unary expression
        return expr[0].getStatus() == Token::notYetRunCmd;
    else //binary expression
        return ( (expr[0].getStatus() == Token::successfulCmd && expr[1].toString() == "&&")
                 || (expr[0].getStatus() == Token::failedCmd && expr[1].toString() == "||") );
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

    //@TODO: Replace with better code lol
    if (token_eval_stack.size() == 1 && !token_eval_stack.top().toString().empty())
    {
        assert(token_eval_stack.top().getStatus() == Token::notYetRunCmd);
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
void Manager::evaluate(vector<Token> binExpression)
{
    assert(binExpression.size() == 3);  //first command, connector, last command

    if(binExpression[0].getStatus() == Token::notYetRunCmd)
    {
        execute(binExpression[0].toString());   //modifies wasSuccess
        binExpression[0].setStatus(wasSuccess);
    }

    if(shouldExecute(binExpression))
        execute(binExpression[2].toString()); //q.front() is the last command
}

queue<Token> Manager::combineCommands(queue<Token> &old_token_queue)
{
    queue<Token> new_token_queue;

    while(!old_token_queue.empty())
    {
        Token t("", Token::notYetRunCmd);

        while(old_token_queue.front().getStatus() == Token::notYetRunCmd
              || old_token_queue.front().getStatus() == Token::quote)
        {
            t += old_token_queue.front();
            old_token_queue.pop();
        }

        new_token_queue.push(t);

        if(!old_token_queue.empty())
        {
            new_token_queue.push(old_token_queue.front());
            old_token_queue.pop();
        }
    }

    return new_token_queue;
}
