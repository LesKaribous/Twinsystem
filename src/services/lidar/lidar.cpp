#include "lidar.h"
#include "os/console.h"
#include "services/intercom/intercom.h"
//#include "services/navigation/navigation.h"
#include "services/motion/motion.h"

INSTANTIATE_SERVICE(Lidar, lidar)

Lidar::Lidar() : Service(ID_LIDAR){}

void Lidar::onAttach(){
    if(!intercom.enabled()){
        Console::error("Lidar") << "is not enabled" << Console::endl;
    }
}

void Lidar::onUpdate(){
    static Vec3 pos;
    if(enabled()){
        //if((millis() - m_lastPosUpdate > 100 && Vec3::distanceBetween(pos, nav.getPosition()) > 10) || millis() - m_lastPosUpdate > 1000){
        if((millis() - m_lastPosUpdate > 100 && Vec3::distanceBetween(pos, motion.getAbsPosition()) > 10) || millis() - m_lastPosUpdate > 1000){
            m_lastPosUpdate = millis();
            //pos = nav.getPosition();
            pos = motion.getAbsPosition();
            intercom.sendRequest("setRobotPosition(" + String(pos.x)  + "," + String(pos.y) + "," + String(pos.z*RAD_TO_DEG) + ")", 100);
        }
        /*
        if(millis() - m_lastOccupancyRequest > 150){
            m_lastOccupancyRequest = millis();
            intercom.sendRequest("getOccupancyMap()", 1000, onOccupancyResponse, onOccupancyTimeout);
        }*/
    }
}

void Lidar::enable(){
    Service::enable();
}

void Lidar::disable(){
    Service::disable();
}

void Lidar::showRadarLED(){
    intercom.sendRequest("displayLidar",100);
    Console::println("displayLidar");
}

void Lidar::showStatusLED(){
    intercom.sendRequest("displayIntercom",100);
    Console::println("displayIntercom");
}
