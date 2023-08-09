#pragma once
#include "system/core/lib.h"
#include "system/core/condition.h"
#include "system/math/geometry.h"

class Condition;

class Command{
    String commandName;
    String arguments;
    int argCount;

    String raw;

public:

    Command(String& raw);
    const String& toString()const;

    String getArgument(int argIndex)const;
    Condition getCondition()const;

    Vec3 getVec3(int argIndex = 0)const;
    Vec2 getVec2(int argIndex = 0)const;
    bool getBool(int argIndex = 0)const;
    char getChar(int argIndex = 0)const;
    float getFloat(int argIndex = 0)const;
    int getInt(int argIndex = 0)const;
    String getString(int argIndex = 0)const;
    bool isValidFormat(String commandFormat) const;
    
};


class Interpreter{
private:
    
public:
    Interpreter();
    ~Interpreter();

    void processCommand(Command c);
    void processCondition(Condition c);
};

