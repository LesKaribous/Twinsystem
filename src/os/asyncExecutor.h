#pragma once
#include "os.h"
#include "console.h"

#define async (AsyncExecutor)
#define RUN_TWICE(x) x; x;

class AsyncExecutor {
Job* job = nullptr;
public:
    explicit AsyncExecutor(Job& x) : job(&x){
        //Console::info("AsyncExecutor") << "Starting task" << Console::endl;
        OS::instance().waitUntil(x, false);
        //Console::info("AsyncExecutor") << "ending task" << Console::endl;
    }

    // Cast operator to bool, for use in logical expressions
    explicit operator bool() const {
        if(job) return job->isCompleted();  // Return true if the job succeeded, false otherwise
        return false;  // Return true if the job succeeded, false otherwise
    }
};