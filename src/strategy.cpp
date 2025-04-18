#include "strategy.h"
#include "poi.h"
#include "score.h"
#include "robot.h"
#include "routines.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void match(){
    //start match
    motion.setFeedrate(1.0);
    matchB();
    /*
    if(ihm.isColor(Settings::BLUE)) 
        matchA();
    else 
        matchB();
    */
}

void waitMs(unsigned long time){
    os.wait(time,false);
    //delay(time);
}

void recalage(){
    motion.engage();
    waitMs(800);
    motion.setFeedrate(0.3);

    motion.setAbsPosition(Vec3(0,0,DEG_TO_RAD *90));
    if(ihm.isColor(Settings::BLUE)){
        motion.disableCruiseMode();
        probeBorder(TableCompass::SOUTH, RobotCompass::BC,100);
        probeBorder(TableCompass::EAST,  RobotCompass::CA,100);

        //motion.enableCruiseMode();
        
        async motion.go(POI::b2);
        //motion.disableCruiseMode();
        async motion.align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));
        motion.setAbsPosition(Vec3(POI::b2, motion.getOrientation()));
        actuators.storePlank(RobotCompass::AB);
        actuators.moveElevator(RobotCompass::BC,ElevatorPose::UP);
        actuators.storePlank(RobotCompass::CA);

    }else{
        //motion.setAbsPosition({-1,-1,PI});
        probeBorder(TableCompass::SOUTH, RobotCompass::BC,100);
        probeBorder(TableCompass::WEST,  RobotCompass::B,100);

        //motion.enableCruiseMode();
        motion.disableCruiseMode();
        async motion.go(POI::y2);
        //motion.disableCruiseMode();
        async motion.align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));
        //motion.setAbsPosition(Vec3(POI::y2, motion.getOrientation()));
        actuators.storePlank(RobotCompass::AB);
        actuators.moveElevator(RobotCompass::BC,ElevatorPose::UP);
        actuators.storePlank(RobotCompass::CA);
    }
    motion.disengage();
    motion.setFeedrate(1.0);
    initPump(); //TODO : Integrate into Actuators 
}

//BLUE
void matchA(){
    //do Match
    //async motion.turn(3600);
    //motion.enableCruiseMode();
    for(int i = 0; i < 10; i++){
        async motion.go(POI::b2 + Vec2(0,-600));
        async motion.go(POI::b2);
    }

    chrono.onMatchNearlyFinished();
    chrono.onMatchFinished();
}

void matchB(){
    motion.setFeedrate(1.0);
    // Select POI on color Team
    bool isYellow = ihm.isColor(Settings::YELLOW);
    motion.enableCruiseMode();
    // -------------------------------------------

    // Step 2 - POI BannerYellow
    async motion.go(
        choose(isYellow,
                POI::BannerYellow + Vec2(0,50),
                POI::BannerBlue   + Vec2(0,50))
    );

    //---- Drop Banner ----
    actuators.moveElevator(RobotCompass::BC, ElevatorPose::DOWN);
    ihm.addScorePoints(Score::BannerPoints);
    waitMs(800);
    async motion.go(
        choose(isYellow,
                POI::y2,
                POI::b2)
    );

    //---- Take Stock ----
    takeStock(
        choose(isYellow,
            POI::stock_4,
            POI::stock_5),
        RobotCompass::AB, 
        TableCompass::NORTH
    );

    //---- Build Tribune ----
    buildTribune(
        choose(isYellow,
               POI::constAreaYellow_2,
               POI::constAreaBlue_2),
        RobotCompass::AB,
        TableCompass::SOUTH
    );

    async motion.go(
        choose(isYellow,
               POI::yellowWaypoint_1,
               POI::blueWaypoint_1)
    );

    async motion.align(RobotCompass::AB, getCompassOrientation(TableCompass::SOUTH));

    async motion.go(
        choose(isYellow,
               POI::yellowWaypoint_2,
               POI::blueWaypoint_2)
    );

    async motion.go(
        choose(isYellow,
               POI::stock_3,
               POI::stock_6)
    );

    //Wait for the end to arrive (left space for PAMI)
    chrono.onMatchNearlyFinished(); 
    chrono.onMatchFinished();
}

void nearEnd(){
    //if(motion.isPending())motion.forceCancel();
    motion.setFeedrate(1.0);
    //nav.setAbsolute();
    actuators.moveElevator(RobotCompass::AB, ElevatorPose::DOWN);
    actuators.moveElevator(RobotCompass::BC, ElevatorPose::DOWN);
    actuators.moveElevator(RobotCompass::CA, ElevatorPose::DOWN);
    safety.enable();

    // Go to the waiting point near SIMAs
    if(ihm.isColor(Settings::BLUE)) {
        async motion.go(POI::waitPointBlueTemp);
        async motion.go(POI::waitPointBlue);
    }
    else {
        async motion.go(POI::waitPointYellowTemp);
        async motion.go(POI::waitPointYellow);
    }

    // Time to wait befor SIMAs leave the Backstage
    unsigned long left = chrono.getTimeLeft();
    unsigned long waitSima = (left > 5000) ? (left - 5000) : 0; 
    // Wait for SIMAs
    waitMs(waitSima);

    // Got to the Backstage
    if(ihm.isColor(Settings::BLUE)) async motion.go(POI::b1);
    else async motion.go(POI::y1);

    ihm.addScorePoints(Score::RobotInArrivalZonePoints);
    waitMs(200);
    //ihm.onUpdate();
    chrono.onMatchFinished();
}

void takeStock(Vec2 target, RobotCompass rc, TableCompass tc){
    if(rc == RobotCompass::BC){
        THROW("wrong compass");
        return;
    }

    RobotCompass nextCompass = (rc == RobotCompass::AB) ? RobotCompass::CA : RobotCompass::AB;

    float approachOffset = 250;
    float grabOffset = 175;
    float canOffset = 100;
    float canGrab = 100;
    unsigned long delayTime = 400;

    Vec2 approach = target - PolarVec(getCompassOrientation(tc)*DEG_TO_RAD, approachOffset).toVec2();
    Vec2 grab = target - PolarVec(getCompassOrientation(tc)*DEG_TO_RAD, grabOffset).toVec2();

    // ---- Take first planks ----
    async motion.go(approach); 
    async motion.align(rc, getCompassOrientation(tc));
    
    actuators.moveElevator(rc, ElevatorPose::UP);
    actuators.grabPlank(rc);
    startPump(rc);
    waitMs(delayTime);
    async motion.go(grab);
    actuators.moveElevatorOffset(rc, ElevatorPose::DOWN, -30,50);
    waitMs(1000);
    actuators.storePlank(rc,50);
    waitMs(delayTime);

    // ---- Take second planks ----
    async motion.go(approach); 
    async motion.align(nextCompass, getCompassOrientation(tc));

    actuators.moveElevator(nextCompass, ElevatorPose::UP);
    actuators.grabPlank(nextCompass);
    startPump(nextCompass);
    waitMs(delayTime);
    async motion.go(grab);
    waitMs(delayTime);
    actuators.moveElevatorOffset(nextCompass, ElevatorPose::DOWN, -15,50);
    waitMs(1000);
    actuators.storePlank(nextCompass,50);
    waitMs(delayTime);

    // ---- Take second can ----
    async motion.goPolar(getCompassOrientation(tc)+90, canOffset); 
    actuators.grab(nextCompass);
    async motion.goPolar(getCompassOrientation(tc), canGrab);
    async motion.goPolar(getCompassOrientation(tc), -canGrab);

    // ---- take first can ---- 
    async motion.align(rc, getCompassOrientation(tc));
    async motion.goPolar(getCompassOrientation(tc)-90, canOffset*2);
    actuators.grab(rc);
    async motion.goPolar(getCompassOrientation(tc), canGrab);
    async motion.goPolar(getCompassOrientation(tc), -canGrab);

    motion.setFeedrate(1.0);
}

void buildTribune(Vec2 target, RobotCompass rc, TableCompass tc){
    if(rc == RobotCompass::BC){
        THROW("wrong compass");
        return;
    }

    RobotCompass nextCompass = (rc == RobotCompass::AB) ? RobotCompass::CA : RobotCompass::AB;

    float approachOffset = 250;
    float buildOffset = 165;
    float dropPlankOffset = 25;
    unsigned long delayTime = 400;

    Vec2 approach = target - PolarVec(getCompassOrientation(tc)*DEG_TO_RAD, approachOffset).toVec2();
    Vec2 build = target - PolarVec(getCompassOrientation(tc)*DEG_TO_RAD, buildOffset).toVec2();

    // ---- Approach construction area ----
    async motion.align(rc, getCompassOrientation(tc));
    async motion.go(approach); 

    // !!!! Disable safety !!!!
    safety.disable();

    // ---- Build level 1 ----
    async motion.go(build);
    actuators.drop(rc);
    waitMs(delayTime);
    async motion.goPolar(getCompassOrientation(tc), -dropPlankOffset);
    waitMs(delayTime);
    actuators.dropPlank(rc,50);
    waitMs(1000);
    stopPump(rc,500);
    waitMs(1000);
    actuators.storePlank(rc);
    waitMs(delayTime);
    async motion.go(approach);
    ihm.addScorePoints(Score::TribuneLevel1Points);

    // ---- Build level 2 ----
    actuators.dropPlank(nextCompass,50);
    actuators.moveElevatorOffset(nextCompass, ElevatorPose::UP, -20,50);
    waitMs(delayTime);
    async motion.align(nextCompass, getCompassOrientation(tc));
    waitMs(delayTime);
    async motion.go(build);
    actuators.drop(nextCompass);
    waitMs(delayTime);
    async motion.goPolar(getCompassOrientation(tc), -dropPlankOffset);
    waitMs(delayTime);
    stopPump(nextCompass,500);
    waitMs(1000);
    actuators.storePlank(nextCompass);
    waitMs(delayTime);
    async motion.go(approach);
    waitMs(delayTime);
    actuators.moveElevator(nextCompass, ElevatorPose::DOWN,50);
    ihm.addScorePoints(Score::TribuneLevel2Points);

    motion.setFeedrate(1.0);
    // !!!! Engage safety !!!!
    safety.enable();
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
    motion.disableCruiseMode();
	bool m_probing = true;
    motion.setFeedrate(0.1);
	async motion.align(rc, getCompassOrientation(tc));

    motion.setRelative();
	async motion.goPolar(getCompassOrientation(rc),approachDist);
	async motion.goPolar(getCompassOrientation(rc),probeDist);

	float _offset = getOffsets(rc);

	Vec3 position = motion.getAbsPosition();

	if(tc == TableCompass::NORTH){
		position.y = 0.0 + _offset; //We hit Xmax
		//_probedX = true;
	}else if(tc == TableCompass::SOUTH){
		position.y = 2000.0 - _offset; //We hit Xmin
		//_probedX = true;
	}else if(tc == TableCompass::EAST){
		position.x = 3000.0 - _offset; //We hit Ymax
		//_probedY = true;
	}else if(tc == TableCompass::WEST){
		position.x = 0.0 + _offset; //We hit Ymin
		//_probedY = true;
	}
    position.c = DEG_TO_RAD * (getCompassOrientation(tc) - getCompassOrientation(rc));
	Console::println(position.c * RAD_TO_DEG);
    motion.setAbsPosition(position);
    delay(500);
	async motion.goPolar(getCompassOrientation(rc),-clearance);
	if(wasAbsolute) motion.setAbsolute();
    motion.setFeedrate(currentFeedrate);

}

void initPump(){
    pwm.begin();
    /*
     * In theory the internal oscillator (clock) is 25MHz but it really isn't
     * that precise. You can 'calibrate' this by tweaking this number until
     * you get the PWM update frequency you're expecting!
     * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
     * is used for calculating things like writeMicroseconds()
     * Analog servos run at ~50 Hz updates, It is importaint to use an
     * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
     * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
     *    the I2C PCA9685 chip you are setting the value for.
     * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
     *    expected value (50Hz for most ESCs)
     * Setting the value here is specific to each individual I2C PCA9685 chip and
     * affects the calculations for the PWM update frequency. 
     * Failure to correctly set the int.osc value will cause unexpected PWM results
     */
    pwm.setOscillatorFrequency(50000000);
    pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
  
    // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
    // some i2c devices dont like this so much so if you're sharing the bus, watch
    // out for this!
    //Wire.setClock(400000);  
}

void setOutput(uint8_t pin, bool state) {
    if (state) {
      pwm.setPWM(pin, 4096, 0);  // ON
    } else {
      pwm.setPWM(pin, 0, 4096);  // OFF
    }
}

void startPump(RobotCompass rc){
    uint8_t evPin ;
    uint8_t pumpPin ;
    if(rc == RobotCompass::AB) evPin = Pin::PCA9685::EV_AB ;
    else evPin = Pin::PCA9685::EV_CA;
    if(rc == RobotCompass::AB) pumpPin = Pin::PCA9685::PUMP_AB;
    else pumpPin = Pin::PCA9685::PUMP_CA;
    setOutput(evPin, false);  // Fermer l'électrovanne
    setOutput(pumpPin, true); // Démarrer la pompe
}

void stopPump(RobotCompass rc, uint16_t evPulseDuration){
    uint8_t evPin ;
    uint8_t pumpPin ;
    if(rc == RobotCompass::AB) evPin = Pin::PCA9685::EV_AB ;
    else evPin = Pin::PCA9685::EV_CA;
    if(rc == RobotCompass::AB) pumpPin = Pin::PCA9685::PUMP_AB;
    else pumpPin = Pin::PCA9685::PUMP_CA;
    setOutput(pumpPin, false); // Stopper la pompe
    setOutput(evPin, true);    // Ouvrir l’EV
    waitMs(evPulseDuration);    // Maintenir l’EV ouverte
    setOutput(evPin, false);   // Fermer l’EV
}
