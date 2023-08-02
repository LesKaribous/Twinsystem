#include "system.h"
#include "settings.h"

SystemBase::SystemBase(){
    m_currentState = BOOT;
}

SystemBase::~SystemBase(){
    //Smart pointers destroyed
}

void SystemBase::enable(ServiceID id) {
    for(auto& mod : m_services){
        if(mod->getID() == id){
            mod->enable();
        }
    }
}

void SystemBase::disable(ServiceID id) {
    for(auto& mod : m_services){
        if(mod->getID() == id){
            mod->disable();
        }
    }
}

void SystemBase::loadService(Service* s){
    //Console::info("System") << m->toString() << " loaded." << Console::endl;
    m_services.push_back(s);
}

bool SystemBase::statusService(ServiceID serviceID){
    for(auto& mod : m_services){
        if(mod->getID() == serviceID){
            return mod->isEnabled();
        }
    }
}


void SystemBase::updateProgram(){
    //TODO: update program
}

void SystemBase::updateServices(){
    for(auto& service : m_services) {
        if(service->isEnabled()) {
            service->update();
        }
    }
}

void SystemBase::update() {
    switch (m_currentState){
    case BOOT:
        //Boot finished
        handleBootState();
        break;
    case IDLE:
        //Wait for program
        handleIdleState();
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

void SystemBase::handleBootState(){
    m_currentState = IDLE;
}


void SystemBase::handleIdleState(){
    // Update each enabled subsystem
    updateServices();

}

void SystemBase::handleRunningState(){
    // Update each enabled subsystem
    updateServices();
    updateProgram();
    //Run the program
}

void SystemBase::handleStoppedState(){
    //Do nothing.
}

SystemState SystemBase::getState(){
    return m_currentState;
}

