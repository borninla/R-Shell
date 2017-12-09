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

    if(trimMe == "")  //needed for when dummy Tokens are created (with empty string) during postfix evaluation
        return trimMe;

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

bool parenthesisChecker(queue<Token> checkMe)
{
    stack<char> s;

    while(!checkMe.empty())
    {
        if(checkMe.front().getStatus() == Token::leftParenthesis)
        {
            s.push('(');
            checkMe.pop();
        }
        else if(checkMe.front().getStatus() == Token::rightParenthesis)
        {
            if(s.empty() || s.top() != '(')
                return false;
            s.pop();
            checkMe.pop();
        }
        else
            checkMe.pop();
    }

    return s.empty();
}

bool myXOR(bool val1, bool val2) {

    return val1 != val2;
}

void eraseBothSides(string& trimMe, size_t thisManyChars) {

    //Make sure there are enough characters in trimMe to erase
    assert(trimMe.size() >= (thisManyChars * 2));

    if (thisManyChars == 0)
        return;

    trimMe.erase(0, thisManyChars);
    trimMe.erase(trimMe.size() - thisManyChars, string::npos);
}

void clearQueue(queue<Token>& queue_to_clear) {
    while(!queue_to_clear.empty())
        queue_to_clear.pop();
}