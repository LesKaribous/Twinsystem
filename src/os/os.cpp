#include "os.h"
#include "console.h"

OS OS::m_instance;

OS& os = OS::instance();

void OS::run(){
    switch(m_state){
        case BOOT:
            boot_routine();
            break;
        case MANUAL:
            manual_routine();
            break;
        case MANUAL_PROGRAM:
            manual_program_routine();
            break;
        case AUTO_PROGRAM:
            auto_program_routine();
            break;
        case AUTO:
            auto_routine();
            break;
        case STOPPED:
            stop_routine();
            break;
        default: 
        break;
    }
}

void OS::start(){
    m_state = AUTO_PROGRAM;
}

void OS::reboot(){
    CPU_RESTART
}

void OS::stop(){
    m_state = STOPPED;
}

void OS::boot_routine(){
    executeRoutine(m_bootRoutine);
    m_state = MANUAL_PROGRAM;
    //Console::println(m_state);
}

void OS::manual_routine(){
    updateServices();
    executeRoutine(m_manualRoutine);
    if(currentJob() != nullptr){
        BEEP
        if(currentJob()->isCompleted() || currentJob()->isCanceled()) killCurrentJob();
        else if(currentJob()->isRunning()) currentJob()->run();
        else if(currentJob()->isIdle()) currentJob()->start();
        BEEP
    }
}

void OS::auto_routine(){
    updateServices();
    executeRoutine(m_autoRoutine);
    if(currentJob() != nullptr){
        if(currentJob()->isCompleted() || currentJob()->isCanceled()) killCurrentJob();
        else if(currentJob()->isRunning()) currentJob()->run();
        else if(currentJob()->isIdle()) currentJob()->start();
    }
}

void OS::auto_program_routine(){
    m_state = AUTO;
    executeRoutine(m_auto_programRoutine);
    auto_routine();
    m_state = STOPPED;
}

void OS::manual_program_routine(){
    m_state = MANUAL;
    updateServices();
    executeRoutine(m_manual_programRoutine);
    manual_routine();
    if(m_state == MANUAL) m_state = MANUAL_PROGRAM;
}

void OS::stop_routine(){
    executeRoutine(m_stopRoutine);
}

void OS::setRountine(SystemState state, routine_ptr func_ptr){
    if(func_ptr == nullptr) return;
    switch(state){
        case BOOT:
            m_bootRoutine = func_ptr;
            break;
        case MANUAL:
            m_manualRoutine = func_ptr;
            break;
        case AUTO:
            m_autoRoutine = func_ptr;
            break;
        case MANUAL_PROGRAM:
            m_manual_programRoutine = func_ptr;
        break;
        case AUTO_PROGRAM:
            m_auto_programRoutine = func_ptr;
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
        service->attach();

        Console::trace() << "Attached service: " << service->id() << Console::endl;

        service->enable();
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

void OS::wait(unsigned long time) {
    auto timer = std::make_unique<Timer>();
    timer->setDuration(time);

    Timer* timerPtr = timer.get();

    // Move the unique_ptr into execute(), which assumes ownership
    execute(std::move(timer), false);  // Or `runasync` if applicable
    m_timer.start(); // What is m_timer? Another Timer?

    // Synchronous wait loop (blocking)
    while (timerPtr->isPending() || timerPtr->isPaused()) {
        timerPtr->run();
        run();
    }
    
}

void OS::execute(std::unique_ptr<Job> obj, bool runasync){
    Job* job = obj.get();  // Keep raw pointer for state checking
    addJob(std::move(obj));  // Transfer ownership

    if (job->isIdle()) job->start();

    if (!runasync) {
        while (job->isPending())
            run();
    }
}

void OS::flush(){
    while(isBusy()) run();
}

bool OS::isBusy() {
    return m_jobs.size() > 0;
};

Job* OS::currentJob(){
    if(m_jobs.size() == 0) return nullptr;
    else return m_jobs.front().get();
}
void OS::addJob(JobHandle job){
    m_jobs.push(std::move(job));
}
void OS::killCurrentJob(){
    if(m_jobs.size() == 0) return;
    m_jobs.pop();
}

void OS::updateServices(){
    for(const auto& service : m_services) {
        if(service.second->enabled() && !service.second->threaded()) {
            service.second->run();
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
    }else{
        Console::error("OS") << "Routine is nullptr" << Console::endl;
    }
}

