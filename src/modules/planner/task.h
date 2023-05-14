#pragma once
#include "core/module.h"
#include "modules/planner/job.h"


class Task : public Job {
public:
    Task(SystemModule module);
    virtual void execute() = 0;

private:
    const SystemModule m_module;
};

Task::Task(SystemModule module) : m_module(module){}