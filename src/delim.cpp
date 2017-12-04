#include "../header/delim.h"

Delim::Delim() {

    _init(NULL, ' ', true);
}

Delim::Delim(char *cstr, char delim, bool quotesSeparately) {

    _init(cstr, delim, quotesSeparately);
}

/*
Delim::Delim(string str, string delim, quotesSeparately) {

    _init(str, delim, quoteSeparately);
}*/

// @TODO: Have constructor tokenize parenthesis (replaces padDelim)
Delim::Delim(string str, char delim, bool quotesSeparately) {

    char* c = _copyStrToCharPtr(str);
    _init(c, delim, quotesSeparately);
    delete [] c;
}

bool Delim::done() const
{
    return q.empty();
}

//Delim& operator >>(Delim& delim, string& str) {

//    assert(!delim.q.empty());

//    str = delim.q.front().toString();
//    delim.q.pop();

//    return delim;

//}

Delim& operator >>(Delim& delim, Token& t) {

    assert(!delim.q.empty());

    t = delim.q.front();
    delim.q.pop();

    return delim;
}

ostream& operator <<(ostream& outs, const Delim& d) {

//    queue<Token> qCopy = d.q;
    queue<Token> qCopy(d.q);

    outs << "{ ";

    while(!qCopy.empty()) {

        outs << "(" << qCopy.front() << ") ";
        qCopy.pop();
    }

    outs << "}";

    return outs;
}

void Delim::dumpToConsole() {

    while (!q.empty()) {

        cout << q.front() << endl;
        q.pop();
    }
}

void Delim::_init(char *cstr, char delim, bool quotesSeparately) {

    this->quotesSeparately = quotesSeparately;

    if (cstr == NULL)
        return;

    char* walker = cstr;
    bool weCareAboutQuotes = quotesSeparately && _properQuotes(cstr);

    //Walk through the cstr
    while(*walker != '\0') {

        string currentStr = "";
        bool inQuoteFlag = false;

        //Walk through a delimited section
//        while(*walker != delim
//              && *walker != '\0')

        //Don't delimit if we're in a quote!
        while (*walker != '\0'
               && ((*walker != delim) || (inQuoteFlag && weCareAboutQuotes))) {

            char currentChar = *walker;
            currentStr += currentChar;

            if (currentChar == '\"')
                inQuoteFlag = !inQuoteFlag;

            walker++;

        }

        if (!currentStr.empty())
        {

//            q.push(currentStr);
            q.push(Token(currentStr));
        }

        if (*walker != '\0')
            walker++;
    }

}

//void Delim::_init(string str, string delim, bool quotesSeparately) {

//    if (str == "")
//        return;

//    assert(str.size() > delim.size());

    
//}

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

size_t Delim::size() const {
    return q.size();
}
