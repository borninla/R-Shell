#include "util.h"

char* _copyStrToCharPtr(const string& str) {

    char* c = new char[str.size() + 1];//+1 to null-terminate

    for (size_t i = 0; i < str.size(); i++) {

        c[i] = str[i];
    }

    c[str.size()] = '\0';

    return c;
}

string trim(const string& trimMe) {

    string whitespace = " \n\t";
    string newStr;

    //Find positions to trim
    size_t left = trimMe.find_first_not_of(whitespace);
    size_t right = trimMe.find_last_not_of(whitespace);

    //If all whitespace, trim all
    if (left == UINT_MAX || right == UINT_MAX) {

        newStr = "";

    } else {

        newStr = trimMe.substr(left, right - left + 1);
    }

    return newStr;
}
