#include "lidar.h"
#include "os/routines.h"
#include "os/console.h"
#include "services/motion/motion.h"

INSTANTIATE_SERVICE(Lidar, lidar)

Lidar::Lidar() : Service(ID_LIDAR){}

void Lidar::onAttach(){
    if(!intercom.enabled()){
        Console::error("Lidar") << "is not enabled" << Console::endl;
    }
}

void Lidar::onUpdate(){
    if(enabled()){
        if(millis() - m_lastPosUpdate > 100){
            m_lastPosUpdate = millis();
            Vec3 pos = motion.getAbsPosition();
            intercom.sendRequest("setRobotPosition(" + String(pos.x)  + "," + String(pos.y) + "," + String(pos.z) + ")", 100);
        }

        if(millis() - m_lastOccupancyRequest > 150){
            m_lastOccupancyRequest = millis();
            intercom.sendRequest("getOccupancyMap()", 1000, onOccupancyResponse, onOccupancyTimeout);
        }
    }
}

void Lidar::enable(){
    Service::enable();
}

void Lidar::disable(){
    Service::disable();
}

void Lidar::showRadarLED(){
    intercom.sendRequest("displayLidar",500);
}

void Lidar::showStatusLED(){
    intercom.sendRequest("displayIntercom",500);
}
