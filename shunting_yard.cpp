#include "shunting_yard.h"

ShuntingYard::ShuntingYard(string command)
{
    delivery = command;
}

void ShuntingYard::run()
{
    char* char_delivery = strtok(_copyStrToCharPtr(delivery), " ");

    while(char_delivery != NULL)
    {
        if(strncmp(char_delivery, "&&", 2) != 0 && strncmp(char_delivery, "||", 2) != 0)
            command_queue.push(char_delivery);
        else
        {
            while(!connector_stack.empty() && (strncmp(char_delivery, "||", 2) != 0
                                               && strncmp(connector_stack.top(),"&&", 2) != 0))
            {
                command_queue.push(connector_stack.top());
                connector_stack.pop();
            }

            connector_stack.push(char_delivery);
        }

        char_delivery = strtok(NULL, " ");
    }

    while(!connector_stack.empty())
    {
        command_queue.push(connector_stack.top());
        connector_stack.pop();
    }

    cout << "Original prompt: " << delivery << endl;
    cout << " Reverse polish: " ;

    while (!command_queue.empty()) {
        cout << command_queue.front() << " ";
        command_queue.pop();
    }

}

//debug for checking what strncmp is returning
//        cout << char_delivery << " &&: " << strncmp(char_delivery, "&&", 2) << " ||: ";
//        cout << strncmp(char_delivery, "||", 2) << endl;

//            cout << connector_stack.top() << endl;
//
//            cout << "does " << char_delivery << " = ||? " << strncmp(char_delivery, "||", 2) << " ";
//            cout << "does " << connector_stack.top() << " = &&? " << strncmp(connector_stack.top(), "&&", 2) << " " << endl;
