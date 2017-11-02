#ifndef TOKEN_H
#define TOKEN_H


#include <iostream>

using namespace std;

class Token
{
public:
    Token();

    virtual string getToken() = 0;
};


#endif //TOKEN_H
