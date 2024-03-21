#pragma once
#include "os.h"
#include "console.h"

#define async (AsyncExecutor)

class AsyncExecutor {
public:
    explicit AsyncExecutor(Job& x){
        Console::info("AsyncExecutor") << "Starting task" << Console::endl;
        OS::instance().waitUntil(x, false);
        Console::info("AsyncExecutor") << "ending task" << Console::endl;
    }
};