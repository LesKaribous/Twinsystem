
#include "positionController.h"
#include "settings.h"
#include "os/console.h"
#include "services/localisation/localisation.h"

#define NORMALIZE(x) (x > 0 ? 1.0 : (x < 0 ? -1.0 : 0.0))

float command(float dt,
              float error,
              float velocity,
              float maxAccel,
              float minSpeed,
              float maxSpeed,
              float minDistance,
              float proportionalThreshold,
              float proportionalVelocityThreshold)
{
    constexpr float EPSILON = 1e-5f;

    // If the target is within minDistance, no acceleration needed.
    if (fabs(error) < minDistance)
    {
        return 0.0f;
    }

    float direction = NORMALIZE(error);

    // Compute desired proportional speed based on position error.
    float targetSpeed = maxSpeed;
    if (fabs(error) < proportionalThreshold)
    {
        targetSpeed = std::max(minSpeed, maxSpeed * (fabs(error) / proportionalThreshold));
    }

    float targetVel = direction * targetSpeed;

    // Compute velocity error.
    float velError = targetVel - velocity;

    // Scale acceleration proportionally based on velocity error.
    
    float accel = maxAccel;
    /**/
    if (fabs(velError) < proportionalVelocityThreshold)
    {
        accel = std::max(0.0f, maxAccel * (fabs(velError) / proportionalVelocityThreshold));
    }
    /**/

    // Compute the required stopping distance from current velocity.
    float stoppingDistance = (velocity * velocity) / (2.0f * (maxAccel + EPSILON));

    float acceleration = 0.0f;

    // Determine motion phase clearly.
    if (stoppingDistance >= fabs(error))
    {
        // Deceleration phase.
        acceleration = -NORMALIZE(velocity) * accel;
    }
    else if (fabs(velError) > EPSILON)
    {
        // Acceleration phase.
        acceleration = NORMALIZE(velError) * accel;
    }
    else
    {
        // Constant velocity phase.
        acceleration = 0.0f;
    }

    // Clamp the final acceleration.
    return std::clamp(acceleration, -maxAccel, maxAccel);
}

PositionController::PositionController() : 
    position(0.0f, 0.0f),
    velocity(0.0f, 0.0f),
    acceleration(0.0f, 0.0f),
    target(0.0f, 0.0f),
    target_velocity(0.0f, 0.0f, 0.0f),
    newTarget(0.0f, 0.0f)
{
    controller.setPIDGains(Settings::Motion::kP, Settings::Motion::kI, Settings::Motion::kD);
    //controller.enable();
    
}

void PositionController::run() {
    static long lastTime = 0;
    if(micros() - lastTime > Settings::Motion::PID_INTERVAL) {
        //THROW(position);
		lastTime = micros();
        if(isPausing()) onPausing();
        else if(isCanceling()) onCanceling();
        else onUpdate();
    }
}

void PositionController::reset() {
    Job::reset();
    position= Vec3(0.0f);
    velocity= Vec3(0.0f);
    acceleration = Vec3(0.0f);
    target= Vec3(0.0f);
    target_velocity= Vec3(0.0f);
    newTarget= Vec3(0.0f);
    last_otos_time = micros();
    controller.reset();
}

void PositionController::start() {
    Job::start();
    controller.enable();
    // Update target if a new one has been set. 
    if (!(newTarget == target)) {
        target = newTarget;
    }
}

void PositionController::complete() {
    Job::complete();
    //Serial.println("Stopping");
    acceleration = Vec3(0);
    velocity = Vec3(0);
    target_velocity= Vec3(0.0f);
    newTarget = target = position;  // Reset target to current position.
    controller.setTargetVelocity(Vec3(0));
    controller.disable();
}

void PositionController::onUpdate(){
    float dt = Settings::Motion::PID_INTERVAL * 1e-6;
        
    Vec2 error = target - position;
    float angle = target.c - position.c;

    if(isPending() && !isPaused()){
        acceleration.x = command(dt, error.x, velocity.x, Settings::Motion::MAX_ACCEL, 5.0f, Settings::Motion::MAX_SPEED, Settings::Motion::MIN_DISTANCE, 100, 50 );
        acceleration.y = command(dt, error.y, velocity.y, Settings::Motion::MAX_ACCEL, 5.0f, Settings::Motion::MAX_SPEED, Settings::Motion::MIN_DISTANCE, 100, 50 );
        acceleration.c = command(dt, angle, velocity.c, Settings::Motion::MAX_ROT_ACCEL, 0.01, Settings::Motion::MAX_ROT_SPEED, Settings::Motion::MIN_ANGLE, 0.5, 0.1 );
    }

    if(fabs(error.x) > Settings::Motion::MIN_DISTANCE){
        target_velocity.x += acceleration.x * dt;
    }else {
        target_velocity.x = 0.98 * (target_velocity.x + acceleration.x * dt);
    }

    if(fabs(error.y) > Settings::Motion::MIN_DISTANCE){
        target_velocity.y += acceleration.y * dt;
    }else {
        target_velocity.y = 0.98 * (target_velocity.y + acceleration.y * dt);
    }

    if(fabs(angle) > Settings::Motion::MIN_ANGLE)  
        target_velocity.c += acceleration.c * dt;
    else  
        target_velocity.c = 0.998 * (target_velocity.c +  acceleration.c * dt);
    

    velocity = controller.getCurrentVelocity();
    velocity.rotateZ(-position.c);
    position = position + ( velocity * dt);

    Vec3 final_target_velocity = target_velocity;
    if(fabs(target_velocity.x) < 5) final_target_velocity.x = 0;
    if(fabs(target_velocity.y) < 5) final_target_velocity.y = 0;
    if(fabs(target_velocity.c) < 0.5) final_target_velocity.c = 0;

    if(final_target_velocity.magSq() > 0){
        final_target_velocity.rotateZ(position.c);
        controller.setTargetVelocity(final_target_velocity);
    }else{
        if (fabs(error.x) < Settings::Motion::MIN_DISTANCE && 
            fabs(error.y) < Settings::Motion::MIN_DISTANCE && 
            fabs(angle) < Settings::Motion::MIN_ANGLE && isRunning()) 
            complete();
        controller.setTargetVelocity(Vec3(0));
    }

    //Console::info() << "p:" << position << " | t:" << target << " | v:" << velocity << " | tv:" << target_velocity << " | a:" << acceleration << Console::endl;
    //Console::plotXY("pos", String(position.x), String(position.y));
    //Console::plotXY("target", String(target.x), String(target.y));
}

void PositionController::onPausing(){
    if(velocity.mag() > 0) deccelerate();
    else Job::onPaused();
}

void PositionController::onCanceling(){
    if(velocity.mag() > 0) deccelerate();
    else Job::onCanceled();
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

void PositionController::setPosition(const Vec3 &t){
    position = t;
    last_otos_position = t;
}

void PositionController::setTarget(const Vec3 &t)
{
    newTarget = t;
}

void PositionController::setSteppers(Stepper* a, Stepper* b, Stepper* c){
    controller.setSteppers(a,b,c);
    controller.setTargetVelocity(Vec3(0));
}
