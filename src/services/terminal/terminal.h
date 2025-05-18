#pragma once
#include "settings.h"
#include "services/service.h"
#include "os/jobs/job.h"

#include <deque>

class Terminal : public Service{
public :
    
    Terminal();

    void onAttach() override;
    void onUpdate() override;

    String dequeCommand();
    int commandAvailable();

private:
    std::deque<String> _pendingCommands;

    SINGLETON(Terminal)
};

SINGLETON_EXTERN(Terminal, terminal)