#include "services/motion/controller/controller.h"
#include "services/motion/stepper.h"


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

class LinStepAccelerator {
public:
    LinStepAccelerator() = default;

    // Must be called before movement begins
    int32_t prepare(int32_t currentPos, int32_t targetPos, uint32_t targetSpeed, uint32_t vStart, uint32_t vEnd, uint32_t accel);

    // Call this during movement to get the current speed
    int32_t updateSpeed(int32_t currentPos);

    // Call if a stop is requested mid-move
    uint32_t initiateStop(int32_t currentPos);

private:
    int32_t s0 = 0, ds = 0;
    int32_t accEnd = 0, decStart = 0;

    uint32_t vStart = 0, vEnd = 0, vTarget = 0;
    int64_t vStart2 = 0, vEnd2 = 0, vTarget2 = 0;
    uint32_t twoA = 0;
};


class StepperController : public Controller{
public:
    StepperController();

    void run() override; 
    void reset() override;  //Set to IDLE
    void start() override;   //Set to PENDING
    void cancel() override;  //Set to CANCELLED
    void complete() override;//Set to COMPLETED
    void pause() override;  //Set to CANCELLED
    void resume() override;//Set to COMPLETED
    void control() override;;

    void onUpdate();
    void onCanceling() override;
    void onCanceled() override;

    void setTarget(long a, long b, long c );
    Vec3 getDisplacement(); //in mm, mm, rad
    void setSteppers(Stepper* a, Stepper* b, Stepper* c);

    void setFeedrate(float feed);

private:
    Stepper* m_sA = nullptr;
    Stepper* m_sB = nullptr;
    Stepper* m_sC = nullptr;    

    float m_feedrate = 1.0;

    double m_totalTime; // The common move time (in seconds) for the longest trajectory.
    double m_startTime; // The time (in seconds) when the move started.

    unsigned long m_last_compute;
    unsigned long m_last_control;

    LinStepAccelerator m_planner;

    Stepper* m_leadStepper = nullptr;
    long m_leadDelta = 0;
    long* m_leadStepsDone = nullptr;
    unsigned long m_lastStepTime = 0;
    unsigned long m_stepInterval = 0; // in microseconds
};
