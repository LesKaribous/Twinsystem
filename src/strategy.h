#pragma once
#include "utils/geometry.h"

void recalage();
void recalageSecondary();
void match();

void matchBlue();
void matchYellow();

void secondaryMatchBlue();
void secondaryMatchYellow();
// Macros
void waitMs(unsigned long time);

RobotCompass nextActuator(RobotCompass rc);         //Recupere l'orientation de l'actionneur suivant
RobotCompass previousActuator(RobotCompass rc);     //Recupere l'orientation de l'actionneur précedant

/*
void SlowElevatorUp(RobotCompass rc, int speed);
void SlowElevatorDown(RobotCompass rc, int speed);
void SlowElevatorGrab(RobotCompass rc, int speed);
void SlowElevatorBorder(RobotCompass rc, int speed);
void SlowElevatorPlanter(RobotCompass rc, int speed);
void SlowClosing(RobotCompass rc, int speed);
void slowOpennig(RobotCompass rc, int speed);
*/

void takePlants(Vec2 target, RobotCompass rc, TableCompass tc);
void placePlants(Vec2 target, RobotCompass rc, TableCompass tc, bool planter = true);
void probeBorder(TableCompass tc, RobotCompass rc, float clearance, float approachDist = 200.0, float probeDist = 80.0 );