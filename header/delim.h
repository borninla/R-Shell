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

    Delim(char* cstr, char delim, bool quotesSeparately, bool worryAboutTests);
    Delim(string str, char delim, bool quotesSeparately, bool worryAboutTests);

    bool done() const;
    size_t size() const;

    /**
     * @brief combineRelated Groups together commands with several parts while leaving non-command Tokens isolated.
     * For use for expressions in normal notation ONLY, i.e. not postfix.
     *
     * Example: (ls) (-a) (;) (cat) (man) => (ls -a) (;) (cat man)
     * To be used in "emergencies" only, since improper use of this function causes erratic behavior.
     *
     * Tokens are compressed if they are a) of the same status,
     * or b) part of a quote encapsulated by space-padded quotation marks.
     *
     * For example, improperly running compressCmds() on the following queue
     *      (ls) (echo hello) (cat m.txt) (&&) (||)
     * will produce the following undesired output:
     *      (ls echo hello cat m.txt) (&& ||)
     *
     * Please be extremely careful with this function!!!!!!
     */
    void compressTokens();

    //Used only to check for test commands
    void checkFlagsAndReinitStatus();

    friend Delim& operator >>(Delim& delim, Token& t);

    friend ostream& operator <<(ostream& outs, const Delim& d);
private:

    /**
     * @brief _init initializes Delim
     * @param cstr Takes the contents of cstr, and populates the queue q within this object
     * @param delim the delimiter character
     */
    void _init(char* cstr, char delim, bool quotesSeparately, bool worryAboutTests);

    bool _properQuotes(string str);
    bool _properQuotes(char* c);

    queue<Token> q;
};

#endif // DELIM_H
