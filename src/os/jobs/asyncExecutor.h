#pragma once
#include "os/os.h"
#include "os/console.h"

#define async (AsyncExecutor)
#define RUN_TWICE(x) x; x;

class AsyncExecutor {
    Job* job = nullptr;

public:
    explicit AsyncExecutor(JobHandle&& x) {
        job = x.get();                    // Save non-owning pointer
        os.execute(std::move(x), false);  // Transfer ownership
    }

    explicit operator bool() const {
        return job && job->isCompleted();
    }
};