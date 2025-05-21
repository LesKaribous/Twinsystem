
#include "positionController.h"
#include "settings.h"
#include "os/console.h"
#include "services/lidar/occupancy.h"
#include "services/localisation/localisation.h"

#define NORMALIZE(x) (x > 0 ? 1.0 : (x < 0 ? -1.0 : 0.0))

/**/
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
        targetSpeed = std::min(minSpeed, maxSpeed * (fabs(error) / proportionalThreshold));
    }
    //targetSpeed = std::min(minSpeed, maxSpeed/10.0f * (fabs(error)));

    float targetVel = direction * targetSpeed;

    // Compute velocity error.
    float velError = targetVel - velocity;

    // Scale acceleration proportionally based on velocity error.
    
    float accel = maxAccel;
    
    if (fabs(velError) < proportionalVelocityThreshold)
    {
        accel = std::min(maxAccel, maxAccel * (fabs(velError) / proportionalVelocityThreshold));
    }
    //accel = std::min(maxAccel, maxAccel/10.0f * (fabs(velError)));

    // Compute the required stopping distance from current velocity.
    float stoppingDistance = (velocity * velocity) / (2.0f * (accel + EPSILON));

    float acceleration = 0.0f;

    // Determine motion phase clearly.
    if (stoppingDistance*1.0 >= fabs(error))
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
    last_position(0.0f, 0.0f),
    velocity(0.0f, 0.0f),
    acceleration(0.0f, 0.0f),
    target(0.0f, 0.0f),
    last_velocity(0.0f, 0.0f, 0.0f),
    target_velocity(0.0f, 0.0f, 0.0f),
    newTarget(0.0f, 0.0f)
{
    controller.setPIDGains(Settings::Motion::kP, Settings::Motion::kI, Settings::Motion::kD);
    //controller.enable();
    
}

void PositionController::setFeedrate(float feed){
    m_feedrate = feed;
}

void PositionController::run() {
    control();
}

void PositionController::reset() {
    Job::reset();
    position= Vec3(0.0f);
    last_position= Vec3(0.0f);
    velocity= Vec3(0.0f);
    acceleration = Vec3(0.0f);
    target= Vec3(0.0f);
    last_velocity= Vec3(0.0f);
    target_velocity= Vec3(0.0f);
    newTarget = Vec3(0.0f);
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
    

    float heading = localisation.getPosition().c;
    velocity = localisation.getVelocity();
    position = localisation.getPosition();
    //velocity = controller.getCurrentVelocity();
    velocity.rotateZ(-heading);
    //position = position + ( velocity * dt);

    Vec2 error = target - position;
    float angle = target.c - position.c;

    if(isPending() && !isPaused()){
        acceleration.x = command(dt, error.x, velocity.x, Settings::Motion::MAX_ACCEL * m_feedrate, 20.0f, Settings::Motion::MAX_SPEED * m_feedrate, Settings::Motion::MIN_DISTANCE, 100, 50 );
        acceleration.y = command(dt, error.y, velocity.y, Settings::Motion::MAX_ACCEL * m_feedrate, 20.0f, Settings::Motion::MAX_SPEED * m_feedrate, Settings::Motion::MIN_DISTANCE, 100, 50 );
        acceleration.c = command(dt, angle, velocity.c, Settings::Motion::MAX_ROT_ACCEL * m_feedrate, 0.01, Settings::Motion::MAX_ROT_SPEED * m_feedrate, Settings::Motion::MIN_ANGLE, 0.5, 0.1 );
    }
    /*
    if(isPending() && !isPaused()){
        acceleration.x = pidCommand(dt, error.x, velocity.x, 0.8, 0.4, Settings::Motion::MAX_ACCEL, 5, Settings::Motion::MAX_SPEED, Settings::Motion::MIN_DISTANCE);
        acceleration.y = pidCommand(dt, error.y, velocity.y, 0.8, 0.4, Settings::Motion::MAX_ACCEL, 5, Settings::Motion::MAX_SPEED, Settings::Motion::MIN_DISTANCE);
        acceleration.c = pidCommand(dt, angle, velocity.c, 0.7, 0.1, Settings::Motion::MAX_ROT_ACCEL, 5, Settings::Motion::MAX_ROT_SPEED, Settings::Motion::MIN_ANGLE);
    }*/

    Vec2 lidar_repulsion = occupancy.repulsiveGradient(position) * 0.1;
    acceleration.x -= lidar_repulsion.x;
    acceleration.y -= lidar_repulsion.y;

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

    if(fabs(angle) > Settings::Motion::MIN_ANGLE){
        target_velocity.c += acceleration.c * dt;
    }else{
        target_velocity.c = 0.998 * (target_velocity.c +  acceleration.c * dt);
    }



    Vec3 final_target_velocity = target_velocity;
    if(fabs(target_velocity.x) < 20) final_target_velocity.x = 0;
    if(fabs(target_velocity.y) < 20) final_target_velocity.y = 0;
    if(fabs(target_velocity.c) < 0.1) final_target_velocity.c = 0;

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
    

    //Console::plot("pos", String(position.x));
    //Console::plot("vel", String(Vec2(velocity).mag()));
    //Console::plot("tvel", String(Vec2(final_target_velocity).mag()));
    

    //Vec3 expected_delta = target_velocity * dt;
    //Vec3 actual_delta = position - last_position; //Console::info("p") << expected_delta << " | " << actual_delta << Console::endl;
    //Console::plot("vel", String(actual_delta.mag()));

    float velocityError = Vec2(final_target_velocity).mag() - Vec2(velocity).mag();
    //Console::plot("evel", String(Vec2(velocityError).mag()));
    //Vec3 delta_error = expected_delta - actual_delta; //Console::info("v") << velocityError << " | " << delta_error << Console::endl;

    //float movement_error = delta_error.mag();
    //float expected_distance = expected_delta.mag();

    // Allow small deviation (e.g. 30% of expected), ignore small commands
    //bool low_progress = expected_distance > 1.0f && movement_error > expected_distance * 0.7f;

    // if (low_progress) {
    //     collisionCounter++;
    // } else {
    //     collisionCounter = 0;
    // }


    bool possibleCollision = fabs(velocityError) > COLLISION_VELOCITY_DIFF * Vec2(final_target_velocity).mag();

    if (possibleCollision && final_target_velocity.mag() > 70) {
        collisionCounter++;
    } else {
        collisionCounter = 0;
    }

    // Console::println(target_velocity.mag());// << collisionCounter  << " | " <<  << Console::endl;
    // Console::println(velocityError.y);// << collisionCounter  << " | " <<  << Console::endl;
    // Console::println(collisionCounter);// << collisionCounter  << " | " <<  << Console::endl;
    //Console::info() << velocity << " | " << last_velocity << Console::endl;
    

    if (collisionCounter > COLLISION_THRESHOLD && !isCanceling()) {
        Console::error("PositionController") << "Collision detected, canceling move" << Console::endl;
        cancel();
        last_position = position;
        last_velocity = velocity;
        return;
    }

    last_position = position;
    last_velocity = velocity;
    //Console::info() << "p:" << position << " | t:" << target << " | v:" << velocity << " | tv:" << target_velocity << " | a:" << acceleration << Console::endl;
    //Console::plotXY("pos", String(position.x), String(position.y));
    //Console::plotXY("target", String(target.x), String(target.y));
}

void PositionController::onPausing(){
    Console::error() << "Not implemented, cancelling instead" << Console::endl;
    onCanceling(); 
}

void PositionController::onCanceling(){

    float dt = Settings::Motion::PID_INTERVAL * 1e-6;

    target_velocity *= 0.9;
    
    velocity = localisation.getVelocity();
    //velocity = controller.getCurrentVelocity();
    velocity.rotateZ(-position.c);
    position = position + ( velocity * dt);

    Vec3 final_target_velocity = target_velocity;
    if(fabs(target_velocity.x) < 5) final_target_velocity.x = 0;
    if(fabs(target_velocity.y) < 5) final_target_velocity.y = 0;
    if(fabs(target_velocity.c) < 0.2) final_target_velocity.c = 0;

    if(final_target_velocity.magSq() > 0){
        final_target_velocity.rotateZ(position.c);
        controller.setTargetVelocity(final_target_velocity);
    }else{
        controller.setTargetVelocity(Vec3(0));
        onCanceled();
    }

}

void PositionController::control() {


    RUN_EVERY(
        Console::plot("posx", String(position.x));
        Console::plot("posy", String(position.y));
        Console::plot("vel", String(Vec2(velocity).mag()));
        Console::plot("tvel", String(Vec2(target_velocity).mag()));
    ,50)


    static long lastTime = 0;
    if(micros() - lastTime > Settings::Motion::PID_INTERVAL) {
        //THROW(position);
		lastTime = micros();
        if(isPausing()) onPausing();
        else if(isCanceling()) onCanceling();
        else if(isCanceled()) return;
        else if(isCompleted()) return;
        else onUpdate();
    }


    controller.control();
}

void PositionController::deccelerate(){
    float dt = Settings::Motion::PID_INTERVAL * 1e-6;
    //Position
    float currentSpeed = velocity.mag();
    if (currentSpeed > 0) {
        acceleration = Vec2::normalize(velocity) * (-Settings::Motion::MAX_ACCEL * m_feedrate);
        //Serial.println(acceleration);
    } else {
        acceleration = Vec2(0);
    }

    //Rotation
    if (velocity.c > 0) {
        acceleration.c = NORMALIZE(velocity.c) * (-Settings::Motion::MAX_ROT_ACCEL * m_feedrate);
    } else {
        acceleration.c = 0;
    }    

    target_velocity.x = 0.98 * (target_velocity.x + acceleration.x * dt);
    target_velocity.y = 0.98 * (target_velocity.y + acceleration.y * dt);
    target_velocity.c = 0.998 * (target_velocity.c +  acceleration.c * dt);

    Vec3 final_target_velocity = target_velocity;
    if(fabs(target_velocity.x) < 5) final_target_velocity.x = 0;
    if(fabs(target_velocity.y) < 5) final_target_velocity.y = 0;
    if(fabs(target_velocity.c) < 0.5) final_target_velocity.c = 0;

    if(final_target_velocity.magSq() > 100){
        final_target_velocity.rotateZ(position.c);
        controller.setTargetVelocity(final_target_velocity);
    }else{
        controller.setTargetVelocity(Vec3(0));
        onCanceled();
        Console::success("PositionController") << "Successfully canceled move" << Console::endl;
        complete();
    }
}

void PositionController::setPosition(const Vec3 &t){
    position = t;
}

void PositionController::setTarget(const Vec3 &t){
    Console::info() << "Setting target to " << t << Console::endl;
    newTarget = t;
}

void PositionController::setSteppers(Stepper* a, Stepper* b, Stepper* c){
    controller.setSteppers(a,b,c);
    controller.setTargetVelocity(Vec3(0));
}
