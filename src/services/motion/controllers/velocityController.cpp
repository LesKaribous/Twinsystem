#include "velocityController.h"

VelocityController::VelocityController() : ControllerBase() {}

void VelocityController::setTargetVelocities(std::vector<float> velocities) {
    if (velocities.size() != m_steppers.size()) return;

    velocities = computeSaturatedVelocities(velocities);

    for (size_t i = 0; i < m_steppers.size(); i++) {
        m_steppers[i]->setTargetVelocity(velocities[i]);
    }
}

void VelocityController::setTargetVelocity(float velocity) {
    velocity = computeSaturatedVelocity(velocity);

    for (size_t i = 0; i < m_steppers.size(); i++) {
        m_steppers[i]->setTargetVelocity(velocity);
    }
}


void VelocityController::control() {
    for (Stepper* stepper : m_steppers) {
        stepper->control();
    }
}

bool VelocityController::isRunning() const {
    for (const Stepper* stepper : m_steppers) {
        if (stepper->isRunning()) return true;
    }
    return false;
}