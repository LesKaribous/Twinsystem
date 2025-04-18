#pragma once
#include "utils/geometry.h"
#include "os/asyncExecutor.h"
#include "velocityController.h"

class PositionController : public Job{
public:
    PositionController();

    // Call this method every control cycle
    void control();

    void run() override; 
    void reset() override;  //Set to IDLE
    void start() override;   //Set to PENDING
    void complete() override;//Set to COMPLETED

    void onUpdate();
    void onPausing();
    void onCanceling();

    // Set a new target position (in world coordinates)
    void setPosition(const Vec3 &t);
    void setTarget(const Vec3 &t);
    void setSteppers(Stepper* a, Stepper* b, Stepper* c);

    // Getters for debugging or further processing
    Vec3 getPosition() const { return position/Settings::Calibration::Primary.Cartesian; }
    Vec3 getVelocity() const { return velocity/Settings::Calibration::Primary.Cartesian; }
    Vec3 getAcceleration() const { return acceleration; }
    Vec3 getTarget() const { return target; }

private:

    void deccelerate();

    Vec3 last_otos_position;      // Current position
    long last_otos_time;      // Current position

    Vec3 position;      // Current position
    Vec3 velocity;      // Current velocity
    Vec3 target_velocity;      // Current velocity
    Vec3 acceleration;  // Current acceleration

    VelocityController controller;

    Vec3 target;        // Active target position
    Vec3 newTarget;     // New target to be applied
};
