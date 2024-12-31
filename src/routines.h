#pragma once
#include "os/console.h"
#include "services/intercom/intercom.h"

void robotProgramManual();//Execute infinetly before match (onRobotIdle will be executed in parallel (when possible) during this program)
void robotProgramAuto();//Execute once (onRobotRun will be executed in parallel (when possible) during this program)

void onRobotBoot(); //Execute once at boot
void onRobotManual(); //Execute before robotProgram (idle loop)
void onRobotAuto();  //Execute during robotProgram (run loop)
void onRobotStop(); //Execute while robot stopped (stop loop)

void onTerminalCommand();
void onIntercomConnected();
void onIntercomDisconnected();

void onIntercomRequest(Request&);
void onIntercomRequestReply(Request&);

void onMatchNearEnd();
void onMatchEnd();

void onOccupancyResponse(Request&);
void onOccupancyTimeout();

