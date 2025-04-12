#pragma once

#include <cmath>
#include <cstdint>

#include "utils/geometry.h"
#include "settings.h"

// Represents an individual stepper motor.

class Stepper {
    public:
    Stepper(int step, int dir, bool inverted = false);
    
    void setDirection(bool forward);
    void step();
    bool shouldStep();

    void enable();
    void disable();
    
    void setVelocity(float vel);
    void setAcceleration(float accel);
    void setDeceleration(float decel);
    
    float getVelocity() const { return m_velocity; }
    float getAccel() const {return m_accel;}

    long getPosition() const { return m_position; }
    void setPosition(long position) { m_position = position; }
    int getStepTime();

    bool isRunning() const;
    
    private:
        bool m_enabled = false;
        unsigned int m_pin_dir, m_pin_step;
        bool m_inverted_dir, m_dir;

        long m_step_delay = 0;
        long m_last_step_time = 0;

        long m_position;
        float m_velocity;

        float m_accel;
        float m_deccel;
    };
