#pragma once
#include <Arduino.h>
#include "Geometry.h"

namespace Debugger{
    void init();

    void printHeader();
    void checkSerial();
    void parseCommand(String);
    String lastCommand();

    void log(String);
    void log(float);
    void log(Vec3);

} // namespace Debugger




