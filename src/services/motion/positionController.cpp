
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
    controller.setPIDGains(Settings::Motion::kP, Settings::Motion::kI, Settings::Motion::kD);
    controller.enable();
    controller.setTargetVelocity(Vec3(0));
}

void PositionController::run() {
    
    static long lastTime = 0;
    if(micros() - lastTime > Settings::Motion::PID_INTERVAL) {
        //THROW(position);
		lastTime = micros();
    	float dt = Settings::Motion::PID_INTERVAL * 1e-6;

        // Compute the relative target vector and its distance.
        Vec2 relDistance = target - position;
        float angle = target.c - position.c;
        float distance = relDistance.mag();


        if(isPaused()) {
            if(velocity.mag() > 0) deccelerate();
        }

        if(isCancelled()) {
            if(velocity.mag() > 0) deccelerate();
            else complete();
        }


        // Stop if close enough to the target.
        if(isPending() && !isPaused()){
            if (target == position || (distance < Settings::Motion::MIN_DISTANCE && fabs(angle) < Settings::Motion::MIN_ANGLE)) {
                complete();
                return;
            }else{
                controlPosition(dt, relDistance);
                controlRotation(dt, angle);
            }
        }

        velocity = velocity + (acceleration * dt);

        position = position + ( velocity * dt);
        //position = position + ( controller.getCurrentVelocity() * dt);

        if(velocity.mag() > 0.1){
            controller.setTargetVelocity(velocity);
        }else controller.setTargetVelocity(Vec3(0));

        //Console::info() << position << " / " << target << " | " << velocity << " | " << acceleration << Console::endl; 
    }
}

void PositionController::reset() {
    Job::reset();
    position = Vec3(0.0f);
    position= Vec3(0.0f);
    velocity= Vec3(0.0f);
    acceleration = Vec3(0.0f);
    target= Vec3(0.0f);
    newTarget= Vec3(0.0f);
}

void PositionController::start() {
    Job::start();
    // Update target if a new one has been set. 
    if (!(newTarget == target)) {
        target = newTarget * Settings::Calibration::Primary.Cartesian;
    }
}

void PositionController::pause() {
    Job::pause();
}

void PositionController::resume() {
    Job::resume();
}

void PositionController::cancel() {
    Job::cancel();
}

void PositionController::complete() {
    Job::complete();
    //Serial.println("Stopping");
    acceleration = Vec3(0);
    velocity = Vec3(0);
    newTarget = target = position;  // Reset target to current position.
    controller.setTargetVelocity(Vec3(0));
}


void PositionController::control() {
    controller.control();
}

void PositionController::deccelerate(){
    //Position
    float currentSpeed = velocity.mag();
    if (currentSpeed > 0) {
        acceleration = Vec2::normalize(velocity) * (-Settings::Motion::MAX_ACCEL);
        //Serial.println(acceleration);
    } else {
        acceleration = Vec2(0);
    }

    //Rotation
    if (velocity.c > 0) {
        acceleration.c = NORMALIZE(velocity.c) * (-Settings::Motion::MAX_ROT_ACCEL);
    } else {
        acceleration.c = 0;
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
        //THROW("Decelerating");
        if (currentSpeed > 0) {
            acceleration = Vec2::normalize(velocity) * (-Settings::Motion::MAX_ACCEL);
        } else {
            acceleration = Vec2(0);
        }
    } else if (velError.mag() > 0.1f) { // Acceleration phase.
        //THROW("Accelerating");
        acceleration = Vec2::normalize(velError) * Settings::Motion::MAX_ACCEL;
    } else { // Maintain constant speed.
        acceleration = Vec2(0);
    }

    // Enforce acceleration limit.
    if (Vec2(acceleration).mag() > Settings::Motion::MAX_ACCEL) {
        acceleration = Vec2::normalize(acceleration) * Settings::Motion::MAX_ACCEL;
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
