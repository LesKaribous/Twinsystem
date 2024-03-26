#include "lidar.h"
#include "os/routines.h"
#include "os/console.h"
#include "robot.h"

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
    intercom.sendRequest("displayLidar",500);
}

void Lidar::disable(){
    Service::disable();
    intercom.sendRequest("displayIntercom",500);
}