
#include "os/asyncExecutor.h"
#include "stepper.h"


#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#define STEPPER_COUNT 3

struct StepperProfile {
    double distanceToGo;
    double currentPosition;
    double acceleration;
    double velocity;
    double timeAccel;
    double timeFlat;
    double totalTime;
    long lastStepTime;
};

class StepperController : public Job{
public:
    StepperController();

    void run() override; 
    void reset() override;  //Set to IDLE
    void start() override;   //Set to PENDING
    void pause() override;   //Set to PAUSED
    void resume() override;  //Set to PENDING
    void cancel() override;  //Set to CANCELLED
    void complete() override;//Set to COMPLETED
    void control();

    void setTarget(long a, long b, long c );
    double computeMoveTime(long maxDistance);

private:
    double computeEffectiveAcceleration(long distance, double totalTime);
    double computeProfileVelocity(long distance, double t, double totalTime);

private:
    Stepper m_sA, m_sB, m_sC;

    // Targets are in steps.
    long targetA, targetB, targetC;
        
    // To compute a consistent synchronized trajectory, store the start positions.
    long m_startA, m_startB, m_startC;

    // And store the total distances (always positive) planned for each axis.
    long m_totalDistanceA, m_totalDistanceB, m_totalDistanceC;

    // The common move time (in seconds) for the longest trajectory.
    double m_totalTime;

    // The time (in seconds) when the move started.
    double m_startTime;

};
