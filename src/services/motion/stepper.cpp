#include "Stepper.h"
#include <Arduino.h>

#include "stepper.h"
#include "settings.h"
#include "os/console.h"

Stepper::Stepper(int step, int dir, bool inverted)
    : m_position(0), m_pin_dir(dir), m_pin_step(step),
      m_inverted_dir(inverted), m_enabled(false), 
      m_target_velocity(0), m_current_velocity(0),
      m_accel(Settings::Stepper::MAX_ACCEL), 
      m_deccel(Settings::Stepper::MAX_ACCEL) {

    pinMode(m_pin_step, OUTPUT);
    pinMode(m_pin_dir, OUTPUT);
}

void Stepper::enable() {
    m_enabled = true;
}

void Stepper::disable() {
    m_enabled = false;
    m_target_velocity = 0;
    m_current_velocity = 0;
}

void Stepper::setDirection(bool forward) {
    bool desired = forward;
    if (m_inverted_dir) desired = !desired;
    
    if (desired != m_dir) {
        digitalWriteFast(m_pin_dir, desired);
        m_dir = desired;
    }
}

void Stepper::setTargetVelocity(float velocity) {
    m_target_velocity = constrain(velocity, -Settings::Stepper::MAX_SPEED, Settings::Stepper::MAX_SPEED);
    //m_step_delay = 1.0e6 / fabs(m_target_velocity * Settings::Stepper::STEP_MODE);
}

void Stepper::setAcceleration(float accel) {
    m_accel = fabs(accel);
}

void Stepper::setDeceleration(float decel) {
    m_deccel = fabs(decel);
}

int Stepper::getStepTime(){
    return m_step_delay;
}

bool Stepper::isRunning() const
{
    return m_enabled && fabs(m_current_velocity) > Settings::Stepper::MIN_SPEED;
}

void Stepper::control() {
    //if (!m_enabled) return;

    double dt = double(Settings::Stepper::STEPPER_DELAY) * 1.0e-6; // Convert µs to seconds
    
    double direction = ((m_target_velocity > m_current_velocity) ? 1.0f : -1.0f);
    // Apply acceleration or deceleration
    if (fabs(m_target_velocity) > fabs(m_current_velocity)) {
        double increment = double(m_accel) * dt;
        m_current_velocity += increment * direction;
    } else if (fabs(m_target_velocity) < fabs(m_current_velocity)) {
        double decrement = double(m_deccel) * dt;
        m_current_velocity -= decrement * direction;
    }

    if(m_target_velocity - m_current_velocity < Settings::Stepper::PULLIN){
        m_current_velocity = m_target_velocity;
    }

    // Prevent overshooting the target velocity
    if ((m_target_velocity > 0 && m_current_velocity > m_target_velocity) ||
        (m_target_velocity < 0 && m_current_velocity < m_target_velocity)) {
        m_current_velocity = m_target_velocity;
    }

    
    // Compute step delay in microseconds (prevent divide by zero)
    m_step_delay = 1.0e6 / fabs(m_current_velocity * Settings::Stepper::STEP_MODE);

    // Update direction
    setDirection(m_current_velocity > 0);
}

void Stepper::step() {
    //if (!m_enabled) return;
    if(m_step_delay == 0) return;

    if (micros() - m_last_step_time >= m_step_delay) {

        if(fabs(m_current_velocity) > Settings::Stepper::PULLIN){
            digitalWriteFast(m_pin_step, HIGH);
            delayMicroseconds(Settings::Stepper::PULSE_WIDTH);
            digitalWriteFast(m_pin_step, LOW);
        }

        if (m_target_velocity >= 0)
            m_position += 1;
        else
            m_position -= 1;

        m_last_step_time = micros();
    }
}
