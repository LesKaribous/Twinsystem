#include "commandHandler.h"
#include "system/core/os->h"
#include "system/core/console.h"

CommandHandler::CommandHandler(OperatingSystem* ptr) {os = ptr;}
void CommandHandler::registerCommand(const String& syntax, const String& description) {
    int numberOfArguments = std::count(syntax.begin(), syntax.end(), ',') + 1;
    int i = syntax.indexOf("(");
    String name = syntax;
    if(i != -1)
        name = syntax.substring(0,i);
    
    
    getCommands()[name] = { syntax, description, numberOfArguments};
}

std::map<String, CommandInfo>& CommandHandler::getCommands() {
    static std::map<String, CommandInfo> commands;
    return commands;
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
        execute_print(args);
        if(arguments.size() == 1){
            Expression e(arguments[0]);
            Vec2 v = Vec2::fromString(e.evaluate());
            execute_go(v);
        }else if(arguments.size() == 2){
            float x = Expression(arguments[0]).evaluate().toFloat();
            float y = Expression(arguments[1]).evaluate().toFloat();
            execute_go(x, y);
        }

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
    } else if (command == "resetCompass") {
        execute_resetCompass();
    } else if (command == "open") {
        execute_open(args);
    } else if (command == "close") {
        execute_close(args);
    } else if (command == "takePlant") {
        execute_takePlant(args);
    } else if (command == "placePlant") {
        execute_placePlant(args);
    } else if (command == "help") {
        execute_help();
    } else if (command == "print") {
        execute_print(args);
    } else {
        // Handle unknown command
        Console::error("CommandHandler") << "Unknown command: " << command << Console::endl;
    }
}
void ;

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
        os->enable(serviceID);
        Console::info("Interpreter") << service <<  " enabled" << Console::endl;
    }else  Console::error("Interpreter") << "unknown service" << Console::endl;
}

void CommandHandler::execute_disable(const String& service){
    ServiceID serviceID = Service::toID(service);
    if(serviceID != NOT_A_SERVICE){
        os->disable(serviceID);
        Console::info("Interpreter") << service <<  " disabled" << Console::endl;
    }else  Console::error("Interpreter") << "unknown service : " << service << Console::endl;
}

void CommandHandler::execute_status(){
    for ( int id = ServiceID::LIDAR; id != ServiceID::NOT_A_SERVICE; id++ ){
        ServiceID sID = static_cast<ServiceID>(id);
        Console::info("Interpreter") << Service::toString(sID) <<  " : " << (os->statusService(sID) ? "ON" : "OFF") << Console::endl;
    }
}

void CommandHandler::execute_status(const String& service){
    ServiceID serviceID = Service::toID(service);
    Console::info("Interpreter") << service <<  " status : " << os->statusService(serviceID) << Console::endl;
}

void CommandHandler::execute_debug(){
    for ( int id = ServiceID::LIDAR; id != ServiceID::NOT_A_SERVICE; id++ ){
        ServiceID serviceID = static_cast<ServiceID>(id);
        os->toggleDebug(serviceID);
        Console::println( Service::toString(serviceID) + " debug " +  (os->debug(serviceID) ? "ON" : "OFF"));
    }
}

void CommandHandler::execute_debug(const String& service){
    ServiceID serviceID = Service::toID(service);
    os->toggleDebug(serviceID);
    Console::println( Service::toString(serviceID) + " debug " +  (os->debug(serviceID) ? "ON" : "OFF"));
}

//Motion
void CommandHandler::execute_go(float x, float y){
    os->motion.go(x, y);
    os->waitUntil(os->motion);
}

void CommandHandler::execute_go(Vec2 v){
    os->motion.go(v);
    os->waitUntil(os->motion);
}

void CommandHandler::execute_turn(float angle){
    os->motion.turn(angle);
    os->waitUntil(os->motion);
}

void CommandHandler::execute_move(float x, float y, float angle){
    os->motion.move(Vec3(x, y, angle));
    os->waitUntil(os->motion);
}

void CommandHandler::execute_pause(){
    os->motion.pause();
}

void CommandHandler::execute_resume(){
    os->motion.resume();
}

void CommandHandler::execute_cancel(){
    os->motion.cancel();
}

void CommandHandler::execute_sleep(){
    os->motion.sleep();
}

void CommandHandler::execute_align(const String& side, float orientation){
    if(side.equalsIgnoreCase("A"))         os->motion.align(RobotCompass::A, orientation);
    else if(side.equalsIgnoreCase("AB"))   os->motion.align(RobotCompass::AB, orientation);
    else if(side.equalsIgnoreCase("B"))    os->motion.align(RobotCompass::B, orientation);
    else if(side.equalsIgnoreCase("BC"))   os->motion.align(RobotCompass::BC, orientation);
    else if(side.equalsIgnoreCase("C"))    os->motion.align(RobotCompass::C, orientation);
    else if(side.equalsIgnoreCase("CA"))   os->motion.align(RobotCompass::CA, orientation);
}

void CommandHandler::execute_setAbsolute(){
    os->motion.setAbsolute();
}

void CommandHandler::execute_setRelative(){
    os->motion.setRelative();
}

void CommandHandler::execute_setAbsPosition(float x, float y, float angle){
    os->motion.setAbsPosition(Vec3(x, y, angle));
}

void CommandHandler::execute_resetCompass(){
    //os->motion.resetCompass();
}

void CommandHandler::execute_open(const String& side){
    if(side.equals("AB")) os->actuators.open(RobotCompass::AB);
    else if(side.equals("BC")) os->actuators.open(RobotCompass::BC);
    else if(side.equals("CA")) os->actuators.open(RobotCompass::CA);
}

void CommandHandler::execute_close(const String& side){
    if(side.equals("AB")) os->actuators.close(RobotCompass::AB);
    else if(side.equals("BC")) os->actuators.close(RobotCompass::BC);
    else if(side.equals("CA")) os->actuators.close(RobotCompass::CA);
}

void CommandHandler::execute_takePlant(const String& side){
    RobotCompass rc;
    if(side.equals("AB")) rc = RobotCompass::AB;
    else if(side.equals("BC")) rc = RobotCompass::BC;
    else if(side.equals("CA")) rc = RobotCompass::CA;

    os->actuators.moveElevator(rc, ElevatorPose::GRAB);
    os->wait(1000, false);
    while(!os->actuators.runClosing(rc)) os->wait(Settings::Actuators::speed, false);
    for(int i=0;i<3;i++){
        while(!os->actuators.runGrabbing(rc)) os->wait(Settings::Actuators::speed, false);
        while(!os->actuators.runClosing(rc)) os->wait(Settings::Actuators::speed, false);
    }
    while(!os->actuators.runGrabbing(rc)) os->wait(Settings::Actuators::speed, false);
    os->actuators.moveElevator(rc, ElevatorPose::UP);
    os->wait(1000, false);
}

void CommandHandler::execute_placePlant(const String& side){
    RobotCompass rc;
    if(side.equals("AB")) rc = RobotCompass::AB;
    else if(side.equals("BC")) rc = RobotCompass::BC;
    else if(side.equals("CA")) rc = RobotCompass::CA;

    os->actuators.moveElevator(rc, ElevatorPose::GRAB);
    os->wait(1000, false);
    while(!os->actuators.runOpening(rc)) os->wait(Settings::Actuators::speed, false);
    while(!os->actuators.runElevatorUp(rc)) os->wait(Settings::Actuators::speed, false);
    os->wait(1000, false);
}



//Terminal
void CommandHandler::execute_help(){
    Console::println("_________________________________________");
    Console::println("Available commands : ");
    for(const auto& c : getCommands()){
        Console::println(c.second.syntax + " : " + c.second.description);
    }
    Console::println("_________________________________________");
}

void CommandHandler::execute_print(const String& str){
    Expression e(str);
    String result = e.evaluate();

    Console::println(result);
}
