#pragma once
#include "system/core/service.h"
#include "system/core/interpreter.h"
#include "console.h"


class Terminal : public Service{
public :
    
    Terminal();
    ~Terminal(){};

    void update() override;

    Command dequeCommand();
    int commandAvailable();

private:
    std::deque<Command> _pendingCommands;
};