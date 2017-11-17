#include "delim.h"

Delim::Delim() {

    _init(NULL, ' ');
}

Delim::Delim(char *cstr, char delim) {

    _init(cstr, delim);
}

Delim::Delim(string str, string delim) {

    _init(str, delim);
}

bool Delim::done() const {

    return q.empty();
}

Delim& operator >>(Delim& delim, string& str) {

    assert(!delim.q.empty());

    str = delim.q.front();
    delim.q.pop();

    return delim;

}

void Delim::dumpToConsole() {

    while (!q.empty()) {

        cout << q.front() << endl;
        q.pop();
    }
}

void Delim::_init(char *cstr, char delim) {

    if (cstr == NULL)
        return;

    char* walker = cstr;

    //Walk through the cstr
    while(*walker != '\0') {

        string currentStr = "";

        //Walk through a section
        while(*walker != delim
              && *walker != '\0') {

            //                const char * c = (const char *) walker; //forgive me for I have sinned
            //                currentStr.append(c);
            char currentChar = *walker;
            currentStr += currentChar;

            walker++;

        }

        if (!currentStr.empty()) {

            q.push(currentStr);
        }
        walker++;
    }

}

void Delim::_init(string str, string delim) {

    if (str == "")
        return;

    assert(str.size() > delim.size());

    
}
