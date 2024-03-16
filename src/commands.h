#pragma once
#include "utils/geometry.h"
#include "utils/commandHandler.h"
#include <Arduino.h>

void registerCommands();

void command_enable(const String& args);
void command_disable(const String& args);
void command_status(const String& args); //Display all status
void command_status(const String& args); //Display single status
void command_debug(const String& args); //Display all status
void command_debug(const String& args); //Display single status

//Motion
void command_go(const String& args);
void command_move(const String& args);
void command_turn(const String& args);
void command_pause(const String& args);
void command_resume(const String& args);
void command_cancel(const String& args);
void command_sleep(const String& args);
void command_wake(const String& args);
void command_align(const String& args);
void command_setAbsolute(const String& args);
void command_setRelative(const String& args);
void command_setAbsPosition(const String& args);
void command_resetCompass(const String& args);

//Actuators
void command_grab(const String& args);
void command_ungrab(const String& args);
void command_open(const String& args);
void command_close(const String& args);
void command_openTrap(const String& args);
void command_closeTrap(const String& args);

//Terminal
void command_help(const String& args);
void command_print(const String& args);