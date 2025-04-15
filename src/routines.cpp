#include "routines.h"
#include "os/commands.h"
#include "robot.h"
#include "strategy.h"
#include "poi.h"
#include "os/os.h"
#include "utils/timer/timer.h"
#include "utils/interpreter/interpreter.h"

// -------------------------------------
//           Main programs
// -------------------------------------

void robotProgramAuto(){
    Console::println("Started match");
    lidar.enable();
    safety.enable();
    chrono.start();
    match();
    //motion.disable();
}

void robotProgramManual(){
    static bool hadStarter = false;
    static bool buttonWasPressed = false;

    ihm.setRobotPosition(motion.getAbsPosition());
    //ihm.setRobotPosition(nav.getPosition());
    
    if(ihm.hasStarter() && !hadStarter){
        lidar.showRadarLED();
        ihm.freezeSettings();
        motion.engage();

        ihm.playTone(329.2, 150);   // C5
        ihm.playTone(349.2, 150);   // C5
        ihm.playTone(440, 150);   // C5
       
        
        noTone(Pin::Outputs::buzzer);

        /*
        ihm.playTone(440.00, 500);   // A4
        noTone(Pin::Outputs::buzzer); delay(50);
        ihm.playTone(440.00, 500);   // A4
        noTone(Pin::Outputs::buzzer); delay(50);
        ihm.playTone(440.00, 500);   // A4
        noTone(Pin::Outputs::buzzer); delay(50);
        ihm.playTone(349.23, 350);   // F4
        noTone(Pin::Outputs::buzzer); delay(50);
        ihm.playTone(523.25, 150);   // C5
        ihm.playTone(440.00, 500);   // A4
        ihm.playTone(349.23, 350);   // F4
        ihm.playTone(523.25, 150);   // C5
        ihm.playTone(440.00, 1000);  // A4
        */
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
            noTone(Pin::Outputs::buzzer);
            toneOn = false;
        }
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
    static const unsigned long CONTROL_PERIOD_MS = Settings::Stepper::STEPPER_DELAY;
    unsigned long lastCall = micros();  
    while (true) {
        unsigned long now = micros();
        unsigned long elapsed = now - lastCall;

        if (elapsed < CONTROL_PERIOD_MS) {
            threads.delay_us(CONTROL_PERIOD_MS - elapsed);
        }

        motion.control();
        lastCall = micros();
    
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
    ihm.onUpdate(); // Read inputs

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
    //ihm.setRobotPosition(nav.getPosition());
    ihm.setRobotPosition(motion.getAbsPosition());
}

void onRobotAuto(){
    //ihm.setRobotPosition(motion.getAbsPosition());
    //lidar.setLidarPosition(nav.getPosition());
}

void onRobotStop(){
    ihm.onUpdate();
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

}

void onMatchNearEnd(){
    //if(motion.isPending())motion.forceCancel();
    //motion.setFeedrate(1.0);
    //nav.setAbsolute();
    //actuators.moveElevator(RobotCompass::AB, ElevatorPose::UP);
    //actuators.moveElevator(RobotCompass::BC, ElevatorPose::UP);
    //actuators.moveElevator(RobotCompass::CA, ElevatorPose::UP);
    waitMs(800);
    //if(ihm.isColorA()) async motion.go(POI::yellowArrival);
    //else async motion.go(POI::blueArrival);
    ihm.addScorePoints(10); //zone d'arrivée
    chrono.onMatchFinished();
}

void onMatchEnd(){
    safety.disable();
    lidar.disable();
    //motion.pause(); //pause the program if possible
    //motion.disable();
    actuators.disable();
    os.stop();
}

void onOccupancyResponse(Request& req){
    
}

void onOccupancyTimeout(){
    
}

void onTerminalCommand(){
    Interpreter interpreter;
    if( terminal.commandAvailable() > 0){
        //Console::println("Received command. parsing...");
        String rawcmd = terminal.dequeCommand();
        Interpreter in;
        Program prgm = in.processScript(rawcmd);
        if(prgm.isValid()){ //TODO Integrate that in OS
            Console::println("Starting program");
            motion.engage();
            prgm.start();
            while(os.isBusy()) os.flush();
            while(prgm.step()){
                while(os.isBusy()) os.flush();
            };
            motion.disengage();
        }else {
            Console::println("Invalid program : Unknown error");
        }
    }
}