#include "core/module.h"
#include "core/lib.h"



class Command{
    String commandName;
    String arguments;
    int argCount;

public:

    Command(String _commandName, String _commandArgs, int _argCount){
        commandName = _commandName;
        arguments = _commandArgs;
        argCount = _argCount;

    }

    String getArgument(int argIndex) {
        int start = 0;
        int end = arguments.indexOf(',', start);

        // Find the start and end position of the argument at the specified index
        for (int i = 1; i < argIndex; i++) {
        if (end == -1) {
            return ""; // Argument index out of range
        }
        start = end + 1;
        end = arguments.indexOf(',', start);
        }

        if (end == -1) {
        end = arguments.length();
        }

        // Extract the argument substring
        return arguments.substring(start, end).trim();
    }


    Vec3 getVec3(int argIndex = 0) {
        if (argCount == 3) {
            // Parse arguments for Vec3 type
            Vec3 result;
            result.x = getFloat(0);
            result.y = getFloat(1);
            result.z = getFloat(2);
            return result;
        }
        return Vec3();
    }

    Vec2 getVec2(int argIndex = 0) {
        if (argCount == 2) {
            // Parse arguments for Vec2 type
            Vec2 result;
            result.x = getFloat(0);
            result.y = getFloat(1);
            return result;
        }
        return Vec2();
    }

    float getFloat(int argIndex = 0) {
        if (argCount >= argIndex) {
            // Parse argument for int type
            String arg = getArgument(argIndex);
            return arg.toFloat();
        }
        return 0;
    }
    
    bool getBool(int argIndex = 0) {
        if (argCount >= argIndex) {
            // Parse argument for bool type
            String arg = getArgument(argIndex);
            arg.toLowerCase();
            if (arg == "true" || arg == "1") {
                return true;
            } else if (arg == "false" || arg == "0") {
                return false;
            }
        }
        return false;
    }
    
    String getString(int argIndex = 0) {
        if (argCount >= argIndex) {
            // Parse argument for char type
            return getArgument(argIndex);
        }
        return "";
    }

    char getChar(int argIndex = 0) {
        if (argCount >= argIndex) {
            // Parse argument for char type
            String arg = getArgument(argIndex);
            if (arg.length() > 0) {
                return arg.charAt(0);
            }
        }
        return '\0';
    }





    bool isValidFormat(String commandFormat){
        // Check for opening parenthesis, comma and closing parenthesis
        commandFormat = commandFormat.trim();
        int openParIndex = commandFormat.indexOf('(');
        int closeParIndex = commandFormat.indexOf(')');
        if (openParIndex == -1 || closeParIndex == -1) {
            Console::error("Terminal") << "Invalid command format : missing parenthesis)" << Console::endl;
            return false; // Missing parentheses
        }
        String command = commandFormat.substring(0, openParIndex);

        if(command != commandName){
            return false;
        }

        String formatArgs = commandFormat.substring(openParIndex + 1, closeParIndex);
        int formatArgCount = 0;

        // Count the commas in formatArgs and add 1 if any args exist
        for (int i = 0; i < formatArgs.length(); i++) {
            if (formatArgs.charAt(i) == ',') {
                formatArgCount++;
            }
        }
        if (formatArgs.length() > 0) {
            formatArgCount++;
        }

        if(argCount < formatArgCount){
            Console::error("Terminal") << "Not enough arguments in function call : expected" << formatArgCount << " provided " << argCount << Console::endl;
            return false;
        }

        if(argCount < formatArgCount){
            Console::error("Terminal") << "Too many arguments in function call : expected" << formatArgCount << " provided " << argCount << Console::endl;
            return false;
        }



        // Compare argument count of command with that of format
        return true;
    }
};


class Terminal : public Module{
public:
    Terminal() : Module(TERMINAL){}

    void update() override{
        if(!m_enabled) return;

        if (Serial.available() > 0) {
            String command = Serial.readStringUntil('\n');  // Read the serial command
            command = command.trim();
            // Split the command into command name and arguments
            String commandName;
            String commandArgs;
            int spaceIndex = command.indexOf('(');
            int argCount = 0;  // initialize argCount
            if (spaceIndex != -1) {
                commandName = command.substring(0, spaceIndex);
                commandArgs = command.substring(spaceIndex + 1, command.length() - 1);  // also exclude the last parenthesis

                // Count the commas in commandArgs
                for (int i = 0; i < commandArgs.length(); i++) {
                    if (commandArgs.charAt(i) == ',') {
                        argCount++;
                    }
                }

                // Add 1 to argCount if any args exist
                if (commandArgs.length() > 0) {
                    argCount++;
                }

            } else {
                commandName = command;
            }
            _pendingCommands.emplace_back(commandName, commandArgs, argCount);  // Assuming _pendingCommands holds a struct or class that can take these 3 parameters
            if(_pendingCommands.size() > 20) _pendingCommands.pop_front();
        }
    }   

    Command dequeCommand(){
        Command command = _pendingCommands.front();
        _pendingCommands.pop_front();
        return command;
    }

    int commandAvailable(){
        return _pendingCommands.size();
    }

private:
    std::deque<Command> _pendingCommands;
};
