#pragma once
#include "utils/geometry.h"
#include "utils/commandHandler.h"
#include <Arduino.h>

void registerCommands();

void command_enable(const args_t& args);
void command_disable(const args_t& args);
void command_status(const args_t& args); //Display all status
void command_debug(const args_t& args); //Display all status
void command_lidarMode(const args_t& args);

//Motion
void command_go(const args_t& args);
void command_goPolar(const args_t& args);
void command_move(const args_t& args);
void command_turn(const args_t& args);
void command_rawTurn(const args_t& args); //turn without optimization (without modulo)
void command_pause(const args_t& args);
void command_resume(const args_t& args);
void command_cancel(const args_t& args);
void command_sleep(const args_t& args);
void command_wake(const args_t& args);
void command_align(const args_t& args);
void command_setAbsolute(const args_t& args);
void command_setRelative(const args_t& args);
void command_setAbsPosition(const args_t& args);
void command_resetCompass(const args_t& args);

//Actuators
void command_raise(const args_t& args);
void command_lower(const args_t& args);
void command_grab(const args_t& args);
void command_open(const args_t& args);
void command_close(const args_t& args);
void command_elevator(const args_t& args);
void command_move_elevator(const args_t& args);

//Routine
void command_recalage(const args_t& args);

//Terminal
void command_help(const args_t& args);
void command_print(const args_t& args);