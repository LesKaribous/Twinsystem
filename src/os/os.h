#pragma once
#include "services/service.h"
#include "utils/interpreter/program.h"
#include "utils/timer/timer.h"
#include "threads/tw_threads.h"
#include <map>
#include <list>
#include <queue>
#include <string>

class Service;
class ThreadedService;
using routine_ptr = void (*)();
using routine_arg = void*;

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

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
    void reboot();
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

    void wait(unsigned long time);
    void execute(JobHandle job, bool runasync = true);
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
    void addJob(JobHandle job);
    void killCurrentJob();

    std::map<ServiceID, Service*> m_services;

    routine_ptr m_bootRoutine = nullptr; 
    routine_ptr m_manualRoutine = nullptr; 
    routine_ptr m_autoRoutine = nullptr; 
    routine_ptr m_stopRoutine = nullptr; 
    routine_ptr m_auto_programRoutine = nullptr; 
    routine_ptr m_manual_programRoutine = nullptr; 
    SystemState m_state = BOOT;

    Program script;

    Timer m_timer;
    std::queue<JobHandle, std::list<JobHandle>> m_jobs;


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