#pragma once
#include "core/lib.h"
#include "core/job.h"
#include "core/event.h"
#include "core/module.h"
#include "debug/console.h"

enum SystemState{
    BOOT,
    IDLE,
    ARMED,
    RUNNING,
    STOPPED,
};

class System{
public :
    System();
    ~System();
    void update();

    void enable(SystemModule module);
    void disable(SystemModule module);

    void registerModule(Module*);

    void pollModules();
    void dispatchEvent(std::shared_ptr<Event> e);

    friend class SystemApplication;

protected:

    SystemState state();

private:

    //Async rountines
    void handleBootState();
    void handleIdleState();
    void handleArmedState();
    void handleRunningState();
    void handleStoppedState();

    std::vector<Module*> m_modules;
    SystemState m_currentState;
};
