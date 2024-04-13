#include "routines.h"
#include "os/commands.h"
#include "robot.h"
#include "strategy.h"
#include "os/os.h"
#include "utils/timer/timer.h"
#include "utils/interpreter/interpreter.h"


void robotProgram(){
    Console::println("Started match");
    lidar.enable();
    safety.enable();
    chrono.start();
    match();
    motion.disable();
}

void distanceCB(Request& req){
    int d = req.getResponse().toInt();
    Console::println(">Distance:" + String(d));
}

void askDistance(int angle){
    intercom.sendRequest("getDistance(" + String(angle) +")", 100, distanceCB);
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

    ihm.drawBootProgress("Linking chrono...");
    chrono.setNearEndCallback(onMatchNearEnd);
    chrono.setEndCallback(onMatchEnd);
    os.attachService(&chrono); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking safety...");
    safety.disable();
    os.attachService(&safety); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking actuators...");
    os.attachService(&actuators); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking intercom");
    intercom.setConnectLostCallback(onIntercomDisconnected);
    intercom.setConnectionSuccessCallback(onIntercomConnected);
    intercom.setRequestCallback(onIntercomRequest);
    os.attachService(&intercom); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking lidar...");
    os.attachService(&lidar); ihm.addBootProgress(10);
    lidar.showStatusLED();
    lidar.enable();//lidar.disable();

    ihm.drawBootProgress("Linking terminal...");
    os.attachService(&terminal); ihm.addBootProgress(10);

    ihm.drawBootProgress("Registering Commands..."); 
    registerCommands(); ihm.addBootProgress(10);

    ihm.setPage(IHM::Page::INIT);

}

void onRobotIdle(){
    ihm.setRobotPosition(motion.getAbsPosition());
    
    int streer = round(RAD_TO_DEG * motion.getAbsoluteTargetDirection()); //We are moving in this direction
    Console::print(">streer:");
    Console::println(streer);
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

void onMatchNearEnd(){
    safety.disable();
    lidar.disable();
    motion.pause(); //pause the program if possible
    motion.disable();
    actuators.disable();
    actuators.disable();
}

void onMatchEnd(){
    os.stop();
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