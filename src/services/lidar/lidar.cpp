#include "lidar.h"
#include "os/console.h"
#include "services/intercom/intercom.h"
//#include "services/navigation/navigation.h"
#include "services/motion/motion.h"

SINGLETON_INSTANTIATE(Lidar, lidar)

Lidar::Lidar() : Service(ID_LIDAR){}

void Lidar::attach(){
    if(!intercom.enabled()){
        Console::error("Lidar") << "is not enabled" << Console::endl;
    }
}

void Lidar::run(){
    static Vec3 pos;
    if(enabled()){
        //if((millis() - m_lastPosUpdate > 100 && Vec3::distanceBetween(pos, nav.getPosition()) > 10) || millis() - m_lastPosUpdate > 1000){
        if(/*(*/millis() - m_lastPosUpdate > 50 /*&& Vec3::distanceBetween(pos, motion.estimatedPosition()) > 10)*/ || millis() - m_lastPosUpdate > 1000){
            m_lastPosUpdate = millis();
            //pos = nav.getPosition();
            pos = motion.estimatedPosition();
            intercom.sendRequest("pos(" + String(pos.x)  + "," + String(pos.y) + "," + String(pos.z*RAD_TO_DEG) + ")");
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
    intercom.sendRequest("on");
    Console::println("displayLidar");
}

void Lidar::showStatusLED(){
    intercom.sendRequest("off");
    //Console::println("displayIntercom");
}

