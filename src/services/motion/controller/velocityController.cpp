#include "velocityController.h"
#include "settings.h"
#include "pin.h"
#include "os/console.h"
#include "services/motion/kinematics.h"

VelocityController::VelocityController() : 
    m_sA(nullptr), 
    m_sB(nullptr),  
    m_sC(nullptr),

    m_target_velocity(0, 0, 0), m_current_velocity(0, 0, 0),
    m_pid_integral(0, 0, 0), m_pid_last_error(0, 0, 0),
    m_kp(Settings::Motion::kP), m_ki(Settings::Motion::kI), m_kd(Settings::Motion::kD) { 
}

void VelocityController::setSteppers(Stepper* a, Stepper* b, Stepper* c){
    m_sA = a;
    m_sB = b;
    m_sC = c;
}

void VelocityController::setPIDGains(float kp, float ki, float kd) {
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
}

void VelocityController::reset(){
    m_sA->reset();
    m_sB->reset();
    m_sC->reset();
}

void VelocityController::enable()
{
    m_sA->enable();
    m_sB->enable();
    m_sC->enable();
}

void VelocityController::disable(){
    m_sA->disable();
    m_sB->disable();
    m_sC->disable();
}

void VelocityController::setTargetVelocity(const Vec3& targetVelocity) {
    // Saturate speeds if necessary
    if(targetVelocity.a == 0 && targetVelocity.b == 0 && targetVelocity.c == 0){
        m_target_velocity = Vec3(0);

        m_sA->setVelocity(0);
        m_sB->setVelocity(0);
        m_sC->setVelocity(0);
        return;
    }

    //Console::info() << "Target Robot velocity : " << targetVelocity << Console::endl;
    Vec3 target = ik(targetVelocity);
    //Console::info() << "Target Wheel velocity : " << target << Console::endl;

    float maxSpeed = Settings::Stepper::MAX_SPEED;
    float M = std::max(std::max(fabs(target.a), fabs(target.b)), fabs(target.c));
    
    if (M > maxSpeed) {
        m_target_velocity = target * (maxSpeed / M);
    } else {
        m_target_velocity = target;
    }

    m_sA->setVelocity(m_target_velocity.a);
    m_sB->setVelocity(m_target_velocity.b);
    m_sC->setVelocity(m_target_velocity.c);
}


void VelocityController::control() {
    m_sA->step();
    m_sB->step();
    m_sC->step();
    m_current_velocity = (Vec3(m_sA->getVelocity(), m_sB->getVelocity(), m_sC->getVelocity()));

}

Vec3 VelocityController::getCurrentVelocity() const {
    return fk(m_current_velocity);
}

Vec3 VelocityController::getTargetVelocity() const {
    return fk(m_target_velocity);
}
