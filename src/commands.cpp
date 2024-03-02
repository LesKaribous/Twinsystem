#include "commands.h"
#include "os.h"
#include "console.h"
#include "services/service.h"
#include "services/motion/motion.h"

void registerCommands() {
    CommandHandler::registerCommand("enable(service)", "Enable a specific service", command_enable);
    CommandHandler::registerCommand("disable(service)", "Disable a specific service", command_disable);
    CommandHandler::registerCommand("status(service)", "Display single status", command_status);
    CommandHandler::registerCommand("go(x,y)", "Move to a specific position", command_go);
    CommandHandler::registerCommand("move(x,y,angle)", "Move to a specific position", command_move);
    CommandHandler::registerCommand("turn(angle)", "Turn to a specific angle", command_turn);
    CommandHandler::registerCommand("pause", "Pause motion", command_pause);
    CommandHandler::registerCommand("resume", "Resume motion", command_resume);
    CommandHandler::registerCommand("cancel", "Cancel motion", command_cancel);
    CommandHandler::registerCommand("sleep", "Put motion to sleep", command_sleep);
    CommandHandler::registerCommand("align(side,angle)", "Align to a specific side and angle", command_align);
    CommandHandler::registerCommand("setAbsolute", "Set motion to absolute mode", command_setAbsolute);
    CommandHandler::registerCommand("setRelative", "Set motion to relative mode", command_setRelative);
    CommandHandler::registerCommand("setAbsPosition(x,y,angle)", "Set absolute position", command_setAbsPosition);
    CommandHandler::registerCommand("resetCompass", "Reset compass and set to 0", command_resetCompass);
    CommandHandler::registerCommand("grab(side)", "Grab object using actuator", command_grab);
    CommandHandler::registerCommand("ungrab(side)", "Ungrab object using actuator", command_ungrab);
    CommandHandler::registerCommand("open(side)", "Open actuator on a specific side", command_open);
    CommandHandler::registerCommand("close(side)", "Close actuator on a specific side", command_close);
    CommandHandler::registerCommand("print(value)", "Print the result of an expression in the terminal", command_print);
    CommandHandler::registerCommand("help", "Display help", command_help);
}

void command_enable(const String& args){
    ServiceID serviceID = Service::toID(args);
    if(serviceID != ID_NOT_A_SERVICE){
        OS::instance().enable(serviceID);
        Console::info("Interpreter") << args <<  " enabled" << Console::endl;
    }else  Console::error("Interpreter") << "unknown service" << Console::endl;
}

void command_disable(const String& args){
    ServiceID serviceID = Service::toID(args);
    if(serviceID != ID_NOT_A_SERVICE){
        OS::instance().disable(serviceID);
        Console::info("Interpreter") << args <<  " disabled" << Console::endl;
    }else  Console::error("Interpreter") << "unknown service" << Console::endl;
}

void command_status(const String& args){
    if(args == ""){
        for ( int id = 0; id != ServiceID::ID_NOT_A_SERVICE; id++ ){
           ServiceID sID = static_cast<ServiceID>(id);
           Console::info("Interpreter") << Service::toString(sID) <<  " : " << (OS::instance().statusService(sID) ? "ON" : "OFF") << Console::endl;
        }
    }else{
        ServiceID serviceID = Service::toID(args);
        Console::info("Interpreter") << args <<  " : " << (OS::instance().statusService(serviceID) ? "ON" : "OFF") << Console::endl;
    }
} 


void command_debug(const String& args){
    if(args == ""){
        for ( int id = 0; id != ServiceID::ID_NOT_A_SERVICE; id++ ){
           ServiceID sID = static_cast<ServiceID>(id);
           OS::instance().toggleDebug(sID);
           Console::info("Interpreter") << Service::toString(sID) <<  " debug : " << (OS::instance().debug(sID) ? "ON" : "OFF") << Console::endl;
        }
    }else{
        ServiceID serviceID = Service::toID(args);
        OS::instance().toggleDebug(serviceID);
        Console::info("Interpreter") << args <<  " debug : "  << (OS::instance().debug(serviceID) ? "ON" : "OFF") << Console::endl;
    }
}



//Motion
void command_go(const String& args){
    std::vector<String> arguments = CommandHandler::extractArguments(args);
    float x = arguments[0].toFloat();
    float y = arguments[1].toFloat();
    Motion::instance().go(x, y);
    OS::instance().waitUntil(Motion::instance());
}


void command_move(const String& args){

}


void command_turn(const String& args){

}


void command_pause(const String& args){

}


void command_resume(const String& args){

}


void command_cancel(const String& args){

}


void command_sleep(const String& args){

}


void command_align(const String& args){

}


void command_setAbsolute(const String& args){

}


void command_setRelative(const String& args){

}


void command_setAbsPosition(const String& args){

}


void command_resetCompass(const String& args){

}



//Actuators
void command_grab(const String& args){

}


void command_ungrab(const String& args){

}


void command_open(const String& args){

}


void command_close(const String& args){

}


void command_openTrap(const String& args){

}


void command_closeTrap(const String& args){

}



//Terminal
void command_help(const String& args){

}


void command_print(const String& args){

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