#ifndef DELIM_H
#define DELIM_H

#include <iostream>
#include <queue>
#include <string>
#include <cassert>

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
    Delim(char* cstr, char delim);

    Delim(string str, string delim);

    bool done() const;

    friend Delim& operator >>(Delim& delim, string& str);

    void dumpToConsole();


private:

    /**
     * @brief _init initializes Delim
     * @param cstr Takes the contents of cstr, and populates the queue q within this object
     * @param delim the delimiter character
     */
    void _init(char* cstr, char delim);
    void _init(string str, string delim);
    queue<string> q;
};

#endif // DELIM_H
