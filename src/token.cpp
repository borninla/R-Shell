//
// Created by Andrew Lvovsky on 11/15/17.
//

#include "../header/token.h"

string CompositeToken::getString() {
    return std::string();
}

CompositeToken::status CommandToken::getStatus() {
    return wasSuccess;
}

CompositeToken::status ConnectorToken::getStatus() {
    return wasSuccess;
}
