#include "sensor.h"
#include <Arduino.h>

Sensor::Sensor(int pin) : m_pin(pin){}
AnalogSensor::AnalogSensor(int pin) : Sensor(pin){}
DigitalSensor::DigitalSensor(int pin) : Sensor(pin){}

void DigitalSensor::attach() const{
    pinMode(m_pin, INPUT_PULLUP);
}

bool DigitalSensor::read() const{
    return digitalRead(m_pin) != m_inverted;
}

int AnalogSensor::read() const{
    return analogRead(m_pin);
}