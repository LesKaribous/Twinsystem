#pragma once
#include "services/motion/stepper.h"
#include "utils/geometry.h"

class VelocityController {
public:
    VelocityController();

    void setTargetVelocity(const Vec3& targetVelocity);
    void setPIDGains(float kp, float ki, float kd);

    void enable();
    void disable();
    void control();

    Vec3 getCurrentVelocity() const;
    Vec3 getTargetVelocity() const;
    
private:
    Stepper m_sA;
    Stepper m_sB;
    Stepper m_sC;

    Vec3 m_target_velocity;
    Vec3 m_current_velocity;
    Vec3 m_pid_integral;
    Vec3 m_pid_last_error;
    
    float m_kp, m_ki, m_kd;
};
