#include "os.h"

OperatingSystem os; //OS instance singloton

void OperatingSystem::loadService(Service *service){
    SystemBase::loadService(service);
    enable(service->getID());
}

OperatingSystem::OperatingSystem() : SystemBase(){
    _state = RobotState::IDLE;
    
    //loadService(&lidar);
    loadService(&screen);
    loadService(&inputs);
    loadService(&motion);
    //loadService(&chrono);
    //loadService(&planner);
    loadService(&neopixel);
    loadService(&intercom);
    loadService(&terminal);
    loadService(&actuators);
    //loadService(&localisation);

    motion.setCalibration(Settings::Calibration::Primary);
}

OperatingSystem::~OperatingSystem(){}

void OperatingSystem::update(){
	SystemBase::update();

    if(terminal.commandAvailable()){
        processCommand(terminal.dequeCommand());
    }
}

void OperatingSystem::setConsoleLevel(ConsoleLevel level){
	console.setLevel(level);
}

void OperatingSystem::processCommand(Command c){  

    if(!terminal.isEnabled()){
        console.error("System") << "Terminal is not enabled" << console.endl;
        return;
    }

    if(c.isValidFormat("go(x,y)")){
        motion.go(c.getVec2());
    }else if(c.isValidFormat("goTurn(x,y,angle)")){
        motion.move(c.getVec3());
    }else if(c.isValidFormat("turn(angle)")){
        motion.turn(c.getFloat());
    }else if(c.isValidFormat("setAbsPosition(x,y)")){
        motion.setAbsPosition(Vec3(c.getFloat(0), c.getFloat(1), motion.getAbsPosition().c));

        THROW(c.getFloat(0));
        THROW(c.getFloat(1));

    }else if(c.isValidFormat("setAbsPosition(x,y,t)")){
        motion.setAbsPosition(Vec3(c.getFloat(0), c.getFloat(1), c.getFloat(2)));

        THROW(c.getFloat(0));
        THROW(c.getFloat(1));
        THROW(c.getFloat(2));

    }else if(c.isValidFormat("align(side, absAngle)")){

        String side = c.getString();
        float orientation = c.getFloat(1);

        if(side.equals("A"))         motion.align(RobotCompass::A, orientation);
        else if(side.equals("AB"))   motion.align(RobotCompass::AB, orientation);
        else if(side.equals("B"))    motion.align(RobotCompass::B, orientation);
        else if(side.equals("BC"))   motion.align(RobotCompass::BC, orientation);
        else if(side.equals("C"))    motion.align(RobotCompass::C, orientation);
        else if(side.equals("CA"))   motion.align(RobotCompass::CA, orientation);

    }else if(c.isValidFormat("status")){

        for ( int id = ServiceID::LIDAR; id != ServiceID::NOT_A_SERVICE; id++ ){
            ServiceID sID = static_cast<ServiceID>(id);
            console.info("System") << Service::toString(sID) <<  " : " << (statusService(sID) ? "ON" : "OFF") << console.endl;
        }

    }else if(c.isValidFormat("status(service)")){

        String service = c.getString();
        ServiceID serviceID = Service::toID(service);
        console.info("System") << service <<  " status : " << statusService(serviceID) << console.endl;

    }else if(c.isValidFormat("enable(service)")){

        String service = c.getString();
        ServiceID serviceID = Service::toID(service);
        if(serviceID != NOT_A_SERVICE){
            enable(serviceID);
            console.info("Service") << service <<  " enabled" << console.endl;
        }else  console.error("Service") << "unknown service" << console.endl;

    }else if(c.isValidFormat("disable(service)")){

        String service = c.getString();
        ServiceID serviceID = Service::toID(service);
        if(serviceID != NOT_A_SERVICE){
            disable(serviceID);
            console.info("Service") << service <<  " disabled" << console.endl;
        }else  console.error("Service") << "unknown service" << console.endl;
             

    }else if(c.isValidFormat("pause()")){
        motion.pause();
    }else if(c.isValidFormat("resume()")){
        motion.resume();
    }else if(c.isValidFormat("cancel()")){
        motion.cancel();
    }else if(c.isValidFormat("setAbsolute()")){
        motion.setAbsolute();
    }else if(c.isValidFormat("setRelative()")){
        motion.setRelative();
    }else{
        console.error("Terminal") << "Unknown command : " << c.toString() << console.endl;
    }
}

