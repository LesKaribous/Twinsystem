
#include "os/asyncExecutor.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#define STEPPER_COUNT 3

struct StepperProfile {
    double distanceToGo;
    double acceleration;
    double velocity;
    double timeAccel;
    double timeFlat;
    double totalTime;
};

class StepperController : public Job{
public:

    void computeSynchronizedProfiles();

    void run() override; 
    void reset() override;  //Set to IDLE
    void start() override;   //Set to PENDING
    void pause() override;   //Set to PAUSED
    void resume() override;  //Set to PENDING
    void cancel() override;  //Set to CANCELLED
    void complete() override;//Set to COMPLETED
    void control();

    void setTarget(Vec3 target);
    void setTarget(float a, float b, float c );

private:
    StepperProfile steppers[STEPPER_COUNT];

    double calcMinDuration(double distance, double maxVel, double maxAcc) const;
    void computeProfile(StepperProfile &stepper, double targetDuration) const;
};
