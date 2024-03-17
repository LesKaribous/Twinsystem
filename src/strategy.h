#pragma once
#include "utils/geometry.h"

void recalage();
void match();

void matchBlue();
void matchYellow();

// Macros
RobotCompass nextActuator(RobotCompass rc);         //Recupere l'orientation de l'actionneur suivant
RobotCompass previousActuator(RobotCompass rc);     //Recupere l'orientation de l'actionneur précedant
void takePlants(Vec2 target, RobotCompass rc, TableCompass tc);
void probeBorder(TableCompass tc, RobotCompass rc, float clearance);