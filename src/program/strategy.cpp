#include "strategy.h"
#include "config/poi.h"
#include "config/score.h"
#include "config/env.h"
#include "routines.h"
#include "mission.h"
#include "services/lidar/occupancy.h"

// TODO : déplacer dans Actuators
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


// ============================================================
//  Helpers internes
// ============================================================

// Durée estimée des déplacements principaux (ms) — à affiner selon les mesures
namespace Timing {
    constexpr uint32_t COLLECT_STOCK_A = 8000;
    constexpr uint32_t COLLECT_STOCK_B = 6000;
}

// Offset commun factored
static void collectStock(Vec2 target, TableCompass tc, RobotCompass rc) {
    constexpr float    APPROACH_OFFSET = 300.0f;
    constexpr float    GRAB_OFFSET     = 155.0f;
    constexpr uint32_t GRAB_DELAY_MS   = 1000;

    Vec2 approach = target - PolarVec(getCompassOrientation(tc) * DEG_TO_RAD, APPROACH_OFFSET).toVec2();
    Vec2 grab     = target - PolarVec(getCompassOrientation(tc) * DEG_TO_RAD, GRAB_OFFSET).toVec2();

    async motion.goAlign(approach, rc, getCompassOrientation(tc));
    async motion.goAlign(grab,     rc, getCompassOrientation(tc));

    actuators.moveElevator(rc, ElevatorPose::DOWN);
    waitMs(GRAB_DELAY_MS);
    actuators.grab(rc);
    waitMs(GRAB_DELAY_MS);
    actuators.moveElevator(rc, ElevatorPose::STORE);

    safety.enable();
    motion.setFeedrate(1.0f);
}


// ============================================================
//  Définition des blocs — une fonction par objectif
//  Retourne SUCCESS ou FAILED selon le résultat
// ============================================================

// Retourne true si la couleur de l'objet justifie une collecte.
// UNKNOWN = bénéfice du doute (on tente quand même).
// NONE    = pas d'objet, inutile d'y aller.
static bool isColorUseful(ObjectColor color) {
    if (color == ObjectColor::NONE)    return false;
    if (color == ObjectColor::UNKNOWN) return true; // lidar/vision non dispo → on tente
    // Adapter selon les règles du match :
    // ex: on ne collecte que les objets d'une certaine couleur
    return true;
}

static BlockResult blockCollectA() {
    // Interroger la couleur avant de se déplacer (sync, 400ms max)
    ObjectColor color = vision.queryColorSync(POI::testA, 400);
    Console::info("Strategy") << "Zone A: " << colorName(color) << Console::endl;
    if (!isColorUseful(color)) return BlockResult::FAILED;

    async motion.goAlign(POI::testA, RobotCompass::AB, getCompassOrientation(TableCompass::SOUTH));
    if (!motion.wasSuccessful()) return BlockResult::FAILED;

    collectStock(POI::testA, TableCompass::SOUTH, RobotCompass::AB);
    return motion.wasSuccessful() ? BlockResult::SUCCESS : BlockResult::FAILED;
}

static BlockResult blockCollectB() {
    ObjectColor color = vision.queryColorSync(POI::testB, 400);
    Console::info("Strategy") << "Zone B: " << colorName(color) << Console::endl;
    if (!isColorUseful(color)) return BlockResult::FAILED;

    async motion.goAlign(POI::testB, RobotCompass::CA, getCompassOrientation(TableCompass::EAST));
    if (!motion.wasSuccessful()) return BlockResult::FAILED;

    collectStock(POI::testB, TableCompass::EAST, RobotCompass::CA);
    return motion.wasSuccessful() ? BlockResult::SUCCESS : BlockResult::FAILED;
}


// ============================================================
//  Conditions de faisabilité
//  Appelées par Mission AVANT d'envoyer le robot vers un bloc.
//  Retourne false → bloc skippé immédiatement, Mission tente le suivant.
//
//  ZONE_CHECK_RADIUS : rayon (mm) autour du POI vérifié dans l'occupancy map.
//  ~450mm couvre le POI + la zone d'approche (offset 300mm).
//  Si la carte est vide (lidar secondaire non connecté),
//  isZoneOccupied retourne false → check passe et le robot tente quand même.
// ============================================================

namespace ZoneCheck {
    constexpr float RADIUS = 450.0f; // mm (~3 cellules de 150mm)
}

static bool isZoneAFree() {
    bool occupied = occupancy.isZoneOccupied(POI::testA, ZoneCheck::RADIUS);
    if (occupied)
        Console::warn("Strategy") << "Zone A occupee — bloc skipe" << Console::endl;
    return !occupied;
}

static bool isZoneBFree() {
    bool occupied = occupancy.isZoneOccupied(POI::testB, ZoneCheck::RADIUS);
    if (occupied)
        Console::warn("Strategy") << "Zone B occupee — bloc skipe" << Console::endl;
    return !occupied;
}


// ============================================================
//  match() — point d'entrée du match
// ============================================================

void match() {
    motion.setFeedrate(1.0f);
    motion.enableCruiseMode();

    Mission mission;
    mission
        .setMode(Mission::SelectMode::PRIORITY)
        .setTimeProvider([]() -> uint32_t {
            long t = chrono.getTimeLeft();
            return (t > 0) ? (uint32_t)t : 0u;
        })
        .setSafetyMargin(5000)  // Ne pas démarrer un bloc si < 5s restantes
        // { nom,           priorité, points, durée estimée (ms), action,       faisabilité }
        .add({ "collect_A", 10,       150,    Timing::COLLECT_STOCK_A, blockCollectA, isZoneAFree })
        .add({ "collect_B",  8,        80,    Timing::COLLECT_STOCK_B, blockCollectB, isZoneBFree });

    mission.run();

    chrono.onMatchNearlyFinished();
    chrono.onMatchFinished();
}

void waitMs(unsigned long time){
    os.wait(time);
    //delay(time);
}

void recalage(){
    motion.engage();
    //motion.disableCruiseMode();
    
    waitMs(600);

    if(ihm.isColor(Settings::BLUE)){
        motion.setAbsPosition(Vec3( Vec2(250,250), -90 * DEG_TO_RAD));

        /*
        motion.setFeedrate(0.2);
        probeBorder(TableCompass::SOUTH, RobotCompass::BC,100);
        probeBorder(TableCompass::EAST,  RobotCompass::CA,100);//when starting this line
        motion.setFeedrate(1.0);
        */
        //calibrate();

        //async motion.go(POI::testB);
        //async motion.go(POI::b2);
        
        //async motion.align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));
        //motion.setAbsPosition(Vec3(POI::b2, motion.getOrientation()));

    }else{
        motion.setAbsPosition(Vec3(3000 - 200 , 250 ,-90 * DEG_TO_RAD));
        /*
        motion.setFeedrate(0.2);
        probeBorder(TableCompass::SOUTH, RobotCompass::BC,100);
        probeBorder(TableCompass::WEST,  RobotCompass::AB,100);
        motion.setFeedrate(1.0);
        */
        //calibrate();

        //async motion.go(POI::y2);
        //async motion.go(POI::y2);

        //async motion.align(RobotCompass::BC, getCompassOrientation(TableCompass::SOUTH));
        //motion.setAbsPosition(Vec3(POI::y2, motion.getOrientation()));
    }
    //motion.disengage();
    motion.setFeedrate(1.0);
    
    initPump(); //TODO : Integrate into Actuators 
}


void nearEnd(){
    //if(motion.isPending())motion.forceCancel();
    motion.setFeedrate(1.0);
    //nav.setAbsolute();
    safety.enable();

    // Go to the waiting point near SIMAs
    if(ihm.isColor(Settings::BLUE)) {
        async motion.go(POI::home);
    }
    else {
        async motion.go(POI::home);
    }

    // // Time to wait befor SIMAs leave the Backstage
    // unsigned long left = chrono.getTimeLeft();
    // unsigned long waitSima = (left > 5000) ? (left - 5000) : 0; 
    // // Wait for SIMAs
    // waitMs(waitSima);

    // // Got to the Backstage
    // if(ihm.isColor(Settings::BLUE)) async motion.go(POI::b1);
    // else async motion.go(POI::y1);

    // ihm.addScorePoints(Score::RobotInArrivalZonePoints);
    // waitMs(200);
    // //ihm.onUpdate();
    chrono.onMatchFinished();
}

// takeAllStock / takeStock ont été mergées en collectStock() — voir ci-dessus


RobotCompass nextActuator(RobotCompass rc){
    int RobotCompassSize = 6;
    return static_cast<RobotCompass>((static_cast<int>(rc) + 2) % RobotCompassSize);
}

RobotCompass previousActuator(RobotCompass rc){
    int RobotCompassSize = 6;
    return static_cast<RobotCompass>((static_cast<int>(rc) + RobotCompassSize - 2) % RobotCompassSize);
}


void calibrate(){
    motion.disableCruiseMode();
    //motion.cancelOnCollide(false);

    float start = localisation.getPosition().x;
    float distance = 0;
    float distanceGoal = 400;
    float scale = 0;
    float current = 0;

    for(int i = 0; i < 3; i++){
        start = localisation.getPosition().x;
	    async motion.goPolar(0,distanceGoal);
        current = Vec2(localisation.getPosition()).mag();
        distance = fabs(current - start);
        Console::info() << "distance : " << current - start << "|" << 400 << Console::endl;
        scale = distanceGoal/distance;
        localisation.setLinearScale(scale);

        start = localisation.getPosition().x;
	    async motion.goPolar(0,-distanceGoal);
        current = Vec2(localisation.getPosition()).mag();
        distance = fabs(current - start);
        Console::info() << "distance : " << current - start << "|" << 400 << Console::endl;
        scale = distanceGoal/distance;
        localisation.setLinearScale(scale);

        
    }

    //motion.cancelOnCollide(false);
    motion.enableCruiseMode();
}


void probeBorder(TableCompass tc, RobotCompass rc, float clearance, float approachDist, float probeDist, float feedrate){
    
	boolean wasAbsolute = motion.isAbsolute();
    float currentFeedrate = motion.getFeedrate();
    actuators.moveElevator(rc, ElevatorPose::UP);
    
    motion.setFeedrate(feedrate);
	async motion.align(rc, getCompassOrientation(tc));
    motion.setRelative();

    motion.disableCruiseMode();
    motion.cancelOnCollide(true);
	async motion.goPolar(getCompassOrientation(rc),approachDist);
	async motion.goPolar(getCompassOrientation(rc),probeDist);
    motion.cancelOnCollide(false);
    motion.enableCruiseMode();
    
	float _offset = getOffsets(rc);
    Console::println(_offset);
	Vec3 position = motion.estimatedPosition();
    Console::println(position);

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
	
    motion.setAbsPosition(position);
    delay(200);
    
    if(clearance != 0){ 
        async motion.goPolar(getCompassOrientation(rc),-clearance);
    }
    
	if(wasAbsolute) motion.setAbsolute();
    motion.setFeedrate(currentFeedrate);
    actuators.moveElevator(rc, ElevatorPose::DOWN);
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

void startPump(RobotCompass rc, bool side){
    uint8_t evPin ;
    uint8_t pumpPin ;
    if(side) evPin = Pin::PCA9685::EV_CA_RIGHT ;
    else evPin = Pin::PCA9685::EV_CA_LEFT;
    if(side) pumpPin = Pin::PCA9685::PUMP_CA_RIGHT;
    else pumpPin = Pin::PCA9685::PUMP_CA_LEFT;
    setOutput(evPin, false);  // Fermer l'électrovanne
    setOutput(pumpPin, true); // Démarrer la pompe
}

void stopPump(RobotCompass rc, uint16_t evPulseDuration, bool side){
    uint8_t evPin ;
    uint8_t pumpPin ;
    if(side) evPin = Pin::PCA9685::EV_CA_RIGHT ;
    else evPin = Pin::PCA9685::EV_CA_LEFT;
    if(side) pumpPin = Pin::PCA9685::PUMP_CA_RIGHT;
    else pumpPin = Pin::PCA9685::PUMP_CA_LEFT;
    setOutput(pumpPin, false); // Stopper la pompe
    setOutput(evPin, true);    // Ouvrir l’EV
    waitMs(evPulseDuration);    // Maintenir l’EV ouverte
    setOutput(evPin, false);   // Fermer l’EV
}
