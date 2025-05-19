#pragma once
#include "services/motion/stepper.h"
#include "services/motion/controller/controller.h"
#include "utils/geometry.h"

class VelocityController : public Controller {
public:
    VelocityController();
    void setSteppers(Stepper* a, Stepper* b, Stepper* c);
    void setTargetVelocity(const Vec3& targetVelocity);
    void setPIDGains(float kp, float ki, float kd);

    void reset();
    void enable();
    void disable();

    void control() override;

    Vec3 getCurrentVelocity() const;
    Vec3 getTargetVelocity() const;
    
private:
    Stepper* m_sA;
    Stepper* m_sB;
    Stepper* m_sC;

    Vec3 m_target_velocity;
    Vec3 m_current_velocity;
    Vec3 m_pid_integral;
    Vec3 m_pid_last_error;
    
    float m_kp, m_ki, m_kd;
};
