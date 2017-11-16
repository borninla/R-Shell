#ifndef TOKEN_H
#define TOKEN_H


#include <iostream>
#include <string>

using namespace std;

class CompositeToken
{
protected:
    enum status { wasSuccess, notSuccess, notCommand, notYetRun };
    string token;
public:
    virtual status getStatus() = 0;
    string getString();
};

class CommandToken : public CompositeToken
{
public:
    status getStatus();
};

class ConnectorToken : public CompositeToken
{
public:
    status getStatus();
};


#endif //TOKEN_H
