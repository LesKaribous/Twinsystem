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

    if(_busy){ 
        if(_currentJob->isPending() || _currentJob->isPaused()){
            _currentJob->run();
        }else _busy = false;
    }else {
        if( terminal.commandAvailable()){
            interpreter.processScript(terminal.dequeCommand());
        }
    }

    currentProgram.run();
}

void OperatingSystem::setConsoleLevel(ConsoleLevel level){
	console.setLevel(level);
}

bool OperatingSystem::isBusy() const {
    return _busy;
}

void OperatingSystem::wait(unsigned long time){
    _busy = true;
    timer.setDuration(time);
    timer.start();
    _currentJob = &timer;
}

void OperatingSystem::waitUntil(Job& obj){
    _busy = true;
    _currentJob = &obj;
}


