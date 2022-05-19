#pragma once

#include <Arduino.h>

namespace Intercom{

    extern bool connected;

    void init();
    void focus();
    void checkSerial();
    void parseRequest(String);
<<<<<<< HEAD
    void setFOV(float angle, float width);

    bool collision();
=======
    void askOpponent();
    void setFOV(float angle, float width, float maxDist);
>>>>>>> 35dc2e9b5c0d19d7c907990ca15c2470d1eb72e7
    
} // namespace Intercom
