#include "os/routines.h"
#include "os/commands.h"
#include "robot.h"
#include "strategy.h"
#include "os/os.h"
#include "utils/interpreter/interpreter.h"


void robotProgram(){
    Console::println("Started match");
    lidar.enable();
    match();
    motion.disable();
}

void robotIdleProgram(){
    static bool hadStarter = false;
    static bool buttonWasPressed = false;

    ihm.setRobotPosition(motion.getAbsPosition());
    
    if(ihm.hasStarter() && !hadStarter){
        lidar.showRadarLED();
        ihm.freezeSettings();
        hadStarter = true;
        return;
    }

    if(!ihm.hasStarter() && hadStarter && !ihm.buttonPressed()){
        ihm.setPage(IHM::Page::MATCH);
        os.start();
        return;
    }
    
    if(!ihm.hasStarter() && hadStarter &&  ihm.buttonPressed()){
        ihm.unfreezeSettings();
        lidar.showStatusLED();
        hadStarter = false;
        buttonWasPressed = true;
        return;
    } 

    if(ihm.buttonPressed() && !buttonWasPressed && !hadStarter && !ihm.hasStarter()){
        recalage();
        return;
    }

    if(!ihm.buttonPressed() && buttonWasPressed){
        buttonWasPressed = false;
        return;
    }

    if(terminal.commandAvailable()){
        onTerminalCommand();
        return;
    }

}

void onRobotBoot(){

    os.attachService(&ihm); 
    ihm.drawBootProgress("Linking ihm...");
    ihm.addBootProgress(10); 
    
    ihm.drawBootProgress("Linking motion...");
    os.attachService(&motion); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking actuators...");
    os.attachService(&actuators); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking intercom");
    os.attachService(&intercom); ihm.addBootProgress(10);
    intercom.setConnectLostCallback(onIntercomDisconnected);
    intercom.setConnectionSuccessCallback(onIntercomConnected);
    intercom.setRequestCallback(onIntercomRequest);

    ihm.drawBootProgress("Linking lidar...");
    os.attachService(&lidar); ihm.addBootProgress(10);
    delay(100);
    lidar.showStatusLED();
    lidar.enable();//lidar.disable();

    ihm.drawBootProgress("Linking terminal...");
    os.attachService(&terminal); ihm.addBootProgress(10);

    ihm.setPage(IHM::Page::INIT);

    registerCommands();
}

void onRobotIdle(){
    ihm.setRobotPosition(motion.getAbsPosition());
    //lidar.setLidarPosition(motion.getAbsPosition());
}

void onRobotRun(){
    ihm.setRobotPosition(motion.getAbsPosition());
    //lidar.setLidarPosition(motion.getAbsPosition());
}

void onRobotStop(){
    ihm.onUpdate();
}

void onIntercomConnected(){
    ihm.setIntercomState(true);
}

void onIntercomDisconnected(){
    ihm.setIntercomState(false);
}


void onIntercomRequest(Request& req){

}

void onIntercomRequestReply(Request& req){

}

void onOccupancyResponse(Request& req){
    
}

void onOccupancyTimeout(){
    
}

void onTerminalCommand(){
    Interpreter interpreter;
    if( terminal.commandAvailable() > 0){
        Console::println("Received command. parsing...");
        String rawcmd = terminal.dequeCommand();
        Interpreter in;
        Program prgm = in.processScript(rawcmd);
        if(prgm.isValid()){ //TODO Integrate that in OS
            Console::println("Starting program");
            prgm.start();
            while(os.isBusy()) os.flush();
            while(prgm.step()){
                while(os.isBusy()) os.flush();
            };
        }else {
            Console::println("Invalid program : Unknown error");
        }
    }
}