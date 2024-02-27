#pragma once
#include "system/core/lib.h"
#include "system/core/job.h"
#include "system/core/event.h"
#include "system/timer/timer.h"
#include "system/core/service.h"
#include "system/interpreter/interpreter.h"

enum SystemState{
    BOOT,    // booting
    IDLE,    // running services but no programs running
    RUNNING, // running services and a program is running
    STOPPED, // stopped services and programs are stopped
};

class Console;
class Service;
enum ServiceID;

class SystemBase{
public :

    SystemBase();
    ~SystemBase();

    void enable(ServiceID serviceID);
    void disable(ServiceID serviceID);
    bool statusService(ServiceID serviceID);

    void execute(String& script);
    void execute(Program& script);

    bool isBusy() const;
    void wait(unsigned long time, bool async = true);  //async
    void waitUntil(Job& obj, bool async = true);       //async

    bool hasService(ServiceID) const;
    bool debug(ServiceID);
    void toggleDebug(ServiceID s);
    void update();

protected:
    Interpreter interpreter;

    SystemState getState();
    inline void setSystemState(SystemState state){m_currentState = state;}

    void loadService(Service* m);
    void loadServiceInterrupt(Service* m, double interuptInterval);
    void updateProgram();
    void updateServices();
    void updateInterupt();

    inline SystemState systemState() const {return m_currentState;}

    Console& console;

    //Async rountines
    virtual void handleBootState() = 0;
    virtual void handleIdleState() = 0;
    virtual void handleRunningState() = 0;
    virtual void handleStoppedState() = 0;

private:
    std::map<ServiceID, Service*> m_services;
    SystemState m_currentState;
    //Utils
    Timer m_timer;
    
    bool m_busy = false;
    Job* m_currentJob = nullptr;
};
