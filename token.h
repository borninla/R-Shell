#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <cassert>

using namespace std;

class Token
{
public:
    const static int TOKEN_WORD = 0;
    const static int TOKEN_NUMBER = 1;
    const static int TOKEN_PUNCTUATION = 2;
    const static int TOKEN_SPACE = 3;
    const static int TOKEN_QUOTE = 4;
    const static int TOKEN_COMMA = 5;
    const static int TOKEN_ASTERISK = 6;

    const static int TOKEN_UNKNOWN = -1;

    const static string STR_WORD;
    const static string STR_NUMBER;
    const static string STR_PUNCTUATION;
    const static string STR_SPACE;
    const static string STR_QUOTE;
    const static string STR_COMMA;
    const static string STR_ASTERISK;

    const static string STR_UNKNOWN;

    Token() {

        _init("", TOKEN_UNKNOWN);
    }

    /**
     * @brief Token Token constructor
     * @param str string
     * @param type MUST be STR
     */
    Token(string str, int type) {

        _init(str, type);
    }

    friend ostream& operator <<(ostream& outs, const Token& t) {

        outs << "Token: " << "[" << t.token_str() << "], Type " << t.type_string();
        return outs;

    }

    int type() const {

        return _type;
    }

    string type_string() const {

        if (_type == TOKEN_WORD) {
            return STR_WORD;

        } else if (_type == TOKEN_NUMBER) {

            return STR_NUMBER;

        } else if (_type == TOKEN_PUNCTUATION) {

            return STR_PUNCTUATION;

        } else if (_type == TOKEN_SPACE) {

            return STR_SPACE;

        } else if (_type == TOKEN_QUOTE) {

            return STR_QUOTE;

        } else if (_type == TOKEN_UNKNOWN) {

            return STR_UNKNOWN;

        } else if (_type == TOKEN_COMMA) {

            return STR_COMMA;

        } else if (_type == TOKEN_ASTERISK) {

            return STR_ASTERISK;

        } else {

            assert(_typeIsValid(_type));
            assert(false);
        }

        return "";  //Should never reach this line
    }

    string token_str() const {

        return _token;
    }



private:

    string _token;
    int _type;

    void _init(const string& str, int type) {

        assert(_typeIsValid(type));

        _token = str;
        _type = type;
    }

    bool _typeIsValid(int type) const {

        return type == TOKEN_NUMBER
                || type == TOKEN_WORD
                || type == TOKEN_SPACE
                || type == TOKEN_PUNCTUATION
                || type == TOKEN_UNKNOWN
                || type == TOKEN_QUOTE
                || type == TOKEN_COMMA
                || type == TOKEN_ASTERISK;
    }

};

// commented out due to a duplicate symbol error

//const string Token::STR_NUMBER = "NUMBER";
//const string Token::STR_WORD = "WORD";
//const string Token::STR_PUNCTUATION = "PUNCTUATION";
//const string Token::STR_SPACE = "SPACE";
//const string Token::STR_QUOTE = "QUOTE";
//const string Token::STR_UNKNOWN = "UNKNOWN";
//const string Token::STR_COMMA = "COMMA";
//const string Token::STR_ASTERISK = "ASTERISK";

#endif // TOKEN_H


