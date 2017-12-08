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
        leftParenthesis, rightParenthesis,
        testE, testF, testD };

    void setStatus(bool wasSuccessful); //can only set a notYetRunCmd

    int getStatus() const;

    string toString() const;

    friend ostream& operator <<(ostream& outs, const Token& printMe);

    friend bool operator ==(const Token& t, const string& str);
    friend bool operator ==(const string& str, const Token& t);

    friend bool operator !=(const Token& t, const string& str);
    friend bool operator !=(const string& str, const Token& t);

    Token& operator +=(const Token& t);

    bool isTest() const;

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



    /**
     * @brief _whatKindOfTest Tests whether str is a test command, and if so what kind of test command.
     * @param str String to be tested (should have "test" or [] brackets, may or may not have a flag)
     * @return The enum of the kind of test it is, or string::npos if it is not a test
     */
    size_t _whatKindOfTest(string str);

    /**
     * @brief _pruneTest Erases flags and either indicator of tests from str (i.e. "test" or [] brackets)
     * Note that this requires that this token is of some Test type
     */
    void _pruneTest();
};


#endif // TOKEN_H
