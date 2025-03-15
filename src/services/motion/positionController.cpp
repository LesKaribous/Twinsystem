
#include "positionController.h"
#include "settings.h"
#include "os/console.h"

#define NORMALIZE(x) (x > 0 ? 1.0 : (x < 0 ? -1.0 : 0.0))

PositionController::PositionController()
  : position(0.0f, 0.0f),
    velocity(0.0f, 0.0f),
    acceleration(0.0f, 0.0f),
    target(0.0f, 0.0f),
    newTarget(0.0f, 0.0f)
{
    controller.setPIDGains(1.0,0.0,0.0);
    controller.enable();
    controller.setTargetVelocity(Vec3(0));
}

void PositionController::control() {

    controller.control();

    static long lastTime = 0;
    if(micros() - lastTime > Settings::Motion::PID_INTERVAL) {
		lastTime = micros();
    	float dt = Settings::Motion::PID_INTERVAL * 1e-6;

        // Update target if a new one has been set. 
        if (!(newTarget == target)) {
            target = newTarget;
        }

        if(target == position){
            acceleration = Vec3(0);
            velocity = Vec3(0);
            controller.setTargetVelocity(Vec3(0));
            return;
        }

        // Compute the relative target vector and its distance.
        Vec2 relDistance = target - position;
        float angle = target.c - position.c;
        float distance = relDistance.mag();

        // Stop if close enough to the target.
        if (distance < Settings::Motion::MIN_DISTANCE && fabs(angle) < Settings::Motion::MIN_ANGLE) {
            Serial.println("Stopping");
            acceleration = Vec3(0);
            velocity = Vec3(0);
            newTarget = target = position;  // Reset target to current position.
            controller.setTargetVelocity(Vec3(0));
            return;
        }
        
        controlPosition(dt, relDistance);
        controlRotation(dt, angle);
        
        velocity = velocity + (acceleration * dt);
        position = position + ( velocity * dt);


        controller.setTargetVelocity(velocity);

        Console::info() << position << " / " << target << " | " << velocity << " | " << acceleration << Console::endl; 
    }
}





void PositionController::controlPosition(float dt, Vec2 positionError) {

    float distance = positionError.mag();
    // Compute the desired velocity toward the target.
    Vec2 direction = positionError.normalize();
    Vec2 targetVel = direction * Settings::Motion::MAX_SPEED;

    // Compute time needed to decelerate and estimated travel time.
    float currentSpeed = velocity.mag();
    float decelTime = currentSpeed / (Settings::Motion::MAX_ACCEL + 1e-5f); // Small offset to avoid division by zero.
    float travelTime = distance / (currentSpeed + 1e-5f);

    // Compute the velocity error.
    Vec2 velError = targetVel - velocity;
    
    // Decide control strategy based on current state.
    if (decelTime >= travelTime) { // Deceleration phase.
        //Serial.println("Decelerating");
        if (currentSpeed > 0) {
            acceleration = direction * (-Settings::Motion::MAX_ACCEL);
        } else {
            acceleration = Vec2(0);
        }
    } else if (velError.mag() > 0.1f) { // Acceleration phase.
        //Serial.println("Accelerating");
        acceleration = velError.normalize() * Settings::Motion::MAX_ACCEL;

    } else { // Maintain constant speed.
        acceleration = Vec2(0);
    }

    // Enforce acceleration limit.
    if (Vec2(acceleration).mag() > Settings::Motion::MAX_ACCEL) {
        acceleration = Vec2(acceleration.normalize()) * Settings::Motion::MAX_ACCEL;
    }
}


void PositionController::controlRotation(float dt, float rotationError) {

    float direction = NORMALIZE(rotationError);
    float targetVel = direction * Settings::Motion::MAX_ROT_SPEED;

    // Compute time needed to decelerate and estimated travel time.

    float decelTime = velocity.c / (Settings::Motion::MAX_ROT_ACCEL + 1e-5f); // Small offset to avoid division by zero.
    float travelTime = rotationError / (velocity.c + 1e-5f);
   
    // Compute the velocity error.
    float velError = targetVel - velocity.c;
    // Decide control strategy based on current state.
    if (decelTime >= travelTime) { // Deceleration phase.
        //Serial.println("Decelerating");
        if (velocity.c > 0) {
            acceleration.c = NORMALIZE(velocity.c) * (-Settings::Motion::MAX_ROT_ACCEL);
        } else {
            acceleration.c = 0;
        }
    } else if (fabs(velError) > 0.05f) { // Acceleration phase.
        //Serial.println("Accelerating");
        acceleration.c = NORMALIZE(velError) * Settings::Motion::MAX_ROT_ACCEL;

    } else { // Maintain constant speed.
        acceleration.c = 0;
    }

    // Enforce acceleration limit.
    if (acceleration.c > Settings::Motion::MAX_ROT_ACCEL) {
        acceleration.c = (acceleration.c > 0 ? 1.0 : -1.0) * Settings::Motion::MAX_ROT_ACCEL;
    }
}


void PositionController::setTarget(const Vec3 &t) {
    newTarget = t;
}
