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


void OperatingSystem::execute(String& script){
    os.console.println("----");
    os.console.prettyPrint(script);
    os.console.println("----");
    Program p = interpreter.processScript(script);

    if(p.isValid()){
        currentProgram = p;
        p.start();
    }

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

void OperatingSystem::update(){
	SystemBase::update();

    if(_waiting){ //TODO handle that properly
        if(_currentJob->isPending() || _currentJob->isPaused()){
        }else _waiting = false;
    }else if(terminal.commandAvailable()){
        interpreter.processScript(terminal.dequeCommand());
    }
}

void OperatingSystem::setConsoleLevel(ConsoleLevel level){
	console.setLevel(level);
}

void OperatingSystem::waitUntil(Job& obj){
    _waiting = true;
    _currentJob = &obj;
}


