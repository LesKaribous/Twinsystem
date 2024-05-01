#include "sensor.h"
#include <Arduino.h>

Sensor::Sensor(int pin) : m_pin(pin){}
AnalogSensor::AnalogSensor(int pin) : Sensor(pin){}
DigitalSensor::DigitalSensor(int pin, bool inverted) : Sensor(pin), m_inverted(inverted){}

void DigitalSensor::attach() const{
    pinMode(m_pin, INPUT_PULLUP);
}

bool DigitalSensor::read() const{
    return digitalRead(m_pin) != m_inverted;
}

int AnalogSensor::read() const{
    return analogRead(m_pin);
}