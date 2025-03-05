
#include "positionController.h"

PositionController::PositionController()
  : position(0.0f, 0.0f),
    velocity(0.0f, 0.0f),
    acceleration(0.0f, 0.0f),
    target(0.0f, 0.0f),
    newTarget(0.0f, 0.0f),
    vMax(60000.0f),
    aMax(2000.0f),
    stopThreshold(1.0f)
{
    controller.setPIDGains(1.0,0.0,0.0);
    controller.enable();
    controller.setTargetVelocity(Vec3(0));
}

void PositionController::control(float dt) {
    dt*=1e-6;
    // Update target if a new one has been set.
    if (!(newTarget == target)) {
        target = newTarget;
    }
    
    // Compute the relative target vector and its distance.
    Vec2 relTarget = target - position;
    float d = relTarget.mag();

    // Stop if close enough to the target.
    if (d < stopThreshold) {
        Serial.println("Stopping");
        acceleration = Vec2(0, 0);
        velocity = Vec2(0, 0);
        target = position;  // Reset target to current position.
        return;
    }

    // Compute the desired velocity toward the target.
    Vec2 targetVel = (d > 0) ? (relTarget.normalize() * vMax) : Vec2(0, 0);

    // Compute time needed to decelerate and estimated travel time.
    float currentSpeed = velocity.mag();
    float decelTime = currentSpeed / (aMax + 1e-5f); // Small offset to avoid division by zero.
    float travelTime = d / (currentSpeed + 1e-5f);

    // Compute the velocity error.
    Vec2 velError = targetVel - velocity;

    // Decide control strategy based on current state.
    if (decelTime >= travelTime) { // Deceleration phase.
        Serial.println("Decelerating");
        if (currentSpeed > 0) {
            acceleration = velocity.normalize() * (-aMax);
        } else {
            acceleration = Vec2(0, 0);
        }
    } else if (velError.mag() > 0.1f) { // Acceleration phase.
        Serial.println("Accelerating");
        acceleration = velError.normalize() * aMax;
    } else { // Maintain constant speed.
        Serial.println("Constant speed");
        acceleration = Vec2(0, 0);
    }

    // Enforce acceleration limit.
    if (acceleration.mag() > aMax) {
        acceleration = acceleration.normalize() * aMax;
    }
    velocity = velocity + (acceleration * dt);

    controller.setTargetVelocity(Vec3(velocity,0));
    controller.control();

    position = position + (velocity * dt);
}

void PositionController::setTarget(const Vec2 &t) {
    newTarget = t;
}
