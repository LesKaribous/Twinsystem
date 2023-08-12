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

    registerCommands();

    screen.addBootProgress(30);
    screen.drawBootProgress("Boot complete...");
    screen.setPage(Page::INIT);
}

void OperatingSystem::registerCommands(){

    // Registering commands with their syntax and description
    CommandHandler::registerCommand("enable(service)", "Enable a specific service");
    CommandHandler::registerCommand("disable(service)", "Disable a specific service");
    CommandHandler::registerCommand("status", "Display all status");
    CommandHandler::registerCommand("status(service)", "Display single status");
    CommandHandler::registerCommand("go(x,y)", "Move to a specific position");
    CommandHandler::registerCommand("move(x,y,angle)", "Move to a specific position");
    CommandHandler::registerCommand("turn(angle)", "Turn to a specific angle");
    CommandHandler::registerCommand("pause", "Pause motion");
    CommandHandler::registerCommand("resume", "Resume motion");
    CommandHandler::registerCommand("cancel", "Cancel motion");
    CommandHandler::registerCommand("sleep", "Put motion to sleep");
    CommandHandler::registerCommand("align(side,angle)", "Align to a specific side and angle");
    CommandHandler::registerCommand("setAbsolute", "Set motion to absolute mode");
    CommandHandler::registerCommand("setRelative", "Set motion to relative mode");
    CommandHandler::registerCommand("setAbsPosition(x,y,angle)", "Set absolute position");
    CommandHandler::registerCommand("grab(side)", "Grab object using actuator");
    CommandHandler::registerCommand("ungrab(side)", "Ungrab object using actuator");
    CommandHandler::registerCommand("open(side)", "Open actuator on a specific side");
    CommandHandler::registerCommand("close(side)", "Close actuator on a specific side");
    CommandHandler::registerCommand("openTrap(side)", "Open trap on a specific side");
    CommandHandler::registerCommand("closeTrap(side)", "Close trap on a specific side");
    CommandHandler::registerCommand("help", "Display help");
}

void OperatingSystem::update(){
	SystemBase::update();

    if( terminal.commandAvailable() > 0){
        //Program p = interpreter.processScript(terminal.dequeCommand());
        //if(p.isValid()){
        //    execute(p);
        //}
        String str = terminal.dequeCommand();
        Expression e(str.trim());
        console.print(e.evaluate());
    }
}

void OperatingSystem::setConsoleLevel(ConsoleLevel level){
	console.setLevel(level);
}


