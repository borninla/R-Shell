#include "../header/token.h"

Token::Token() : status(error), str("") {}

//Token::Token(string str, int status) : status(status), str(str) {}

Token::Token(string str) : str(str) {

    if (str == "&&" || str == "||") {

        status = connector;

    } else if (str == "(") {

        status = leftParenthesis;

    } else if (str == ")") {

        status = rightParenthesis;

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

ostream& operator <<(ostream& outs, const Token& printMe) {

    outs << printMe.str << " | Enum " << printMe.status;
    return outs;
}

queue<Token> stringsToTokens(queue<string> convMe) {

    queue<Token> q;

    while (!convMe.empty()) {

        Token t(convMe.front());
	q.push(t);
	convMe.pop();

    }

    return q;
}
