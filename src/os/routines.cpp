#include "os/routines.h"
#include "os/commands.h"
#include "strategy.h"

OS& os = OS::instance();
IHM& ihm = IHM::instance();
Motion& motion = Motion::instance();
Actuators& actuators = Actuators::instance();
Intercom& intercom = Intercom::instance();
Terminal& terminal = Terminal::instance();
Lidar& lidar = Lidar::instance();

void robotProgram(){
    Console::println("Started match");
    lidar.enable();
    match();
    motion.disable();
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

    ihm.drawBootProgress("Linking lidar...");
    os.attachService(&lidar); ihm.addBootProgress(10);
    delay(100);
    lidar.disable();

    ihm.drawBootProgress("Linking terminal...");
    os.attachService(&terminal); ihm.addBootProgress(10);

    ihm.setPage(IHM::Page::INIT);

    registerCommands();
}

void onRobotIdle(){
    static bool hadStarter = false;
    static bool buttonWasPressed = false;

    if(ihm.hasStarter() && !hadStarter){
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

    ihm.setRobotPosition(motion.getAbsPosition());
    //delay(10);
}

void onRobotRun(){
    Console::println("running...");
    ihm.setRobotPosition(motion.getAbsPosition());
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

void onOppenentDetected(const String& arg){
    //Console::println(arg);
}

void onIntercomMessage(){
    
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