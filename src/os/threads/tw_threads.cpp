#include "tw_threads.h"
 
tw_Thread::tw_ThreadInfo tw_Thread::createThread(wrapper_ptr func_ptr, int stack_size, const std::string& threadName){
    if (!func_ptr) {
        // Invalid function pointer
        tw_ThreadInfo tinfo;
        tinfo.threadID = -1;
        return tinfo;
    }

    int threadID = threads.addThread(func_ptr, 0, stack_size);


    if (threadID < 0) {
        tw_ThreadInfo tinfo;
        tinfo.threadID = -1;
        return tinfo;
    }

    // Store thread info in our registry
    tw_ThreadInfo tinfo;
    tinfo.threadID = threadID;
    tinfo.funcPtr  = func_ptr;
    tinfo.name     = threadName;

    m_threadRegistry[threadID] = tinfo;

    return tinfo;
}

bool tw_Thread::killThread(int threadID) {
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

bool tw_Thread::suspendThread(int threadID) {
    auto it = m_threadRegistry.find(threadID);
    if (it == m_threadRegistry.end()) {
        // Thread not found
        return false;
    }

    // threads.suspend() returns 0 on success, -1 on error
    int result = threads.suspend(threadID);
    return (result == 0);
}

bool tw_Thread::resumeThread(int threadID) {
    auto it = m_threadRegistry.find(threadID);
    if (it == m_threadRegistry.end()) {
        return false;
    }

    // threads.restart() returns 0 on success, -1 on error
    int result = threads.restart(threadID);
    return (result == 0);
}

bool tw_Thread::waitThread(int threadID, unsigned long timeout_ms) {
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

std::vector<tw_Thread::tw_ThreadInfo> tw_Thread::listThreads() {
    // Collect current threads in a vector
    std::vector<tw_ThreadInfo> out;
    out.reserve(m_threadRegistry.size());
    for (auto& kv : m_threadRegistry) {
        out.push_back(kv.second);
    }
    return out;
}