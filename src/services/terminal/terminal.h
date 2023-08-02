#pragma once
#include "system/core/service.h"
#include "console.h"



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

class Terminal : public Service{
public :
    
    Terminal();
    ~Terminal(){};

    void update() override;

    Command dequeCommand();
    int commandAvailable();

private:
    std::deque<Command> _pendingCommands;
};