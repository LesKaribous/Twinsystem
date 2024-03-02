#include "advancedServo.h"
AdvancedServo::AdvancedServo(int pin, int defaultPos, int minPos, int maxPos) :
    m_pin(pin), m_enabled(false), m_sleeping(false), m_minPos(minPos), m_maxPos(maxPos), m_defaultPos(defaultPos)
{}

void AdvancedServo::moveToDefault(){
    moveTo(m_defaultPos);
}

void AdvancedServo::moveTo(int _pos){
    if(m_enabled){
        if(m_sleeping) wakeUp(); //Wake
        // Limite _pos à l'intervalle [0, 180]
        if(_pos < m_minPos) _pos = m_minPos;
        else if(_pos > m_maxPos) _pos = m_maxPos;
        m_servo.write(_pos);
    }
}

void AdvancedServo::moveToPose(int index){
    moveTo(getPose(index));
}

int AdvancedServo::getPosition(){
    return m_servo.read();
}

void AdvancedServo::setPose(int index, int pose){
    m_poses[index] = pose;
}

int AdvancedServo::getPose(int index) const{
    if(m_poses.find(index) == m_poses.end()){
        //Console::error("AdvancedServo") << "Invalid index" << index << " in  m_poses array " << HERE << Console::endl;
        return m_defaultPos;
    }
    return m_poses[index];
}

void AdvancedServo::sleep(){
    m_servo.detach();
    m_sleeping = true;
}

void AdvancedServo::wakeUp(){
    if(m_enabled){
        m_servo.attach(m_pin);
    }
    m_sleeping = false;
}

void AdvancedServo::enable(){
    m_servo.attach(m_pin);
    m_enabled = true;
    m_sleeping = false;
}

void AdvancedServo::disable(){
    m_servo.detach();
    m_enabled = false;
}
