#include "PositionController.h"

PositionController::PositionController() : ControllerBase() {}

void PositionController::setTargetPositions(std::vector<long> positions) {
    if (positions.size() != m_steppers.size()) return;

    long max_distance = 0;
    std::vector<long> distances(m_steppers.size());

    for (size_t i = 0; i < m_steppers.size(); i++) {
        distances[i] = abs(positions[i] - m_steppers[i]->getPosition());
        if (distances[i] > max_distance) max_distance = distances[i];
    }

    for (size_t i = 0; i < m_steppers.size(); i++) {
        if (distances[i] == 0) continue;
        float speed = (float)distances[i] / max_distance * Settings::Stepper::MAX_SPEED;
        m_steppers[i]->setTargetVelocity(speed);
    }
}

void PositionController::control() {
    for (Stepper* stepper : m_steppers) {
        stepper->control();
    }
}
