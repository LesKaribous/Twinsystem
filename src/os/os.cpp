#include "os.h"
#include "console.h"

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
    updateServices();
    executeRoutine(m_idleRoutine);
}

void OS::run(){
    updateServices();
    executeRoutine(m_runRoutine);
}

void OS::stop(){
    executeRoutine(m_stopRoutine);
}

void OS::flush(){
    while(isBusy())m_currentJob->run();
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
        Console::trace() << "Attached service: " << service->id() << Console::endl;
        service->enable();
        service->onAttach();
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

bool OS::debug(ServiceID s){
    if(hasService(s))
        return m_services[s]->debug();
    else
        return false;
}

void OS::toggleDebug(ServiceID s){
    if(hasService(s))
        m_services[s]->toggleDebug();
}

void OS::wait(unsigned long time, bool async) {
    m_timer.setDuration(time);
    m_timer.start();
    m_currentJob = &m_timer;
    if(!async)while(isBusy())loop();
}

void OS::waitUntil(Job& obj, bool async){
    m_currentJob = &obj;
    if(!async)while(isBusy())loop();
}

void OS::execute(Job& obj, bool async){
    m_currentJob = &obj;
    if(!async)while(isBusy()) loop();
}

bool OS::isBusy() const{
    if(m_currentJob == nullptr) return false;
    else{
        return m_currentJob->isPending();
    }
};

void OS::updateServices(){
    for(const auto& service : m_services) {
        if(service.second->enabled()) {
            service.second->onUpdate();
        }
    }
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
