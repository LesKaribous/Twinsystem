#pragma once
#include "system/core/lib.h"
#include "system/core/job.h"
#include "system/core/event.h"

#include "system/core/service.h"


enum SystemState{
    BOOT,    // booting
    IDLE,    // running services but no programs running
    RUNNING, // running services and a program is running
    STOPPED, // stopped services and programs are stopped
};

class SystemBase{
public :
    SystemBase();
    ~SystemBase();

    void enable(ServiceID serviceID);
    void disable(ServiceID serviceID);
    bool statusService(ServiceID serviceID);

protected:
    SystemState getState();
    void loadService(Service* m);
    void update();
    void updateProgram();
    void updateServices();

    inline SystemState systemState() const {return m_currentState;}

private:
    //Async rountines
    void handleBootState();
    void handleIdleState();
    void handleRunningState();
    void handleStoppedState();

    //Program* program;
    std::vector<Service*> m_services;
    SystemState m_currentState;
    
};
