#pragma once
#include "services/service.h"
#include "utils/timer/timer.h"
#include "tw_threads.h"
#include <map>
#include <queue>
#include <string>

class Service;
class ThreadedService;
using routine_ptr = void (*)();
using routine_arg = void*;

class OS{
public:
    enum SystemState{
        BOOT,    // booting
        MANUAL,    // running services but no programs running
        AUTO, // running services and a program is running
        STOPPED, // stopped services and programs are stopped
        AUTO_PROGRAM, // main program (not really a state since Running will be executed during program)
        MANUAL_PROGRAM, // idle program (not really a state since IDLE will be executed during program)
    };

    void start();
    void stop();
    void run();
    void flush(); //Run the current task until it's done. (Blocking)

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

    Job& wait(unsigned long time, bool runasync = false);
    void waitUntil(Job& job, bool runasync = false);
    void execute(Job& job, bool runasync = true);
    bool isBusy();

private:
    void boot_routine();
    void manual_routine();
    void auto_routine();
    void auto_program_routine();
    void manual_program_routine();
    void stop_routine();
    void executeRoutine(routine_ptr routine);

    Job* currentJob();
    void addJob(Job* job);
    void killCurrentJob();

    std::map<ServiceID, Service*> m_services;
    routine_ptr m_bootRoutine = nullptr; 
    routine_ptr m_manualRoutine = nullptr; 
    routine_ptr m_autoRoutine = nullptr; 
    routine_ptr m_stopRoutine = nullptr; 
    routine_ptr m_auto_programRoutine = nullptr; 
    routine_ptr m_manual_programRoutine = nullptr; 
    SystemState m_state = BOOT;

    Timer m_timer;
    std::queue<Job*> m_jobs;


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
extern OS& os;