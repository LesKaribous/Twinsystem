#pragma once
#include "system/core/lib.h"
#include "system/core/job.h"
#include "system/core/event.h"
#include "system/debug/console.h"
#include "modules/module.h"

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

    void loadModule(Module* m);

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
