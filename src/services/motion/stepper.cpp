#include "Stepper.h"
#include <Arduino.h>

#include "stepper.h"
#include "settings.h"
#include "os/console.h"

Stepper::Stepper(int step, int dir, bool inverted)
    : m_position(0), m_pin_dir(dir), m_pin_step(step),
      m_inverted_dir(inverted), m_enabled(false), 
      m_velocity(0),
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
    m_velocity = 0;
    m_step_delay = 0;
}

void Stepper::setDirection(bool forward) {
    bool desired = forward;
    if (m_inverted_dir) desired = !desired;
    
    if (desired != m_dir) {
        digitalWriteFast(m_pin_dir, desired);
        m_dir = desired;
    }
}

void Stepper::setVelocity(float velocity) {
    m_velocity = constrain(velocity, -Settings::Stepper::MAX_SPEED, Settings::Stepper::MAX_SPEED);
    m_step_delay = 1.0e6f / fabs(m_velocity);

    if (m_velocity == 0.0f) {
        m_step_delay = 0;
        return;
    }

    if (m_step_delay < Settings::Stepper::MIN_STEP_DELAY)
    m_step_delay = Settings::Stepper::MIN_STEP_DELAY;
    //Console::info("Stepper") << m_step_delay << Console::endl;
    setDirection(m_velocity >= 0);
    //m_step_delay = 1.0e6 / fabs(m_target_velocity * Settings::Stepper::STEP_MODE);
}

void Stepper::setAcceleration(float accel){
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
    return m_enabled && fabs(m_velocity) > Settings::Stepper::PULL_IN;
}

void Stepper::step() {
    if (!m_enabled || m_step_delay == 0)
        return;

    unsigned long now = micros();
    if (now - m_last_step_time >= m_step_delay) {
        // Step pulse
        if (std::abs(m_velocity) >= Settings::Stepper::PULL_IN) {
            digitalWriteFast(m_pin_step, HIGH);
            delayMicroseconds(Settings::Stepper::PULSE_WIDTH);
            digitalWriteFast(m_pin_step, LOW);
        }

        // Update position
        m_position += (m_velocity >= 0) ? 1 : -1;

        m_last_step_time = now;
    }
}

bool Stepper::shouldStep() {
    bool ok = m_enabled && m_step_delay > 0 && (micros() - m_last_step_time >= m_step_delay);
    return ok;
}
