#ifndef STR_TOKENIZER_H
#define STR_TOKENIZER_H

#include <string>
#include <iostream>
#include <cassert>
#include <cstring>

#include "token.h"

using namespace std;

class StrTokenizer
{
public:

    StrTokenizer() {

        _init(nullptr, 0);
    }

    StrTokenizer(const char * chrBuffer, int size) {

        _init(chrBuffer, size);
    }

    StrTokenizer(const string& str) {

        _init(str.c_str(), str.size());
    }

    StrTokenizer(const StrTokenizer& other) {

        _init(other.buffer, other.bufferSize);
    }

    void operator =(const StrTokenizer& other) {

        _safeDestruct();

        _init(other.buffer, other.bufferSize);
    }

    ~StrTokenizer() {

        _safeDestruct();
    }

    /**
     * @brief done Checks if this StrTokenizer is internally finished
     * @return True if this object is done with its buffer, or if it's empty - false if it has tokens
     */
    bool done() const {

        return buffer == nullptr    //if empty
                || cursor - buffer >= bufferSize   //if populated and done
                || *cursor == '\0'; //if reached null character
    }

    /**
     * @brief startCursorFromScratch Internally resets object
     * When StrTokenizer >> Token is called, the output will match the first >> call from when it was first instantiated
     */
    void startCursorFromScratch() {

        cursor = buffer;
    }

    void changeString(const string& newStr) {

        changeString(newStr.c_str(), newStr.length());
    }

    void changeString(const char* newBuffer, int newBufferSize) {

        assert(newBufferSize >= 0);

        _safeDestruct();

        _init(newBuffer, newBufferSize);
    }

    friend StrTokenizer& operator >>(StrTokenizer& s, Token& t) {

        assert(!s.done());

        s._tokenize(t);
        s.cursor++;
        return s;

    }

private:

    char * buffer;
    char * cursor;
    int bufferSize;

    /**
     * @brief NUM_OF_ROWS Number of states
     */
    const static int NUM_OF_ROWS = 21;
    /**
     * @brief NUM_OF_COLUMNS 128 ascii chars, 2 status columns (state and sucfail)
     */
    const static int NUM_OF_COLUMNS = 128 + 2;

    const static int STATE_COLUMN = 0;
    const static int SUCFAIL_COLUMN = 1;

    /**
     * @brief stateTable Table of states. ASCII values start at index 2.
     * Indices 0 and 1 are reserved for STATE_COLUMN and SUCFAIL_COLUMN, respectively.
     * STATE_COLUMN values are always equal to the row index.
     * SUCFAIL_COLUMN are 1 for a success state, and 0 for a fail state.
     */
    int stateTable[NUM_OF_ROWS][NUM_OF_COLUMNS];

    /**
     * @brief _init Allocates/fills this StrTokenizer's buffer, sets cursor, and manages bufferSize
     * @param src Source buffer to be copied from
     * @param size Size of the buffer
     */
    void _init(const char * src, int size) {

        assert(size >= 0);

        _initTable();

        if (size > 0) {

            buffer = cursor = new char[size/* + 1*/];

//            strcpy(buffer, src);
            strncpy(buffer, src, size);

//            *(buffer + size + 1) = '\0';

        } else {

            buffer = cursor = nullptr;
        }


        bufferSize = size;

    }

    void _safeDestruct() {

        if (buffer != nullptr       //If not empty, delete and reset
                && bufferSize > 0) {

            delete [] buffer;

            bufferSize = 0;
            buffer = cursor = nullptr;

        } else {    //Else, make sure you were completely empty to start with!

            assert (bufferSize == 0
                    && buffer == nullptr
                    && cursor == nullptr);
        }


    }

    void _initTable() {

        _initAllToNeg1();
        _initTableStateLabels();

        //State 0
        _initSuccessLabel(0, 0);
        _initTableAthruZ(0, 1);
        _initTable0thru9(0, 12);
        _initTableChar(0, '\"', 14);    //Uncomment for Quote tokens
        _initTableChar(0, '-', 17);
        _initTableChar(0, ' ', 13);
        _initTableChar(0, ',', 19);     //Uncomment for , tokens
        _initTableChar(0, '*', 20);     //Uncomment for * tokens

        //State 1
        _initSuccessLabel(1, 1);
        _initTableAthruZ(1, 1);
        _initTableChar(1, '\'', 3);
        _initTableChar(1, '-', 2);

        //State 2
        _initSuccessLabel(2, 0);
        _initTableAthruZ(2, 16);

        //State 3
        _initSuccessLabel(3, 1);
        _initTableAthruZ(3, 3);

        //State 4
        _initSuccessLabel(4, 1);
        _initTable0thru9(4, 4);
        _initTableChar(4, '.', 5);

        //State 5
        _initSuccessLabel(5, 0);
        _initTable0thru9(5, 6);

        //State 6
        _initSuccessLabel(6, 1);
        _initTable0thru9(6, 6);

        //State 7
        _initSuccessLabel(7, 0);
        _initTable0thru9(7, 18);

        //State 8
        _initSuccessLabel(8, 0);
        _initTable0thru9(8, 9);

        //State 9
        _initSuccessLabel(9, 1);
        _initTableChar(9, ',', 7);
        _initTableChar(9, '.', 5);

        //State 10
        _initSuccessLabel(10, 1);
        _initTable0thru9(10, 4);
        _initTableChar(10, ',', 7);
        _initTableChar(10, '.', 5);

        //State 11
        _initSuccessLabel(11, 1);
        _initTable0thru9(11, 10);
        _initTableChar(11, ',', 7);
        _initTableChar(11, '.', 5);

        //State 12
        _initSuccessLabel(12, 1);
        _initTable0thru9(12, 11);
        _initTableChar(12, ',', 7);
        _initTableChar(12, '.', 5);

        //State 13
        _initSuccessLabel(13, 1);
        _initTableChar(13, ' ', 13);    //Uncomment for multi-space tokens, comment for single-space

        //State 14
        _initSuccessLabel(14, 0);
        _initEntireRow(14, 14);
        _initTableChar(14, '\"', 15);

        //State 15
        _initSuccessLabel(15, 1);

        //State 16
        _initSuccessLabel(16, 1);
        _initTableAthruZ(16, 16);
        _initTableChar(16, '-', 2);

        //State 17
        _initSuccessLabel(17, 1);
        _initTable0thru9(17, 12);

        //State 18
        _initSuccessLabel(18, 0);
        _initTable0thru9(18, 8);

        //State 19
        _initSuccessLabel(19, 1);

        //State 20
        _initSuccessLabel(20, 1);
        _initTableChar(20, '*', 20);



    }

    void _initAllToNeg1() {

        for (int i = 0; i < NUM_OF_ROWS; i++) {

            for (int j = 0; j < NUM_OF_COLUMNS; j++) {

                stateTable[i][j] = -1;
            }
        }
    }

    void _initTableStateLabels() {

        for (int i = 0; i < NUM_OF_ROWS; i++) {

            stateTable[i][STATE_COLUMN] = i;
        }
    }

    void _initSuccessLabel(int state, int val) {

        stateTable[state][SUCFAIL_COLUMN] = val;
    }

    void _initTableAthruZ(int row, int val) {

        for (int i = (int) 'A'; i <= (int) 'z'; i++) {

            stateTable[row][i + 2] = val;
        }

    }

    void _initTable0thru9(int row, int val) {

        for (int i = (int) '0'; i <= (int) '9'; i++) {

            stateTable[row][i + 2] = val;
        }
    }

    void _initTableChar(int row, char character, int val) {

        int charIndex = (int) character;
        stateTable[row][charIndex + 2] = val;

    }

    /**
     * @brief _initEntireRow Initializes entire row of stateTable to val, not including state or sucfail columns
     * @param row Row to be initialized
     * @param val Value which will be placed into the entire row
     */
    void _initEntireRow(int row, int val) {

        for (int i = 0; i < NUM_OF_COLUMNS; i++) {

            stateTable[row][i + 2] = val;
        }
    }

    /**
     * @brief _extractString Constructs a string whose endpoints are given
     * @param begin First index
     * @param end Last index
     * @return A string containing all characters from begin to end, inclusive
     * Note that the string need not be null-terminated
     * PRECONDITIONS:   begin is the first character of a string
     *                  end is the end of a string
     *                  neither begin nor end are nullptr
     */
    string _extractString(char * begin, char * end) {

        assert(begin != nullptr && end != nullptr);

        int len = end - begin + 1;

        assert(len > 0);

        return string(begin, static_cast <size_t> (len));
        //http://www.cplusplus.com/reference/string/string/string/
        //Uses 5th constructor, "from buffer"
    }

    /**
     * @brief tokenize If there is a token in buffer that cursor is pointing to the start,
     * tokenizes and moves cursor to the end of the token
     * Else, does not move cursor
     * @param tokenHolder If token is found, updates tokenHolder to the token that was created
     * @return True if it was able to tokenize, false otherwise
     * Note that whether the token found was 1 char or not,
     * the client is responsible for moving the cursor once the function exits
     */
    bool _tokenize(Token& tokenHolder) {

        char * beginMarker = cursor;
        char * lastSuccessfulCursor = cursor;

        int currentState = 0;
        int lastSuccessfulState = 0;

        while (!done()) {

            char currentChar = *cursor;

            if (!_isASCII(currentChar)) {

                return false;
            }

            currentState = stateTable[currentState][2 + (int) currentChar];

            if (currentState == -1) {

                break;
            }

            //If we're in a success state,
            //keep track of our last success state,
            //and our cursor when last we were successful
            if (stateTable[currentState][SUCFAIL_COLUMN] == 1) {

                lastSuccessfulState = currentState;
                lastSuccessfulCursor = cursor;

            }

            //Move on in buffer
            cursor++;
        }


        cursor = lastSuccessfulCursor;  //Without this line, cursor overflows buffer by 1 on last token

        string str = _extractString(beginMarker, cursor);

        tokenHolder = Token(str, _stateToTokenType(lastSuccessfulState));

        return true;
    }

    int _stateToTokenType(int state) {

        switch(state) {

        case 0:
        case 17:
            return Token::TOKEN_PUNCTUATION;
        case 2:
        case 5:
        case 7:
        case 8:
        case 14:
            return Token::TOKEN_UNKNOWN;
        case 1:
        case 3:
        case 16:
            return Token::TOKEN_WORD;
        case 4:
        case 6:
        case 9:
        case 10:
        case 11:
        case 12:
            return Token::TOKEN_NUMBER;
        case 13:
            return Token::TOKEN_SPACE;
        case 15:
            return Token::TOKEN_QUOTE;
        case 19:
            return Token::TOKEN_COMMA;
        case 20:
            return Token::TOKEN_ASTERISK;
        default:
            assert(false);
            return -3;
        }

    }

    bool _isASCII(char testMe) {

        int asciiIndex = static_cast <int> (testMe);

        const int ASCII_MIN = 0;
        const int ASCII_MAX = 127;

        return ASCII_MIN <= asciiIndex
                && asciiIndex <= ASCII_MAX;
    }

};

#endif //STR_TOKENIZER_H
