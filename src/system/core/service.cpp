#include "service.h"
#include "os.h"

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
    if(str.equalsIgnoreCase("LIDAR"))
        return LIDAR;

    if(str.equalsIgnoreCase("CHRONO"))
        return CHRONO;

    if(str.equalsIgnoreCase("SCREEN"))
        return SCREEN;

    if(str.equalsIgnoreCase("INPUTS"))
        return INPUTS;

    if(str.equalsIgnoreCase("MOTION"))
        return MOTION;

    if(str.equalsIgnoreCase("PLANNER"))
        return PLANNER;

    if(str.equalsIgnoreCase("NEOPIXEL"))
        return NEOPIXEL;

    if(str.equalsIgnoreCase("INTERCOM"))
        return INTERCOM;

    if(str.equalsIgnoreCase("TERMINAL"))
        return TERMINAL;
  
    if(str.equalsIgnoreCase("ACTUATORS"))
        return ACTUATORS;

    if(str.equalsIgnoreCase("LOCALISATION"))
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