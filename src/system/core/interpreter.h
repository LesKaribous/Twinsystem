#pragma once
#include "system/core/lib.h"
#include "system/math/geometry.h"

class Command{
    String commandName;
    String arguments;
    int argCount;

    String raw;

public:

    Command(String& raw);
    const String& toString();

    String getArgument(int argIndex);

    Vec3 getVec3(int argIndex = 0);
    Vec2 getVec2(int argIndex = 0);
    bool getBool(int argIndex = 0);
    char getChar(int argIndex = 0);
    float getFloat(int argIndex = 0);
    String getString(int argIndex = 0);
    bool isValidFormat(String commandFormat);
    
};


class Interpreter{
private:
    
public:
    Interpreter();
    ~Interpreter();

    void processCommand(Command c);

};

