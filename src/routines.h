#pragma once
#include "os.h"
extern OS& os;

void onRobotBoot();
void onRobotIdle();
void onRobotRun();
void onRobotStop();

void onTerminalCommand();