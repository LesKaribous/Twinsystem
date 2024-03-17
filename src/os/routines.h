#pragma once
#include "os/os.h"
#include "console.h"
#include "poi.h"
#include "services/ihm/ihm.h"
#include "services/motion/motion.h"
#include "services/intercom/intercom.h"
#include "services/actuators/actuators.h"
#include "services/terminal/terminal.h"
#include "utils/interpreter/interpreter.h"

extern OS& os;
extern IHM& ihm;
extern Motion& motion;
extern Actuators& actuators;
extern Intercom& intercom;
extern Terminal& terminal;

void onRobotBoot();
void onRobotIdle();
void onRobotRun();
void onRobotStop();

void onTerminalCommand();
void onIntercomConnected();
void onIntercomDisconnected();

void matchBlue();
void matchYellow();

void recalage();

// Macros
RobotCompass nextActuator(RobotCompass rc);         //Recupere l'orientation de l'actionneur suivant
RobotCompass previousActuator(RobotCompass rc);     //Recupere l'orientation de l'actionneur précedant
void takePlants(Vec2 target, RobotCompass rc, TableCompass tc);
void probeBorder(TableCompass tc, RobotCompass rc, float clearance);