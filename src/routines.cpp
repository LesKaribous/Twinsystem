#include "routines.h"

OS& os = OS::instance();
IHM& ihm = IHM::instance();
Motion& motion = Motion::instance();
Actuators& actuators = Actuators::instance();
Terminal& terminal = Terminal::instance();

void onRobotBoot(){
    os.attachService(&ihm); 
    ihm.drawBootProgress("Linking ihm...");
    ihm.addBootProgress(10); 
    
    ihm.drawBootProgress("Linking motion...");
    os.attachService(&motion); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking actuators...");
    os.attachService(&actuators); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking terminal...");
    os.attachService(&terminal); ihm.addBootProgress(10);

    ihm.setPage(IHM::Page::INIT);
}

void onTerminalCommand(){
    Interpreter interpreter;
    if( terminal.commandAvailable() > 0){
        Console::println("Received command. parsing...");
        String cmd = terminal.dequeCommand();
        
    }
}

void onRobotIdle(){
    if(terminal.commandAvailable()){
        onTerminalCommand();
    }
}

void onRobotRun(){

}

void onRobotStop(){

}