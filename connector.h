#ifndef CONNECTOR_H
#define CONNECTOR_H


#include "token.h"

class Connector : public Token
{
public:
    string getToken();
private:
    string token;
};


#endif //CONNECTOR_H
