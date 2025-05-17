#include "routines.h"
#include "os/commands.h"
#include "robot.h"
#include "strategy.h"
#include "poi.h"
#include "score.h"
#include "os/os.h"
#include "utils/timer/timer.h"
#include "utils/interpreter/interpreter.h"

// -------------------------------------
//           Main programs
// -------------------------------------

void robotProgramAuto(){
    Console::println("Started match");
    ihm.setPage(IHM::Page::MATCH);
    lidar.enable();
    safety.enable();
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
    
    if(ihm.hasStarter() && !hadStarter){
        robotArmed();

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
    ,100);

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
    ihm.onUpdate(); //TODO : Check why nothing happened when the robot stopped here
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
    //return;
    if(req.getContent().startsWith("oM")){ //occupancyMap
        lidar.decompressOccupancyMap(req.getResponse());
        /*
        Console::println("Occupancy map received");
        Console::println("Occupancy map : ");
        for(int i = 0; i < GRID_WIDTH; i++){
            for(int j = 0; j < GRID_HEIGHT; j++){
                Console::print(lidar.isOccupied(i * 3000.0/GRID_WIDTH, j* 2000.0/GRID_HEIGHT) ? "1" : "0");
            }
            Console::println("");
        }*/
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

void onTerminalCommand(){
    Interpreter interpreter;
    if( terminal.commandAvailable() > 0){
        //Console::println("Received command. parsing...");
        String rawcmd = terminal.dequeCommand();
        Interpreter in;
        Program prgm = in.processScript(rawcmd);
        
        if(prgm.isValid()){ //TODO Integrate that in OS
            Console::println("Starting program");
            //motion.engage();
            os.execute(prgm);

            //prgm.start();
            //os.flush();
            //while(prgm.step()){
            //    os.flush();
            //};
            //motion.disengage();
        }else {
            Console::println("Invalid program : Unknown error");
        }
    }
}