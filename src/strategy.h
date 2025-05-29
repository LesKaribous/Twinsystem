#pragma once
#include "utils/geometry.h"
#include <Adafruit_PWMServoDriver.h>

// Permet la selection du POI selon l'équipe
template<typename T>
inline const T& choose(bool cond, const T& a, const T& b) {
    return cond ? a : b;
}

void recalage();
void match();

void matchA();
void matchB();

// Macros
void waitMs(unsigned long time);
void takeStock(Vec2 target, RobotCompass rc, TableCompass tc);
void takeStockFast(Vec2 target, RobotCompass rc, TableCompass tc);
void buildTribune(Vec2 target, RobotCompass rc, TableCompass tc);
void dropOneLevel(Vec2 target, RobotCompass rc, TableCompass tc);
void nearEnd();
//------------------------------------------------------
// TODO : Integrate Pump and EV into Actuators <3
void initPump();
void startPump(RobotCompass rc);
void stopPump(RobotCompass rc, uint16_t evPulseDuration);
extern Adafruit_PWMServoDriver pwm;
//------------------------------------------------------

RobotCompass nextActuator(RobotCompass rc);         //Recupere l'orientation de l'actionneur suivant
RobotCompass previousActuator(RobotCompass rc);     //Recupere l'orientation de l'actionneur précedant
void probeBorder(TableCompass tc, RobotCompass rc, float clearance, float approachDist = 200.0, float probeDist = 80.0, float feedrate = 0.2 );