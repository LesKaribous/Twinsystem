#include "safety.h"

#include "os/console.h"
#include "services/motion/motion.h"
#include "services/intercom/intercom.h"
#include "utils/timer/timer.h"

INSTANTIATE_SERVICE(Safety, safety)

void getDistanceCallback(Request& req){
    int d = req.getResponse().toInt();
    if(d > 200 && d < 3000) safety.setSafeDistance(d);
    else safety.setSafeDistance(1000000);
}

void Safety::onAttach(){
}

void Safety::onUpdate(){
    RUN_EVERY(
        if(motion.isMoving()){
            int streer = RAD_TO_DEG * motion.getAbsoluteTargetDirection(); //We are moving in this direction
            intercom.sendRequest("getDistance("+ String(streer) +")", 100, getDistanceCallback);
        }
    , 100)

    if(motion.isPaused() && m_currentDistance > 200){
        motion.resume();
        Console::println("resume");
    }else if(motion.isMoving() && m_currentDistance < 300){
        motion.pause();
        Console::println("pause");
    }
}

void Safety::setSafeDistance(int d){
    m_currentDistance = d;
}