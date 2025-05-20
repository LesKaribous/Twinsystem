#pragma once
#include "os/jobs/job.h"

class Controller : public Job{

public:
    Controller(){};
    virtual void control() = 0; //call with an interupt

    void setControlPeriod(long period) { control_period = period; }
    long getControlPeriod() const { return control_period; }

private:
    long control_period = 0;
    long last_control_time = 0;


};