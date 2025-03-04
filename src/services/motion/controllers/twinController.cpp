#include "twinController.h"
#include "settings.h"
#include "pin.h"

TwinController::TwinController() : 
    m_sA(Pin::Stepper::dirA, Pin::Stepper::stepA, !Settings::Stepper::STEP_A_POLARITY), 
    m_sB(Pin::Stepper::dirB, Pin::Stepper::stepB, !Settings::Stepper::STEP_B_POLARITY),  
    m_sC(Pin::Stepper::dirC, Pin::Stepper::stepC, !Settings::Stepper::STEP_C_POLARITY),

      m_target_velocity(0, 0, 0), m_current_velocity(0, 0, 0),
      m_pid_integral(0, 0, 0), m_pid_last_error(0, 0, 0),
      m_kp(Settings::Motion::kP), m_ki(Settings::Motion::kI), m_kd(Settings::Motion::kD) { 

        pinMode(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
      }

void TwinController::setPIDGains(float kp, float ki, float kd) {
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
}

void TwinController::enable(){
    digitalWrite(Pin::Stepper::enable, Settings::Stepper::ENABLE_POLARITY);
}

void TwinController::disable(){
    digitalWrite(Pin::Stepper::enable, !Settings::Stepper::ENABLE_POLARITY);
}

void TwinController::setTargetVelocity(const Vec3& targetVelocity) {
    // Saturate speeds if necessary
    float maxSpeed = Settings::Stepper::MAX_SPEED;;
    float M = std::max(std::max(fabs(targetVelocity.a), fabs(targetVelocity.b)), fabs(targetVelocity.c));

    if (M > maxSpeed) {
        m_target_velocity = targetVelocity * (maxSpeed / M);
    } else {
        m_target_velocity = targetVelocity;
    }
}

void TwinController::control() {
    float dt = float(Settings::Stepper::STEPPER_DELAY) * 1e-3f; // Convert ms to seconds
    
    Vec3 error = m_target_velocity - m_current_velocity;
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

    // Read actual velocities for feedback control
    m_current_velocity = Vec3(m_sA.getVelocity(), m_sB.getVelocity(), m_sC.getVelocity());
}

Vec3 TwinController::getCurrentVelocity() const {
    return m_current_velocity;
}

Vec3 TwinController::getTargetVelocity() const {
    return m_target_velocity;
}
