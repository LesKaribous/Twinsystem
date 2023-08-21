#pragma once
#include "system/core/lib.h"
#include "commandHandler.h"

//To add a new command, please create a new function in the function handler.
//Then register the function in the 


struct CommandInfo {
    String syntax;
    String description;
    int numberOfArguments;
};

class CommandHandler {
private :
    std::vector<String> extractArguments(const String& args);

protected :
    void execute(const String& command, const String& args);

public:
    friend class Program;
    CommandHandler();

    static void registerCommand(const String& syntax, const String& description);
    static std::map<String, CommandInfo>& getCommands();

    //Service
    void execute_enable(const String& service);
    void execute_disable(const String& service);
    void execute_status(); //Display all status
    void execute_status(const String& service); //Display single status

    void execute_debug(); //Display all status
    void execute_debug(const String& service); //Display single status


    //Motion
    void execute_go(float x, float y);
    void execute_move(float x, float y, float angle);
    void execute_turn(float angle);
    void execute_pause();
    void execute_resume();
    void execute_cancel();
    void execute_sleep();
    void execute_align(const String& side, float angle);
    void execute_setAbsolute();
    void execute_setRelative();
    void execute_setAbsPosition(float x, float y, float angle);
    void execute_resetCompass();

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
    void execute_print(const String& str);
    
    // ... Other command execution methods ...
};

