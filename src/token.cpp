#include "../header/token.h"

Token::Token() : status(error), str("") {}

Token::Token(string str, int status) : status(status), str(trim(str)) {}

Token::Token(string str, bool worryAboutTests) : str(trim(str)) {

    size_t testType = _whatKindOfTest(this->str);

    if (testType != string::npos && worryAboutTests) {

        status = testType;
        //_pruneTest();

    } else if (this->str == "&&" || this->str == "||") {

        status = connector;

    } else if (this->str == "(") {

        status = leftParenthesis;

    } else if (this->str == ")") {

        status = rightParenthesis;

    } else if (this->str[0] == '[') {   //jankyyy

        status = testE;

    } else if (!(this->str.empty()) && (this->str[0] == '\"' && this->str[str.size() - 1] == '\"')) {

        status = quote;
        eraseBothSides(this->str, 1);
        this->str = trim(this->str);

    } else {

        status = notYetRunCmd;
    }
}

Token::Token(const vector<Token>& combineUs) {

    if (combineUs.empty()) {

        str = "";
        status = error;
    } else {

        //Set status explicitly
        status = combineUs[0].getStatus();

        //Combine strings, delimiting with ' '
        string strAggregate = "";

        for (size_t i = 0; i < combineUs.size(); i++) {

            Token currentToken = combineUs[i];  //changed from Token& to Token due to compile error
            strAggregate += currentToken.toString();
            strAggregate += ' ';
        }

        str = strAggregate;
        str = trim(str);
    }
}

void Token::setStatus(bool wasSuccessful) {

    assert(status == notYetRunCmd || this->isTest());   //could also be test command - added to assertion

    if (wasSuccessful) {

        this->status = successfulCmd;
    } else {

        this->status = failedCmd;
    }
}

void Token::setStatus(Status s) {

    this->status = s;

}

void Token::setString(string str) {

    this->str = str;

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

Token &Token::operator+=(const Token &t) {

    assert(this->getStatus() == Token::notYetRunCmd);

    this->str += " " + t.str;

    this->str = trim(this->str);

    return *this;
}

bool Token::isTest() const
{
    return (this->status == Token::testD)
           || (this->status == Token::testE)
           || (this->status == Token::testF);
}

size_t Token::_whatKindOfTest(string str) {

    str = trim(str);

    if (str.size() < 2) {

        return string::npos;
    }

    if ( !(str[0] == '[' && str[str.size() - 1] == ']') && toLower(str.substr(0, 4)) != "test")
        return string::npos;

    //Now, we know it's a token - just figure out what kind of token it is
    if (str.find("-d") != string::npos) {

        return Token::testD;

    } else if (str.find("-e") != string::npos) {

        return Token::testE;

    } else if (str.find("-f") != string::npos) {

        return Token::testF;

    } else {    //Default, if there is no flag included

        return Token::testE;
    }
}

void Token::_pruneTest() {

    //assert(this->isTest());

    //Get rid of [] brackets, or "test"
    if (str[0] == '[' && str[str.size() - 1] == ']') {

        eraseBothSides(str, 1);

    } else {

        assert(toLower(str.substr(0, 4)) == "test");

        str.erase(0, 4);
    }



    //Get rid of flags, if there is any
    size_t foundIndex;

    switch (status) {

        case Token::testD: {

            foundIndex = str.find("-d");
            assert(foundIndex != string::npos);

            break;
        }

        case Token::testF: {

            foundIndex = str.find("-f");
            assert(foundIndex != string::npos);

            break;
        }

        default: {

            //assert(status == Token::testE);

            foundIndex = str.find("-e");

            if (foundIndex == string::npos) {

                str = trim(str);
                return;
            }
        }
    }

    str = trim(str.substr(foundIndex + 3, string::npos));
}