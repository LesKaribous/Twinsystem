#pragma once

#include <Arduino.h>

namespace Intercom{

    extern bool connected;

    void init();
    void focus();
    void reboot();
    void checkSerial();
    void parseRequest(String);

    bool collision();
    void lookAt(float angle, float dist);
    void setFOV(float angle);
    void setThreshold(float t);

    extern int count;
    
} // namespace Intercom
