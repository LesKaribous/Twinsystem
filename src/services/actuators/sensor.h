#pragma once

class Sensor{
public:
    Sensor(int pin);
protected:
    const int m_pin;
};


class AnalogSensor : public Sensor{
public:
    AnalogSensor(int pin);
    int read() const;
private:

};

class DigitalSensor : public Sensor{
public:
    DigitalSensor(int pin, bool inverted = false);
    bool read() const;
    void attach() const;
private:
    bool m_inverted = false;
};