#include <iostream>
#include <string>
#include <climits>
#include <queue>
#include <stack>

#include "token.h"
class Token;    //Token constructor uses trim, so there is circular dependency

using namespace std;

//IMPORTANT: need to deallocate the char* after finishing the use of _copyStrToCharPtr
char* _copyStrToCharPtr(const string& str);

string trim(const string& trimMe);

bool equals(const char * cStr, string str, bool capsSensitive);

char toLower(char c);

string toLower(string str);

string padDelim(string str, char delim);

bool parenthesisChecker(queue<Token> checkMe);

bool testChecker(queue<Token> checkMe);

bool myXOR(bool val1, bool val2);

void eraseBothSides(string& trimMe, size_t thisManyChars);

void clearQueue(queue<Token>& queue_to_clear);