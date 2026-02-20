#include "cycles.h"
#include <Arduino.h>

SINGLETON_INSTANTIATE(CycleManager, cycle_manager)

#define CYCLE_US(name, period, period_enum) void name(){\
    static const unsigned long CONTROL_PERIOD_US = period;\
    unsigned long lastCall = micros();\
    while (true) {\
        cycle_manager.instance().runCycle(period_enum);\
        lastCall += CONTROL_PERIOD_US;\
        long waitTime = lastCall - micros();\
        if (waitTime > 0)\
            threads.delay_us(waitTime);\
        threads.yield();\
    }\
}

#define CYCLE_MS(name, period, period_enum) void name(){\
    static const unsigned long CONTROL_PERIOD_MS = period;\
    unsigned long lastCall = millis();\
    while (true) {\
        cycle_manager.instance().runCycle(period_enum);\
        lastCall += CONTROL_PERIOD_MS;\
        long waitTime = lastCall - millis();\
        if (waitTime > 0)\
            threads.delay(waitTime);\
        threads.yield();\
    }\
}

CYCLE_US(cycle_1us, 1, T_1US)
CYCLE_US(cycle_10us, 10, T_10US)
CYCLE_US(cycle_100us, 100, T_100US)
CYCLE_US(cycle_1ms, 1000, T_1MS)
CYCLE_US(cycle_10ms, 10000, T_10MS)
CYCLE_MS(cycle_100ms, 100, T_100MS)
CYCLE_MS(cycle_1s, 1000, T_1S)
CYCLE_MS(cycle_10s, 10000, T_10S)

std::map<CycleFrequency, routine_ptr> freq_to_routine = {
    {T_1US, cycle_1us},
    {T_10US, cycle_10us},
    {T_100US, cycle_100us},
    {T_1MS, cycle_1ms},
    {T_10MS, cycle_10ms},
    {T_100MS, cycle_100ms},
    {T_1S, cycle_1s},
    {T_10S, cycle_10s}
};

long CycleManager::getCycleFootprint_ms(CycleFrequency freq) {
    return cycle_footprint[freq] * 1000; //ms
}

long CycleManager::getCycleFootprint_us(CycleFrequency freq) {
    return cycle_footprint[freq]; //us
}

void CycleManager::start(){
    threads.setSliceMicros(10);
    for (auto& pair : cycleFunctions){
        int id = threads.addThread(freq_to_routine[pair.first], 0, 8192);
        cycle_thread_ids.push_back(id);
    }
    running = true;
}

void CycleManager::stop(){
    threads.setSliceMicros(10);
    
    for(auto& id : cycle_thread_ids)
        threads.kill(id);

    cycleFunctions.clear();
    running = false;
}

void CycleManager::runCycle(CycleFrequency freq){
    static long lastCall = 0;
    for(auto& func : cycleFunctions[freq]){
        func();
    }
    updateStats(freq, lastCall - micros());
    lastCall = micros();
}

void CycleManager::updateStats(CycleFrequency freq, long time){
    cycle_footprint[freq] = time;
}

void CycleManager::registerCycle(CycleFrequency freq, routine_ptr func){
    if(cycleFunctions.find(freq) == cycleFunctions.end()){
        cycleFunctions[freq] = std::list<routine_ptr>();
    }
    cycleFunctions[freq].push_back(func);
}
