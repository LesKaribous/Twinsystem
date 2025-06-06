#include "actuatorGroup.h"
#include "os/console.h"

ActuatorGroup::ActuatorGroup(){

}

void ActuatorGroup::enable(){
    for(auto i = m_servos.begin(); i != m_servos.end(); i++){
        i->second.enable();
    }
}

void ActuatorGroup::disable(){
    for(auto i = m_servos.begin(); i != m_servos.end(); i++){
        i->second.disable();
    }
}

void ActuatorGroup::sleep(){
    for(auto i = m_servos.begin(); i != m_servos.end(); i++){
        i->second.sleep();
    }
}

void ActuatorGroup::createServo(int id, int pin, int defaultPos, int minPos, int maxPos){
    if(m_servos.size() < MAX_SERVOS) m_servos.emplace(id, SmartServo(pin, defaultPos, minPos, maxPos));
    else Console::error("ActuatorGroup") << "Max servo limit reached, ActuatorGroup can handle up to " << MAX_POSES << " poses" << Console::endl;
}

bool ActuatorGroup::hasServo(int id){
    return m_servos.find(id) != m_servos.end();
}

void ActuatorGroup:: moveServoToPose(int servo, int pose, int speed){
    if(hasServo(servo))
        getServo(servo).moveToPose(pose, speed);
    else Console::error("ActuatorGroup") << "servo " << servo << " does not exist " << Console::endl; 
}

SmartServo& ActuatorGroup::getServo(int id){
    if(id < 0 || size_t(id) >= m_servos.size()) Console::error("ActuatorGroup") << "servo " << id << " does not exist " << Console::endl; 
    return m_servos[id];
}

/*
void ActuatorGroup::createDigitalSensor(int pin, bool inverted){
    if(m_dsensors.size() < MAX_SENSORS) m_dsensors.emplace_back(pin, inverted);
    else Console::error("ActuatorGroup") << "Max sensor limit reached, SmartServo can handle up to " << MAX_POSES << " poses" << Console::endl;
}

DigitalSensor& ActuatorGroup::getDigitalSensor(int id){
    if(id < 0 || size_t(id) >= m_dsensors.size()) Console::error("ActuatorGroup") << "sensor" << id << " does not exist " << Console::endl; 
    return m_dsensors[id];
}


void ActuatorGroup::createAnalogSensor(int pin){
    if(m_asensors.size() < MAX_SENSORS) m_asensors.emplace_back(pin);
    else Console::error("ActuatorGroup") << "Max sensor limit reached, SmartServo can handle up to " << MAX_POSES << " poses" << Console::endl;
}

AnalogSensor& ActuatorGroup::getAnalogSensor(int id){
    if(id < 0 || size_t(id) >= m_asensors.size()) Console::error("ActuatorGroup") << "sensor " << id << " does not exist " << Console::endl; 
    return m_asensors[id];
}
*/