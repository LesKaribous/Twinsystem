#pragma once
#include "system/core/lib.h"
#include "system/core/job.h"
#include "system/core/event.h"
#include "system/timer/timer.h"
#include "system/core/service.h"
//#include "system/core/interpreter.h"

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

    //void execute(String& script);
    //void execute(Program& script);

    bool isBusy() const;
    void wait(unsigned long time);  //async
    void waitUntil(Job& obj);       //async

    bool hasService(ServiceID) const;
    bool debug(ServiceID);
    void toggleDebug(ServiceID s);


protected:
    //Interpreter interpreter;

    SystemState getState();
    void loadService(Service* m);
    void update();
    void updateProgram();
    void updateServices();

    inline SystemState systemState() const {return m_currentState;}

    Console& console;

private:
    //Async rountines
    void handleBootState();
    void handleIdleState();
    void handleRunningState();
    void handleStoppedState();

    std::map<ServiceID, Service*> m_services;
    //Program m_program;
    SystemState m_currentState;
    //Utils
    Timer m_timer;
    
    bool m_busy = false;
    Job* m_currentJob = nullptr;
};
