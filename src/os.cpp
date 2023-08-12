#include "os.h"

OperatingSystem OperatingSystem::instance;

OperatingSystem& OperatingSystem::getInstance(){
    return instance;
}

void OperatingSystem::loadService(Service *service){
    SystemBase::loadService(service);
    SystemBase::enable(service->getID());
}

void OperatingSystem::enable(ServiceID id) {
    SystemBase::enable(id);
}

void OperatingSystem::disable(ServiceID id) {
    SystemBase::disable(id);
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

void OperatingSystem::update(){
	SystemBase::update();

    if( terminal.commandAvailable() > 0){
        Program p = interpreter.processScript(terminal.dequeCommand());
        if(p.isValid()){
            execute(p);
        }
    }
}

void OperatingSystem::setConsoleLevel(ConsoleLevel level){
	console.setLevel(level);
}


