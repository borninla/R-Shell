#ifndef COMMAND_H
#define COMMAND_H


#include "token.h"

class Command : public Token
{
public:
    string get_token();
private:
    string token;
};


#endif //COMMAND_H
