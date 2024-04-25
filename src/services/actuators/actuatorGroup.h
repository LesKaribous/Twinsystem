#include "smartServo.h"
#include "sensor.h"
#include <vector>

#define MAX_SERVOS 12
#define MAX_SENSORS 24

class ActuatorGroup{
private:
    std::vector<SmartServo> m_servos;
    std::vector<AnalogSensor> m_asensors;
    std::vector<DigitalSensor> m_dsensors;

public:
    ActuatorGroup();  

    void enable();
    void disable();
    void sleep();

    void createServo(int pin, int defaultPos = 90, int minPos = 0, int maxPos = 180);
    SmartServo& getServo(int id);

    void createAnalogSensor(int pin);
    AnalogSensor& getAnalogSensor(int id);

    void createDigitalSensor(int pin, bool inverted = false);
    DigitalSensor& getDigitalSensor(int id);
};
