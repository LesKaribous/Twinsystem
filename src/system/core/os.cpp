#include "os.h"

OperatingSystem::OperatingSystem() : SystemBase(){
    setConsoleLevel(INFO);
    Console::start();
    interpreter.SetOS(this);
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
    CommandHandler::registerCommand("resetCompass", "Reset compass and set to 0");
    CommandHandler::registerCommand("open(side)", "Open actuator on a specific side");
    CommandHandler::registerCommand("close(side)", "Close actuator on a specific side");
    CommandHandler::registerCommand("takePlant(side)", "wip");
    CommandHandler::registerCommand("placePlant(side)", "wip");
    CommandHandler::registerCommand("print(value)", "Print the result of an expression in the terminal");
    CommandHandler::registerCommand("help", "Display help");

    //Variables
    // Expression::registerVariables("time");
    // Expression::registerVariables("posA", "[1,2]");
    // Expression::registerVariables("posB", "[3,4]");
}

void OperatingSystem::handleBootState(){
    screen.drawBootProgress("Linking modules...");
    screen.addBootProgress(10);

    loadService(&screen);     screen.addBootProgress(10); screen.drawBootProgress("Linking lidar...");
    loadService(&lidar);      screen.addBootProgress(10); screen.drawBootProgress("Linking inputs...");
    loadService(&inputs);     screen.addBootProgress(10); screen.drawBootProgress("Linking motion...");
    loadService(&motion);     screen.addBootProgress(10); screen.drawBootProgress("Linking intercom...");
    //loadService(&planner);  screen.addBootProgress(10); screen.drawBootProgress("Loading Lidar...");
    //loadService(&neopixel); screen.addBootProgress(10); screen.drawBootProgress("Linking neopixel...");
    loadService(&intercom);   screen.addBootProgress(10); screen.drawBootProgress("Linking terminal...");
    loadService(&terminal);   screen.addBootProgress(10); screen.drawBootProgress("Linking actuators...");
    loadService(&actuators);  screen.addBootProgress(10); screen.drawBootProgress("Loading commands...");
    //loadService(&localisation);
    motion.setCalibration(inputs.isPrimary() ? Settings::Calibration::Primary : Settings::Calibration::Secondary);
    //motion.enable();
    screen.addBootProgress(10); screen.drawBootProgress("Starting motion...");

    registerCommands();

    screen.addBootProgress(30);
    screen.drawBootProgress("Boot complete...");
    screen.setPage(Page::INIT);
    screen.draw();
    setSystemState(SystemState::IDLE);
}

void OperatingSystem::handleIdleState(){}

void OperatingSystem::handleRunningState(){}

void OperatingSystem::handleStoppedState(){}

void OperatingSystem::control(){
    //motion.control();
}

void OperatingSystem::loadService(Service *service){
    service->setOS(this);
    SystemBase::loadService(service);
    SystemBase::enable(service->getID());
}

void OperatingSystem::enable(ServiceID id) {
    SystemBase::enable(id);
}

void OperatingSystem::disable(ServiceID id) {
    SystemBase::disable(id);
}

void OperatingSystem::setConsoleLevel(ConsoleLevel level){
	console.setLevel(level);
}


