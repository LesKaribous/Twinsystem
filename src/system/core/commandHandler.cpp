#include "commandHandler.h"
#include "os.h"

std::vector<CommandInfo> CommandHandler::_commands;
 
CommandHandler::CommandHandler(){}

void CommandHandler::registerCommand(const String& syntax, const String& description) {
    int numberOfArguments = std::count(syntax.begin(), syntax.end(), ',') + 1;
    _commands.push_back({syntax, description, numberOfArguments});
}

const std::vector<CommandInfo>& CommandHandler::commands() {
    return _commands;
}

void CommandHandler::execute(const String& command, const String& args) {

    std::vector<String> arguments = extractArguments(args);

    if (command == "enable") {
        execute_enable(args);
    } else if (command == "disable") {
        execute_disable(args);
    } else if (command == "status") {
        if (args.length() == 0) {
            execute_status();
        } else {
            execute_status(args);
        }
    } else if (command == "debug") {
        if (args.length() == 0) {
            execute_debug();
        } else {
            execute_debug(args);
        }
    } else if (command == "go") {
        float x = arguments[0].toFloat();
        float y = arguments[1].toFloat();
        execute_go(x, y);
    } else if (command == "move") {
        float x = arguments[0].toFloat();
        float y = arguments[1].toFloat();
        float a = arguments[2].toFloat();
        execute_move(x, y, a);
    }  else if (command == "turn") {
        execute_turn(args.toFloat());
    } else if (command == "pause") {
        execute_pause();
    } else if (command == "resume") {
        execute_resume();
    } else if (command == "cancel") {
        execute_cancel();
    } else if (command == "sleep") {
        execute_sleep();
    } else if (command == "align") {
        String side = arguments[0];
        float angle = arguments[1].toFloat();
        execute_align(side, angle);
    } else if (command == "setAbsolute") {
        execute_setAbsolute();
    } else if (command == "setRelative") {
        execute_setRelative();
    } else if (command == "setAbsPosition") {
        float x = arguments[0].toFloat();
        float y = arguments[1].toFloat();
        float angle = arguments[2].toFloat();
        execute_setAbsPosition(x, y, angle);
    } else if (command == "grab") {
        execute_grab(args);
    } else if (command == "ungrab") {
        execute_ungrab(args);
    } else if (command == "open") {
        execute_open(args);
    } else if (command == "close") {
        execute_close(args);
    } else if (command == "openTrap") {
        execute_openTrap(args);
    } else if (command == "closeTrap") {
        execute_closeTrap(args);
    } else if (command == "help") {
        execute_help();
    } else {
        // Handle unknown command
        os.console.error("CommandHandler") << "Unknown command: " << command << os.console.endl;
    }
}

std::vector<String> CommandHandler::extractArguments(const String& args) {
    std::vector<String> arguments;
    int start = 0;
    int end = args.indexOf(',');

    if(end != -1){
        while ((unsigned int)(end) != args.length()-1 && end != -1) {
            arguments.push_back(args.substring(start, end));
            start = end + 1;
            end = args.indexOf(',', start);
        }
        arguments.push_back(args.substring(start));
    }
    return arguments;
}


bool CommandHandler::execute_testTRUE(){
    return true;
}
bool CommandHandler::execute_testFALSE(){
    return false;
}

//Service
void CommandHandler::execute_enable(const String& service){
    ServiceID serviceID = Service::toID(service);
    if(serviceID != NOT_A_SERVICE){
        os.enable(serviceID);
        os.console.info("Interpreter") << service <<  " enabled" << os.console.endl;
    }else  os.console.error("Interpreter") << "unknown service" << os.console.endl;
}

void CommandHandler::execute_disable(const String& service){
    ServiceID serviceID = Service::toID(service);
    if(serviceID != NOT_A_SERVICE){
        os.disable(serviceID);
        os.console.info("Interpreter") << service <<  " disabled" << os.console.endl;
    }else  os.console.error("Interpreter") << "unknown service : " << service << os.console.endl;
}

void CommandHandler::execute_status(){
    for ( int id = ServiceID::LIDAR; id != ServiceID::NOT_A_SERVICE; id++ ){
        ServiceID sID = static_cast<ServiceID>(id);
        os.console.info("Interpreter") << Service::toString(sID) <<  " : " << (os.statusService(sID) ? "ON" : "OFF") << os.console.endl;
    }
}

void CommandHandler::execute_status(const String& service){
    ServiceID serviceID = Service::toID(service);
    os.console.info("Interpreter") << service <<  " status : " << os.statusService(serviceID) << os.console.endl;
}

void CommandHandler::execute_debug(){
    for ( int id = ServiceID::LIDAR; id != ServiceID::NOT_A_SERVICE; id++ ){
        ServiceID serviceID = static_cast<ServiceID>(id);
        os.toggleDebug(serviceID);
        os.console.println( Service::toString(serviceID) + " debug " +  (os.debug(serviceID) ? "ON" : "OFF"));
    }
}

void CommandHandler::execute_debug(const String& service){
    ServiceID serviceID = Service::toID(service);
    os.toggleDebug(serviceID);
    os.console.println( Service::toString(serviceID) + " debug " +  (os.debug(serviceID) ? "ON" : "OFF"));
}

//Motion
void CommandHandler::execute_go(float x, float y){
    os.motion.go(x, y);
    os.waitUntil(os.motion);
}

void CommandHandler::execute_turn(float angle){
    os.motion.turn(angle);
    os.waitUntil(os.motion);
}

void CommandHandler::execute_move(float x, float y, float angle){
    os.motion.move(Vec3(x, y, angle));
    os.waitUntil(os.motion);
}

void CommandHandler::execute_pause(){
    os.motion.pause();
}

void CommandHandler::execute_resume(){
    os.motion.resume();
}

void CommandHandler::execute_cancel(){
    os.motion.cancel();
}

void CommandHandler::execute_sleep(){
    os.motion.sleep();
}

void CommandHandler::execute_align(const String& side, float orientation){
    if(side.equalsIgnoreCase("A"))         os.motion.align(RobotCompass::A, orientation);
    else if(side.equalsIgnoreCase("AB"))   os.motion.align(RobotCompass::AB, orientation);
    else if(side.equalsIgnoreCase("B"))    os.motion.align(RobotCompass::B, orientation);
    else if(side.equalsIgnoreCase("BC"))   os.motion.align(RobotCompass::BC, orientation);
    else if(side.equalsIgnoreCase("C"))    os.motion.align(RobotCompass::C, orientation);
    else if(side.equalsIgnoreCase("CA"))   os.motion.align(RobotCompass::CA, orientation);
}

void CommandHandler::execute_setAbsolute(){
    os.motion.setAbsolute();
}

void CommandHandler::execute_setRelative(){
    os.motion.setRelative();
}

void CommandHandler::execute_setAbsPosition(float x, float y, float angle){
    os.motion.setAbsPosition(Vec3(x, y, angle));
}


//Actuators
void CommandHandler::execute_grab(const String& side){
    if(side.equals("AB")) os.actuators.grab(RobotCompass::AB);
    else if(side.equals("BC")) os.actuators.grab(RobotCompass::BC);
    else if(side.equals("CA")) os.actuators.grab(RobotCompass::CA);
}

void CommandHandler::execute_ungrab(const String& side){
    if(side.equals("AB")) os.actuators.ungrab(RobotCompass::AB);
    else if(side.equals("BC")) os.actuators.ungrab(RobotCompass::BC);
    else if(side.equals("CA")) os.actuators.ungrab(RobotCompass::CA);
}

void CommandHandler::execute_open(const String& side){
    if(side.equals("AB")) os.actuators.open(RobotCompass::AB);
    else if(side.equals("BC")) os.actuators.open(RobotCompass::BC);
    else if(side.equals("CA")) os.actuators.open(RobotCompass::CA);
}

void CommandHandler::execute_close(const String& side){
    if(side.equals("AB")) os.actuators.close(RobotCompass::AB);
    else if(side.equals("BC")) os.actuators.close(RobotCompass::BC);
    else if(side.equals("CA")) os.actuators.close(RobotCompass::CA);
}

void CommandHandler::execute_openTrap(const String& side){
    os.actuators.openTrap();
}

void CommandHandler::execute_closeTrap(const String& side){
    os.actuators.closeTrap();
}


//Terminal
void CommandHandler::execute_help(){
    os.console.println("_________________________________________");
    os.console.println("Available commands : ");
    for(const CommandInfo& c : commands()){
        os.console.println(c.syntax + " : " + c.description);
    }
    os.console.println("_________________________________________");
}

