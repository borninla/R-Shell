#ifndef DELIM_H
#define DELIM_H

#include <iostream>
#include <queue>
#include <string>
#include <cassert>

#include "token.h"
#include "util.h"

using namespace std;

class Delim
{
public:
    Delim();

    /**
     * @brief Delim
     * @param cstr Null-terminated cstring
     * @param delim
     */
    Delim(char* cstr, char delim, bool quotesSeparately);
    Delim(string str, char delim, bool quotesSeparately);
//    Delim(string str, string delim);

    bool done() const;
    size_t size() const;

    friend Delim& operator >>(Delim& delim, Token& t);
//    friend Delim& operator >>(Delim& delim, string& str);

    friend ostream& operator <<(ostream& outs, const Delim& d);

    void dumpToConsole();


private:

    /**
     * @brief _init initializes Delim
     * @param cstr Takes the contents of cstr, and populates the queue q within this object
     * @param delim the delimiter character
     */
    void _init(char* cstr, char delim, bool quotesSeparately);
//    void _init(string str, string delim, bool quotesSeparately);

    bool _properQuotes(string str);
    bool _properQuotes(char* c);

    queue<Token> q;
    bool quotesSeparately;
};

#endif // DELIM_H
