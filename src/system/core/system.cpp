#include "system.h"
#include "settings.h"

System::System(){
    m_currentState = BOOT;
    Console::initialize();
}

System::~System(){
    //Smart pointers destroyed
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

void System::loadModule(Module* m){
    Console::info("System") << m->toString() << " loaded." << Console::endl;
    m_modules.push_back(m);
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
        //Wait for program
        handleIdleState();
        break;
    case ARMED:
        //Ready to start program
        handleArmedState();
        break;
    case RUNNING:
        //Run the program
        handleRunningState();
        break;
    case STOPPED:
        //Program stopped
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



