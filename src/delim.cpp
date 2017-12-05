#include "../header/delim.h"

Delim::Delim() {

    _init(NULL, ' ', true);
}

Delim::Delim(char *cstr, char delim, bool quotesSeparately) {

    _init(cstr, delim, quotesSeparately);
}

// @TODO: Have constructor tokenize parenthesis (replaces padDelim)
Delim::Delim(string str, char delim, bool quotesSeparately) {

    char* c = _copyStrToCharPtr(str);
    _init(c, delim, quotesSeparately);
    delete [] c;
}

bool Delim::done() const { return q.empty(); }

size_t Delim::size() const { return q.size(); }

Delim& operator >>(Delim& delim, Token& t) {

    assert(!delim.q.empty());

    t = delim.q.front();
    delim.q.pop();

    return delim;
}

ostream& operator <<(ostream& outs, const Delim& d) {

    queue<Token> qCopy(d.q);

    outs << "{ ";

    while(!qCopy.empty()) {

        outs << "(" << qCopy.front() << ") ";
        qCopy.pop();
    }

    outs << "}";

    return outs;
}

void Delim::_init(char *cstr, char delim, bool quotesSeparately) {

    if (cstr == NULL)
        return;

    char* walker = cstr;
    bool weCareAboutQuotes = quotesSeparately && _properQuotes(cstr);

    //Walk through the cstr
    while(*walker != '\0') {

        string currentStr = "";
        bool inQuoteFlag = false, commentFound = false;

        //Don't delimit if we're in a quote!
        while (*walker != '\0'
               && ((*walker != delim) || (inQuoteFlag && weCareAboutQuotes))) {

            char currentChar = *walker;


            if (currentChar == '\"')
                inQuoteFlag = !inQuoteFlag; //toggle whether we're in or not

//            if (currentChar == '#')
//                continue;
            if (currentChar == '#' && !inQuoteFlag) {
                commentFound = true;
                break;
            }

            currentStr += currentChar;

            walker++;

        }

        if (!currentStr.empty())
            q.push(Token(currentStr));



        if (commentFound)
            return;

        if (*walker != '\0')
            walker++;
    }

}

bool Delim::_properQuotes(char* c) {

    string str(c);
    return _properQuotes(str);
}

bool Delim::_properQuotes(string str) {

    size_t totalQuotes = 0;

    for (size_t i = 0; i < str.size(); i++) {

        if (str[i] == '\"')
            totalQuotes++;
    }

    return totalQuotes % 2 == 0;
}