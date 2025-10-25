#pragma once
#include "os/singleton.h"
#include "threads/tw_threads.h"
#include <map>
#include <list>

using routine_ptr = void (*)();
using routine_arg = void*;

enum CycleFrequency{
    T_1US,
    T_10US,
    T_100US,
    T_1MS,
    T_10MS,
    T_100MS,
    T_1S,
    T_10S,
    T_ONCE
};

class CycleManager{
    SINGLETON(CycleManager)
    CycleManager(){};

public:
    void start();
    void stop();
    void registerCycle(CycleFrequency freq, routine_ptr func);
    void runCycle(CycleFrequency freq);
    void updateStats(CycleFrequency freq, long time);

    long getCycleFootprint_ms(CycleFrequency freq);
    long getCycleFootprint_us(CycleFrequency freq);
private:
    bool running = false;
    std::map<CycleFrequency, std::list<routine_ptr>> cycleFunctions;
    std::map<CycleFrequency, long> cycle_footprint;
    std::list<int> cycle_thread_ids;
};

SINGLETON_EXTERN(CycleManager, cycle_manager)