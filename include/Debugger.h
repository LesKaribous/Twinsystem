#pragma once
#include <Arduino.h>
#include "Geometry.h"

namespace Debugger{
    void init();
    void header();
    void checkSerial();
    void parseCommand(String);
    String lastCommand();

    void println(String = "");
    void println(char);
    void println(int);
    void println(float);
    void println(Vec3);

    void print(String);
    void print(float);
    void print(char);
    void print(int);
    void print(Vec3);

    void log(char);
    void log(int);
    void log(float);
    void log(String);
    void log(Vec3);

    void log(String prefix,   String data, String suffix = "");
    void log(String prefix,   int data, String suffix = "");
    void log(String prefix, float data, String suffix = "");

    void logArray(String prefix, int array[], size_t size, char separator = ',', String suffix = "");
    void logArrayN(String prefix, int element, String interFix, int array[], size_t size, char separator = ',', String suffix = "");


}


