#pragma once
#include "services/service.h"
#include "utils/timer/timer.h"
#include <map>

class Service;
using routine_ptr = void (*)();

class OS{
public:
    enum SystemState{
        BOOT,    // booting
        IDLE,    // running services but no programs running
        RUNNING, // running services and a program is running
        STOPPED, // stopped services and programs are stopped
    };

    void loop();
    void setRountine(SystemState state, routine_ptr func_ptr);
    void setState(SystemState state);

    void updateServices();
    void enable(ServiceID);
    void disable(ServiceID);
    void attachService(Service*);
    bool hasService(ServiceID) const;
    bool statusService(ServiceID serviceID) const;
    bool debug(ServiceID);
    void toggleDebug(ServiceID s);

    void wait(unsigned long time, bool async = false);
    void waitUntil(Job& time, bool async = false);
    bool isBusy() const;

private:
    void boot();
    void idle();
    void run();
    void stop();
    void executeRoutine(routine_ptr routine);

    std::map<ServiceID, Service*> m_services;
    routine_ptr m_bootRoutine = nullptr; 
    routine_ptr m_idleRoutine = nullptr; 
    routine_ptr m_runRoutine = nullptr; 
    routine_ptr m_stopRoutine = nullptr; 
    SystemState m_state = BOOT;

    Timer m_timer;
    Job* m_currentJob = nullptr;

//Singleton
public:
    static inline OS& instance(){return m_instance;}

    OS(const OS&) = delete;
    OS(OS&&) = delete;
    OS& operator=(const OS&) = delete;
    OS& operator=(OS&&) = delete;

private:
    static OS m_instance;
    OS(){};
};
  