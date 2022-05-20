#pragma once
#include <Arduino.h>
#include "Geometry.h"
#include <EasyStringStream.h>

namespace Debugger{
    void init();
    void header();
    void checkSerial();
    void parseCommand(String);
    String lastCommand();

    void print(String);
    void print(int);
    void print(float);
    void print(Vec3);

    void println(String);
    void println(int);
    void println(float);
    void println(Vec3);

    void printBuffer();

    extern EasyStringStream log;
}


