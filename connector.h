#ifndef CONNECTOR_H
#define CONNECTOR_H


#include "token.h"

class Connector : public Token
{
public:
    string get_token();
private:
    string token;
};


#endif //CONNECTOR_H
