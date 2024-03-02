#pragma once
#include "utils/geometry.h"
#include "commandHandler.h"

#include <map>
#include <vector>
#include <Arduino.h>

using command_func_ptr = void (*)(const String& args) ;

class Command {
public:
    friend class CommandHandler;
    friend class Interpreter;
    friend class Program;
    Command(){};
protected:
    void execute(String arguments = "");
    Command(const String& synt, const String& desc, int argsCounts, command_func_ptr func);

    String syntax;
    String description;
    int numberOfArguments;

private:
    command_func_ptr callback = nullptr;
};

class CommandHandler {
private :
    
    CommandHandler();
protected :
   static void execute(const String& command, const String& args);

public:
    friend class Program;
    static void registerCommand(const String& syntax, const String& description, command_func_ptr func);
    static std::vector<String> extractArguments(const String& args);
    static bool hasCommand(const String& command);
    static std::map<String, Command>& getCommands();
};

