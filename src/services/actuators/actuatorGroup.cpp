#include "actuatorGroup.h"
#include "os/console.h"

ActuatorGroup::ActuatorGroup(){

}

void ActuatorGroup::enable(){
    /*
    for(auto i = m_asensors.begin(); i != m_asensors.end(); i++){
        i->second.enable();
        
    }*/

    for(auto i = m_dsensors.begin(); i != m_dsensors.end(); i++){
        i->attach();
    }

    for(auto i = m_servos.begin(); i != m_servos.end(); i++){
        i->enable();
    }
}

void ActuatorGroup::disable(){
    for(auto i = m_servos.begin(); i != m_servos.end(); i++){
        i->disable();
    }
}

void ActuatorGroup::sleep(){
    for(auto i = m_servos.begin(); i != m_servos.end(); i++){
        i->sleep();
    }
}

void ActuatorGroup::createServo(int pin, int defaultPos, int minPos, int maxPos){
    if(m_servos.size() < MAX_SERVOS) m_servos.emplace_back(pin, defaultPos, minPos, maxPos);
    else Console::error("ActuatorGroup") << "Max servo limit reached, ActuatorGroup can handle up to " << MAX_POSES << " poses" << Console::endl;
}

SmartServo& ActuatorGroup::getServo(int id){
    if(id < 0 || id >= m_servos.size()) Console::error("ActuatorGroup") << "servo " << id << " does not exist " << Console::endl; 
    return m_servos[id];
}


void ActuatorGroup::createDigitalSensor(int pin, bool inverted){
    if(m_dsensors.size() < MAX_SENSORS) m_dsensors.emplace_back(pin, inverted);
    else Console::error("ActuatorGroup") << "Max sensor limit reached, SmartServo can handle up to " << MAX_POSES << " poses" << Console::endl;
}

DigitalSensor& ActuatorGroup::getDigitalSensor(int id){
    if(id < 0 || id >= m_dsensors.size()) Console::error("ActuatorGroup") << "sensor" << id << " does not exist " << Console::endl; 
    return m_dsensors[id];
}


void ActuatorGroup::createAnalogSensor(int pin){
    if(m_asensors.size() < MAX_SENSORS) m_asensors.emplace_back(pin);
    else Console::error("ActuatorGroup") << "Max sensor limit reached, SmartServo can handle up to " << MAX_POSES << " poses" << Console::endl;
}

AnalogSensor& ActuatorGroup::getAnalogSensor(int id){
    if(id < 0 || id >= m_asensors.size()) Console::error("ActuatorGroup") << "sensor " << id << " does not exist " << Console::endl; 
    return m_asensors[id];
}
