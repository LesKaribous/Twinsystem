#pragma once
#include "modules/planner/task.h"

class MotionTask : public Task{
public:
    MotionTask();

}

MotionTask::MotionTask() : Task(MOTION){}

