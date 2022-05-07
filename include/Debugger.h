#pragma once
#include <Arduino.h>

namespace Debugger{
    void init();

    void printHeader();
    void checkSerial();
    void parseCommand(String);
    String lastCommand();

    void log(String);
    void log(float);


} // namespace Debugger




