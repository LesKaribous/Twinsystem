#pragma once
#include "utils/geometry.h"
#include "os/jobs/asyncExecutor.h"
#include "velocityController.h"

#include "pid.h"

class PositionController : public Controller{
public:
    PositionController();

    // Call this method every control cycle
    void control() override;

    void exec() override; 
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

    void setFeedrate(float feed);
private:
    float m_feedrate = 1.0;

    int collisionCounter = 0; // Reset on movement or normal progress
    constexpr static int COLLISION_THRESHOLD = 60; // Number of cycles to confirm collision
    constexpr static float COLLISION_VELOCITY_DIFF = 0.5f; // %

    void deccelerate();

    Vec3 position;      // Current position
    Vec3 last_position;      // Current position
    Vec3 velocity;      // Current velocity
    Vec3 last_velocity;      // Current velocity
    Vec3 target_velocity;      // Current velocity
    Vec3 acceleration;  // Current acceleration

    VelocityController controller;

    PIDController x_controller;
    PIDController y_controller;
    PIDController rot_controller;

    PIDController vx_controller;
    PIDController vy_controller;
    PIDController vrot_controller;
    

    Vec3 target;        // Active target position
    Vec3 newTarget;     // New target to be applied
};
