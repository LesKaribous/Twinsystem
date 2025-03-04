#include "controllerBase.h"
#include "settings.h"

ControllerBase::ControllerBase()
    : m_max_speed(Settings::Motion::SPEED), 
      m_accel(Settings::Stepper::MAX_ACCEL),
      m_decel(Settings::Stepper::MAX_ACCEL) { }

void ControllerBase::addStepper(Stepper& stepper) {
    m_steppers.push_back(&stepper);
}

void ControllerBase::removeStepper(Stepper& stepper) {
    m_steppers.erase(std::remove(m_steppers.begin(), m_steppers.end(), &stepper), m_steppers.end());
}

void ControllerBase::setMaxSpeed(float maxSpeed) {
    m_max_speed = maxSpeed;
}

void ControllerBase::setAcceleration(float accel) {
    m_accel = fabs(accel);
}

void ControllerBase::setDeceleration(float decel) {
    m_decel = fabs(decel);
}

std::vector<float> ControllerBase::computeSaturatedVelocities(std::vector<float> targetSpeeds) {
    float maxSpeed = m_max_speed;
    
    float M = *std::max_element(targetSpeeds.begin(), targetSpeeds.end(),
                                [](float a, float b) { return fabs(a) < fabs(b); });

    if (M > maxSpeed) {
        float scale = maxSpeed / M;
        for (float &speed : targetSpeeds) {
            speed *= scale;
        }
    }

    return targetSpeeds;
}

float ControllerBase::computeSaturatedVelocity(float targetSpeed) {
    float maxSpeed = m_max_speed;
    
    if (targetSpeed > maxSpeed) {
        float scale = maxSpeed / targetSpeed;
        targetSpeed *= scale;
        
    }

    return targetSpeed;
}