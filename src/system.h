#pragma once
#include "core/lib.h"
#include "core/module.h"

#include "modules/screen/screen.h"
#include "modules/lidar/lidar.h"
#include "modules/motion/motion.h"
#include "modules/planner/planner.h"
#include "modules/actuators/actuators.h"

enum SystemModule{
    LIDAR,
    SCREEN,
    INPUTS,
    MOTION,
    PLANNER,
    NEOPIXEL,
    ACTUATOR
};

enum SystemState{
    BOOT,
    IDLE,
    ARMED,
    RUNNING,
    STOPPED,
}

class System{
public :
    System();
    ~System();
    void update();
    void enable(SystemModule module);
    void disable(SystemModule module);

    void execute(const String& command);
    void execute(std::shared_ptr<Task> task);

    void freezeSettings();
    void unFreezeSettings();

private:

    void handleBootState();
    void handleIdleState();
    void handleArmedState();
    void handleRunningState();
    void handleStoppedState();

    int m_score = 0;

    SystemState m_currentState;

    std::unique_ptr<Lidar> m_lidar = nullptr;
    std::unique_ptr<Screen> m_screen = nullptr;
    std::unique_ptr<Inputs> m_inputs = nullptr;
    std::unique_ptr<Motion> m_motion = nullptr;
    std::unique_ptr<Planner> m_planner = nullptr;
    std::unique_ptr<NeoPixel> m_neopixel = nullptr;
    std::unique_ptr<Actuators> m_actuators = nullptr;

    std::array<Module*, 7> m_modules;
};

System::System(){
    m_score = 0;
    m_currentState = BOOT;

    m_lidar = std::make_unique<Lidar>();
    m_screen = std::make_unique<Screen>();
    m_inputs = std::make_unique<Inputs>();
    m_motion = std::make_unique<Motion>();
    m_planner = std::make_unique<Planner>();
    m_neopixel = std::make_unique<NeoPixel>();
    m_actuators = std::make_unique<Actuators>();

    m_modules[0] = m_lidar.get();
    m_modules[1] = m_screen.get();
    m_modules[2] = m_inputs.get();
    m_modules[3] = m_motion.get();
    m_modules[4] = m_planner.get();
    m_modules[5] = m_neopixel.get();
    m_modules[6] = m_actuators.get();
}

System::~System(){
    //Smart pointers destroyed
}

void System::enable(SystemModule component) {
    // Enable a component
    switch (component) {
        case LIDAR:
            m_lidar->enable();
            break;
        case SCREEN:
            m_screen->enable();
            break;
        case INPUTS:
            m_inputs->enable();
            break;
        case MOTION:
            m_motion->enable();
            break;
        case PLANNER:
            m_planner->enable();
            break;
        case NEOPIXEL:
            m_neopixel->enable();
            break;
        case ACTUATOR:
            m_actuators->enable();
            break;
        // ... handle other components
    }
}

void System::disable(SystemModule component) {
    // Enable a component
    switch (component) {
        case LIDAR:
            m_lidar->disable();
            break;
        case SCREEN:
            m_screen->disable();
            break;
        case INPUTS:
            m_inputs->disable();
            break;
        case MOTION:
            m_motion->disable();
            break;
        case PLANNER:
            m_planner->disable();
            break;
        case NEOPIXEL:
            m_neopixel->disable();
            break;
        case ACTUATOR:
            m_actuators->disable();
            break;
        // ... handle other components
    }
}


void System::execute(const String& command) {

}

void System::execute(std::shared_ptr<Task> task){

}

void System::update() {
    // Update each enabled subsystem
    for(auto& module : m_modules) {
        if(module->isEnabled()) {
            module->update();
        }
    }

    switch (m_currentState){
    case BOOT:
        //Boot finished
        handleBootState();
        break;
    case IDLE:
        //Wait for launch
        handleIdleState();
        break;
    case ARMED:
        //Wait for launch
        handleIdleState();
        break;
    case RUNNING:
        //Match
        handleRunningState();
        break;
    case STOPPED:
        //Do nothing and display score
        handleStoppedState();
        break;
    default:
        break;
    }
}

void System::handleBootState(){
	sys.enable(LIDAR);
    sys.enable(INPUTS);
    sys.enable(SCREEN);
	sys.enable(MOTION);
    sys.enable(NEOPIXEL);
	sys.enable(ACTUATOR);
    m_currentState = IDLE;
}

void System::handleIdleState(){
    if(m_ui->isStarterPlaced()){ //Arm
        m_currentState = ARMED;
        freezeSettings();

        if(intercom.IsConnected()){
            intercom.SendRequest("displayLidar");
        }
    }
    if(ButtonReleased()){ //Recalage
        if	   (IsBlue()  && IsPrimary() ) 					RecalagePrimaryBlue();
        else if(IsBlue()  && IsSecondary() && IsCherry() ) 	RecalageSecondaryBlue();
        else if(IsBlue()  && IsSecondary() && IsCake() ) 	RecalageSecondaryCakeBlue();
        else if(IsGreen() && IsPrimary() ) 					RecalagePrimaryGreen();
        else if(IsGreen() && IsSecondary() && IsCherry() ) 	RecalageSecondaryGreen();
        else if(IsGreen() && IsSecondary() && IsCake() ) 	RecalageSecondaryCakeGreen();
        //TestOrientation();
        //TestSteppers();
    }
}

void System::handleArmedState(){
    if(StarterPulled()){ //Start match
        _state = RobotState::STARTING;
        DisableDisguisement();
    }else if(StarterCancelled()){ //Unarm
        _state = RobotState::IDLE;
        unfreezeSettings();
        EnableDisguisement();
        intercom.SendRequest("displayIntercom");
        while(ButtonPressed()) Update(); //Wait for resetButton to be released
    }
}

void System::handleRunningState(){
    //Execute Objectives
    if (m_planner->isEnabled()) {
        // Get a task to execute if planner has one
        if (auto task = m_planner->getTaskToExecute()) {
            executeTask(task);
        }

        // Handle objective completion
        if (m_planner->isObjectiveComplete()) {
            m_score += m_planner->collectReward(); //Reward can be collected only once
        }
    }
}

void System::handleStoppedState(){

}