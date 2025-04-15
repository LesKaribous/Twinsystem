#include "commands.h"
#include "robot.h"
#include "routines.h"
#include "strategy.h"

void registerCommands() {
    CommandHandler::registerCommand("enable(service)", "Enable a specific service", command_enable);
    CommandHandler::registerCommand("disable(service)", "Disable a specific service", command_disable);
    CommandHandler::registerCommand("status(service)", "Display single status", command_status);
    CommandHandler::registerCommand("lidarMode(mode)", "Change neopixel display mode on lidar", command_lidarMode);
    CommandHandler::registerCommand("go(x,y)", "Move to a specific position", command_go);
    CommandHandler::registerCommand("goPolar(angle,dist)", "Move to a relative polar position", command_goPolar);
    CommandHandler::registerCommand("move(x,y,angle)", "Move to a specific position", command_move);
    CommandHandler::registerCommand("turn(angle)", "Turn to a specific angle", command_turn);
    CommandHandler::registerCommand("rawTurn(angle)", "Turn to a specific angle without optimization", command_rawTurn);
    CommandHandler::registerCommand("pause", "Pause motion", command_pause);
    CommandHandler::registerCommand("resume", "Resume motion", command_resume);
    CommandHandler::registerCommand("cancel", "Cancel motion", command_cancel);
    //CommandHandler::registerCommand("sleep", "Put motion to sleep", command_sleep);
    //CommandHandler::registerCommand("wake", "Wake up motion", command_wake);
    CommandHandler::registerCommand("align(side,angle)", "Align to a specific side and angle", command_align);
    CommandHandler::registerCommand("setAbsolute", "Set motion to absolute mode", command_setAbsolute);
    CommandHandler::registerCommand("setRelative", "Set motion to relative mode", command_setRelative);
    CommandHandler::registerCommand("setAbsPosition(x,y,angle)", "Set absolute position", command_setAbsPosition);
    CommandHandler::registerCommand("setAbsolutePosition(x,y,angle)", "Set absolute position", command_setAbsPosition);
    CommandHandler::registerCommand("resetCompass", "Reset compass and set to 0", command_resetCompass);
    CommandHandler::registerCommand("elevator(side, pose)", "Raise elevator to desired pose", command_elevator);
    CommandHandler::registerCommand("moveElevator(side,angle)", "Raise elevator to desired angle", command_move_elevator);
    CommandHandler::registerCommand("raise(side)", "Raise elevator", command_raise);
    CommandHandler::registerCommand("lower(side)", "Lower elevator", command_lower);
    CommandHandler::registerCommand("grab(side)", "Grab object using actuator", command_grab);
    CommandHandler::registerCommand("drop(side)", "Drop object using actuator", command_drop);
    //CommandHandler::registerCommand("open(side)", "Open actuator on a specific side", command_open);
    //CommandHandler::registerCommand("close(side)", "Close actuator on a specific side", command_close);
    CommandHandler::registerCommand("recalage()", "Execute recalage routine", command_recalage);
    CommandHandler::registerCommand("print(value)", "Print the result of an expression in the terminal", command_print);
    CommandHandler::registerCommand("help", "Display help", command_help);

    
}

void command_enable(const args_t& args){
    if(args.size() != 1) return;
    ServiceID serviceID = Service::toID(args[0]);
    if(serviceID != ID_NOT_A_SERVICE){
        os.enable(serviceID);
        Console::info("Interpreter") << args[0] <<  " enabled" << Console::endl;
    }else  Console::error("Interpreter") << "unknown service" << Console::endl;
}

void command_disable(const args_t& args){
    if(args.size() != 1) return;
    ServiceID serviceID = Service::toID(args[0]);
    if(serviceID != ID_NOT_A_SERVICE){
        os.disable(serviceID);
        Console::info("Interpreter") << args[0] <<  " disabled" << Console::endl;
    }else  Console::error("Interpreter") << "unknown service" << Console::endl;
}

void command_status(const args_t& args){
    if(args.size() != 1){
        for ( int id = 0; id != ServiceID::ID_NOT_A_SERVICE; id++ ){
           ServiceID sID = static_cast<ServiceID>(id);
           Console::info("Interpreter") << Service::toString(sID) <<  " : " << (OS::instance().statusService(sID) ? "ON" : "OFF") << Console::endl;
        }
    }else{
        ServiceID serviceID = Service::toID(args[0]);
        Console::info("Interpreter") << args[0] <<  " : " << (OS::instance().statusService(serviceID) ? "ON" : "OFF") << Console::endl;
    }
} 


void command_debug(const args_t& args){
    if(args.size() != 1){
        for ( int id = 0; id != ServiceID::ID_NOT_A_SERVICE; id++ ){
           ServiceID sID = static_cast<ServiceID>(id);
           os.toggleDebug(sID);
           Console::info("Interpreter") << Service::toString(sID) <<  " debug : " << (OS::instance().debug(sID) ? "ON" : "OFF") << Console::endl;
        }
    }else{
        ServiceID serviceID = Service::toID(args[0]);
        os.toggleDebug(serviceID);
        Console::info("Interpreter") << args[0] <<  " debug : "  << (OS::instance().debug(serviceID) ? "ON" : "OFF") << Console::endl;
    }
}


//Lidar

void command_lidarMode(const args_t& args){
    if(args.size() != 1) return;
    if(args[0].equalsIgnoreCase("radar"))lidar.showRadarLED();
    else if(args[0].equalsIgnoreCase("intercom"))lidar.showStatusLED();
}



//Motion
void command_go(const args_t& args){
    /**/
    if(args.size() != 2) return;
    float x = args[0].toFloat();
    float y = args[1].toFloat();
    async motion.go(x, y);
    /**/
}

//Motion
void command_goPolar(const args_t& args){
    /**/
    if(args.size() != 2) return;
    float angle = args[0].toFloat();
    float dist = args[1].toFloat();
    async motion.goPolar(angle, dist);
    /**/
}

void command_move(const args_t& args){
    /**/
    if(args.size() != 3) return;
    float x = args[0].toFloat();
    float y = args[1].toFloat();
    float z = args[2].toFloat();
    Console::info("Interpreter") << "Move to (" << x << ", " << y << ", " << z << ")" << Console::endl;
    async motion.move({x, y, z});
    /**/
}


void command_turn(const args_t& args){
    /**/
    if(args.size() != 1)return;
    float x = args[0].toFloat();
    async motion.turn(x);
    /**/
}

void command_rawTurn(const args_t& args){
    /**/
    if(args.size() != 1)return;
    float x = args[0].toFloat();
    motion.disableOptimization();
    async motion.turn(x);
    motion.enableOptimization();
    /**/
}

void command_pause(const args_t& args){
    motion.pause();
}

void command_resume(const args_t& args){
    motion.resume();
}

void command_cancel(const args_t& args){
    motion.cancel();
}


void command_align(const args_t& args){
    /**/
    if(args.size() != 2)return;
    String side = args[0];
    float orientation = args[1].toFloat();
    if(side.equalsIgnoreCase("A"))         async motion.align(RobotCompass::A, orientation);
    else if(side.equalsIgnoreCase("AB"))   async motion.align(RobotCompass::AB, orientation);
    else if(side.equalsIgnoreCase("B"))    async motion.align(RobotCompass::B, orientation);
    else if(side.equalsIgnoreCase("BC"))   async motion.align(RobotCompass::BC, orientation);
    else if(side.equalsIgnoreCase("C"))    async motion.align(RobotCompass::C, orientation);
    else if(side.equalsIgnoreCase("CA"))   async motion.align(RobotCompass::CA, orientation);
    /**/
}



void command_setAbsolute(const args_t& args){
    /**/
    motion.setAbsolute();
    /**/
}


void command_setRelative(const args_t& args){
    /**/
    motion.setRelative();
    /**/
}


void command_setAbsPosition(const args_t& args){
    /**/
    if(args.size() != 3)return;
    float x = args[0].toFloat();
    float y = args[1].toFloat();
    float angle = args[2].toFloat() * DEG_TO_RAD;
    motion.setAbsPosition({x, y, angle});
    /**/
}


void command_resetCompass(const args_t& args){
    //Not implemented yet
}



//Actuators

void command_drop(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) actuators.drop(RobotCompass::AB);
    //else if(side.equals("BC")) actuators.drop(RobotCompass::BC);
    else if(side.equals("CA")) actuators.drop(RobotCompass::CA);
}


void command_grab(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) actuators.grab(RobotCompass::AB);
    //else if(side.equals("BC")) actuators.grab(RobotCompass::BC);
    else if(side.equals("CA")) actuators.grab(RobotCompass::CA);
}

void command_elevator(const args_t& args){
    if(args.size() != 2)return;
    const String& side = args[0];
    const String& poseStr = args[1];
    int pose = poseStr.toInt();

    if(side.equals("AB")) actuators.moveElevator(RobotCompass::AB, ElevatorPose(pose));
    else if(side.equals("BC")) actuators.moveElevator(RobotCompass::BC, ElevatorPose(pose));
    else if(side.equals("CA")) actuators.moveElevator(RobotCompass::CA, ElevatorPose(pose));
}

void command_move_elevator(const args_t& args){

    if(args.size() != 2)return;
    const String& side = args[0];
    const String& poseStr = args[1];
    int pose = poseStr.toInt();

    if(side.equals("AB")) actuators.moveElevatorAngle(RobotCompass::AB, pose);
    else if(side.equals("BC")) actuators.moveElevatorAngle(RobotCompass::BC, pose);
    else if(side.equals("CA")) actuators.moveElevatorAngle(RobotCompass::CA, pose);
}

void command_raise(const args_t& args){

    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) actuators.moveElevator(RobotCompass::AB, ElevatorPose::UP);
    else if(side.equals("BC")) actuators.moveElevator(RobotCompass::BC, ElevatorPose::UP);
    else if(side.equals("CA")) actuators.moveElevator(RobotCompass::CA, ElevatorPose::UP);
}

void command_lower(const args_t& args){
    if(args.size() != 1)return;
    const String& side = args[0];
    if(side.equals("AB")) actuators.moveElevator(RobotCompass::AB, ElevatorPose::DOWN);
    else if(side.equals("BC")) actuators.moveElevator(RobotCompass::BC, ElevatorPose::DOWN);
    else if(side.equals("CA")) actuators.moveElevator(RobotCompass::CA, ElevatorPose::DOWN);
}


//Routines 

void command_recalage(const args_t& args){
    recalage();
}


//Terminal
void command_help(const args_t& args){
    auto& commands = CommandHandler::getCommands();
    for (auto i = commands.begin(); i != commands.end(); i++){
        Command c = i->second;
        Console::print(c.getSyntax()); 
        Console::print(" : "); 
        Console::println(c.getDesc()); 
    }
}


void command_print(const args_t& args){
    for(arg_t a : args){
        Console::print(a);   
    }
}



//std::vector<String> arguments = extractArguments(args);

/*
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

*/