#include "app.h"

void SystemApplication::loadModule(Module *module){
    system.loadModule(module);
}

SystemApplication::SystemApplication(){
    _state = RobotState::IDLE;
    
    loadModule(&lidar);
    loadModule(&screen);
    loadModule(&inputs);
    loadModule(&motion);
    loadModule(&chrono);
    //Planner planner;
    loadModule(&neopixel);
    loadModule(&intercom);
    loadModule(&terminal);
    loadModule(&actuators);
    loadModule(&localisation);

    motion.setCalibration(Settings::Calibration::Primary);

    system.enable(LIDAR);
    system.enable(SCREEN);
    system.enable(INPUTS);
	system.enable(MOTION);
    system.enable(CHRONO);
    //system.enable(PLANNER);
    system.enable(NEOPIXEL);
    system.enable(INTERCOM);
    system.enable(TERMINAL);
    system.enable(ACTUATORS);
    system.enable(LOCALISATION);
}

SystemApplication::~SystemApplication(){}

void SystemApplication::update(){
	system.update();

    if(terminal.commandAvailable()){
        processCommand(terminal.dequeCommand());
    }
}

void SystemApplication::processCommand(Command c){  

    if(!terminal.isEnabled())return;
    if(c.isValidFormat("go(x,y)")){
        motion.go(c.getVec2());
    }else if(c.isValidFormat("goTurn(x,y,angle)")){
        motion.move(c.getVec3());
    }else if(c.isValidFormat("turn(angle)")){
        motion.turn(c.getFloat());
    }else if(c.isValidFormat("setAbsPosition(x,y)")){
        motion.setAbsPosition(Vec3(c.getFloat(0), c.getFloat(1), motion.getAbsPosition().c));

        THROW(c.getFloat(0));
        THROW(c.getFloat(1));

    }else if(c.isValidFormat("setAbsPosition(x,y,t)")){
        motion.setAbsPosition(Vec3(c.getFloat(0), c.getFloat(1), c.getFloat(2)));

        THROW(c.getFloat(0));
        THROW(c.getFloat(1));
        THROW(c.getFloat(2));

    }else if(c.isValidFormat("align(side, absAngle)")){

        String side = c.getString();
        float orientation = c.getFloat(1);

        if(side.equals("A"))         motion.align(RobotCompass::A, orientation);
        else if(side.equals("AB"))   motion.align(RobotCompass::AB, orientation);
        else if(side.equals("B"))    motion.align(RobotCompass::B, orientation);
        else if(side.equals("BC"))   motion.align(RobotCompass::BC, orientation);
        else if(side.equals("C"))    motion.align(RobotCompass::C, orientation);
        else if(side.equals("CA"))   motion.align(RobotCompass::CA, orientation);

    }else if(c.isValidFormat("pause()")){
        motion.pause();
    }else if(c.isValidFormat("resume()")){
        motion.resume();
    }else if(c.isValidFormat("cancel()")){
        motion.cancel();
    }else if(c.isValidFormat("enableMotion()")){
        system.enable(MOTION);
    }else if(c.isValidFormat("disableMotion()")){
        system.disable(MOTION);
    }else if(c.isValidFormat("setAbsolute()")){
        motion.setAbsolute();
    }else if(c.isValidFormat("setRelative()")){
        motion.setRelative();
    }else{
        Console::error("Terminal") << "Unknown command" << Console::endl;
    }
}

