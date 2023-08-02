#include "service.h"


Service::Service(ServiceID id) : m_ID(id){}

void Service::enable() {
    m_enabled = true;
}

void Service::disable() {
    m_enabled = false;
}

bool Service::isEnabled() const { 
    return m_enabled; 
}

ServiceID Service::getID() const { 
    return m_ID; 
}

ServiceID Service::toID(String str){
    if(str == "LIDAR")
        return LIDAR;

    if(str == "CHRONO")
        return CHRONO;

    if(str == "SCREEN")
        return SCREEN;

    if(str ==  "INPUTS")
        return INPUTS;

    if(str ==  "MOTION")
        return MOTION;

    if(str ==  "PLANNER")
        return PLANNER;

    if(str ==  "NEOPIXEL")
        return NEOPIXEL;

    if(str ==  "INTERCOM")
        return INTERCOM;

    if(str ==  "TERMINAL")
        return TERMINAL;
  
    if(str ==  "ACTUATORS")
        return ACTUATORS;

    if(str ==  "LOCALISATION")
        return LOCALISATION;
    
    return NOT_A_SERVICE;
}

 String Service::toString(ServiceID id){
    if(id == LIDAR)
        return "LIDAR";

    if(id == CHRONO)
        return "CHRONO";

    if(id == SCREEN)
        return "SCREEN";

    if(id ==  INPUTS)
        return "INPUTS";

    if(id ==  MOTION)
        return "MOTION";

    if(id ==  PLANNER)
        return "PLANNER";

    if(id ==  NEOPIXEL)
        return "NEOPIXEL";

    if(id ==  INTERCOM)
        return "INTERCOM";

    if(id ==  TERMINAL)
        return "TERMINAL";
  
    if(id ==  ACTUATORS)
        return "ACTUATORS";

    if(id ==  LOCALISATION)
        return "LOCALISATION";
    
    return "NOT_A_SERVICE";
}

String Service::toString() const{
    return toString(m_ID);
}