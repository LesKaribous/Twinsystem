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
            while(os.isBusy()) os.flush();
            while(prgm.step()){
                while(os.isBusy()) os.flush();
            };
        }
    }
}

void onRobotIdle(){
    //if(!ihm.starterPulled()){
        //os.setState(OS::RUNNING);
        //ihm.setPage(IHM::Page::MATCH);
    //}
    //if(ihm.hasStarter()) return;
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

}

void onRobotStop(){

}

void recalage(){
    motion.setAsync();
    motion.go(-100,0);
}

void probeBorder(TableCompass tc, RobotCompass rc){
	boolean wasAbsolute = motion.isAbsolute();
	bool m_probing = true;

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
	}
	if(tc == TableCompass::SOUTH){
		position.a = 0.0 + _offset; //We hit Xmin
		//_probedX = true;
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::EAST){
		position.b = 2000.0 - _offset; //We hit Ymax
		//_probedY = true;
		motion.setAbsPosition(position);
	}
	if(tc == TableCompass::WEST){
		position.b = 0.0 + _offset; //We hit Ymin
		//_probedY = true;
		motion.setAbsPosition(position);
	}

	motion.setAbsPosition(position);

	motion.goPolar(getCompassOrientation(rc),-100);

	if(wasAbsolute) motion.setAbsolute();
	//_probing = false;
}