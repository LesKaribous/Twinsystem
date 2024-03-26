#include "strategy.h"
#include "poi.h"
#include "robot.h"
#include "os/routines.h"

void match(){
    //start match
    motion.setFeedrate(0.4);
    //testEvitemment();
    //return;
    if(ihm.isColorBlue()) matchBlue();
    else matchYellow();
}

void recalage(){
    //motion.setSync();
    actuators.moveElevator(RobotCompass::BC,ElevatorPose::UP);
    if(ihm.isColorBlue()){
        probeBorder(TableCompass::SOUTH, RobotCompass::BC,100);
        probeBorder(TableCompass::WEST,  RobotCompass::BC,100);
        async motion.go(POI::b1);
        async motion.align(RobotCompass::CA, getCompassOrientation(TableCompass::EAST));
        actuators.moveElevator(RobotCompass::AB,ElevatorPose::DOWN);
        actuators.moveElevator(RobotCompass::BC,ElevatorPose::DOWN);
        actuators.moveElevator(RobotCompass::CA,ElevatorPose::DOWN);
    }
    else{
        probeBorder(TableCompass::NORTH, RobotCompass::BC,100);
        probeBorder(TableCompass::EAST,  RobotCompass::BC,100);
        async motion.go(POI::y1);
        async motion.align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
        actuators.moveElevator(RobotCompass::AB,ElevatorPose::DOWN);
        actuators.moveElevator(RobotCompass::BC,ElevatorPose::DOWN);
        actuators.moveElevator(RobotCompass::CA,ElevatorPose::DOWN);
    }
}

void testEvitemment(){
    motion.setAsync();

    async motion.go(100, 0);
    async motion.go(0, 0);

    return;
    for(int i=0;i<5; i++){
        motion.go(1000,1000);
        motion.go(250,250);
        os.wait(2000);
    }
}

void takePlants(Vec2 target, RobotCompass rc, TableCompass tc){
    float startOffset = 260.0;
    float grabOffset = 80.0;
    float pushOffset = 60.0;
    float newTargetY = target.y;

    // Ralentir
    motion.setFeedrate(0.25);

    // Mettre les bras en position Grab
    actuators.moveElevator(RobotCompass::AB,ElevatorPose::GRAB);
    actuators.moveElevator(RobotCompass::BC,ElevatorPose::GRAB);
    actuators.moveElevator(RobotCompass::CA,ElevatorPose::GRAB);

    // On se positionne en bordure de zone
    if(tc == TableCompass::EAST) newTargetY = newTargetY - startOffset;
    else if(tc == TableCompass::WEST) newTargetY = newTargetY + startOffset;
    async motion.go(target.x, newTargetY);
    
    for(int i = 0; i < 3; i++){
        actuators.open(rc);

        // Avancer vers la plante seulement si pas la premiere fois
        if(i > 0){
            if(tc == TableCompass::EAST) newTargetY = newTargetY + grabOffset;
            else if(tc == TableCompass::WEST) newTargetY = newTargetY - grabOffset;
            async motion.go(target.x, newTargetY);
        }
        // Rapprocher les plantes
        actuators.close(rc);
        waitMs(1000);

        // Avancer un peu avant de grab
        if(tc == TableCompass::EAST) async motion.go(target.x, newTargetY + pushOffset);
        else if(tc == TableCompass::WEST) async motion.go(target.x, newTargetY - pushOffset);

        // Prendre les plantes
        actuators.grab(rc);
        waitMs(1000);

        // Reculer
        if(tc == TableCompass::EAST) async motion.go(target.x, newTargetY);
        else if(tc == TableCompass::WEST) async motion.go(target.x, newTargetY);
        
        // Lever les plantes et suivant
        actuators.moveElevator(rc,ElevatorPose::UP);
        rc = nextActuator(rc);
        if(i<2) async motion.align(rc, getCompassOrientation(tc)); // Ne pas effectuer la rotation sur la derniere action
    }

    motion.setFeedrate(0.25);    
}

void placePlants(Vec2 target, RobotCompass rc, TableCompass tc, bool planter){

    float clearance = 200.0;
    // Ralentir
    motion.setFeedrate(0.4);
    // Vérifier que le bras est en position UP
    actuators.moveElevator(rc,ElevatorPose::UP);
    // S'orienter vers la position de placement
    async motion.align(rc, getCompassOrientation(tc));
    if(planter){
        // Se positionner sur la bordure de zone
        if(tc == TableCompass::WEST) async motion.go(target.x, target.y + clearance);
        else if(tc == TableCompass::EAST) async motion.go(target.x, target.y - clearance);
        else if(tc == TableCompass::NORTH) async motion.go(target.x - clearance, target.y);
        else if(tc == TableCompass::SOUTH) async motion.go(target.x + clearance, target.y);
        // Probe border
        probeBorder(tc, rc, 0, 100, 40);
    }
    else{
        async motion.go(target);
    }
    // Poser les plantes
    if(planter)actuators.moveElevator(rc,ElevatorPose::BORDER);
    else actuators.moveElevator(rc,ElevatorPose::GRAB);
    
    waitMs(1000);
    // Ouvrir les bras
    actuators.open(rc);
    waitMs(1000);
    if(planter){
        // Lever l'ascensceur
        actuators.moveElevator(rc,ElevatorPose::UP);
        waitMs(1000);
        // Se reculer
        motion.setRelative();
        async motion.goPolar(getCompassOrientation(rc),-200);
        motion.setAbsolute();
    }
    else{
        // Se reculer
        motion.setRelative();
        async motion.goPolar(getCompassOrientation(rc),-200);
        motion.setAbsolute();
        // Lever l'ascensceur
        actuators.moveElevator(rc,ElevatorPose::UP);
        waitMs(1000);
    }
    
    // Vitesse normale
    motion.setFeedrate(0.4);
}

void matchBlue(){
    async motion.go(1000,400);
    // Macro to take plants
    takePlants(POI::plantSupplySW, RobotCompass::CA, TableCompass::EAST);
    // Macro place plants
    placePlants(POI::planterBlueWest, RobotCompass::AB, TableCompass::WEST);
    placePlants(POI::b1, RobotCompass::BC, TableCompass::WEST, false);
    // Dégagement des pots
    async motion.go(200,300); // Possitionnement face bordure
    probeBorder(TableCompass::SOUTH, RobotCompass::CA,0,100,50); // Approche de la bordure
    async motion.go(110,612); // Dégagement latéral des pots
    placePlants(POI::planterBlueSouth, RobotCompass::CA, TableCompass::SOUTH);
    // Dégagement de zone
    //async motion.go(POI::plantSupplySW);
    async motion.go(887,1000);
    async motion.go(POI::b2);
    probeBorder(TableCompass::EAST, RobotCompass::AB,100);
    probeBorder(TableCompass::SOUTH, RobotCompass::CA,100);
    async motion.go(POI::b2);
    // Ajuster le bras pour le panneaux
    actuators.moveElevator(RobotCompass::AB,ElevatorPose::BORDER);
    waitMs(1000);
    actuators.close(RobotCompass::AB);
    waitMs(1000);
    // S'approcher et tourner les panneaux
    async motion.go(POI::solarPanelBlue_1);
    async motion.go(POI::solarPanelBlue_3);
    async motion.go(850,1670); // Dégagement
    // Ranger les bras
    actuators.moveElevator(RobotCompass::AB,ElevatorPose::UP);
    actuators.open(RobotCompass::AB);
    // Aller en zone de recharge 
    async motion.go(POI::b2);
    // Fin de match
    motion.disengage();
    actuators.disable();
}

void matchYellow(){
    async motion.go(2000,400);
    // Macro to take plants
    takePlants(POI::plantSupplyNW, RobotCompass::AB, TableCompass::EAST);
    placePlants(POI::planterYellowWest, RobotCompass::AB, TableCompass::WEST);
}

void waitMs(unsigned long time){
    // To fix with asynch wait
    os.wait(time,false);
    //delay(time); // WIP -> To fix
}

RobotCompass nextActuator(RobotCompass rc){
    int RobotCompassSize = 6;
    return static_cast<RobotCompass>((static_cast<int>(rc) + 2) % RobotCompassSize);
}

RobotCompass previousActuator(RobotCompass rc){
    int RobotCompassSize = 6;
    return static_cast<RobotCompass>((static_cast<int>(rc) + RobotCompassSize - 2) % RobotCompassSize);
}

void probeBorder(TableCompass tc, RobotCompass rc, float clearance, float approachDist, float probeDist){
	boolean wasAbsolute = motion.isAbsolute();
    float currentFeedrate = motion.getFeedrate();
	//bool m_probing = true;
    //motion.setSync();
    //Console::println("Recalling");

    motion.setFeedrate(0.15);
	async motion.align(rc, getCompassOrientation(tc));

    motion.setRelative();
	async motion.goPolar(getCompassOrientation(rc),approachDist);
	async motion.goPolar(getCompassOrientation(rc),probeDist);

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

	async motion.goPolar(getCompassOrientation(rc),-clearance);

	if(wasAbsolute) motion.setAbsolute();
    motion.setFeedrate(currentFeedrate);
	//_probing = false;
}
