#pragma once
#include "os/os.h"
#include "os/asyncExecutor.h"
#include "console.h"
#include "poi.h"
#include "services/ihm/ihm.h"
#include "services/motion/motion.h"
#include "services/lidar/lidar.h"
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
extern Lidar& lidar;

void robotProgram();//Execute once (onRobotRun will be executed when possible during this program)

void onRobotBoot(); //Execute once at boot
void onRobotIdle(); //Execute before robotProgram (idle loop)
void onRobotRun();  //Execute during robotProgram (run loop)
void onRobotStop(); //Execute while robot stopped (stop loop)

void onTerminalCommand();
void onIntercomConnected();
void onIntercomDisconnected();

void onIntercomMessage();
void onOppenentDetected(const String& arg);
