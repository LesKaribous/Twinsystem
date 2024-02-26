#pragma once
#include "system/core/lib.h"
#include "system/core/service.h"

class Terminal : public Service{
public :
    
    Terminal();
    ~Terminal(){};

    void update() override;

    String dequeCommand();
    int commandAvailable();

private:
    std::deque<String> _pendingCommands;
};