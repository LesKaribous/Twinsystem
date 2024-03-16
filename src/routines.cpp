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

void onRobotIdle(){
    //if(!ihm.starterPulled()){
        //os.setState(OS::RUNNING);
        //ihm.setPage(IHM::Page::MATCH);
    //}
    if(ihm.hasStarter()){
        ihm.freezeSettings();
        while(true){
            ihm.onUpdate();
            if(ihm.starterPulled() && !ihm.buttonPressed()){
                ihm.setPage(IHM::Page::MATCH);
                os.setState(OS::RUNNING);
                break;
            }else if(ihm.starterPulled() && ihm.buttonPressed()){
                ihm.unfreezeSettings();
                break;
            }
            delay(10);
        }
    }

    if(ihm.buttonPressed()){
        recalage();
    };

    if(terminal.commandAvailable()){
        onTerminalCommand();
    }

    ihm.setRobotPosition(motion.getAbsPosition());
    //delay(10);
}

void onRobotRun(){
    motion.go(300,300);


    //end match
    ihm.setRobotPosition(motion.getAbsPosition());
    motion.disable();
    os.setState(OS::STOPPED);
}


void onRobotStop(){
    ihm.onUpdate();
}

void recalage(){
    probeBorder(TableCompass::SOUTH, RobotCompass::BC);
    probeBorder(TableCompass::WEST, RobotCompass::BC);
}

void probeBorder(TableCompass tc, RobotCompass rc){
	boolean wasAbsolute = motion.isAbsolute();
    float currentFeed = motion.getFeedrate();
	bool m_probing = true;
    motion.setSync();

    motion.setFeedrate(0.5);
	motion.setRelative();
	motion.align(rc, getCompassOrientation(tc));

	motion.goPolar(getCompassOrientation(rc),200);
	motion.goPolar(getCompassOrientation(rc),80);

	float _offset = getOffsets(rc);

	Vec3 position = motion.getAbsPosition();

	if(tc == TableCompass::NORTH){
		position.a = 3000.0 - _offset; //We hit Xmax
		//_probedX = true;
		motion.setAbsPosition(position);
	}else if(tc == TableCompass::SOUTH){
		position.a = 0.0 + _offset; //We hit Xmin
		//_probedX = true;
		motion.setAbsPosition(position);
	}else if(tc == TableCompass::EAST){
		position.b = 2000.0 - _offset; //We hit Ymax
		//_probedY = true;
		motion.setAbsPosition(position);
	}else if(tc == TableCompass::WEST){
		position.b = 0.0 + _offset; //We hit Ymin
		//_probedY = true;
		motion.setAbsPosition(position);
	}

	//motion.setAbsPosition(position);

	motion.goPolar(getCompassOrientation(rc),-100);

	if(wasAbsolute) motion.setAbsolute();
    motion.setFeedrate(currentFeed);
	//_probing = false;
}