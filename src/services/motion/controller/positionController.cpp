
#include "positionController.h"
#include "settings.h"
#include "os/console.h"
#include "services/lidar/occupancy.h"
#include "services/localisation/localisation.h"


#define NORMALIZE(x) (x > 0 ? 1.0 : (x < 0 ? -1.0 : 0.0))


PositionController::PositionController() : 
    position(0.0f, 0.0f),
    last_position(0.0f, 0.0f),
    velocity(0.0f, 0.0f),
    acceleration(0.0f, 0.0f),
    target(0.0f, 0.0f),
    last_velocity(0.0f, 0.0f, 0.0f),
    target_velocity(0.0f, 0.0f, 0.0f),
    newTarget(0.0f, 0.0f),
    collisionCounter(0),
    // vx_controller  (4.0, 0.0,100.0),
    // vy_controller  (4.0, 0.0, 100.0),
    // vrot_controller(10, 0.0,70.0)
    vx_controller  (4.2, 0, 0.0),
    vy_controller  (4.2, 0, 0.0),
    vrot_controller(20.0, 0, 0.6)
{
    //controller.setPIDGains(Settings::Motion::kP, Settings::Motion::kI, Settings::Motion::kD);
    //controller.enable();
}

void PositionController::setFeedrate(float feed){
    m_feedrate = feed;
}

void PositionController::exec() {
    //control();
}

void PositionController::reset() {
    Job::reset();
    position= Vec3(0.0f);
    last_position= Vec3(0.0f);
    velocity= Vec3(0.0f);
    last_velocity= Vec3(0.0f);
    acceleration = Vec3(0.0f);
    target= Vec3(0.0f);
    last_velocity= Vec3(0.0f);
    target_velocity= Vec3(0.0f);
    newTarget = Vec3(0.0f);
    controller.reset();
    vx_controller.reset();
    vy_controller.reset();
    vrot_controller.reset();
    collisionCounter = 0;
    controller.disable();
    //Console::info("PositionController") << "reset" << Console::endl;
}

/*
void PositionController::start() {
    reset();
    m_state = JobState::IDLE;
    Job::start();

    controller.enable();
    // Update target if a new one has been set. 
    if (!(newTarget == target)) {
        target = newTarget;
    }
}
/**/

/**/
void PositionController::start() {
    Job::start();
    controller.enable();
    // Update target if a new one has been set. 
    if (!(newTarget == target)) {
        target = newTarget;
    }

    startPos = localisation.getPosition();
    moveStart = millis();

    //Console::info("PositionController") << "start" << Console::endl;
}
/**/

void PositionController::complete() {
    //Console::info("PositionController") << "next reset is from complete" << Console::endl;
    reset();
    m_state = JobState::COMPLETED;
    newTarget = target = position;  // Reset target to current position.
    //Console::info("PositionController") << "complete" << Console::endl;
}

bool PositionController::collision() const {
    return (collisionCounter >= COLLISION_COUNT_LIMIT && !isCanceling());
}

float PositionController::shortestAngleDiff(float target, float current)
{
    float diff = fmodf(target - current + M_PI, 2.0f * M_PI);
    if (diff < 0)
        diff += 2.0f * M_PI;
    diff -= M_PI;

    if (diff == -M_PI)
        diff = M_PI;

    return diff;
}

void PositionController::onUpdate(){
    static uint32_t lastMicros = micros();
    uint32_t now = micros();
    float dt = (now - lastMicros) * 1e-6f;
    lastMicros = now;

    dt = std::clamp(dt, 1e-5f, Settings::Motion::PID_INTERVAL * 1e-6f * 2.0f);

    float heading = localisation.getPosition().c;
    velocity = localisation.getVelocity();
    position = localisation.getPosition();
    //velocity = controller.getCurrentVelocity();
    velocity.rotateZ(-heading);
    //position = position + ( velocity * dt);

    float angle = PositionController::shortestAngleDiff(target.c, position.c);
    Vec2 error = target - position;
    
    /*
    if(isPending() && !isPaused()){
        //acceleration.x = command(dt, error.x, velocity.x, Settings::Motion::MAX_ACCEL * m_feedrate, 0, Settings::Motion::MAX_SPEED * m_feedrate, Settings::Motion::MIN_DISTANCE, 100, 50 );
        //acceleration.y = command(dt, error.y, velocity.y, Settings::Motion::MAX_ACCEL * m_feedrate, 0, Settings::Motion::MAX_SPEED * m_feedrate, Settings::Motion::MIN_DISTANCE, 100, 50 );
        //acceleration.c = command(dt, angle, velocity.c, Settings::Motion::MAX_ROT_ACCEL * m_feedrate, 0, Settings::Motion::MAX_ROT_SPEED * m_feedrate, Settings::Motion::MIN_ANGLE, 1.0, 0.6 );
        static bool saturatedX = false;
        acceleration.x = x_controller.compute(error.x,  dt, saturatedX);
        acceleration.x = std::clamp(acceleration.x, -Settings::Motion::MAX_ACCEL, Settings::Motion::MAX_ACCEL);
        saturatedX = acceleration.x == -Settings::Motion::MAX_ACCEL || acceleration.x == Settings::Motion::MAX_ACCEL;

        static bool saturatedY = false;
        acceleration.y = y_controller.compute(error.y, dt, saturatedY);
        acceleration.y = std::clamp(acceleration.y, -Settings::Motion::MAX_ACCEL, Settings::Motion::MAX_ACCEL);
        saturatedY = acceleration.y == -Settings::Motion::MAX_ACCEL || acceleration.x == Settings::Motion::MAX_ACCEL;

        static bool saturatedZ = false;
        acceleration.c = rot_controller.compute(angle, dt, saturatedZ);
        acceleration.c = std::clamp(acceleration.c, -Settings::Motion::MAX_ROT_ACCEL, Settings::Motion::MAX_ROT_ACCEL );
        saturatedZ = acceleration.z == -Settings::Motion::MAX_ROT_ACCEL || acceleration.x == Settings::Motion::MAX_ROT_ACCEL;

    }
    
    // lidar repulsion

    //Vec2 lidar_repulsion = occupancy.repulsiveGradient(position) * 200.0;
    //acceleration.x -= lidar_repulsion.x;
    //acceleration.y -= lidar_repulsion.y;

    //Console::plotXY("attractor", position.x - lidar_repulsion.x, position.y - lidar_repulsion.y);
    //Console::plotXY("robot", position.x, position.y);

    //acceleration.x = std::clamp(acceleration.x, -Settings::Motion::MAX_ACCEL * m_feedrate, Settings::Motion::MAX_ACCEL * m_feedrate);
    //acceleration.y = std::clamp(acceleration.y, -Settings::Motion::MAX_ACCEL * m_feedrate, Settings::Motion::MAX_ACCEL * m_feedrate);

    
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
    */

    Vec3 desired = Vec3(0);

    if(!isCanceling() && !isPausing()){
        static bool saturatedX = false;
        desired.x = vx_controller.compute(error.x,  dt, saturatedX);
        desired.x = std::clamp(desired.x, -Settings::Motion::MAX_SPEED * m_feedrate, Settings::Motion::MAX_SPEED * m_feedrate);
        saturatedX = desired.x == -Settings::Motion::MAX_SPEED || desired.x == Settings::Motion::MAX_SPEED;

        static bool saturatedY = false;
        desired.y = vy_controller.compute(error.y, dt, saturatedY);
        desired.y = std::clamp(desired.y, -Settings::Motion::MAX_SPEED * m_feedrate, Settings::Motion::MAX_SPEED * m_feedrate);
        saturatedY = desired.y == -Settings::Motion::MAX_SPEED || desired.x == Settings::Motion::MAX_SPEED;

        static bool saturatedZ = false;
        desired.z = vrot_controller.compute(angle, dt, saturatedZ);
        desired.c = std::clamp(desired.c, -Settings::Motion::MAX_ROT_SPEED * m_feedrate, Settings::Motion::MAX_ROT_SPEED * m_feedrate);
        saturatedZ = desired.c == -Settings::Motion::MAX_ROT_SPEED * m_feedrate || desired.c == Settings::Motion::MAX_ROT_SPEED * m_feedrate;
    }

    /*
    Vec2 velocityDiff = Vec2(target_velocity - desired);
    if(velocityDiff.mag() > Settings::Motion::MAX_ACCEL * 3.0){ //Stalled{
        target_velocity.x = 0.1 * velocity.x + 0.9 * target_velocity.x;
        target_velocity.y = 0.1 * velocity.y + 0.9 * target_velocity.y;
    }
    
    float rotVelDiff = fabs(target_velocity.c - desired.c);
    if(rotVelDiff > Settings::Motion::MAX_ROT_ACCEL * 2.0){ //Stalled{
        target_velocity.z = 0.1 * velocity.z + 0.9 * target_velocity.z;
    }*/
    //target_velocity = desired;
    //target_velocity = velocity;
    

    /*
    if(desired.x > target_velocity.x) target_velocity.x += Settings::Motion::MAX_ACCEL * dt;
    if(desired.x < target_velocity.x) target_velocity.x -= Settings::Motion::MAX_ACCEL * dt;
    if(desired.y > target_velocity.y) target_velocity.y += Settings::Motion::MAX_ACCEL * dt;
    if(desired.y < target_velocity.y) target_velocity.y -= Settings::Motion::MAX_ACCEL * dt;
    if(desired.z > target_velocity.z) target_velocity.c += Settings::Motion::MAX_ROT_ACCEL * dt;
    if(desired.z < target_velocity.z) target_velocity.c -= Settings::Motion::MAX_ROT_ACCEL * dt;
    */



    float velocityError = Vec2(target_velocity).mag() - Vec2(velocity).mag();
    bool possibleCollision = fabs(velocityError) > COLLISION_VELOCITY_DIFF * Vec2(target_velocity).mag();

    if (possibleCollision && target_velocity.mag() > COLLISION_THRESHOLD) {
        collisionCounter++;
    } else {
        collisionCounter = 0;
    }
    

    if(millis() - moveStart > 4000 && 
        (fabs(startPos.x - position.x) < 80 && fabs(startPos.y - position.y) < 80 ) && fabs(startPos.z - position.z) < 5.0 * DEG_TO_RAD){
        collisionCounter++;
    }




    Vec3 velocityDiff = desired - velocity;
    velocityDiff.x = std::clamp(velocityDiff.x, -Settings::Motion::MAX_ACCEL*dt, Settings::Motion::MAX_ACCEL * dt);
    velocityDiff.y = std::clamp(velocityDiff.y, -Settings::Motion::MAX_ACCEL*dt, Settings::Motion::MAX_ACCEL * dt);
    velocityDiff.z = std::clamp(velocityDiff.z, -Settings::Motion::MAX_ROT_ACCEL*dt, Settings::Motion::MAX_ROT_ACCEL*dt);

    target_velocity = velocity + velocityDiff;


    //target_velocity += (velocity - target_velocity)*0.01;


    Vec3 final_target_velocity = target_velocity;
    if(fabs(error.x) < Settings::Motion::MIN_DISTANCE && fabs(target_velocity.x) < 90 * m_feedrate) final_target_velocity.x = 0;
    if(fabs(error.y) < Settings::Motion::MIN_DISTANCE && fabs(target_velocity.y) < 90 * m_feedrate) final_target_velocity.y = 0;
    if(fabs(angle) < Settings::Motion::MIN_ANGLE && fabs(target_velocity.c) < 0.08) final_target_velocity.c = 0;

    final_target_velocity.x = std::clamp(final_target_velocity.x, -Settings::Motion::MAX_SPEED * m_feedrate, Settings::Motion::MAX_SPEED * m_feedrate);
    final_target_velocity.y = std::clamp(final_target_velocity.y, -Settings::Motion::MAX_SPEED * m_feedrate, Settings::Motion::MAX_SPEED * m_feedrate);
    final_target_velocity.z = std::clamp(final_target_velocity.z, -Settings::Motion::MAX_ROT_SPEED * m_feedrate, Settings::Motion::MAX_ROT_SPEED * m_feedrate);

    if(final_target_velocity.magSq() > 0){
        final_target_velocity.rotateZ(position.c);
        controller.setTargetVelocity(final_target_velocity);
    }else{
        if (fabs(error.x) < Settings::Motion::MIN_DISTANCE && 
            fabs(error.y) < Settings::Motion::MIN_DISTANCE && 
            fabs(angle) < Settings::Motion::MIN_ANGLE && isRunning()){
                complete();
            }
            // else if(isPausing() || isCanceling()){
            //     onCanceled();
            // }
        controller.setTargetVelocity(Vec3(0));
    }

    //Velocity based collisions
    /**/




    /*
    RUN_EVERY(
        Console::info("velocityError") << velocityError << Console::endl;
        Console::info("collision") << possibleCollision << Console::endl;
        Console::info("collision") << final_target_velocity.mag() << ">" << COLLISION_THRESHOLD << Console::endl;
        Console::info("collisionCounter") << collisionCounter << ">" << COLLISION_COUNT_LIMIT << Console::endl;
    ,100)
    /**/

    /*
    static int  collisionCounter = 0;

    Vec2 vel2d = Vec2(final_target_velocity);
    float expectedDisp = vel2d.mag() * dt;

    Vec2 cur2d{position.x, position.y};
    float actualDisp   = (cur2d - last_position).mag();
    last_position = cur2d;

    if (expectedDisp > MIN_EXPECTED_DISP) {
        float ratioMiss = (expectedDisp - actualDisp) / expectedDisp;  

        if (ratioMiss > DISP_RATIO_THRESH) {
            collisionCounter++;
        } else {
            collisionCounter = 0;
        }
    } else {
        // tiny commanded move → ignore
        collisionCounter = 0;
    }
    /**/
    last_position = position;
    last_velocity = velocity;
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
    if(fabs(target_velocity.x) < 20) final_target_velocity.x = 0;
    if(fabs(target_velocity.y) < 20) final_target_velocity.y = 0;
    if(fabs(target_velocity.c) < 0.1) final_target_velocity.c = 0;

    if(final_target_velocity.magSq() > 0){
        final_target_velocity.rotateZ(position.c);
        controller.setTargetVelocity(final_target_velocity);
    }else{
        controller.setTargetVelocity(Vec3(0));
        //Console::info("PositionController") << "cancel from start" << Console::endl;
        reset();
        m_state = JobState::CANCELING;
        onCanceled();
    }
}

void PositionController::control() {

    /*
    RUN_EVERY(
        Console::plot("posx", String(position.x));
        Console::plot("posy", String(position.y));
        Console::plot("posz", String(position.z));

        Console::plot("velx", String(velocity.x));
        Console::plot("vely", String(velocity.y));
        Console::plot("velz", String(velocity.z));

        Console::plot("tvelx", String(target_velocity.x));
        Console::plot("tvely", String(target_velocity.y));
        Console::plot("tvelz", String(target_velocity.z));
    ,100)
    */

    if(!isBusy()) return;

    static long lastTime = 0;
    if(micros() - lastTime > Settings::Motion::PID_MIN_INTERVAL) {
        //THROW(position);
		lastTime = micros();
        if(isPausing()) onCanceling();
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
        reset();
        m_state = JobState::CANCELED;
        Console::success("PositionController") << "Successfully canceled move" << Console::endl;
        //complete();
        newTarget = target = position;  // Reset target to current position.
    }
}

void PositionController::setPosition(const Vec3 &t){
    position = t;
}

void PositionController::setTarget(const Vec3 &t){
    //Console::info() << "Setting target to " << t << Console::endl;
    newTarget = t;
}

void PositionController::setSteppers(Stepper* a, Stepper* b, Stepper* c){
    controller.setSteppers(a,b,c);
    controller.setTargetVelocity(Vec3(0));
}



/*
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
/**/