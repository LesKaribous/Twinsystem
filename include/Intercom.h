#pragma once

#include <Arduino.h>

namespace Intercom{

    extern bool connected;

    void init();
    void focus();
    void checkSerial();
    void parseRequest(String);
    void setFOV(float angle, float width);

    bool collision();
    
} // namespace Intercom
