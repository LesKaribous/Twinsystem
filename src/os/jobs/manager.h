#pragma once
#include "job.h"
#include <memory>
/*
class JobManager {
public:
    JobManager();
    virtual ~JobManager() = default;

    template<typename J, typename... Args>
    std::unique_ptr<J> createJob(Args&&... args) {
        auto job = std::make_unique<J>(std::forward<Args>(args)...);
        job->setOwner(this);
        return job;
    }

    static JobManager* ownerOf(const Job* job) {
        return static_cast<JobManager*>(job->owner());
    }
}*/