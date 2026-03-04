#pragma once
#include "utils/geometry.h"
#include <Adafruit_PWMServoDriver.h>

// Permet la selection du POI selon l'équipe
template<typename T>
inline const T& choose(bool cond, const T& a, const T& b) {
    return cond ? a : b;
}

#define RIGHT true
#define LEFT false

void recalage();
void match();

void matchA();
void matchB();

// Macros
void waitMs(unsigned long time);
void takeStock(Vec2 target, TableCompass tc);
void takeAllStock(Vec2 target, TableCompass tc);
void nearEnd();

//------------------------------------------------------
// TODO : Integrate Pump and EV into Actuators <3
void initPump();
void startPump(RobotCompass rc, bool side);
void stopPump(RobotCompass rc, uint16_t evPulseDuration, bool side);
extern Adafruit_PWMServoDriver pwm;
//------------------------------------------------------

void calibrate();
void probeBorder(TableCompass tc, RobotCompass rc, float clearance, float approachDist = 200.0, float probeDist = 80.0, float feedrate = 0.2 );