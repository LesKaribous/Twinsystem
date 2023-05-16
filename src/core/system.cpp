#include "system.h"
#include "Settings.h"

System::System(){
    m_currentState = BOOT;
}

System::~System(){
    //Smart pointers destroyed
}

void System::registerModule(Module* m){
    m_modules.push_back(m);
}

void System::enable(SystemModule module) {
    for(auto& mod : m_modules){
        if(mod->getModule() == module){
            mod->enable();
        }
    }
}

void System::disable(SystemModule module) {
    for(auto& mod : m_modules){
        if(mod->getModule() == module){
            mod->disable();
        }
    }
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
        handleArmedState();
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
    m_currentState = IDLE;
}


void System::handleIdleState(){

}

void System::handleArmedState(){

}

void System::handleRunningState(){

}

void System::handleStoppedState(){

}



