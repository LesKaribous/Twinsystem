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
        if(currentJob()->isCompleted() || currentJob()->isCancelled()) killCurrentJob();
        else currentJob()->run();
    }
}

void OS::auto_routine(){
    updateServices();
    executeRoutine(m_autoRoutine);
    if(currentJob() != nullptr){
        if(currentJob()->isCompleted() || currentJob()->isCancelled()) killCurrentJob();
        else currentJob()->run();
    }
}

void OS::auto_program_routine(){
    m_state = AUTO;
    executeRoutine(m_auto_programRoutine);
    m_state = STOPPED;
}

void OS::manual_program_routine(){
    m_state = MANUAL;
    updateServices();
    executeRoutine(m_manual_programRoutine);
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

Job& OS::wait(unsigned long time, bool runasync) {
    m_timer.setDuration(time);
    m_timer.start();
    //addJob(&m_timer);
    if(!runasync)while(m_timer.isPending()|| m_timer.isPaused()){
        m_timer.run();
        run();
    }
    return m_timer;
}

void OS::waitUntil(Job& obj, bool runasync){
    addJob(&obj);
    if(!runasync) while(obj.isPending() || obj.isPaused()) run();
}

void OS::execute(Job& obj, bool runasync){
    addJob(&obj);
    if(!runasync)while(obj.isPending()|| obj.isPaused()) run();
}

void OS::flush(){
    while(isBusy()) run();
}

bool OS::isBusy() {
    return m_jobs.size() != 0;
};

Job* OS::currentJob(){
    if(m_jobs.size() == 0) return nullptr;
    else return m_jobs.front();
}
void OS::addJob(Job* job){
    m_jobs.push(job);
}
void OS::killCurrentJob(){
    if(m_jobs.size() == 0) return;
    m_jobs.pop();
}

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
    }else{
        Console::error("OS") << "Routine is nullptr" << Console::endl;
    }
}

int OS::runThreadedRoutine(routine_ptr func_ptr, 
                           int stack_size, 
                           const std::string& threadName)
{
    if (!func_ptr) {
        // Invalid function pointer
        return -1;
    }

    // Create a new thread. The TeensyThreads call returns a thread ID >= 0 on success, -1 on failure.
    // Note: The library also supports a form: addThread(func_ptr, arg, stack_size), 
    // but here we assume 'func_ptr' is void(*)(), so no extra arg is needed.
    int threadID = threads.addThread(func_ptr, stack_size);

    if (threadID < 0) {
        // Thread creation failed
        return -1;
    }

    // Store thread info in our registry
    ThreadInfo tinfo;
    tinfo.threadID = threadID;
    tinfo.funcPtr  = func_ptr;
    tinfo.name     = threadName;

    m_threadRegistry[threadID] = tinfo;

    return threadID;
}

bool OS::killThread(int threadID) {
    auto it = m_threadRegistry.find(threadID);
    if (it == m_threadRegistry.end()) {
        // Not found
        return false;
    }

    // threads.kill() returns 0 on success, -1 on error
    int result = threads.kill(threadID);
    if (result == 0) {
        // Remove from registry
        m_threadRegistry.erase(it);
        return true;
    }

    return false;
}

bool OS::suspendThread(int threadID) {
    auto it = m_threadRegistry.find(threadID);
    if (it == m_threadRegistry.end()) {
        // Thread not found
        return false;
    }

    // threads.suspend() returns 0 on success, -1 on error
    int result = threads.suspend(threadID);
    return (result == 0);
}

bool OS::resumeThread(int threadID) {
    auto it = m_threadRegistry.find(threadID);
    if (it == m_threadRegistry.end()) {
        return false;
    }

    // threads.restart() returns 0 on success, -1 on error
    int result = threads.restart(threadID);
    return (result == 0);
}

bool OS::waitThread(int threadID, unsigned long timeout_ms) {
    auto it = m_threadRegistry.find(threadID);
    if (it == m_threadRegistry.end()) {
        // Invalid thread
        return false;
    }

    // threads.wait(threadID, timeout_ms)
    //   Waits until the thread ends or until 'timeout_ms' elapses
    //   If 0, wait indefinitely. The return is the "state" of the thread.
    //   Typical states: RUNNING, ENDED, SUSPENDED, etc.
    int finalState = threads.wait(threadID, timeout_ms);

    // If finalState == Threads::ENDED, we can assume the thread is done.
    // Let's check if that means we should remove from the registry.
    if (finalState == Threads::ENDED) {
        m_threadRegistry.erase(it);
        return true; // The thread ended successfully
    }

    // If it times out or is still running, return false
    return false;
}

std::vector<OS::ThreadInfo> OS::listThreads() const {
    // Collect current threads in a vector
    std::vector<ThreadInfo> out;
    out.reserve(m_threadRegistry.size());
    for (auto& kv : m_threadRegistry) {
        out.push_back(kv.second);
    }
    return out;
}