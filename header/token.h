#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

class Token
{
public:
    Token(); //don't use this please, ill-defined Token

    //Token(string str, int status);//uncomment to manually set status

    Token(string str); //infers status based on input

    enum Status { successfulCmd, failedCmd, notYetRunCmd,
        connector,
        error,
        leftParenthesis, rightParenthesis };

    void setStatus(bool wasSuccessful); //can only set a notYetRunCmd

    int getStatus() const;

    string toString() const;

    friend ostream& operator <<(ostream& outs, const Token& printMe);
    
    friend queue<Token> stringsToTokens(queue<string> convMe);

    friend bool operator ==(const Token& t, const string& str);
    friend bool operator ==(const Token& t, const string& str);

    friend bool operator !=(const Token& t, const string& str);
    friend bool operator !=(const string& str, const Token& t);

private:

    int status;
    const string str;
};

#endif // TOKEN_H
