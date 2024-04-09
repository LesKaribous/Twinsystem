#include "os/routines.h"
#include "os/commands.h"
#include "robot.h"
#include "strategy.h"

#include "os/os.h"
#include "os/asyncExecutor.h"

void robotProgram(){
    Console::println("Started match");
    lidar.enable();
    match();
    motion.disable();
}

void robotIdleProgram(){
    static bool hadStarter = false;
    static bool buttonWasPressed = false;

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
    lidar.disable();

    ihm.drawBootProgress("Linking terminal...");
    os.attachService(&terminal); ihm.addBootProgress(10);

    ihm.setPage(IHM::Page::INIT);

    registerCommands();
}

void onRobotIdle(){
    //ihm.setRobotPosition(motion.getAbsPosition());
    static int lastUpdate = 0;

    if(millis() - lastUpdate > 100){
        //Console::println("setRobotPosition sent");
        lastUpdate = millis();
        Vec3 pos = motion.getAbsPosition();
        String posStr = "setRobotPosition(";
        posStr+= String(pos.x) + ",";
        posStr+= String(pos.y) + ",";
        posStr+= String(pos.z) + ")";
        intercom.sendRequest(posStr, 50, onIntercomRequestReply);
    }
}

void onRobotRun(){
    //ihm.setRobotPosition(motion.getAbsPosition());
    //lidar.setRobotPosition(motion.getAbsPosition())
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
    if(req.getContent().startsWith("setRobotPosition")){
        Console::println("setRobotPosition replied successfully");
        String command = req.getResponse();
        Console::println(command);
        int openBracket = command.indexOf("(");
        int closedBracket = command.indexOf(")");
        if(openBracket == -1 || closedBracket == -1)return;

        String argString = command.substring(openBracket + 1, closedBracket);

        std::vector<String> args;
        int comma;
        
        comma = argString.indexOf(",");if(comma == -1) return;
        args.push_back(argString.substring(0, comma));
        argString = argString.substring(comma+1);

        comma = argString.indexOf(",");if(comma == -1) return;
        args.push_back(argString.substring(0, comma));
        argString = argString.substring(comma+1);

        args.push_back(argString);

        if(args.size() == 3){
            float x = args[0].toFloat();
            float y = args[1].toFloat();
            float z = args[2].toFloat();

            ihm.setRobotPosition({x, y, z});
        }
    }
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