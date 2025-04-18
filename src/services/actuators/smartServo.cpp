#include "smartServo.h"
#include "os/console.h"

#define MAX_ITERATIONS 1000

SmartServo::SmartServo() :
    m_position(0), m_enabled(false), m_sleeping(false), m_pin(-1), m_minPos(0), m_maxPos(0), m_defaultPos(0)
{}

SmartServo::SmartServo(int pin, int defaultPos, int minPos, int maxPos) :
    m_position(0), m_enabled(false), m_sleeping(false), m_pin(pin), m_minPos(minPos), m_maxPos(maxPos), m_defaultPos(defaultPos)
{
    m_poses.reserve(MAX_POSES);
}

SmartServo::SmartServo(const SmartServo& cpy) : m_pin(cpy.m_pin), 
    m_minPos(cpy.m_minPos),
    m_maxPos(cpy.m_maxPos),
    m_defaultPos(cpy.m_defaultPos),
    m_servo(cpy.m_servo),
    m_position(cpy.m_position),
    m_poses(cpy.m_poses),
    m_enabled(cpy.m_enabled),
    m_sleeping(cpy.m_sleeping)
{}


bool SmartServo::moveToDefault(int speed, bool runAsync){
    return moveTo(m_defaultPos, speed, runAsync);
}

bool SmartServo::moveTo(int target, int speed, bool runAsync){ //true for non blocking mode
    if(!m_enabled) return;
    if(m_sleeping) wakeUp(); //Wake

    constrain(target, m_minPos, m_maxPos);
    constrain(speed, 0, 100);

    m_target = target;
    m_speed = speed;

    int ms = map(speed, 0, 100, 50, 0);
    int currrentPos = 0;
    //TODO safety exit based on max iteration.
    do{
        if(!runAsync) delay(ms);
        else if(millis() - m_lastUpdate < ms){
            return false;
        }

        m_lastUpdate = millis();
        currrentPos = getPosition();

        if(abs(currrentPos - m_target) < 2){
            currrentPos = m_target;
            m_servo.write(m_target);
            return true;
        }

        if (currrentPos < target){
            m_servo.write(currrentPos + 2); //use multiple of two to avoid infinite loop an bricked
            if(runAsync) return false;
        }else if (currrentPos > target){
            m_servo.write(currrentPos - 2);
            if(runAsync) return false;
        }else{
            if(runAsync) return true;
        }
    }while(!runAsync && currrentPos != m_target);
    return true;
}

bool SmartServo::moveToPose(int index,  int speed, bool runAsync){
    return moveTo(getPose(index), speed, runAsync);
}

int SmartServo::getPosition(){
    return m_servo.read();
}

void SmartServo::setPose(int index, int pose){
    if(!hasPose(index)){
        if(m_poses.size() < MAX_POSES) m_poses[index] = pose;
        else Console::error("Servo") << "Max pose limit reached, SmartServo can handle up to " << MAX_POSES << " poses" << Console::endl;
    }else m_poses[index] = pose;
}

bool SmartServo::hasPose(int pose) const{
    return (m_poses.find(pose) == m_poses.end());
}

int SmartServo::getPose(int index) const{
    if(m_poses.find(index) == m_poses.end()){
        Console::error("SmartServo") << "Invalid index" << index << " in  m_poses array " << HERE << Console::endl;
        return m_defaultPos;
    }
    return m_poses.at(index);
}

void SmartServo::sleep(){
    if(m_pin == -1) return;
    m_servo.detach();
    m_sleeping = true;
}

void SmartServo::wakeUp(){
    
    if(m_enabled){
        m_servo.attach(m_pin);
    }
    m_sleeping = false;
}

void SmartServo::enable(){
    if(m_pin == -1) return;
    m_servo.attach(m_pin);
    moveToDefault();
    m_enabled = true;
    m_sleeping = false;
}

void SmartServo::disable(){
    m_enabled = false;
    if(m_pin == -1) return;
    m_servo.detach();
}
