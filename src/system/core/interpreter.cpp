#include "interpreter.h"
#include "os.h"

void parseCommand(String& command, String& cmd, int& argc, String& args){
    String raw = command.trim();
    // Split the command into command name and arguments
    
    cmd = "";  // initialize argCount
    argc = 0;  // initialize argCount
    args = "";  // initialize argCount
    if (raw.indexOf('(') != -1 && raw.lastIndexOf(')') != -1) {
        cmd = raw.substring(0, raw.indexOf('('));

        if (raw.lastIndexOf(')') - raw.indexOf('(') - 1 <= 0){
           return;
        };

        args = raw.substring(raw.indexOf('(') + 1, raw.lastIndexOf(')'));  // also exclude the last parenthesis

        
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
    raw = _raw.trim();
    raw.replace(" ", "");
    parseCommand(_raw, commandName, argCount, arguments);
}

const String& Command::toString() const{
    return raw;
}

Condition Command::getCondition() const{
    return Condition(arguments);
}

String Command::getArgument(int argIndex) const{
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


Vec3 Command::getVec3(int argIndex) const{
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

Vec2 Command::getVec2(int argIndex) const{
    if (argCount == 2) {
        // Parse arguments for Vec2 type
        Vec2 result;
        result.x = getFloat(0);
        result.y = getFloat(1);
        return result;
    }
    return Vec2();
}

float Command::getFloat(int argIndex) const{
    if (argCount >= argIndex) {
        // Parse argument for int type
        String arg = getArgument(argIndex);
        return arg.toFloat();
    }
    return 0;
}

bool Command::getBool(int argIndex) const{
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

String Command::getString(int argIndex) const{
    if (argCount >= argIndex) {
        // Parse argument for char type
        return getArgument(argIndex);
    }
    return "";
}

char Command::getChar(int argIndex) const{
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
bool Command::isValidFormat(String commandFormat) const{
    // Check for opening parenthesis, comma and closing parenthesis
    commandFormat = commandFormat.trim();
    commandFormat.replace(" ", "");

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







Interpreter::Interpreter(){

}

Interpreter::~Interpreter(){
    
}

void Interpreter::processCondition(Condition c){ 
    for(Command& a : c.commands){
        if(a.isValidFormat("motion")){
            c.addCommandOuput(String(true));
            os.console.print(c.outputs[c.outputs.size()-1]);
        }else if(a.isValidFormat("actuators")){
            c.addCommandOuput(String(true));
             os.console.print(c.outputs[c.outputs.size()-1]);
        }else if(a.isValidFormat("lidar")){
            c.addCommandOuput(String(false));
             os.console.print(c.outputs[c.outputs.size()-1]);
        }
    }
    os.console.println("");

    os.console.print("Commands : {");
    for(Command& d:  c.commands) os.console.print(d.toString() + ",");
    os.console.println("}");

    os.console.print("Operators : {");
    for(String& d:  c.operators) os.console.print(d + ",");
    os.console.println("}");


    c.solve();
}



void Interpreter::processCommand(Command c){ 
    if(c.isValidFormat("if(condition)")){
        processCondition(c.getCondition()); //Change state according to condition
    }else if(c.isValidFormat("else")){
        
    }else if(c.isValidFormat("endif")){
        
    }else if(c.isValidFormat("go(x,y)")){
        os.console.trace("Interpreter") << "Command parsed at " << int(millis()) << "ms" << os.console.endl;
        os.motion.go(c.getVec2());

    }else if(c.isValidFormat("goTurn(x,y,angle)")){
        os.motion.move(c.getVec3());

    }else if(c.isValidFormat("turn(angle)")){
        os.motion.turn(c.getFloat());

    }else if(c.isValidFormat("setAbsPosition(x,y)")){
        os.motion.setAbsPosition(Vec3(c.getFloat(0), c.getFloat(1), os.motion.getAbsPosition().c));

    }else if(c.isValidFormat("setAbsPosition(x,y,t)")){
        os.motion.setAbsPosition(Vec3(c.getFloat(0), c.getFloat(1), c.getFloat(2)));

    }else if(c.isValidFormat("align(side, absAngle)")){
        String side = c.getString();
        float orientation = c.getFloat(1);

        if(side.equals("A"))         os.motion.align(RobotCompass::A, orientation);
        else if(side.equals("AB"))   os.motion.align(RobotCompass::AB, orientation);
        else if(side.equals("B"))    os.motion.align(RobotCompass::B, orientation);
        else if(side.equals("BC"))   os.motion.align(RobotCompass::BC, orientation);
        else if(side.equals("C"))    os.motion.align(RobotCompass::C, orientation);
        else if(side.equals("CA"))   os.motion.align(RobotCompass::CA, orientation);

    }else if(c.isValidFormat("status")){
        for ( int id = ServiceID::LIDAR; id != ServiceID::NOT_A_SERVICE; id++ ){
            ServiceID sID = static_cast<ServiceID>(id);
            os.console.info("Interpreter") << Service::toString(sID) <<  " : " << (os.statusService(sID) ? "ON" : "OFF") << os.console.endl;
        }

    }else if(c.isValidFormat("status(service)")){
        String service = c.getString();
        ServiceID serviceID = Service::toID(service);
        os.console.info("Interpreter") << service <<  " status : " << os.statusService(serviceID) << os.console.endl;

    }else if(c.isValidFormat("enable(service)")){
        String service = c.getString();
        ServiceID serviceID = Service::toID(service);
        if(serviceID != NOT_A_SERVICE){
            os.enable(serviceID);
            os.console.info("Interpreter") << service <<  " enabled" << os.console.endl;
        }else  os.console.error("Interpreter") << "unknown service" << os.console.endl;

    }else if(c.isValidFormat("disable(service)")){
        String service = c.getString();
        ServiceID serviceID = Service::toID(service);
        if(serviceID != NOT_A_SERVICE){
            os.disable(serviceID);
            os.console.info("Interpreter") << service <<  " disabled" << os.console.endl;
        }else  os.console.error("Interpreter") << "unknown service : " << service << os.console.endl;
             

    }else if(c.isValidFormat("close(side)")){
        String side = c.getString();
        if(side.equals("AB")) os.actuators.close(RobotCompass::AB);
        else if(side.equals("BC")) os.actuators.close(RobotCompass::BC);
        else if(side.equals("CA")) os.actuators.close(RobotCompass::CA);

    }else if(c.isValidFormat("open(side)")){
        String side = c.getString();
        THROW(side)
        if(side.equals("AB")) os.actuators.open(RobotCompass::AB);
        else if(side.equals("BC")) os.actuators.open(RobotCompass::BC);
        else if(side.equals("CA")) os.actuators.open(RobotCompass::CA);

    }else if(c.isValidFormat("grab(side)")){
        String side = c.getString();
        if(side.equals("AB")) os.actuators.grab(RobotCompass::AB);
        else if(side.equals("BC")) os.actuators.grab(RobotCompass::BC);
        else if(side.equals("CA")) os.actuators.grab(RobotCompass::CA);

    }else if(c.isValidFormat("ungrab(side)")){
        String side = c.getString();
        if(side.equals("AB")) os.actuators.ungrab(RobotCompass::AB);
        else if(side.equals("BC")) os.actuators.ungrab(RobotCompass::BC);
        else if(side.equals("CA")) os.actuators.ungrab(RobotCompass::CA);

    }else if(c.isValidFormat("openTrap()")){
        os.actuators.openTrap();

    }else if(c.isValidFormat("closeTrap()")){
        os.actuators.closeTrap();

    }else if(c.isValidFormat("wait")){
        os.waitUntil(os.motion);

    }else if(c.isValidFormat("sleep")){
        os.motion.sleep();

    }else if(c.isValidFormat("pause")){
        os.motion.pause();
        os.console.info("Motion") << "motion paused" << os.console.endl;

    }else if(c.isValidFormat("resume")){
        os.motion.resume();
        os.console.info("Motion") << "motion resumed" << os.console.endl;

    }else if(c.isValidFormat("cancel")){
        os.motion.cancel();
        os.console.info("Motion") << "motion canceled" << os.console.endl;
        os.motion.sleep();
        os.console.info("Motion") << "Entering sleep mode" << os.console.endl;

    }else if(c.isValidFormat("setAbsolute")){
        os.motion.setAbsolute();
        os.console.info("Motion") << "motion set to absolute mode" << os.console.endl;

    }else if(c.isValidFormat("setRelative")){
        os.motion.setRelative();
        os.console.info("Motion") << "motion set to relative mode" << os.console.endl;

    }else if(c.isValidFormat("help")){
        os.console.trace("Interpreter") << "Command parsed at " << int(millis()) << "ms" << os.console.endl;

    }else{
        os.console.error("Interpreter") << "Unknown command : " << c.toString() << os.console.endl;

    }
}


