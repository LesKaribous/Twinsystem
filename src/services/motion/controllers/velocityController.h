#pragma once
#include "controllerBase.h"

class VelocityController : public ControllerBase{
public:
    VelocityController();

    
    void setTargetVelocities(std::vector<float> velocities);
    void forceVelocities(std::vector<float> velocities);

    void setTargetVelocity(float velocity);
    void forceVelocity(float velocity);
    
    void control();
    
    bool isRunning() const;
    bool isAtTargetVelocity() const;

private:
    Stepper& m_stepper;
};
