#pragma once
#include "utils/geometry.h"
#include "velocityController.h"

class PositionController {
public:
    PositionController();

    // Call this method every control cycle with the timestep (dt in seconds)
    void control(float dt);

    // Set a new target position (in world coordinates)
    void setTarget(const Vec2 &t);

    // Getters for debugging or further processing
    const Vec2& getPosition() const { return position; }
    const Vec2& getVelocity() const { return velocity; }
    const Vec2& getAcceleration() const { return acceleration; }
    const Vec2& getTarget() const { return target; }

private:
    Vec2 position;      // Current position
    Vec2 velocity;      // Current velocity
    Vec2 acceleration;  // Current acceleration

    VelocityController controller;

    Vec2 target;        // Active target position
    Vec2 newTarget;     // New target to be applied

    // Control parameters
    float vMax;         // Maximum desired velocity
    float aMax;         // Maximum acceleration
    float stopThreshold;// Distance below which we stop
};
