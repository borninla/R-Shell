#include <iostream>
#include <string>
#include <climits>
#include <queue>
#include <stack>

#include "token.h"
class Token;    //Token constructor uses trim, so there is circular dependency

using namespace std;

char* _copyStrToCharPtr(const string& str);

string trim(const string& trimMe);

string _parseUntilConnector(string& parseThis);

bool _isConnector(const string& str);

queue<string> returnParsedData(string parseThisString);

string toDelimitedString(queue<string> q, string delim);

bool equals(const char * cStr, string str, bool capsSensitive);

char toLower(char c);

string toLower(string str);

string padDelim(string str, char delim);

queue<Token> stringsToTokens(queue<string> convMe);

bool parenthesisChecker(string checkMe);