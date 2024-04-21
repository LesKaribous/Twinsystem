#include "strategy.h"
#include "poi.h"
#include "robot.h"
#include "routines.h"

/*
void testConditionnal(){
    async motion.go(1200,300) || async motion.go(500,1500);
}
*/

void match(){
    //start match
    motion.setFeedrate(1.0);
    if(ihm.isPrimary()){
        if(ihm.isColorBlue()) matchBlue();
        else matchYellow();
    }
    else {
        if(ihm.isColorBlue()) secondaryMatchBlue();
        else secondaryMatchYellow();
    }
}

void recalage(){
    //motion.setSync();
    actuators.moveElevator(RobotCompass::BC,ElevatorPose::UP);
    waitMs(800);
    if(ihm.isColorBlue()){
        probeBorder(TableCompass::SOUTH, RobotCompass::BC,100);
        probeBorder(TableCompass::WEST,  RobotCompass::BC,100);
        async motion.go(POI::b1);
        async motion.align(RobotCompass::CA, getCompassOrientation(TableCompass::EAST));
        actuators.moveElevator(RobotCompass::AB,ElevatorPose::GRAB);
        actuators.moveElevator(RobotCompass::BC,ElevatorPose::GRAB);
        actuators.moveElevator(RobotCompass::CA,ElevatorPose::GRAB);
    }
    else{
        motion.setAbsPosition({-1,-1,PI});
        probeBorder(TableCompass::NORTH, RobotCompass::BC,100);
        probeBorder(TableCompass::WEST,  RobotCompass::BC,100);
        async motion.go(POI::y1);
        async motion.align(RobotCompass::AB, getCompassOrientation(TableCompass::EAST));
        actuators.moveElevator(RobotCompass::AB,ElevatorPose::GRAB);
        actuators.moveElevator(RobotCompass::BC,ElevatorPose::GRAB);
        actuators.moveElevator(RobotCompass::CA,ElevatorPose::GRAB);
    }
}

void testEvitemment(){
    motion.setAsync();
    async motion.go(2775, 225 + 500) || async motion.go(2775, 225 + 250) || async motion.go(2775, 225);
}

void takePlants(Vec2 target, RobotCompass rc, TableCompass tc){
    float startOffset = 260.0;
    float grabOffset = 80.0;
    float pushOffset = 70.0;
    float newTargetY = target.y;

    // Ralentir
    motion.setFeedrate(0.7);

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
        waitMs(500);

        // Avancer un peu avant de grab
        if(tc == TableCompass::EAST) async motion.go(target.x, newTargetY + pushOffset);
        else if(tc == TableCompass::WEST) async motion.go(target.x, newTargetY - pushOffset);

        // Prendre les plantes
        actuators.grab(rc);
        waitMs(500);

        // Reculer
        if(tc == TableCompass::EAST) async motion.go(target.x, newTargetY);
        else if(tc == TableCompass::WEST) async motion.go(target.x, newTargetY);
        
        // Lever les plantes et suivant
        actuators.moveElevator(rc,ElevatorPose::UP);
        rc = nextActuator(rc);
        if(i<2) async motion.align(rc, getCompassOrientation(tc)); // Ne pas effectuer la rotation sur la derniere action
    }

    motion.setFeedrate(1.0);    
}

void placePlants(Vec2 target, RobotCompass rc, TableCompass tc, bool planter){

    float clearance = 200.0;
    // Ralentir
    motion.setFeedrate(0.8);
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
    if(planter)actuators.moveElevator(rc,ElevatorPose::PLANTER);
    else actuators.moveElevator(rc,ElevatorPose::GRAB);
    
    waitMs(800);
    // Ouvrir les bras
    slowOpennig(rc, 50);
    if(planter){
        // Se reculer un peu (ATTENTION ON EST DE L'AUTRE COTE DE LA BORDURE)
        motion.setRelative();
        async motion.goPolar(getCompassOrientation(rc),-10);
        motion.setAbsolute();
        // Lever l'ascensceur
        SlowElevatorBorder(rc, 10);
        // Se reculer
        motion.setRelative();
        async motion.goPolar(getCompassOrientation(rc),-150);
        motion.setAbsolute();
        actuators.moveElevator(rc,ElevatorPose::UP);
    }
    else{
        // Se reculer
        motion.setRelative();
        async motion.goPolar(getCompassOrientation(rc),-100);
        motion.setAbsolute();
        // Lever l'ascensceur
        actuators.moveElevator(rc,ElevatorPose::UP);
        //waitMs(1000);
    }
    // Vitesse normale
    motion.setFeedrate(1.0);
}

void matchBlue(){
    motion.setFeedrate(1.0);
    async motion.go(1000,400);
    // Macro to take plants
    takePlants(POI::plantSupplySW, RobotCompass::CA, TableCompass::EAST);
    // Macro place plants
    int plants = actuators.howManyPlant(RobotCompass::AB);
    placePlants(POI::planterBlueWest, RobotCompass::AB, TableCompass::WEST);
    ihm.addScorePoints(plants * 4); //plante valid + jardiniere x2

    placePlants(POI::b1, RobotCompass::BC, TableCompass::WEST, false);
    //ihm.addScorePoints(8); We don't know the plant type here

    // Dégagement des pots
    async motion.go(200,300); // Positionnement face bordure
    probeBorder(TableCompass::SOUTH, RobotCompass::CA,0,100,50); // Approche de la bordure
    async motion.go(110,750); // Dégagement latéral des pots

    plants = actuators.howManyPlant(RobotCompass::CA);
    placePlants(POI::planterBlueSouth, RobotCompass::CA, TableCompass::SOUTH);
    ihm.addScorePoints(plants * 4); //plante valid + jardiniere x2

    // Dégagement de zone
    //async motion.go(POI::plantSupplySW);
    async motion.go(887,1000);
    async motion.go(POI::b2);
    probeBorder(TableCompass::EAST, RobotCompass::AB,100);
    probeBorder(TableCompass::SOUTH, RobotCompass::CA,100);
    async motion.go(POI::b2);
    // Ajuster le bras pour le panneaux
    actuators.moveElevator(RobotCompass::AB,ElevatorPose::BORDER);
    waitMs(800);
    actuators.close(RobotCompass::AB);
    waitMs(800);
    // S'approcher et tourner les panneaux
    motion.setFeedrate(1.0);
    async motion.go(POI::solarPanelBlue_1);
    ihm.addScorePoints(5); //1 panneaux couleur retournés
    async motion.go(POI::solarPanelBlue_3);
    ihm.addScorePoints(10); //2 panneaux couleur retournés
    async motion.go(850,1670); // Dégagement Bleu
    /*async motion.go(POI::solarPanelOther_1);
    ihm.addScorePoints(5); //1 panneaux gris retournés
    async motion.go(POI::solarPanelOther_3);
    ihm.addScorePoints(10); //2 panneaux gris retournés
    async motion.go(1850,1670); // Dégagement Gris*/

    // Ranger les bras
    actuators.moveElevator(RobotCompass::AB,ElevatorPose::UP);
    actuators.open(RobotCompass::AB);
    // Aller en zone de recharge 
    async motion.go(POI::b2);
    ihm.addScorePoints(10); //zone d'arrivée
    // Fin de match
    motion.disengage();
    actuators.disable();
}

void matchYellow(){
    motion.setFeedrate(1.0);
    async motion.go(2000,400);
    // Macro to take plants
    takePlants(POI::plantSupplyNW, RobotCompass::AB, TableCompass::EAST);
    // Macro place plants
    int plants = actuators.howManyPlant(RobotCompass::CA);
    placePlants(POI::planterYellowWest, RobotCompass::CA, TableCompass::WEST);
    ihm.addScorePoints( plants * 4); //plante valid + jardiniere x2

    placePlants(POI::y1, RobotCompass::BC, TableCompass::WEST, false);
    //ihm.addScorePoints(8); We don't know the plant type here

    // Dégagement des pots
    async motion.go(2800,300); // Positionnement face bordure
    probeBorder(TableCompass::NORTH, RobotCompass::AB,0,100,50); // Approche de la bordure
    async motion.go(2890,750); // Dégagement latéral des pots

    plants = actuators.howManyPlant(RobotCompass::AB);
    placePlants(POI::planterYellowNorth, RobotCompass::AB, TableCompass::NORTH);
    ihm.addScorePoints( plants * 4); //plante valid + jardiniere x2
    // Dégagement de zone
    //async motion.go(POI::plantSupplySW);
    async motion.go(2113,1000);
    async motion.go(POI::y2);
    probeBorder(TableCompass::EAST, RobotCompass::CA,100);
    probeBorder(TableCompass::NORTH, RobotCompass::AB,100);
    async motion.go(POI::y2);
    // Ajuster le bras pour le panneaux
    actuators.moveElevator(RobotCompass::CA,ElevatorPose::BORDER);
    waitMs(800);
    actuators.close(RobotCompass::CA);
    waitMs(800);

    // S'approcher et tourner les panneaux
    motion.setFeedrate(1.0);
    async motion.go(POI::solarPanelYellow_1);
    ihm.addScorePoints(5); //1 panneaux couleur retournés
    async motion.go(POI::solarPanelYellow_3);
    ihm.addScorePoints(10); //2 panneaux couleur retournés
    async motion.go(2150,1670); // Dégagement
    /*async motion.go(POI::solarPanelOther_3);
    ihm.addScorePoints(5); //1 panneaux gris retournés
    async motion.go(POI::solarPanelOther_1);
    ihm.addScorePoints(10); //2 panneaux gris retournés
    async motion.go(1150,1670); // Dégagement Gris*/

    
    // Ranger les bras
    actuators.moveElevator(RobotCompass::CA,ElevatorPose::UP);
    actuators.open(RobotCompass::CA);
    // Aller en zone de recharge 
    async motion.go(POI::y2);
    ihm.addScorePoints(10); //zone d'arrivée
    // Fin de match
    motion.disengage();
    actuators.disable();
}

void secondaryMatchBlue(){
    Serial.println("Start Match Secondary");
    //while(1){
        actuators.moveForkElevator(RobotCompass::AB,ElevatorPose::DOWN);
        actuators.moveForkElevator(RobotCompass::BC,ElevatorPose::DOWN);
        actuators.moveForkElevator(RobotCompass::CA,ElevatorPose::DOWN);
        actuators.forkDown(RobotCompass::AB);
        //actuators.forkDown(RobotCompass::BC);
        actuators.forkDown(RobotCompass::CA);
        waitMs(2000);
        actuators.moveForkElevator(RobotCompass::AB,ElevatorPose::UP);
        actuators.moveForkElevator(RobotCompass::BC,ElevatorPose::UP);
        actuators.moveForkElevator(RobotCompass::CA,ElevatorPose::UP);
        actuators.forkGrab(RobotCompass::AB);
        //actuators.forkUp(RobotCompass::BC);
        actuators.forkUp(RobotCompass::CA);
        waitMs(2000);
    //}
}

void secondaryMatchYellow(){

}

void waitMs(unsigned long time){
    os.wait(time,false);
    //delay(time);
}

void SlowElevatorUp(RobotCompass rc, int speed){
    //convert %speed into ms
    speed = constrain(speed, 0, 100);
    int ms = map(speed, 0, 100, 50, 0);
    while(!actuators.runElevatorUp(rc)) waitMs(ms);
}

void SlowElevatorDown(RobotCompass rc, int speed){
    //convert %speed into ms
    speed = constrain(speed, 0, 100);
    int ms = map(speed, 0, 100, 50, 0);
    while(!actuators.runElevatorDown(rc)) waitMs(ms);
}

void SlowElevatorGrab(RobotCompass rc, int speed){
    //convert %speed into ms
    speed = constrain(speed, 0, 100);
    int ms = map(speed, 0, 100, 50, 0);
    while(!actuators.runElevatorGrab(rc)) waitMs(ms);
}

void SlowElevatorBorder(RobotCompass rc, int speed){
    //convert %speed into ms
    speed = constrain(speed, 0, 100);
    int ms = map(speed, 0, 100, 50, 0);
    while(!actuators.runElevatorBorder(rc)) waitMs(ms);
}

void SlowElevatorPlanter(RobotCompass rc, int speed){
    //convert %speed into ms
    speed = constrain(speed, 0, 100);
    int ms = map(speed, 0, 100, 50, 0);
    while(!actuators.runElevatorPlanter(rc)) waitMs(ms);
}

void SlowClosing(RobotCompass rc, int speed){
    //convert %speed into ms
    speed = constrain(speed, 0, 100);
    int ms = map(speed, 0, 100, 50, 0);
    while(!actuators.runClosing(rc)) waitMs(ms);
}

void slowOpennig(RobotCompass rc, int speed){
    //convert %speed into ms
    speed = constrain(speed, 0, 100);
    int ms = map(speed, 0, 100, 50, 0);
    while(!actuators.runOpening(rc)) waitMs(ms);
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

    motion.setFeedrate(0.3);
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
