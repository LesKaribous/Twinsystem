#pragma once
#include "core/lib.h"
#include "core/module.h"

#include "debug/console.h"
#include "core/chrono.h"

//Stand alone modules
#include "modules/lidar/lidar.h"
#include "modules/inputs/inputs.h"
#include "modules/motion/motion.h"
#include "modules/screen/screen.h"
#include "modules/planner/planner.h"
#include "modules/neopixel/neopixel.h"
#include "modules/actuators/actuators.h"

//Temporary system extension
#include "extension/test/extTest.h"
#include "extension/motion/extMotion.h"
#include "extension/strategy/extStrategy.h"

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
    void updateUiFields();
    void enable(SystemModule module);
    void disable(SystemModule module);

    void execute(const String& command);
    void execute(std::shared_ptr<Task> task);

    void addToScore(int points, int multiplicateur = 1.0);
    int getScore();

    friend class ExtTest;
    friend class ExtMotion;
    friend class ExtStrategy;

protected:
    SystemState m_currentState;

    //Standalone modules
    std::unique_ptr<Lidar> lidar = nullptr;
    std::unique_ptr<Screen> screen = nullptr;
    std::unique_ptr<Inputs> inputs = nullptr;
    std::unique_ptr<Motion> motion = nullptr;
    std::unique_ptr<Planner> planner = nullptr;
    std::unique_ptr<NeoPixel> neopixel = nullptr;
    std::unique_ptr<Actuators> actuators = nullptr;

    Chronometer chrono;


    //Dependent Modules
    std::unique_ptr<ExtTest> e_test = nullptr;
    std::unique_ptr<ExtMotion> e_motion = nullptr;
    std::unique_ptr<ExtStrategy> e_strategy = nullptr;
    
private:

    //Async rountines
    void handleBootState();
    void handleIdleState();
    void handleArmedState();
    void handleRunningState();
    void handleStoppedState();

    //Warning blocking routines
    void wait(int time);
	void waitUntil(Job& job);

    int m_score = 0;



    std::array<Module*, 7> m_modules;
};
