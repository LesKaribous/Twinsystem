#pragma once
#include "system/core/lib.h"
#include "commandHandler.h"

class CommandHandler {

    std::vector<String> extractArguments(const String& args);
public:
    CommandHandler(){};

    void execute(const String& command, const String& args);

    //Service
    void execute_enable(const String& service);
    void execute_disable(const String& service);
    void execute_status(); //Display all status
    void execute_status(const String& service); //Display single status

    //Motion
    void execute_go(float x, float y);
    void execute_turn(float angle);
    void execute_pause();
    void execute_resume();
    void execute_cancel();
    void execute_sleep();
    void execute_align(const String& side, float angle);
    void execute_setAbsolute();
    void execute_setRelative();
    void execute_setAbsPosition(float x, float y, float angle);

    //Actuators
    void execute_grab(const String& side);
    void execute_ungrab(const String& side);
    void execute_open(const String& side);
    void execute_close(const String& side);
    void execute_openTrap(const String& side);
    void execute_closeTrap(const String& side);
    
    bool execute_testTRUE();
    bool execute_testFALSE();

    //Terminal
    void execute_help(); //Display help
    
    // ... Other command execution methods ...
};

struct CommandInfo {
    String syntax;
    String description;
    int numberOfArguments;
};