#include "../header/delim.h"

Delim::Delim() {

    _init(NULL, ' ', true);
}

Delim::Delim(char *cstr, char delim, bool quotesSeparately) {

    _init(cstr, delim, quotesSeparately);
}

Delim::Delim(string str, char delim, bool quotesSeparately) {

    str = padDelim(str, '(');
    str = padDelim(str, ')');

    char* c = _copyStrToCharPtr(str);
    _init(c, delim, quotesSeparately);
    delete [] c;
}

bool Delim::done() const { return q.empty(); }

size_t Delim::size() const { return q.size(); }

void Delim::compressTokens() {

    queue<Token> replacementQueue;



    while(!q.empty()) {

        //Prepare to append to current Token
        vector<Token> aggregateVector;
        Token initialToken = q.front();
        aggregateVector.push_back(initialToken);
        q.pop();



        while(!q.empty()

              //Iterate through q for as long as the type matches initialToken
              && (q.front().getStatus() == initialToken.getStatus()

                  //Or, iterate through q for as long as the quote extends
                  || initialToken.toString().at(0) == '\"')) {

            Token appendMe = q.front();
            aggregateVector.push_back(appendMe);
            q.pop();

            //Exit the loop if we were in a quote, and there's a \" at the end of this token to end the quote
            if (initialToken.toString().at(0) == '\"'
                    && appendMe.toString().at(appendMe.toString().size() - 1) == '\"')
                break;
        }

        //Make the combine token based on what you collected, and push it into replacementQueue
        Token aggregateToken(aggregateVector);
        replacementQueue.push(aggregateToken);
    }



    //Replace queue
    q.swap(replacementQueue);
    assert(replacementQueue.empty());
}

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
        while (*walker != '\0' && ( (*walker != delim) || (inQuoteFlag && weCareAboutQuotes)) ) {

            char currentChar = *walker;

            if (currentChar == '\"')
                inQuoteFlag = !inQuoteFlag; //toggle whether we're in or not

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
