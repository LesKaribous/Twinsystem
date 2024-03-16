#include "routines.h"
#include "commands.h"

OS& os = OS::instance();
IHM& ihm = IHM::instance();
Motion& motion = Motion::instance();
Actuators& actuators = Actuators::instance();
Terminal& terminal = Terminal::instance();


void recalage();

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

    registerCommands();
}

void onTerminalCommand(){
    Interpreter interpreter;
    if( terminal.commandAvailable() > 0){
        Console::println("Received command. parsing...");
        String rawcmd = terminal.dequeCommand();
        Interpreter in;
        Program prgm = in.processScript(rawcmd);
        if(prgm.isValid()){ //TODO Integrate that in OS
            prgm.start();
            while(os.isBusy()) os.loop();
            while(prgm.step()){
                while(os.isBusy()) os.loop();
            };
        }
    }
}

void onRobotIdle(){
    if(ihm.starterPulled()){
        os.setState(OS::RUNNING);
        ihm.setPage(IHM::Page::MATCH);
    }
    if(ihm.hasStarter()) return;
    if(ihm.buttonPressed()){
        recalage();
    };

    if(terminal.commandAvailable()){
        onTerminalCommand();
    }
    delay(10);
}

void onRobotRun(){

}

void onRobotStop(){

}

void recalage(){
    os.waitUntil(motion.go(-100,0));
}