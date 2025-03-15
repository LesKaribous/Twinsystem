#pragma once
#include "utils/geometry.h"
#include "velocityController.h"

class PositionController {
public:
    PositionController();

    // Call this method every control cycle
    void control();
    

    // Set a new target position (in world coordinates)
    void setTarget(const Vec3 &t);

    // Getters for debugging or further processing
    const Vec3& getPosition() const { return position; }
    const Vec3& getVelocity() const { return velocity; }
    const Vec3& getAcceleration() const { return acceleration; }
    const Vec3& getTarget() const { return target; }

private:

    void controlPosition(float dt, Vec2 positionError);
    void controlRotation(float dt, float rotationError);

    Vec3 position;      // Current position
    Vec3 velocity;      // Current velocity
    Vec3 acceleration;  // Current acceleration

    VelocityController controller;

    Vec3 target;        // Active target position
    Vec3 newTarget;     // New target to be applied
};
