#pragma once

#include <cmath>
#include <cstdint>

#include "utils/geometry.h"
#include "settings.h"

// Represents an individual stepper motor.
class Stepper {
    public:
    Stepper(int step, int dir, bool inverted = false);
    
    friend class StepperController;

    void setDirection(bool forward);
    void step();
    bool shouldStep();

    void enable();
    void disable();
    
    void setVelocity(float vel);
    float getVelocity() const { return m_velocity; }
    
    int getStepTime();
    bool isRunning() const;
    
    private:
    bool m_enabled = false;
    unsigned int m_pin_dir, m_pin_step;
    bool m_inverted_dir, m_dir;

    
    protected:
    long m_step_delay = 0;
    long m_last_step_time = 0;

    long m_position;
    long m_target;
    long m_finalTarget;
    long m_start;
    long m_totalDistance;

    long m_delta;
    long m_stepsDone;
    long m_error;

    float m_velocity;

    float m_accel;
    float m_deccel;
};