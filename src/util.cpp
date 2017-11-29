#include "../header/util.h"

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

string _parseUntilConnector(string& parseThis)
{
    size_t foundAnd, foundOr, foundSmallest;

    if(parseThis.compare(0, 2, "&&") == 0 || parseThis.compare(0, 2, "&&") == 0 )   //if connector is first token
        return parseThis;

    foundAnd = parseThis.find("&&");
    foundOr = parseThis.find("||");

    if(foundAnd != UINT_MAX && foundOr != UINT_MAX && foundAnd < foundOr)
        foundSmallest = foundAnd;
    else if(foundAnd != UINT_MAX && foundOr != UINT_MAX && foundOr < foundAnd)
        foundSmallest = foundOr;
    else
        return parseThis;

    string parsedStr = parseThis.substr(0, foundSmallest - 1);
    parseThis.erase(0, foundSmallest - 1);

    return parsedStr;
}

bool _isConnector(const string& str) {

    return str == "&&" || str == "||";

}

queue<string> returnParsedData(string parseThisString)
{
    string newThis;

    queue<string> tokens;

    while (!parseThisString.empty()) {
        if (parseThisString.compare(0, 2, "&&") == 0 || parseThisString.compare(0, 2, "||") == 0) //if connector is next
        {
            newThis = parseThisString.substr(0, 2);
            parseThisString.erase(0, 3);   //erases connectors from string and space
        } else if (parseThisString.find("||") != string::npos ||
                   parseThisString.find("&&") != string::npos)  //if connectors are in string
        {
            newThis = _parseUntilConnector(parseThisString);
            parseThisString = trim(parseThisString);
        } else {
            newThis = _parseUntilConnector(parseThisString);
            parseThisString.clear();
        }

        tokens.push(newThis);
    }

    return tokens;
}

string toSpaceDelimitedString(queue<string> q, string delim)
{
    string concatenated = "";

    while (!q.empty()) {

	concatenated += q.front();
	q.pop();

	concatenated += delim;

    }

    return concatenated;
}

bool equals(const char * cStr, string str, bool capsSensitive)
{
	string compareMeString;
	const char * compareMeCStr;
	bool shouldDelete;

	if (capsSensitive)
    {
		compareMeString = str;
		compareMeCStr = reinterpret_cast<char *>(*cStr);
		shouldDelete = false;
	} else
    {

		compareMeCStr = _copyStrToCharPtr(toLower(cStr));
		compareMeString = toLower(str);
		shouldDelete = true;
	}

	for (size_t i = 0; i < compareMeString.size(); i++)
    {
		if (compareMeString[i] != compareMeCStr[i])
        {

			if (shouldDelete)
				delete [] compareMeCStr;

			return false;

		}
	}

	if (shouldDelete)
		delete[] compareMeCStr;

	return true;
}

char toLower(char c)
{
    if ('A' <= c && c <= 'Z') {

	    return c - 'A' + 'a';
    } else {

	    return c;
    }
}

string toLower(string str)
{

	for (size_t i = 0; i < str.size(); i++) {

		str[i] = toLower(str[i]);
	}

	return str;
}

string padDelim(string str, char delim)
{
    for (size_t i = 0; i < str.size(); i++) {

        //Pad left of delim, if necessary
        if (str[i] == delim
	    && (i == 0 || str[i - 1] != ' ')) {//safe from i=0 by short-circuit evaluation

	    string delimStr(1, ' ');
	    str.insert(i, delimStr);

	    i++;    //Keep up the loop with the insertion
	}

	    //Pad right of delim, if necessary
        if (str[i] == delim
	    && (i == str.size() - 1 || str[i + 1] != ' ')) {

	    string delimStr(1, ' ');
	    str.insert(i + 1, delimStr);

	    i++;
	}
    }

    return str;
}

queue<Token> stringsToTokens(queue<string> convMe) {

    queue<Token> q;

    while (!convMe.empty())
    {
        Token t(convMe.front());
        q.push(t);
        convMe.pop();
    }

    return q;
}

bool parenthesisChecker(string checkMe)
{
    stack<char> s;

    for (size_t i = 0; i < checkMe.size(); i++) {

        if (checkMe[i] == '(')
	    s.push('(');

	else if (checkMe[i] == ')') {

            if (s.empty() || s.top() != '(')
                return false;

	    s.pop();
	}

    }

    return s.empty();
}

