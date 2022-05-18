#pragma once

#include <Arduino.h>

namespace Intercom{

    extern bool connected;

    void init();
    
    void checkSerial();
    void parseRequest(String);
    void askOpponent();
    void setFOV(float angle, float width, float maxDist);
    
} // namespace Intercom
