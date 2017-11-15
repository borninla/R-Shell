#include <iostream>
#include <string>
#include <climits>
#include <queue>

using namespace std;

char* _copyStrToCharPtr(const string& str);

string trim(const string& trimMe);

string _parseUntilConnector(string& parseThis);

bool _isConnector(const string& str);

queue<string> returnParsedData(string parseThisString);

string toSpaceDelimitedString(queue<string> q, string delim);
