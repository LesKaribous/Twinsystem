#pragma once
#include "os/console.h"
#include "services/intercom/intercom.h"

void robotIdleProgram();//Execute infinetly before match (onRobotIdle will be executed in parallel (when possible) during this program)
void robotProgram();//Execute once (onRobotRun will be executed in parallel (when possible) during this program)

void onRobotBoot(); //Execute once at boot
void onRobotIdle(); //Execute before robotProgram (idle loop)
void onRobotRun();  //Execute during robotProgram (run loop)
void onRobotStop(); //Execute while robot stopped (stop loop)

void onTerminalCommand();
void onIntercomConnected();
void onIntercomDisconnected();

void onIntercomMessage(const Request&);
void onOccupancyResponse(const Request&);
void onOccupancyTimeout();

