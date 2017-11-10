#include "shunting_yard.h"

ShuntingYard::ShuntingYard(string command)
{
    delivery = command;
    run();
}

void ShuntingYard::run()
{
    char* char_delivery = strtok(_copyStrToCharPtr(delivery), " "); //tokenize input

    while(char_delivery != NULL)    //while there are still tokens to be read
    {
        //if token is a command, push it in the queue
        if(strncmp(char_delivery, "&&", 2) != 0 && strncmp(char_delivery, "||", 2) != 0)
            command_queue.push(char_delivery);
        else
        {
            //else, if the precedence of the incoming token is greater than or equal to the token at the
            //top of the connector_stack, pop the token from the stack and push it into the command_queue

            //NOTE: there are 4 seperate cases of either the && or || connectors popping up
            //(&& ||, || ||, || &&, and && &&). However, since && takes precedence over ||, there is
            //only one case where that happens (if an && connector coming in from delivery is being compared
            //with an || connector at the top of the stack).
            while(!connector_stack.empty() && (strncmp(char_delivery, "||", 2) != 0
                                               && strncmp(connector_stack.top(),"&&", 2) != 0))
            {
                command_queue.push(connector_stack.top());
                connector_stack.pop();
            }

            //if the precedence of the incoming token is less than the token at the top of the connector_stack,
            //simply push that token into the stack
            connector_stack.push(char_delivery);
        }

        //iterate to the next token
        char_delivery = strtok(NULL, " ");
    }

    //once all tokens have been read, push the remaining connectors in the stack to the queue
    while(!connector_stack.empty())
    {
        command_queue.push(connector_stack.top());
        connector_stack.pop();
    }
}

string ShuntingYard::getReversePolish()
{
    string returnThisString;

    while (!command_queue.empty())
    {
        returnThisString += command_queue.front();

        if(command_queue.size() != 1)
            returnThisString += " ";

        command_queue.pop();
    }

    return returnThisString;
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