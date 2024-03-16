#include "commandHandler.h"
#include "console.h"
#include "utils/interpreter/expression.h"
#include <algorithm>
#include <map>


Command::Command(const String& synt, const String& desc, int argsCounts, command_func_ptr func) : 
    syntax(synt),
    description(desc),
    numberOfArguments(argsCounts),
    callback(func)
{}


std::map<String, Command>& CommandHandler::getCommands() {
    static std::map<String, Command> commands;
    return commands;
}

void CommandHandler::registerCommand(const String& syntax, const String& description, command_func_ptr func) {
    int numberOfArguments = std::count(syntax.begin(), syntax.end(), ',') + 1;
    int i = syntax.indexOf("(");
    String name = syntax;
    if(i != -1)
        name = syntax.substring(0,i);
    
    Command cmd = Command(syntax, description, numberOfArguments, func);
    getCommands()[name] =  cmd;
}

bool CommandHandler::hasCommand(const String &command){
    return getCommands().find(command) != getCommands().end();
}

void CommandHandler::execute(const String& command, const String& args) {
    if(hasCommand(command)){
        getCommands()[command].callback(args);
    }else{
        Console::println("Unknown command: " + command);
    }
}

std::vector<String> CommandHandler::extractArguments(const String& args) {
    std::vector<String> arguments;
    int start = 0;
    int end = args.indexOf(',');
    bool ignoreVectorComma = false;
    if(end != -1){
        String argBuf = "";
        uint index = 0;
        bool ignoreVectorComma = false;
        while (index != args.length()) {
            if (args.charAt(index) == '['){
                ignoreVectorComma = true;
            }
            if (args.charAt(index) == ']'){
                ignoreVectorComma = false;
            }
            if (args.charAt(index) == ',' && !ignoreVectorComma) {
                arguments.push_back(argBuf);
                argBuf = "";
            }else argBuf += args.charAt(index);
            index++;
        }
        if(argBuf.length() > 0) arguments.push_back(argBuf);
    }
    return arguments;
}


