#include "commandHandler.h"
#include "os.h"

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


//Motion
void CommandHandler::execute_go(float x, float y){
    os.motion.go(x, y);
}

void CommandHandler::execute_turn(float angle){
    os.motion.turn(angle);
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
    
}

