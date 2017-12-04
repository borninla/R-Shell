#include "../header/token.h"

Token::Token() : status(error), str("") {}

Token::Token(string str, int status) : status(status), str(trim(str)) {}

Token::Token(string str) : str(trim(str)) {

    if (this->str == "&&" || this->str == "||") {

        status = connector;

    } else if (this->str == "(") {

        status = leftParenthesis;

    } else if (this->str == ")") {

        status = rightParenthesis;

    } else if (!(this->str.empty())
               && (this->str[0] == '\"' && this->str[str.size() - 1] == '\"')) {

        status = quote;
        _stripEndQuotes();

    } else {

        status = notYetRunCmd;
    }
}

void Token::setStatus(bool wasSuccessful) {

    assert(status == notYetRunCmd);

    if (wasSuccessful) {

        this->status = successfulCmd;
    } else {

        this->status = failedCmd;
    }
}

int Token::getStatus() const {
    return status;
}

string Token::toString() const {
    return str;
}

ostream& operator <<(ostream& outs, const Token& printMe)
{
    outs << printMe.str << " | Enum " << printMe.status;
    return outs;
}

bool operator ==(const Token& t, const string& str) {

    return t.toString() == str;
}

bool operator ==(const string& str, const Token& t) {

    return str == t.toString();
}

bool operator !=(const Token& t, const string& str) {

    return t.toString() != str;
}

bool operator !=(const string& str, const Token& t) {

    return str == t.toString();
}

void Token::_stripEndQuotes() {

    if (this->status != quote)
        return;

    assert(this->str.size() >= 2);

    str.erase(0, 1);
    str.erase(str.size() - 1, 1);
}
