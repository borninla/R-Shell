#include "../header/shunting_yard.h"

ShuntingYard::ShuntingYard(queue<string> command)
{
    delivery = command;
    run();
}

void ShuntingYard::run()
{
    while(!delivery.empty())    //while there are still tokens to be read
    {
        string currentToken = delivery.front();
        delivery.pop();

        //if token is a command, push it in the command stack
        if(currentToken != "&&" && currentToken != "||")
            command_queue.push(currentToken);
        else
        {
            //else, if the precedence of the incoming token is less than the token at the
            //top of the connector_stack, pop tokens from the stack and push it into the command_queue

            //NOTE: there are 4 seperate cases of either the && or || connectors popping up
            //(&& ||, || ||, || &&, and && &&). However, since && takes precedence over ||, there is
            //only one case where that happens (if an && connector coming in from delivery is being compared
            //with an || connector at the top of the stack).
            while(!connector_stack.empty() && (currentToken != "&&" || connector_stack.top() != "||"))
            {
                command_queue.push(connector_stack.top());
                connector_stack.pop();
            }
            //if the precedence of the incoming token is less than the token at the top of the connector_stack,
            //simply push that token into the stack
            connector_stack.push(currentToken);
        }

        if(currentToken == "(")
            connector_stack.push(currentToken);

        if(currentToken == ")")
        {
            while(connector_stack.top() != "(")
            {
                command_queue.push(connector_stack.top());
                connector_stack.pop();
            }
            connector_stack.pop();  //pops left bracket
        }
    }

    //once all tokens have been read, push the remaining connectors in the stack to the queue
    while(!connector_stack.empty())
    {
        command_queue.push(connector_stack.top());
        connector_stack.pop();
    }
}

queue<string> ShuntingYard::getReversePolish()
{
    return command_queue;
}

//    //debug print for comparing notations
//    cout << "Original prompt: " << delivery << endl;
//    cout << " Reverse polish: " ;
//
//    while (!command_queue.empty()) {
//        cout << command_queue.front() << " ";
//        command_queue.pop();
//    }

//debug for checking what strncmp is returning
//        cout << char_delivery << " &&: " << strncmp(char_delivery, "&&", 2) << " ||: ";
//        cout << strncmp(char_delivery, "||", 2) << endl;

//            cout << connector_stack.top() << endl;
//
//            cout << "does " << char_delivery << " = ||? " << strncmp(char_delivery, "||", 2) << " ";
//            cout << "does " << connector_stack.top() << " = &&? " << strncmp(connector_stack.top(), "&&", 2) << " " << endl;