#include "velocityController.h"
#include "settings.h"
#include "pin.h"
#include "os/console.h"
#include "kinematics.h"

VelocityController::VelocityController() : 
    m_sA(Pin::Stepper::stepA, Pin::Stepper::dirA, !Settings::Stepper::DIR_A_POLARITY), 
    m_sB(Pin::Stepper::stepB, Pin::Stepper::dirB, !Settings::Stepper::DIR_B_POLARITY),  
    m_sC(Pin::Stepper::stepC, Pin::Stepper::dirC, !Settings::Stepper::DIR_C_POLARITY),

      m_target_velocity(0, 0, 0), m_current_velocity(0, 0, 0),
      m_pid_integral(0, 0, 0), m_pid_last_error(0, 0, 0),
      m_kp(Settings::Motion::kP), m_ki(Settings::Motion::kI), m_kd(Settings::Motion::kD) { 

        pinMode(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
      }

void VelocityController::setPIDGains(float kp, float ki, float kd) {
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
}

void VelocityController::enable(){
    digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
   
    m_sA.enable();
    m_sB.enable();
    m_sC.enable();

}

void VelocityController::disable(){
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);

    m_sA.disable();
    m_sB.disable();
    m_sC.disable();
}

void VelocityController::setTargetVelocity(const Vec3& targetVelocity) {
    // Saturate speeds if necessary
    if(targetVelocity.a == 0 && targetVelocity.b == 0 && targetVelocity.c == 0){
        m_target_velocity = Vec3(0);
        return;
    }

    Vec3 target = ik(targetVelocity);

    float maxSpeed = Settings::Stepper::MAX_SPEED;;
    float M = std::max(std::max(fabs(target.a), fabs(target.b)), fabs(target.c));
    
    if (M > maxSpeed) {
        m_target_velocity = target * (maxSpeed / M);
    } else {
        m_target_velocity = target;
    }    
}


void VelocityController::control() {
    float dt = float(Settings::Stepper::STEPPER_DELAY) * 1e-6f; // Convert ms to seconds
    
    Vec3 error = (m_target_velocity - m_current_velocity);

    if(error.a + error.b + error.c < Settings::Stepper::PULLIN*3){
        m_current_velocity = m_target_velocity;
        m_sA.setTargetVelocity(0);
        m_sB.setTargetVelocity(0);
        m_sC.setTargetVelocity(0);
        return;
    }


    m_pid_integral += error * dt;
    Vec3 derivative = (error - m_pid_last_error) / dt;

    Vec3 pid_output = (error * m_kp) + (m_pid_integral * m_ki) + (derivative * m_kd);
    m_pid_last_error = error;

    
    
    // Apply PID-controlled velocity commands to steppers
    m_sA.setTargetVelocity(pid_output.a);
    m_sB.setTargetVelocity(pid_output.b);
    m_sC.setTargetVelocity(pid_output.c);

    // Update each stepper's velocity state
    m_sA.control();
    m_sB.control();
    m_sC.control();
    
    m_sA.step();
    m_sB.step();
    m_sC.step();

    
    /*
    m_sA.stepUp();
    m_sB.stepUp();
    m_sC.stepUp();

    delayMicroseconds(Settings::Stepper::PULSE_WIDTH);

    m_sA.stepDown();
    m_sB.stepDown();
    m_sC.stepDown();
    /**/
    // Read actual velocities for feedback control
    m_current_velocity = (Vec3(m_sA.getVelocity(), m_sB.getVelocity(), m_sC.getVelocity()));
}

Vec3 VelocityController::getCurrentVelocity() const {
    return fk(m_current_velocity);
}

Vec3 VelocityController::getTargetVelocity() const {
    return fk(m_target_velocity);
}
