#include "os.h"

OS OS::m_instance;

void OS::loop(){
    switch(m_state){
        case BOOT:
            boot();
            break;
        case IDLE:
            idle();
            break;
        case RUNNING:
            run();
            break;
        case STOPPED:
            stop();
            break;
        default: 
        break;
    }
}

void OS::boot(){
    executeRoutine(m_bootRoutine);
    m_state = IDLE;
}

void OS::idle(){
    executeRoutine(m_idleRoutine);
}

void OS::run(){
    executeRoutine(m_runRoutine);
}

void OS::stop(){
    executeRoutine(m_stopRoutine);
}

void OS::setRountine(SystemState state, routine_ptr func_ptr){
    if(func_ptr == nullptr) return;
    switch(state){
        case BOOT:
            m_bootRoutine = func_ptr;
            break;
        case IDLE:
            m_idleRoutine = func_ptr;
            break;
        case RUNNING:
            m_runRoutine = func_ptr;
            break;
        case STOPPED:
            m_stopRoutine = func_ptr;
            break;
        default:
        break;
    }
}

void OS::setState(SystemState state){
    m_state = state;
}

void OS::attachService(Service* service){
    if(service != nullptr){
        m_services[service->id()] = service;
    }
}

bool OS::hasService(ServiceID s) const{
    return m_services.find(s) != m_services.end();
}

bool OS::statusService(ServiceID serviceID) const{
    if(hasService(serviceID))
        return m_services.at(serviceID)->enabled();
    else
        return false;
}

void OS::enable(ServiceID id){
    if(hasService(id)) m_services[id]->enable();
}

void OS::disable(ServiceID id){
    if(hasService(id)) m_services[id]->disable();
}

void OS::executeRoutine(routine_ptr routine){
    if(routine != nullptr){
        routine();
    }
}