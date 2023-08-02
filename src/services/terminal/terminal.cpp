#include "terminal.h"
#include "os.h"


void parseCommand(String& command, String& cmd, int& argc, String& args){
    String raw = command.trim();
    // Split the command into command name and arguments
    
    cmd = "";  // initialize argCount
    argc = 0;  // initialize argCount
    args = "";  // initialize argCount
    if (raw.indexOf('(') != -1 && raw.indexOf(')') != -1) {
        cmd = raw.substring(0, raw.indexOf('('));

        if (raw.indexOf(')') - raw.indexOf('(') - 1 <= 0){
           return;
        };

        args = raw.substring(raw.indexOf('(') + 1, raw.indexOf(')'));  // also exclude the last parenthesis

        
        // Add 1 to argCount if any args exist
        argc++;
        if (args.length() > 0) {
            // Count the commas in commandArgs
            int i = 0;
            while(true){
                i = args.indexOf(",", i);
                if(i != -1) {
                    argc++;
                    i++;
                }else break;
            };
        }

    } else {
        cmd = raw;
    }
}



Command::Command(String& _raw){
    raw = _raw;
    parseCommand(_raw, commandName, argCount, arguments);
}

const String& Command::toString(){
    return raw;
}

String Command::getArgument(int argIndex) {
    int start = 0;
    int end = arguments.indexOf(',', start);

    // Find the start and end position of the argument at the specified index
    for (int i = 0; i < argIndex; i++) {
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


Vec3 Command::getVec3(int argIndex) {
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

Vec2 Command::getVec2(int argIndex) {
    if (argCount == 2) {
        // Parse arguments for Vec2 type
        Vec2 result;
        result.x = getFloat(0);
        result.y = getFloat(1);
        return result;
    }
    return Vec2();
}

float Command::getFloat(int argIndex) {
    if (argCount >= argIndex) {
        // Parse argument for int type
        String arg = getArgument(argIndex);
        return arg.toFloat();
    }
    return 0;
}

bool Command::getBool(int argIndex) {
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

String Command::getString(int argIndex) {
    if (argCount >= argIndex) {
        // Parse argument for char type
        return getArgument(argIndex);
    }
    return "";
}

char Command::getChar(int argIndex) {
    if (argCount >= argIndex) {
        // Parse argument for char type
        String arg = getArgument(argIndex);
        if (arg.length() > 0) {
            return arg.charAt(0);
        }
    }
    return '\0';
}

//ex : go(x,y) | "go("
bool Command::isValidFormat(String commandFormat){
    // Check for opening parenthesis, comma and closing parenthesis
    commandFormat = commandFormat.trim();

    // Split the command into command name and arguments
    
    String exp_command;
    int exp_argc = 0;  // initialize argCount
    String exp_args = 0;  // initialize argCount

    parseCommand(commandFormat, exp_command, exp_argc, exp_args);

    if(!commandName.equalsIgnoreCase(exp_command)){
        os.console.trace("Terminal") << "Command name mismatch : " << raw << " | " << commandFormat << os.console.endl;
        return false;
    }

    if(argCount < exp_argc){
        os.console.error("Terminal") << "Not enough arguments in function call : expected " << exp_argc << " provided " << argCount << os.console.endl;
        os.console.error("Terminal") << "Syntax is " << commandFormat << os.console.endl;
        return false;
    }

    if(argCount > exp_argc){
        os.console.error("Terminal") << "Too many arguments in function call : expected" << exp_argc << " provided " << argCount << os.console.endl;
        os.console.error("Terminal") << "Syntax is " << commandFormat << os.console.endl;
        return false;
    }
    return true;
}



Terminal::Terminal() : Service(TERMINAL){
    os.screen.addBootProgress(10);
	os.screen.drawBootProgress("Loading Terminal...");
}

void Terminal::update(){
    if(!m_enabled) return;

    
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');  // Read the serial command
        
        _pendingCommands.emplace_back(command);  // Assuming _pendingCommands holds a struct or class that can take these 3 parameters
        if(_pendingCommands.size() > 20) _pendingCommands.pop_front();
    }
}   

Command Terminal::dequeCommand(){
    Command command = _pendingCommands.front();
    _pendingCommands.pop_front();
    return command;
}

int Terminal::commandAvailable(){
    return _pendingCommands.size();
}
