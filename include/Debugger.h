#pragma once
#include <Arduino.h>

namespace Debugger{
    void init();

    void checkSerial();
    void execCommand();

    void log(String);
    void log(int);
    void log(float);


} // namespace Debugger




