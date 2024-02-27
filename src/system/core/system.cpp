#include "system.h"
#include "settings.h"
#include "system/core/console.h"

SystemBase::SystemBase() {
    m_currentState = BOOT;
}

SystemBase::~SystemBase(){
    //Smart pointers destroyed
}

void SystemBase::enable(ServiceID id) {
    if(hasService(id)) m_services[id]->enable();
    else console.error("System") << "Service : " << Service::toString(id) << " not found." << console.endl;
}

void SystemBase::disable(ServiceID id) {
    if(hasService(id)) m_services[id]->disable();
    else console.error("System") << "Service : " << Service::toString(id) << " not found." << console.endl;
}

bool SystemBase::hasService(ServiceID s ) const {
    return m_services.find(s) != m_services.end();
}

void SystemBase::loadService(Service* s){
    if(hasService(s->getID())){
        console.error("System") << s->toString() << " loaded." << Console::endl;
    }else m_services[s->getID()] = s;
}

void SystemBase::loadServiceInterrupt(Service* s, double interuptInterval){
    if(hasService(s->getID())){
        console.error("System") << s->toString() << " loaded." << Console::endl;
    }else m_services[s->getID()] = s;
}

bool SystemBase::statusService(ServiceID serviceID){
    if(hasService(serviceID))
        return m_services[serviceID]->isEnabled();
    else
        return false;
}


bool SystemBase::isBusy() const {
    return m_busy;
}

void SystemBase::wait(unsigned long time, bool async) {
    m_busy = true;
    m_timer.setDuration(time);
    m_timer.start();
    m_currentJob = &m_timer;
    if(!async)while(isBusy())update();
}

void SystemBase::waitUntil(Job& obj, bool async){
    m_busy = true; 
    m_currentJob = &obj;
    if(!async)while(isBusy())update();
}


bool SystemBase::debug(ServiceID s){
    if(hasService(s))
        return m_services[s]->debug();
    else
        return false;
}

void SystemBase::toggleDebug(ServiceID s){
    if(hasService(s))
        m_services[s]->toggleDebug();
}


void SystemBase::execute(String& script){
    m_program = interpreter.processScript(script);
    if(m_program.isValid()){
        m_currentState = RUNNING;
        m_program.start();
    }else{
        m_currentState = IDLE;
    }
}

void SystemBase::execute(Program& prgm){
    m_program = prgm;
    if(m_program.isValid()){
        m_currentState = RUNNING;
        m_program.start();
    }else{
        m_currentState = IDLE;
    }
}


void SystemBase::updateProgram(){
    if(m_busy){ 
        if(m_currentJob->isPending() || m_currentJob->isPaused()){
            m_currentJob->run(); //System is busy
        }else{
            m_busy = false;
        }
    }
    
    if(!m_busy){
        if(m_program.isValid() && m_program.isPending()) m_program.step(); //Last statement finished step the program
    }
}

void SystemBase::updateServices(){
    for(const auto& service : m_services) {
        if(service.second->isEnabled()) {
            service.second->update();
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

    if(m_currentJob){
        if(m_currentJob->isPending()){
            m_currentJob->run();
            m_busy = true;
        }
        if(m_currentJob->isCompleted())m_busy = false;
        if(m_currentJob->isCancelled())m_busy = false;
        if(m_currentJob->isPaused())m_busy = true;
    }
    
}

SystemState SystemBase::getState(){
    return m_currentState;
}

