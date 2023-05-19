#include "core/module.h"
#include "core/lib.h"

struct Command{
    String commandName;
    String arguments;
    
    int argToInt(){

    }
    bool argToBool(){
        
    }
    float argToFloat(){

    }

    Vec2 argToVec2(){

    }
    Vec3 argToVec3(){

    }
};

class CommandLine : public Module{
public:
    CommandLine() : Module(CLI){}

    void update() override{
        if (Serial.available() > 0) {
            String command = Serial.readStringUntil('\n');  // Read the serial command

            // Split the command into command name and arguments
            String commandName;
            String commandArgs;
            int spaceIndex = command.indexOf('(');
            if (spaceIndex != -1) {
                commandName = command.substring(0, spaceIndex);
                commandArgs = command.substring(spaceIndex + 1);
            } else {
                commandName = command;
            }
            _pendingCommands.push_back({commandName, commandArgs});
            if(_pendingCommands.size() > 20) _pendingCommands.pop_front();
        }
    }

    Command dequeCommand(){
        Command command = _pendingCommands.front();
        _pendingCommands.pop_front();
        return command;
    }


private:
    std::deque<Command> _pendingCommands;
};
