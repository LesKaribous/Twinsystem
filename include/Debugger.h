#pragma once
#include <Arduino.h>
#include "Geometry.h"

#define VERBOSE Debugger::Level::LVERBOSE
#define INFO Debugger::Level::LINFO
#define WARN Debugger::Level::LWARN
#define ERROR Debugger::Level::LERROR

namespace Debugger{

    enum class Level{
        LVERBOSE = 0,
        LINFO = 1,
        LWARN = 2,
        LERROR = 3
    };

    Level level();

    void init(Level = INFO);
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

    void log(char, Level level = Level::LVERBOSE);
    void log(int, Level level = Level::LVERBOSE);
    void log(float, Level level = Level::LVERBOSE);
    void log(String, Level level = Level::LVERBOSE);
    void log(Vec3, Level level = Level::LVERBOSE);

    void log(String prefix,   int data, Level level = Level::LVERBOSE);
    void log(String prefix,  char data, Level level = Level::LVERBOSE);
    void log(String prefix, float data, Level level = Level::LVERBOSE);
    void log(String prefix, Vec3 data, Level level = Level::LVERBOSE);
    void log(String prefix, String data, Level level = Level::LVERBOSE);

    void log(String prefix,   int data, String suffix, Level level = Level::LVERBOSE);
    void log(String prefix,  char data, String suffix, Level level = Level::LVERBOSE);
    void log(String prefix, float data, String suffix, Level level = Level::LVERBOSE);
    void log(String prefix, Vec3 data, String suffix, Level level = Level::LVERBOSE);
    void log(String prefix, String data, String suffix, Level level = Level::LVERBOSE);

    void logArray(String prefix, int array[], size_t size, char separator = ',', String suffix = ")", Level level = Level::LVERBOSE);
    void logArrayN(String prefix, int element, String interFix, int array[], size_t size, char separator = ',', String suffix = ")", Level level = Level::LVERBOSE);


}


