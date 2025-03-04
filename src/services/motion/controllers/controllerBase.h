#pragma once
#include "../stepper.h"
#include <vector>

class ControllerBase {
public:
    ControllerBase();
    
    void addStepper(Stepper& stepper);
    void removeStepper(Stepper& stepper);
    
    void setMaxSpeed(float maxSpeed);
    void setAcceleration(float accel);
    void setDeceleration(float decel);

protected:
    std::vector<Stepper*> m_steppers;
    float m_max_speed;
    float m_accel;
    float m_decel;

    std::vector<float> computeSaturatedVelocities(std::vector<float> targetSpeeds);
    float computeSaturatedVelocity(float targetSpeeds);
};