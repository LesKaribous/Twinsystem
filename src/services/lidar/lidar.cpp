#include "lidar.h"
#include "os/routines.h"
INSTANTIATE_SERVICE(Lidar)

Lidar::Lidar() : Service(ID_LIDAR){}

void Lidar::onAttach(){
    if(!intercom.enabled()){
        Console::error("Lidar") << "is not enabled" << Console::endl;
    }
}

void Lidar::onUpdate(){
    //Console::info("Lidar") << "send request" << Console::endl;
    if(enabled())
        intercom.sendRequest("checkLidar", 1000, onOppenentDetected, onIntercomDisconnected);
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
