#include "safety.h"

#include "os/console.h"
#include "services/motion/motion.h"
#include "services/intercom/intercom.h"
#include "utils/timer/timer.h"

INSTANTIATE_SERVICE(Safety, safety)

void getDistanceCallback(Request& req){
    int d = req.getResponse().toInt();

    /*
    if(d > 200 && d < 3000) safety.setSafeDistance(d);
    else safety.setSafeDistance(infinityf());*/

    if(d == 1)safety.setObstacleDetected(true);
    else safety.setObstacleDetected(false);

    //Console::println( d );
}


void Safety::enable(){
    Service::enable();
}

void Safety::disable(){
    Service::disable();
    m_obstacleDetected = false;
}


void Safety::onAttach(){
}

void Safety::onUpdate(){
    if(!enabled()){return;}
/*
    RUN_EVERY(
        if(!motion.hasFinished()){
            int streer = RAD_TO_DEG * motion.getAbsoluteTargetDirection(); //We are moving in this direction
            int distanceToGo = motion.getTargetDistance() + 350;
            if(distanceToGo > 500) distanceToGo = 500;
            if(distanceToGo < 350) distanceToGo = 350;
            //intercom.sendRequest("getDistance("+ String(streer) +")", 100, getDistanceCallback);
            //Console::println( String("streer:") + String(streer));
            intercom.sendRequest("checkObstacle("+ String(streer) + "," + String(distanceToGo) + ")", 100, getDistanceCallback);
            //CONSOLE_SERIAL.println(m_obstacleDetected);
        }
    , 50)
*/
    
    //if(m_obstacleDetected/*m_currentDistance <= 350*/){
    //    if(!motion.hasFinished() && !motion.isRotating()) motion.pause();
    //    if(!motion.isRotating()) m_lastSeen = millis();
    //}
/*
    if(m_obstacleDetected && !motion.hasFinished() && motion.isPaused() && motion.pauseDuration() > 1000){
        motion.cancel();
        m_obstacleDetected = false;
        Console::println("cancelled");
    }
*/
    if(millis() - m_lastSeen > 2000){
        m_obstacleDetected = false;
    }
    
    //if(motion.isPaused() && !m_obstacleDetected/*m_currentDistance > 350 */&& millis() - m_lastSeen > 1000){
    //    motion.resume();
    //}



}

void Safety::setSafeDistance(int d){
    m_currentDistance = d;
}

void Safety::setObstacleDetected(bool state){
    m_obstacleDetected = state;
}