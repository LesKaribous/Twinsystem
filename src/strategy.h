#pragma once
#include "utils/geometry.h"

void recalage();
void match();

void matchA();
void matchB();

// Macros
void waitMs(unsigned long time);
void takeStock(Vec2 target, RobotCompass rc, TableCompass tc);
void buildTribune(Vec2 target, RobotCompass rc, TableCompass tc);

RobotCompass nextActuator(RobotCompass rc);         //Recupere l'orientation de l'actionneur suivant
RobotCompass previousActuator(RobotCompass rc);     //Recupere l'orientation de l'actionneur précedant
void probeBorder(TableCompass tc, RobotCompass rc, float clearance, float approachDist = 200.0, float probeDist = 80.0 );