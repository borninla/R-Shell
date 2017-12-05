#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <cassert>
#include <queue>

#include "util.h"

using namespace std;

class Token
{
public:
    Token(); //don't use this please, ill-defined Token
    Token(string str, int status);//uncomment to manually set status
    Token(string str); //infers status based on input

    enum Status { successfulCmd, failedCmd, notYetRunCmd,
        connector,
        error,
        quote,
        leftParenthesis, rightParenthesis };

    void setStatus(bool wasSuccessful); //can only set a notYetRunCmd

    int getStatus() const;

    string toString() const;

    friend ostream& operator <<(ostream& outs, const Token& printMe);

    friend bool operator ==(const Token& t, const string& str);
    friend bool operator ==(const string& str, const Token& t);

    friend bool operator !=(const Token& t, const string& str);
    friend bool operator !=(const string& str, const Token& t);

    Token& operator +=(const Token& t);

private:
    int status;

    /**
     * @brief str String representation of the token.
     * Using const deletes the implicit copy ctor for Token for some reason,
     * so I can't make it const,
     * but DO NOT change str after it is constructed!
     * Make a new token instead.
     * Tokens should be almost-immutable after construction,
     * with the only exception being commands which must be updated after they are run.
     */
    /*const*/ string str;

    void _stripEndQuotes();
};

#endif // TOKEN_H
