#pragma once
#include "os.h"
#include "console.h"
#include "services/ihm/ihm.h"
#include "services/motion/motion.h"
#include "services/actuators/actuators.h"
#include "services/terminal/terminal.h"
#include "utils/interpreter/interpreter.h"

extern OS& os;
extern IHM& ihm;
extern Motion& motion;
extern Actuators& actuators;
extern Terminal& terminal;

void onRobotBoot();
void onRobotIdle();
void onRobotRun();
void onRobotStop();

void onTerminalCommand();

void probeBorder(TableCompass tc, RobotCompass rc);