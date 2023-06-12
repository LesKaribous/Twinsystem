#pragma once
#include "system/core/job.h"
#include "modules/module.h"



class Task : public Job {
public:
    Task(SystemModule module);
    virtual void execute() = 0;

private:
    const SystemModule m_module;
};

Task::Task(SystemModule module) : m_module(module){}


class Go : public Task{

};

class Turn : public Task{

};

class Turn : public Task{

};