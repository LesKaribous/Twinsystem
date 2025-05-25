#include "routines.h"
#include "os/commands.h"
#include "robot.h"
#include "strategy.h"
#include "poi.h"
#include "score.h"
#include "os/os.h"
#include "utils/timer/timer.h"

// -------------------------------------
//           Main programs
// -------------------------------------

void robotProgramAuto(){
    Console::println("Started match");
    ihm.setPage(IHM::Page::MATCH);
    lidar.enable();
    //lidar.disable();
    safety.enable();
    //safety.disable();
    chrono.start();
    match();
    //motion.disable();
}

void robotArmed(){
    lidar.showRadarLED();
    ihm.freezeSettings();
    motion.engage();

    ihm.playTone(329.2, 150);   // C5
    ihm.playTone(349.2, 150);   // C5
    ihm.playTone(440, 150);   // C5
   
    
    noTone(Pin::Outputs::buzzer);
}

void robotProgramManual(){
    static bool hadStarter = false;
    static bool buttonWasPressed = false;

    //ihm.setRobotPosition(motion.estimatedPosition());
    //ihm.setRobotPosition(nav.getPosition());
    

    /*
    RUN_EVERY(
        Vec2 position = motion.estimatedPosition();
        Vec2 lidar_repulsion = occupancy.repulsiveGradient(position) * 1000.0;
        Console::plotXY("attractor", position.x - lidar_repulsion.x, position.y - lidar_repulsion.y);
        Console::plotXY("robot", position.x, position.y);

        Console::plotXY("table", 0, 0);
        Console::plotXY("table", 3000, 0);
        Console::plotXY("table", 3000, 2000);
        Console::plotXY("table", 0, 2000);

    ,100)*/

    if(ihm.hasStarter() && !hadStarter){
        robotArmed();
        hadStarter = true;
        return;
    }

    if(!ihm.hasStarter() && hadStarter && !ihm.buttonPressed()){
        ihm.setPage(IHM::Page::MATCH);
        os.start();
        return;
    }
    
    if(!ihm.hasStarter() && hadStarter &&  ihm.buttonPressed()){
        motion.disengage();
        ihm.unfreezeSettings();

        ihm.playTone(440, 150);   // C5
        ihm.playTone(349.2, 150);   // C5
        ihm.playTone(329.2, 150);   // C5
        noTone(Pin::Outputs::buzzer);

        lidar.showStatusLED();
        hadStarter = false;
        buttonWasPressed = true;
        return;
    } 

    
    static bool toneOK = false;
    static bool toneOn = false;

    
    
    if(ihm.buttonPressed() && ihm.resetButton.pressDuration() > 1500){ 
        
        if(!toneOK){
            tone(Pin::Outputs::buzzer, 442);
            toneOK = true;
            toneOn = true;
        }
        if(ihm.resetButton.pressDuration() > 1950 && toneOn){
            ihm.playTone(783.99, 150);
            noTone(Pin::Outputs::buzzer);
            toneOn = false;
            toneOK = false;
        }
    }

    if((!ihm.buttonPressed()) && (ihm.resetButton.pressDuration() > 1500)  && (ihm.resetButton.pressDuration() < 1950) && toneOn){
        noTone(Pin::Outputs::buzzer);
        toneOn = false;
        toneOK = false;
        ihm.playTone(440, 150);   // C5
        ihm.playTone(349.2, 150);   // C5
    }

    if( (!ihm.buttonPressed()) &&
        (ihm.resetButton.pressDuration() > 2000)  && (ihm.resetButton.pressDuration() < 6000) && 
        (!buttonWasPressed) &&
        (!hadStarter) && 
        (!ihm.hasStarter())
    ){
        Console::println("Recalage :");
        noTone(Pin::Outputs::buzzer);
        ihm.resetButton.resetDuration();


        ihm.playTone(523.25, 150);
        ihm.playTone(659.25, 150);
        ihm.playTone(783.99, 150);
        ihm.playTone(880.00, 100);
        ihm.playTone(783.99, 100);
        ihm.playTone(659.25, 100);
        ihm.playTone(987.77, 250);

        recalage();
        toneOK = false;
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


// -------------------------------------
//           CONTROL LOOP
// -------------------------------------

void control() {
    static const unsigned long CONTROL_PERIOD_US = Settings::Stepper::STEPPER_DELAY;
    unsigned long lastCall = micros();  
    while (true) {
        motion.control();
        lastCall += CONTROL_PERIOD_US;

        long waitTime = lastCall - micros();
        if (waitTime > 0)
            threads.delay_us(waitTime);

        threads.yield();
    }
}



// -------------------------------------
//              EVENTS
// -------------------------------------

void onRobotBoot(){

    os.attachService(&ihm);
    ihm.drawBootProgress("Linking ihm...");
    ihm.addBootProgress(10);
    ihm.run(); // Read inputs
    //TODO 
    Console::println("TODO : Add important modifier to moves to dupplicate move");

    ihm.drawBootProgress("Linking actuators...");
    os.attachService(&actuators); ihm.addBootProgress(10);
    
    ihm.drawBootProgress("Linking motion...");
    os.attachService(&motion); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking chrono...");
    chrono.setNearEndCallback(onMatchNearEnd);
    chrono.setEndCallback(onMatchEnd);
    os.attachService(&chrono); ihm.addBootProgress(10);

    ihm.drawBootProgress("Linking safety...");
    os.attachService(&safety); ihm.addBootProgress(10);
    safety.disable();

    motion.engage();
    ihm.drawBootProgress("Linking Localisation...");
    os.attachService(&localisation); ihm.addBootProgress(10);
    localisation.calibrate();
    motion.disengage();
    
    //ihm.drawBootProgress("Linking Navigation...");
    //os.attachService(&nav); ihm.addBootProgress(10);

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

    ihm.resetButton.resetDuration();

    Expression::registerVariables("A", "A");
    Expression::registerVariables("AB", "AB");
    Expression::registerVariables("B", "B");
    Expression::registerVariables("BC", "BC");
    Expression::registerVariables("C", "C");
    Expression::registerVariables("CA", "CA");

    Expression::registerVariables("NORTH", "NORTH");
    Expression::registerVariables("SOUTH", "SOUTH");
    Expression::registerVariables("WEST", "WEST");
    Expression::registerVariables("EAST", "EAST");

    Expression::registerVariables("MOTION", "MOTION");
    Expression::registerVariables("ACTUATORS", "ACTUATORS");
    Expression::registerVariables("SAFETY", "SAFETY");
    Expression::registerVariables("CHRONO", "CHRONO");
    Expression::registerVariables("IHM", "IHM");
    Expression::registerVariables("INTERCOM", "INTERCOM");
    Expression::registerVariables("LOCALISATION", "LOCALISATION");
    Expression::registerVariables("LIDAR", "LIDAR");
    Expression::registerVariables("TERMINAL", "TERMINAL");

    Expression::registerVariables("UP", "1");
    Expression::registerVariables("DOWN", "2");
    Expression::registerVariables("GRAB", "1");
    Expression::registerVariables("DROP", "0");

    ihm.drawBootProgress("Boot done."); 
    ihm.setPage(IHM::Page::INIT);
    
}

void onRobotManual(){
    RUN_EVERY(
    ihm.setRobotPosition(motion.estimatedPosition());
    ,300);
    
    RUN_EVERY(
        if(ihm.teamSwitch.getState() == Settings::YELLOW){
            intercom.sendRequest("team(Y)");
        }else{
            intercom.sendRequest("team(B)");
        }
    ,1000);

    RUN_EVERY(
    intercom.sendRequest("oM", 100, onIntercomRequestReply);
    ,500);
}

void onRobotAuto(){
    RUN_EVERY(
    ihm.setRobotPosition(motion.estimatedPosition());
    //intercom.sendRequest("oM", 100, onIntercomRequestReply);
    ,100);
}

void onRobotStop(){
    ihm.run(); //TODO : Check why nothing happened when the robot stopped here
}




// -------------------------------------
//           MODULES EVENTS
// -------------------------------------
void onIntercomConnected(){
    ihm.setIntercomState(true);
}

void onIntercomDisconnected(){
    ihm.setIntercomState(false);
}


void onIntercomRequest(Request& req){

}


void onIntercomRequestReply(Request& req){
    if(req.getContent().startsWith("oM")){ //occupancyMap
        occupancy.decompress(req.getResponse());
    }
}

void onMatchNearEnd(){
    nearEnd();
}

void onMatchEnd(){
    safety.disable();
    lidar.disable();
    //motion.pause(); //pause the program if possible
    motion.cancel();
    motion.disable();
    motion.disengage();
    actuators.disable();
    os.stop();
}

void onOccupancyResponse(Request& req){
    
}

void onOccupancyTimeout(){
    
}

void onTerminalCommand() {
    if (terminal.commandAvailable() > 0) {
        String rawcmd = terminal.dequeCommand();
        os.execute(rawcmd);
    }
}