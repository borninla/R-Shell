#include "../header/shunting_yard.h"

ShuntingYard::ShuntingYard(queue<Token> command)
{
    delivery = command;
    run();
}

void ShuntingYard::run()
{
    while(!delivery.empty())    //while there are still tokens to be read
    {
        Token currentToken = delivery.front();
        delivery.pop();

        //if token is a command, push it into the command stack
        if( currentToken.getStatus() == Token::notYetRunCmd || currentToken.getStatus() == Token::quote
                || currentToken.isTest() )
            command_queue.push(currentToken);
        if(currentToken.getStatus() == Token::connector)
        {
            //else, if the precedence of the incoming token is less than the token at the
            //top of the operator_stack, pop tokens from the stack and push it into the command_queue

            //NOTE: there are 4 seperate cases of either the && or || connectors popping up
            //(&& ||, || ||, || &&, and && &&). However, since && takes precedence over ||, there is
            //only one case where that happens (if an && connector coming in from delivery is being compared
            //with an || connector at the top of the stack).
            while(!operator_stack.empty() && operator_stack.top().getStatus() != Token::leftParenthesis)
            {
                command_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            //if the precedence of the incoming token is less than the token at the top of the operator_stack,
            //simply push that token into the stack
            operator_stack.push(currentToken);
        }

        if(currentToken.getStatus() == Token::leftParenthesis)
            operator_stack.push(currentToken);

        if(currentToken.getStatus() == Token::rightParenthesis)
        {
            while(operator_stack.top().getStatus() != Token::leftParenthesis)
            {
                command_queue.push(operator_stack.top());
                operator_stack.pop();
            }

            assert(!operator_stack.empty());
            operator_stack.pop();  //pops left bracket
        }
    }

    //once all tokens have been read, push the remaining connectors in the stack to the queue
    while(!operator_stack.empty())
    {
        command_queue.push(operator_stack.top());
        operator_stack.pop();
    }

}

queue<Token> ShuntingYard::getReversePolish() { return command_queue; }