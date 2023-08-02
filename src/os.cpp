#include "os.h"

OperatingSystem os; //OS instance singloton

void OperatingSystem::loadService(Service *service){
    SystemBase::loadService(service);
    enable(service->getID());
}

OperatingSystem::OperatingSystem() : SystemBase(){
    _state = RobotState::IDLE;
    

    motion.setCalibration(inputs.isPrimary() ? Settings::Calibration::Primary : Settings::Calibration::Secondary);
    
    screen.drawBootProgress("Linking modules...");
    screen.addBootProgress(10);

    //loadService(&lidar);
    loadService(&screen);       screen.addBootProgress(10); screen.drawBootProgress("Linking lidar...");
    loadService(&inputs);       screen.addBootProgress(10); screen.drawBootProgress("Linking inputs...");
    loadService(&motion);       screen.addBootProgress(10); screen.drawBootProgress("Linking motion...");
    //loadService(&chrono);
    //loadService(&planner); screen.addBootProgress(10); screen.drawBootProgress("Loading Lidar...");
    loadService(&neopixel);     screen.addBootProgress(10); screen.drawBootProgress("Linking neopixel...");
    loadService(&intercom);     screen.addBootProgress(10); screen.drawBootProgress("Linking intercom...");
    loadService(&terminal);     screen.addBootProgress(10); screen.drawBootProgress("Linking terminal...");
    loadService(&actuators);    screen.addBootProgress(10); screen.drawBootProgress("Linking actuators...");
    //loadService(&localisation);

    screen.addBootProgress(30);
    screen.drawBootProgress("Boot complete...");
    screen.setPage(Page::INIT);
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
        motion.turn(c.getFloat());motion.sleep();
    }else if(c.isValidFormat("setAbsPosition(x,y)")){
        motion.setAbsPosition(Vec3(c.getFloat(0), c.getFloat(1), motion.getAbsPosition().c));
    }else if(c.isValidFormat("setAbsPosition(x,y,t)")){
        motion.setAbsPosition(Vec3(c.getFloat(0), c.getFloat(1), c.getFloat(2)));
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
        }else  console.error("Service") << "unknown service : " << service << console.endl;
             

    }else if(c.isValidFormat("sleep")){
        motion.sleep();
    }else if(c.isValidFormat("pause")){
        motion.pause();
        console.info("Motion") << "motion paused" << console.endl;
    }else if(c.isValidFormat("resume")){
        motion.resume();
        console.info("Motion") << "motion resumed" << console.endl;
    }else if(c.isValidFormat("cancel")){
        motion.cancel();
        console.info("Motion") << "motion canceled" << console.endl;
    }else if(c.isValidFormat("setAbsolute")){
        motion.setAbsolute();
        console.info("Motion") << "motion set to absolute mode" << console.endl;
    }else if(c.isValidFormat("setRelative")){
        motion.setRelative();
        console.info("Motion") << "motion set to relative mode" << console.endl;
    }else{
        console.error("Terminal") << "Unknown command : " << c.toString() << console.endl;
    }
}

