#pragma once
#include <string>
#include <vector>
#include <map>
#include <TeensyThreads.h>

using wrapper_ptr = void (*)();

class tw_Thread{
public:
    struct tw_ThreadInfo {
        int threadID = -1;            // TeensyThreads thread ID
        wrapper_ptr funcPtr = nullptr; 
        std::string name;             // Optional: for debugging/logging
    };

    // Control existing threads
    static bool killThread(int threadID);
    static bool suspendThread(int threadID);
    static bool resumeThread(int threadID);
    static bool waitThread(int threadID, unsigned long timeout_ms = 0);

    static tw_ThreadInfo createThread(wrapper_ptr func_ptr, 
                           int stack_size, 
                           const std::string& threadName = "");

    static std::vector<tw_ThreadInfo> listThreads();

private:
    tw_Thread(){};//disable constructor
    inline static std::map<int, tw_ThreadInfo> m_threadRegistry;
};

